#!/bin/bash


# Alternative to mac_craft.sh for input .pkg packages that are not (yet) codesigned.
#
# mac_craft.sh derives the app's and extensions' entitlements by reading them back out of the
# already-signed bundles in the input .pkg (see its "Extract entitlements" section) - this only
# works if that .pkg was already signed, since unsigned bundles carry no entitlements blob to
# read. This script instead resolves the *.entitlements.cmake templates directly (textual
# @PLACEHOLDER@ substitution), so it never depends on the input package being signed at all.
#
# Trade-off: this only stays correct as long as the *.entitlements.cmake templates in this
# checkout match the version that actually built the input package. mac_craft.sh's extraction
# approach doesn't have that dependency, since it always reflects whatever was truly built.
# When the input .pkg is already signed, prefer mac_craft.sh.

# Parse the command line arguments
while getopts "b:p:s:n:k:q:civtuw" opt; do
  case ${opt} in
    b )REL_BASE_DIR=$OPTARG;;
    p )REL_PATH_TO_PKG=$OPTARG ;;
    s )IONOS_TEAM_IDENTIFIER=$OPTARG ;;
    n )NC_TEAM_IDENTIFIER=$OPTARG ;;
    k )SPARKLE_KEY=$OPTARG ;;   # not used
    q )KEYCHAIN_PROFILE_OVERRIDE=$OPTARG ;;
    c )CLEAN_REBUILD=true ;;
    i )PACKAGE_INSTALLER=true ;;
    v )VERBOSE=true ;;
    t )TEAM_PATCHING=true ;;
    u )BUILD_UPDATER=true ;;
    w )USE_STRATO=true ;;
    \? )
      echo "Usage: mac_craft_unsigned.sh [-b <REL_BASE_DIR>] [-p <REL_PATH_TO_PKG>] [-s <IONOS_TEAM_IDENTIFIER>] [-n <NC_TEAM_IDENTIFIER>] [-k <SPARKLE_KEY>] [-q <KEYCHAIN_PROFILE>] [-c CLEAN_REBUILD] [-i PACKAGE_INSTALLER] [-v VERBOSE] [-t TEAM_PATCHING] [-u BUILD_UPDATER] [-w USE_STRATO]"
      exit 1
      ;;
  esac
done

if [ "$VERBOSE" = true ]; then
  echo "VERBOSE MODE"
  set -xe
fi

if [ "$TEAM_PATCHING" == "true" ] && [ -z "$NC_TEAM_IDENTIFIER" ]; then
  echo "Patching aktivated, but NC_TEAM_IDENTIFIER not set. Exiting."
  exit 0
fi

# Check if CODE_SIGN_IDENTITY is set, if not exit
if [ -z "$IONOS_TEAM_IDENTIFIER" ]; then
  echo "IONOS_TEAM_IDENTIFIER not set. Exiting."
  exit 0
fi

if [ -z "$REL_BASE_DIR" ]; then
  echo "REL_BASE_DIR not set. Exiting."
  exit 0
fi

if [ -z "$REL_PATH_TO_PKG" ]; then
  echo "REL_PATH_TO_PKG not set. Exiting."
  exit 0
fi

# Some variables
BASE_DIR="$( cd "$REL_BASE_DIR" && pwd )"
PATH_TO_PKG="$( realpath "$REL_PATH_TO_PKG")"

PKG_FULLNAME=$(basename "$PATH_TO_PKG")
PKG_FILENAME="${PKG_FULLNAME%.pkg}"
if [ "$USE_STRATO" = true ]; then
  PRODUCT_NAME="STRATO HiDrive Next"
  UNDERSCORE_PRODUCT_NAME="STRATO_HiDrive_Next"
  CODE_SIGN_IDENTITY="STRATO AG ($IONOS_TEAM_IDENTIFIER)"
  KEYCHAIN_PROFILE="STRATO AG HiDrive Next"
  APPLICATION_REV_DOMAIN="com.strato.hidrivenext.desktopclient"
else
  PRODUCT_NAME="IONOS HiDrive Next"
  UNDERSCORE_PRODUCT_NAME="IONOS_HiDrive_Next"
  CODE_SIGN_IDENTITY="IONOS SE ($IONOS_TEAM_IDENTIFIER)"
  KEYCHAIN_PROFILE="IONOS SE HiDrive Next"
  APPLICATION_REV_DOMAIN="com.ionos.hidrivenext.desktopclient"
fi

if [ -n "$KEYCHAIN_PROFILE_OVERRIDE" ]; then
  KEYCHAIN_PROFILE="$KEYCHAIN_PROFILE_OVERRIDE"
fi

INSTALLER_CERT="Developer ID Installer: $CODE_SIGN_IDENTITY"
APPLICATION_CERT="Developer ID Application: $CODE_SIGN_IDENTITY"

WORK_DIR="ex"
EXTRACTED_DIR="${BASE_DIR%/}/$WORK_DIR"

PRODUCT_DIR=$EXTRACTED_DIR/$UNDERSCORE_PRODUCT_NAME.pkg/Payload/Applications
SCRIPTS_DIR=$EXTRACTED_DIR/$UNDERSCORE_PRODUCT_NAME.pkg/Scripts
INNER_PKG=$EXTRACTED_DIR/$UNDERSCORE_PRODUCT_NAME.pkg
PAYLOAD_DIR=$EXTRACTED_DIR/$UNDERSCORE_PRODUCT_NAME.pkg/Payload
INSTALLER_PKG=${BASE_DIR%/}/INSTALLER.pkg
APP_PATH=$PRODUCT_DIR/$PRODUCT_NAME.app
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "$SCRIPT_DIR/../../.." && pwd )"
MACCRAFTER_DIR="$( cd "$SCRIPT_DIR/../mac-crafter" && pwd )"

APP_ENTITLEMENTS_TEMPLATE="$REPO_ROOT/admin/osx/macosx.entitlements.cmake"
FILE_PROVIDER_ENTITLEMENTS_TEMPLATE="$REPO_ROOT/shell_integration/MacOSX/FileProviderExt.entitlements.cmake"
FILE_PROVIDER_UI_ENTITLEMENTS_TEMPLATE="$REPO_ROOT/shell_integration/MacOSX/FileProviderUIExt.entitlements.cmake"
FINDER_SYNC_ENTITLEMENTS_TEMPLATE="$REPO_ROOT/shell_integration/MacOSX/FinderSyncExt.entitlements.cmake"


# Sparkle Variables
PACKAGE_PATH="${BASE_DIR%/}/$PKG_FILENAME.resigned.pkg"
SPARKLE_TBZ_PATH="${PACKAGE_PATH}.tbz"
SPARKLE_DIR="${BASE_DIR%/}/sparkle"
SPARKLE_DOWNLOAD_URI="https://github.com/sparkle-project/Sparkle/releases/download/1.27.3/Sparkle-1.27.3.tar.xz"


echo "Expanding original package..."

if [ -d "$EXTRACTED_DIR" ]; then

  echo "$EXTRACTED_DIR already exits."

  if [ "$CLEAN_REBUILD" = true ]; then
    echo "Clean Rebuild Enabled - Deleting folder: $EXTRACTED_DIR"
    rm -rf "$EXTRACTED_DIR"
    pkgutil --expand-full "$PATH_TO_PKG" "$EXTRACTED_DIR"
  fi
else
  pkgutil --expand-full "$PATH_TO_PKG" "$EXTRACTED_DIR"
fi

# ---------------------------------------------------
# Resolve entitlements from the .cmake templates

# @DEVELOPMENT_TEAM@ resolves straight to IONOS_TEAM_IDENTIFIER (the identity this script signs
# with at the end), not to NC_TEAM_IDENTIFIER, since the entitlements must match the actual
# signing team regardless of whether -t/TEAM_PATCHING was requested - a mismatched
# application-groups team ID would leave sandboxed IPC between app and extensions broken even if
# the code signature itself is otherwise valid. @DEBUG_ENTITLEMENTS@ (only present in the
# extension templates) is dropped entirely, matching the Release-config CMake behaviour.

echo "Resolving entitlements from templates..."

ENTITLEMENTS_DIR="${EXTRACTED_DIR%/}/entitlements"
mkdir -p "$ENTITLEMENTS_DIR"

APP_ENTITLEMENTS="$ENTITLEMENTS_DIR/$UNDERSCORE_PRODUCT_NAME.entitlements"
FILE_PROVIDER_ENTITLEMENTS="$ENTITLEMENTS_DIR/FileProviderExt.entitlements"
FILE_PROVIDER_UI_ENTITLEMENTS="$ENTITLEMENTS_DIR/FileProviderUIExt.entitlements"
FINDER_SYNC_ENTITLEMENTS="$ENTITLEMENTS_DIR/FinderSyncExt.entitlements"

resolve_entitlements_template() {
  local template_path=$1
  local out_path=$2

  if [ ! -f "$template_path" ]; then
    echo "Entitlements template not found: $template_path. Exiting."
    open "$BASE_DIR"
    exit 1
  fi

  sed -e "s/@DEVELOPMENT_TEAM@/$IONOS_TEAM_IDENTIFIER/g" \
      -e "s/@APPLICATION_REV_DOMAIN@/$APPLICATION_REV_DOMAIN/g" \
      -e "/@DEBUG_ENTITLEMENTS@/d" \
      "$template_path" > "$out_path"
}

resolve_entitlements_template "$APP_ENTITLEMENTS_TEMPLATE" "$APP_ENTITLEMENTS"
resolve_entitlements_template "$FILE_PROVIDER_ENTITLEMENTS_TEMPLATE" "$FILE_PROVIDER_ENTITLEMENTS"
resolve_entitlements_template "$FILE_PROVIDER_UI_ENTITLEMENTS_TEMPLATE" "$FILE_PROVIDER_UI_ENTITLEMENTS"
resolve_entitlements_template "$FINDER_SYNC_ENTITLEMENTS_TEMPLATE" "$FINDER_SYNC_ENTITLEMENTS"

# ---------------------------------------------------
# Patch Team Identifier

# check wether patching is needed. ".com" is important because otherwise the ID in the signature will be found

if [[ -n "$TEAM_PATCHING" ]]; then
  echo "Team Patching Enabled - Start Patching Detection"

  PLIST_MATCHES=$(find "$APP_PATH" -name "*.plist" -exec grep -q "$NC_TEAM_IDENTIFIER.com" {} \; -print | wc -l)
  BIN_MATCHES=$(find "$APP_PATH" -type f -exec grep -q --binary-files=text "$NC_TEAM_IDENTIFIER.com" {} \; -print | wc -l)

  if [[ "$PLIST_MATCHES" -gt 0 || "$BIN_MATCHES" -gt 0 ]]; then
    # Ensure both IDs are same lengt
    if [[ ${#NC_TEAM_IDENTIFIER} -ne ${#IONOS_TEAM_IDENTIFIER} ]]; then
      echo "NC_TEAM_IDENTIFIER and IONOS_TEAM_IDENTIFIER must be the same length for binary-safe patching."
      open $BASE_DIR
      exit 1
    fi

    if [[ "$PLIST_MATCHES" -gt 0 ]]; then
    # --- Replace in .plist files (plain XML) ---
      echo "Replacing Team Identifier in .plist files..."
      find "$APP_PATH" -name "*.plist" -exec grep -q "$NC_TEAM_IDENTIFIER" {} \; -exec sed -i '' "s/$NC_TEAM_IDENTIFIER/$IONOS_TEAM_IDENTIFIER/g" {} \;
    fi

    if [[ "$BIN_MATCHES" -gt 0 ]]; then
      # Find and patch all binaries containing the old ID
      find "$APP_PATH" -type f -exec grep -q --binary-files=text "$NC_TEAM_IDENTIFIER" {} \; -print | while read -r file; do
        echo "Patching Team Identifier in $file"
        perl -pi -e "s/$NC_TEAM_IDENTIFIER/$IONOS_TEAM_IDENTIFIER/g" "$file"
      done
    fi
  else
    echo "Nothing to patch"
  fi
fi

# ---------------------------------------------------
# Sign the client



echo "start signing the client"

swift run --package-path "$MACCRAFTER_DIR" \
    mac-crafter codesign \
    "$APP_PATH" \
    "$CODE_SIGN_IDENTITY" \
    "$APP_ENTITLEMENTS" \
    "$FILE_PROVIDER_ENTITLEMENTS" \
    "$FILE_PROVIDER_UI_ENTITLEMENTS" \
    "$FINDER_SYNC_ENTITLEMENTS"

# Validate that the key used for signing the binary matches the expected TeamIdentifier
# needed to pass the SocketApi through the sandbox for communication with virtual file system
if ! codesign -dv "$APP_PATH" 2>&1 | grep -q "TeamIdentifier=$IONOS_TEAM_IDENTIFIER"; then
  echo "TeamIdentifier does not match. Exiting."
  open $BASE_DIR
  exit 0
fi

# ---------------------------------------------------
# Installer

echo "start building the installer"

# Build the installer, if enabled
if [ -z "$PACKAGE_INSTALLER" ]; then
  echo "Installer packaging not enabled. Exiting."
  open $BASE_DIR
  exit 0
fi

echo "Renew BOM"
mkbom "$PAYLOAD_DIR" "$INNER_PKG/Bom"
echo "Reassembling the package..."
(cd "$PAYLOAD_DIR" && \
 find . | cpio -o --format odc | gzip -c) > $PAYLOAD_DIR.new

rm -rf $PAYLOAD_DIR
mv $PAYLOAD_DIR.new $PAYLOAD_DIR

(cd $EXTRACTED_DIR && \
  pkgutil --flatten $UNDERSCORE_PRODUCT_NAME.pkg $UNDERSCORE_PRODUCT_NAME.pkg.flat)

rm -rf $INNER_PKG
mv $INNER_PKG.flat $INNER_PKG

productsign --timestamp --sign "$INSTALLER_CERT" \
  $INNER_PKG \
  $INNER_PKG.signed

rm -rf $INNER_PKG
mv $INNER_PKG.signed $INNER_PKG

(cd $BASE_DIR && productbuild \
  --distribution ex/Distribution \
  --resources ex/Resources \
  --package-path ex \
  $INSTALLER_PKG.unsigned)

productsign --timestamp --sign "$INSTALLER_CERT" $INSTALLER_PKG.unsigned "$PACKAGE_PATH"

# catch the output of the notarytool command
OUTPUT=$(xcrun notarytool submit --wait "$PACKAGE_PATH"\
  --keychain-profile "$KEYCHAIN_PROFILE")

SUBMISSION_STATUS=$(echo $OUTPUT | grep -o 'status: [^ ]*' | cut -d ' ' -f 2)

# Check if the notarization was successful
if [ $SUBMISSION_STATUS != "Accepted" ]; then
  echo "Notarization failed. Exiting."
  open $BASE_DIR
  exit 1
fi

# staple
xcrun stapler staple "$PACKAGE_PATH"
xcrun stapler validate "$PACKAGE_PATH"


# Sparkle

SPARKLE_TBZ_PATH="${PACKAGE_PATH}.tbz"

echo "Creating Sparkle package archive: $SPARKLE_TBZ_PATH"

# Load Sparkle

if [ "$BUILD_UPDATER" == "true" ]; then
  echo "Creating Sparkle package archive: $SPARKLE_TBZ_PATH"

  if [ -d "$SPARKLE_DIR" ]; then

    echo "$SPARKLE_DIR already exits."
    echo "Deleting..."
    rm -rf "$SPARKLE_DIR"
  fi

  echo "Download Sparkle"

  mkdir -p $SPARKLE_DIR
  wget $SPARKLE_DOWNLOAD_URI -O ${SPARKLE_DIR%/}/Sparkle.tar.xz
  tar -xvf ${SPARKLE_DIR%/}/Sparkle.tar.xz -C $SPARKLE_DIR

  if tar cf "$SPARKLE_TBZ_PATH" -C "$(dirname "$PACKAGE_PATH")" "$(basename "$PACKAGE_PATH")"; then
      echo "✅ Sparkle package created successfully."
  else
      echo "❌ Could not create Sparkle package tbz!" >&2
      exit 1
  fi

  echo "Signing Sparkle package: $SPARKLE_TBZ_PATH"
  if "${SPARKLE_DIR%/}/bin/sign_update" "$SPARKLE_TBZ_PATH"; then
      echo "✅ Sparkle package signed successfully."
  else
      echo "❌ Could not sign Sparkle package tbz!" >&2
      exit 1
  fi

fi

open $BASE_DIR
