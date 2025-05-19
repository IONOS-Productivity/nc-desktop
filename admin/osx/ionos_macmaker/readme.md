# sign.sh - IONOS HiDrive Next - macOS Resign and Notarization Script

This script automates the **patching**, **re-signing**, **packaging**, **notarization**, and **stapling** of the `IONOS HiDrive Next` macOS `.pkg` installer.

---

## 🔧 What it does

1. **Expands** a `.pkg` installer.
2. **Patches** the Team Identifier in `.plist` files and binaries if required.
3. **Code-signs** all components of the `.app` bundle and related scripts.
4. **Reassembles** and **signs** the final `.pkg` installer.
5. **Submits** the `.pkg` for notarization.
6. **Staples** the notarization ticket onto the package.

---

## 📝 Usage

```bash
./sign.sh -b <base_dir> -p <path_to_pkg> -s <signing_identity> [-c] [-i] [-v]
````

### Parameters

| Flag | Description                                                               |
| ---- | ------------------------------------------------------------------------- |
| `-b` | **Base directory** where output and working directories will be stored.   |
| `-p` | **Path to the original `.pkg`** to be processed.                          |
| `-s` | **Code signing identity** (common name of your Developer ID certificate). |
| `-c` | Optional: Perform a **clean rebuild** of the expanded `.pkg`.             |
| `-i` | Optional: Enable **installer creation** (required for final output).      |
| `-v` | Optional: Enable **verbose mode** for detailed logging.                   |
| `-t` | Optional: Enable patching of the Team Identifer in Binaries               |

---

## 📦 Example

```bash
./sign.sh -b ~/Desktop/build -p ./IONOS_HiDrive.pkg -s "IONOS SE (SAMPLE_ID)" -c -i -v
```

---

## 📁 Output

After successful execution, you’ll find the final notarized and stapled installer here:

```
<base_dir>/<original_pkg_name>.resigned.pkg
```

---

## 🔐 Code Signing Notes

* `APPLICATION_CERT` is used to sign the `.app`, its frameworks, and extensions.
* `INSTALLER_CERT` is used to sign the final `.pkg` with `productsign`.
* `TEAM_IDENTIFIER` is **validated** in the final app signature to ensure it matches expectations.

---

## 🧩 Patch Logic

If the script detects the **old Team Identifier** (`NC_TEAM_IDENTIFIER`) inside any `.plist` or binary:

* It **replaces** it with the new identifier (`IONOS_TEAM_IDENTIFIER`), only if the lengths match.
* Ensures a **binary-safe patch** using `perl` and `sed`.
* Only files with actual matches are patched.

---

## 🍏 Notarization

The script uses:

```bash
xcrun notarytool submit --wait --keychain-profile "IONOS SE HiDrive Next"
```

Make sure the keychain profile is **set up properly** and can access your notarization credentials.

---

## 🛠 Requirements

* macOS with Xcode Command Line Tools installed
* Access to a valid **Developer ID Application** and **Installer certificate**
* A configured **notarytool profile** in your keychain
* Tools used:

  * `pkgutil`, `codesign`, `productsign`, `productbuild`, `xcrun`, `plutil`, `grep`, `sed`, `perl`, `mkbom`, `cpio`, `gzip`

---

## 🛡️ Safety Checks

* Team ID patching only happens if both IDs are the **same length**.
* Binary patching is skipped unless needed.
* Final code signature is checked for correct TeamIdentifier.
* If notarization fails, the script exits with an error.

---
