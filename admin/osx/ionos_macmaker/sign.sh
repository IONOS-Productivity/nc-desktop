#!/bin/bash

recursive_sign(){
  local path="$1"
  local extension="${path##*.}"
  if [[ "$extension" == "dylib" || "$extension" == "framework" || "$extension" == "appex" ]]; then
    echo "Signing directory: $path"
    codesign -s "$2" --force --preserve-metadata=entitlements --verbose=4 --deep --options=runtime --timestamp "${path}" 
  fi
}

export -f recursive_sign

sign_folder_content(){
  local folder="$1"
  local identity="$2"
  local entitlements="$3"
  codesign -s "$identity" --force $entitlements --verbose=4 --deep --options=runtime --timestamp "${folder}" 
}

export -f sign_folder_content

# This script is used to build the Mac OS X version of the IONOS client.
set -xe

# Parse the command line arguments
while getopts "b:p:s:ci" opt; do
  case ${opt} in
    b )BASE_DIR=$OPTARG;;
    p )PATH_TO_PKG=$OPTARG ;;
    s )CODE_SIGN_IDENTITY=$OPTARG ;;
    c )CLEAN_REBUILD=true ;;
    i )PACKAGE_INSTALLER=true ;;
    \? )
      echo "Usage: sign.sh [-b <base_dir>] [-s <code_sign_identity>] [-c] [-i]"
      exit 1
      ;;
  esac
done

# Set the deployment target
export MACOSX_DEPLOYMENT_TARGET=10.15

# Some variables
PRODUCT_NAME="IONOS HiDrive Next"
UNDERSCORE_PRODUCT_NAME="IONOS_HiDrive_Next"
TEAM_IDENTIFIER="5TDLCVD243"
WORK_DIR="ex"
FINAL_PKG="Reassembled.pkg"

EXTRACTED_DIR="${BASE_DIR%/}/$WORK_DIR"
FINAL_TARGET_PATH="${BASE_DIR%/}/$FINAL_PKG"

PRODUCT_DIR=$EXTRACTED_DIR/$UNDERSCORE_PRODUCT_NAME.pkg/Payload/Applications


echo "Expanding original package..."

pkgutil --expand-full "$PATH_TO_PKG" "$EXTRACTED_DIR"

# TODO: 
# Load Sparkle
SPARKLE_DIR=$BASE_DIR/sparkle
SPARKLE_DOWNLOAD_URI="https://github.com/sparkle-project/Sparkle/releases/download/1.27.3/Sparkle-1.27.3.tar.xz"

if [ "$CLEAN_REBUILD" == "true" ]; then
  mkdir -p $SPARKLE_DIR
  wget $SPARKLE_DOWNLOAD_URI -O $SPARKLE_DIR/Sparkle.tar.xz
  tar -xvf $SPARKLE_DIR/Sparkle.tar.xz -C $SPARKLE_DIR

  # Sign Sparkle
  if [ -n "$CODE_SIGN_IDENTITY" ]; then
    SPARKLE_FRAMEWORK_DIR=$SPARKLE_DIR/Sparkle.framework
    find "$SPARKLE_FRAMEWORK_DIR/Resources/Autoupdate.app/Contents/MacOS" -mindepth 1 -print0 | xargs -0 -I {} bash -c 'sign_folder_content "$@" "$CODE_SIGN_IDENTITY"' _ {} "$CODE_SIGN_IDENTITY"
    codesign -s "$CODE_SIGN_IDENTITY" --force --preserve-metadata=entitlements --verbose=4 --deep --options=runtime --timestamp "$SPARKLE_FRAMEWORK_DIR/Sparkle"
  fi
fi



# ---------------------------------------------------
# Sign the client
# CODE_SIGN_IDENTITY="Developer ID Application: IONOS SE (5TDLCVD243)"

# Check if CODE_SIGN_IDENTITY is set, if not exit
if [ -z "$CODE_SIGN_IDENTITY" ]; then
  echo "Code sign identity not set. Exiting."
  open $PRODUCT_DIR
  exit 0
fi

PRODUCT_PATH=$PRODUCT_DIR/$PRODUCT_NAME.app

CLIENT_CONTENTS_DIR=$PRODUCT_PATH/Contents
CLIENT_FRAMEWORKS_DIR=$CLIENT_CONTENTS_DIR/Frameworks
CLIENT_PLUGINS_DIR=$CLIENT_CONTENTS_DIR/PlugIns
CLIENT_RESOURCES_DIR=$CLIENT_CONTENTS_DIR/Resources

find "$CLIENT_FRAMEWORKS_DIR" -print0 | xargs -0 -I {} bash -c 'recursive_sign "$@" "$CODE_SIGN_IDENTITY"' _ {} "$CODE_SIGN_IDENTITY"
find "$CLIENT_PLUGINS_DIR" -print0 | xargs -0 -I {} bash -c 'recursive_sign "$@" "$CODE_SIGN_IDENTITY"' _ {} "$CODE_SIGN_IDENTITY"
find "$CLIENT_RESOURCES_DIR" -print0 | xargs -0 -I {} bash -c 'recursive_sign "$@" "$CODE_SIGN_IDENTITY"' _ {} "$CODE_SIGN_IDENTITY"

codesign -s "$CODE_SIGN_IDENTITY" --force --preserve-metadata=entitlements --verbose=4 --deep --options=runtime --timestamp "$PRODUCT_PATH"



# Sign the client
find "$CLIENT_CONTENTS_DIR/MacOS" -mindepth 1 -print0 | xargs -0 -I {} bash -c 'sign_folder_content "$@" "$CODE_SIGN_IDENTITY" "$entitlements" ' _ {} "$CODE_SIGN_IDENTITY" "--preserve-metadata=entitlements"

# Validate that the key used for signing the binary matches the expected TeamIdentifier
# needed to pass the SocketApi through the sandbox for communication with virtual file system
if ! codesign -dv "$PRODUCT_PATH" 2>&1 | grep -q "TeamIdentifier=$TEAM_IDENTIFIER"; then
  echo "TeamIdentifier does not match. Exiting."
  exit 0
fi

# ---------------------------------------------------
# Installer

# Build the installer, if enabled
if [ -z "$PACKAGE_INSTALLER" ]; then
  echo "Installer packaging not enabled. Exiting."
  open $PRODUCT_DIR
  exit 0
fi

echo "Reassembling the package..."
pkgutil --flatten "$EXTRACTED_DIR" "$FINAL_TARGET_PATH"

# codesign -s "$CODE_SIGN_IDENTITY" --force --preserve-metadata=entitlements --verbose=4 --deep --options=runtime --timestamp "$FINAL_TARGET_PATH"
productsign --timestamp --sign 'Developer ID Installer: IONOS SE (5TDLCVD243)' "$FINAL_TARGET_PATH" "$FINAL_TARGET_PATH.new"
exit 0

# package
# $BASE_DIR/admin/osx/create_mac.sh "$PRODUCT_DIR" "$BASE_DIR" 'Developer ID Installer: IONOS SE (5TDLCVD243)'

# notariaze
# Extract package filename from filesystem per .pkg extension
PACKAGE_FILENAME=$(ls $PRODUCT_DIR/*.pkg)

# catch the output of the notarytool command
OUTPUT=$(xcrun notarytool submit --wait $PACKAGE_FILENAME\
  --keychain-profile "IONOS SE HiDrive Next")

SUBMISSION_STATUS=$(echo $OUTPUT | grep -o 'status: [^ ]*' | cut -d ' ' -f 2)

# Check if the notarization was successful
if [ $SUBMISSION_STATUS != "Accepted" ]; then
  echo "Notarization failed. Exiting."
  exit 1
fi

# staple
xcrun stapler staple $PACKAGE_FILENAME
xcrun stapler validate $PACKAGE_FILENAME

open $PRODUCT_DIR
