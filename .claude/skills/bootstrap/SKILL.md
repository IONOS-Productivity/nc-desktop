---
name: bootstrap
description: Richtet die lokale Entwicklungsumgebung für nc-desktop auf der aktuellen Plattform (Windows/macOS/Linux) größtenteils automatisch ein — Voraussetzungen-Check, lokale Git-Konfiguration (Hooks, Merge-Treiber), Craft-Bootstrap und Dependency-Installation. Setzt ein bereits geklontes Repo voraus (für den komplett leeren Rechner ohne Repo siehe Confluence "Setup: KI-gestützt oder manuell", Abschnitt "Bootstrap auf frischem Rechner"). Spiegelt die Confluence-Seiten "Setup: KI-gestützt oder manuell", "Git Hooks" und die Craft-Installation-Seiten je Plattform; bei Abweichung gilt Confluence als Quelle der Wahrheit, dieser Skill dann als veraltet zu korrigieren. Nur manuell auslösen. Trigger — "Bootstrap", "bootstrap ausführen", "richte meine Dev-Umgebung ein", "Setup-Leitfaden abarbeiten", "frisches Repo einrichten", "Craft installieren", "ich hab neu geclont, was jetzt", "Onboarding-Setup".
---

# Bootstrap

Ein frischer Clone von nc-desktop braucht mehrere manuelle Schritte, bevor gebaut werden kann: lokale Git-Konfiguration, Craft-Bootstrap, plattformspezifische Abhängigkeiten. Dieser Skill arbeitet den Leitfaden weitgehend automatisch ab und markiert klar, was aus technischen Gründen nicht automatisierbar ist (GUI-Installer, App-Store-Downloads, interaktive Konto-Einrichtung).

**Automatisierungsgrenze:** Read-only-Checks und rein repo-lokale Git-Konfiguration laufen automatisch. Alles, was System-Software installiert oder einen langen Download/Build auslöst, wird pro Schritt vorgeschlagen und **erst nach Bestätigung** ausgeführt — kein unbeaufsichtigtes `sudo`/Systeminstaller/Craft-Dependency-Build.

## Ablauf

1. **Plattform ermitteln** aus der Umgebungsangabe der Session (win32/darwin/linux) — nicht nochmal per Shell-Befehl raten.

2. **Voraussetzungen-Check (read-only, immer automatisch):**
   - `git --version`, `python3 --version` (bzw. `python --version` auf Windows)
   - Windows: Visual Studio Build Tools vorhanden? (`vswhere` falls verfügbar, sonst `Get-ChildItem "C:\Program Files (x86)\Microsoft Visual Studio\2022"` o.ä.), `$env:WindowsSDKVersion`, Windows Developer Mode Status (`Get-ItemProperty HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\AppModelUnlock -Name AllowDevelopmentWithoutDevLicense` falls lesbar), Inkscape vorhanden (`Get-Command inkscape -ErrorAction SilentlyContinue` bzw. `Test-Path "C:\Program Files\Inkscape\bin\inkscape.exe"`) — wird zur Icon-Generierung gebraucht, `cmake`-Konfiguration bricht sonst mit `FATAL_ERROR` ab; png2ico vorhanden (`Get-Command png2ico -ErrorAction SilentlyContinue`, erst nach Craft-Setup sinnvoll prüfbar, siehe Schritt 4)
   - macOS: `xcode-select -p` (Command Line Tools), `xcodebuild -version` (volles Xcode), `brew --version`
   - Linux: `dpkg -l | grep -E 'build-essential|cmake|ninja-build'` (Kernpakete aus dem Linux-Build-Runbook, Abschnitt 3.1)
   - Craft vorhanden: `Test-Path C:\Craft64` oder `Test-Path C:\CraftRoot` (Windows, beide Pfade sind laut Confluence gültige Installationsziele) bzw. `test -d ~/Craft64` (macOS) bzw. `test -d ~/linux-gcc-x86_64` (Linux)
   - Ergebnis als Checkliste (✅/❌ pro Punkt) ausgeben, bevor der nächste Schritt beginnt.

3. **Lokale Git-Konfiguration (automatisch, ohne Rückfrage — repo-lokal, reversibel, keine Systemänderung):**
   ```
   git config core.hooksPath .githooks
   git config merge.nc-take-incoming.driver "cp -- '%B' '%A'"
   ```
   Auf macOS/Linux zusätzlich, falls die Hook-Dateien nicht ausführbar sind (`ls -l .githooks/post-merge` prüfen):
   ```
   chmod +x .githooks/post-commit .githooks/post-merge .githooks/prepare-commit-msg .githooks/lib-translation-merge.sh
   ```
   Hintergrund und aktueller Stand: Confluence "Git Hooks" (`https://bmjira.atlassian.net/wiki/spaces/Strato/pages/874741773`) — bei Widerspruch zwischen dort und hier gilt Confluence, dieser Abschnitt ist dann zu aktualisieren.

4. **Plattform-spezifisches Craft-Setup** — je nach Schritt 1 nur den passenden Abschnitt ausführen:

   **Windows** (Quelle: Confluence "Craft Installation (Windows)"):
   - Prüfen/hinweisen: Windows Developer Modus aktiv? Falls nicht, Nutzer bitten, ihn manuell in den Windows-Einstellungen zu aktivieren (kein Automatisierungspfad ohne Admin-Registry-Eingriff, den dieser Skill nicht selbstständig vornimmt).
   - VS 2022 Build Tools fehlt? → Download-Link nennen (`https://my.visualstudio.com/Downloads?q=visual%20studio%202022`), NICHT automatisch installieren (Workload-Auswahl "Desktop development with C++" ist interaktiv).
   - Inkscape fehlt? Wird zur Icon-Generierung gebraucht (siehe Confluence "Windows", Abschnitt "Weitere benötigte Tools") und ist kein Craft-Paket — nach Bestätigung:
     ```powershell
     winget install -e --id Inkscape.Inkscape --source winget
     ```
   - Craft-Bootstrap nur nach Bestätigung:
     ```powershell
     iex ((new-object net.webclient).DownloadString('https://raw.githubusercontent.com/KDE/craft/master/setup/install_craft.ps1'))
     ```
     Bei der interaktiven Installation: Pfad `C:\Craft64`, "Support for colored log" eher deaktivieren.
   - Danach, jeweils nach Bestätigung:
     ```powershell
     cd C:\Craft64
     .craft\craftenv.ps1
     craft --add-blueprint-repository [git]https://github.com/nextcloud/desktop-client-blueprints.git
     craft craft
     ```
   - **Qt-Version fixieren, bevor Dependencies gebaut werden:** Ohne Vorgabe zieht Craft die im Blueprint-Repo aktuell hinterlegte Qt6-Version, die von der für diesen Fork vorgesehenen abweichen kann. Maßgeblich ist `craftmaster.ini` (Repo-Root, Abschnitt `[BlueprintSettings]`, Schlüssel `libs/qt6.version`) — den Wert dort live nachschlagen, nicht aus diesem Skill-Text übernehmen, da er sich mit dem Nextcloud-Upstream ändert. Nach Bestätigung in `C:\Craft64\etc\CraftSettings.ini` ergänzen (Abschnitt/Datei ggf. neu anlegen):
     ```ini
     [BlueprintSettings]
     libs/qt6.version = <aktueller Wert aus craftmaster.ini>
     ```
     Optional zusätzlich die übrigen in `craftmaster.ini` unter `[BlueprintSettings]` gepinnten Werte (`libs/zlib.version`, `libs/openssl.version`, `craft/craft-blueprints-kde.revision`) übernehmen, wenn reproduzierbare Ergebnisse wichtiger sind als der schnellste Weg.
   - Danach:
     ```powershell
     craft --install-deps nextcloud-client
     ```
   - Nach der Installation die tatsächlich gebaute Qt-Version verifizieren (z.B. `& "C:\Craft64\bin\qmake6.exe" --version`, Pfad je nach Craft-Layout anpassen) und mit dem Wert aus `craftmaster.ini` abgleichen — bei Abweichung nicht einfach weiterbauen, sondern mit dem Nutzer klären.
   - Danach prüfen: `Get-Command png2ico -ErrorAction SilentlyContinue` (siehe Confluence "Windows", Abschnitt "Weitere benötigte Tools"). Sollte als Blueprint-Abhängigkeit von `nextcloud-client` mit installiert worden und über `.craft\craftenv.ps1` im PATH sein — falls trotzdem nicht gefunden, keinen automatischen Installationsweg annehmen, sondern auf `cmake/modules/FindPng2Ico.cmake` (Bezugsquellen winterdrache.de/kdewin) verweisen und Nutzer fragen, wie weiter vorgegangen werden soll.
   - Bekannter Stolperstein (Windows SDK): siehe Confluence-Seite, Abschnitt "Trouble Shooting" — bei `LNK2001 guard_check_icall` `$env:WindowsSDKVersion` auf ein installiertes SDK (`ls "C:\Program Files (x86)\Windows Kits\10\Lib"`) umbiegen, nicht blind versuchen zu reparieren, sondern die dort dokumentierten 4 Variablen (`WindowsSDKVersion`, `INCLUDE`, `LIB`, `LIBPATH`) einzeln zeigen und Anpassung bestätigen lassen.
   - Nächster Schritt danach: Confluence "HiDrive Next mit Visual Studio Code" (`604996038`).

   **macOS** (Quelle: Confluence "Craft Installation (macOS)"):
   - AppleId + Xcode aus dem App Store: rein manuell, nur als Checklistenpunkt abfragen ("Xcode installiert und einmal gestartet, AGB akzeptiert?"), nicht automatisierbar.
   - macOS SDK (aktuell Version 12, Stand siehe Confluence-Seite — vor Verwendung dort den aktuell geforderten Wert gegenprüfen, ändert sich mit Nextcloud-Upstream-Version) von `https://github.com/phracker/MacOSX-SDKs/releases` laden und nach `/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs` entpacken — Download/Entpacken nach Bestätigung ausführen, Zielversion vorher explizit nennen statt fest zu verdrahten.
   - Homebrew-Installation (`https://brew.sh`) nur anstoßen, wenn `brew --version` fehlschlägt, und vorher fragen — der Installer ist interaktiv (Sudo-Passwort, PATH-Hinweis am Ende, den der Nutzer selbst ausführen muss).
   - Nach Bestätigung:
     ```bash
     brew install git glib cmocka inkscape cmake wget ccmake ninja
     curl https://raw.githubusercontent.com/KDE/craft/master/setup/CraftBootstrap.py -o setup.py && python3 setup.py --prefix ~/Craft64
     source ~/Craft64/craft/craftenv.sh
     export MAC_DEVELOPMENT_TARGET=12.0
     craft --add-blueprint-repository https://github.com/nextcloud/desktop-client-blueprints.git
     craft craft
     ```
   - **Qt-Version fixieren** wie im Windows-Abschnitt beschrieben: aktuellen Wert aus `craftmaster.ini` (`[BlueprintSettings]`, `libs/qt6.version`) live nachschlagen und nach Bestätigung in `~/Craft64/etc/CraftSettings.ini` unter `[BlueprintSettings]` eintragen, bevor `craft --install-deps` läuft.
   - Danach:
     ```bash
     craft --install-deps nextcloud-client
     ```
   - Nach der Installation verifizieren (z.B. `~/Craft64/bin/qmake6 --version`) und mit `craftmaster.ini` abgleichen.

   **Linux** (Quelle: Confluence "Linux-Build-Runbook für nc-desktop" — Stand 14.08.2026, als internes Runbook mit "Offene Punkte" markiert, nicht als vollständig verifizierter Standardweg):
   - Nur relevant, wenn tatsächlich auf einer Linux-Maschine/VM gearbeitet wird — nicht versuchen, unter Windows/macOS eine Linux-Umgebung zu simulieren.
   - Vor dem Klonen auf der Linux-Maschine: aktueller Feature-Branch muss auf `origin` gepusht sein (siehe Runbook, Problem #3) — sonst schlägt `git checkout` dort fehl.
   - Pakete (nach Bestätigung, braucht `sudo`):
     ```bash
     sudo apt update
     sudo apt install -y git python3 python3-pip build-essential cmake ninja-build patch
     sudo apt install -y libgl1-mesa-dev libglu1-mesa-dev libglvnd-dev mesa-common-dev \
       libxkbcommon-dev libxkbcommon-x11-0 libx11-dev libx11-xcb1 libxext-dev libxfixes-dev \
       libxi-dev libxrender-dev libxcb1-dev libxcb-cursor0 libxcb-glx0 libxcb-keysyms1 \
       libxcb-image0 libxcb-shm0 libxcb-icccm4 libxcb-sync1 libxcb-xfixes0 libxcb-shape0 \
       libxcb-randr0 libxcb-render-util0 libxcb-util1 libxcb-xinerama0 libxcb-xkb1 \
       libfontconfig1-dev libfreetype-dev libdbus-1-dev libssl-dev
     sudo apt install -y librsvg2-bin
     ```
   - Qt-Version: hier kein separater Fixierungsschritt nötig — `--config ~/nc-desktop/craftmaster.ini` unten übergibt dessen `[BlueprintSettings]` (inkl. `libs/qt6.version`) direkt an Craft.
   - Craftmaster-Setup (nach Bestätigung, Ziel-Branch/ABI vorher mit dem Nutzer abgleichen statt den im Runbook genannten Beispiel-Branch blind zu übernehmen):
     ```bash
     git clone --depth=1 https://invent.kde.org/packaging/craftmaster.git ~/craftmaster
     craft() {
         python3 ~/craftmaster/CraftMaster.py --config ~/nc-desktop/craftmaster.ini --target linux-gcc-x86_64 -c "$@"
     }
     grep -A3 'linux-gcc-x86_64' craftmaster.ini   # verifizieren: General/ABI = linux-gcc-x86_64
     craft --add-blueprint-repository "https://github.com/nextcloud/craft-blueprints-kde.git|stable-33.0|"
     craft --add-blueprint-repository "https://github.com/nextcloud/desktop-client-blueprints.git|stable-33.0|"
     craft craft
     craft --install-deps nextcloud-client
     ```
   - `.vscode/settings.json`/`launch.json` enthalten eingecheckte Windows-Pfade — lokal auf `~/linux-gcc-x86_64` umbiegen und `git update-index --skip-worktree .vscode/settings.json .vscode/launch.json` setzen, NICHT committen (Runbook, Problem #8/#9).
   - Bekannte Probleme/Fixes: siehe Runbook-Tabelle "4. Bekannte Probleme & Lösungen" — bei einem der dort gelisteten Fehlerbilder direkt auf den passenden Tabelleneintrag verweisen statt neu zu debuggen.

5. **VS Code Extensions installieren (nach Bestätigung, plattformübergreifend):**
   - Vorhandene prüfen: `code --list-extensions --show-versions`
   - Fehlende der folgenden nach Bestätigung installieren:
     ```
     code --install-extension ms-vscode.cpptools
     code --install-extension ms-vscode.cpptools-extension-pack
     code --install-extension ms-vscode.cpptools-themes
     code --install-extension ms-vscode.cmake-tools
     code --install-extension ms-vscode.cpp-devtools
     ```
   - Weiterführende Konfiguration (CMake-Kit, Compiler-Pfad) siehe Confluence "HiDrive Next mit Visual Studio Code" (`604996038`) — hier nur Installation der Extensions, keine Workspace-Konfiguration.

6. **Abschluss:** Zusammenfassen was automatisch erledigt wurde, was der Nutzer selbst bestätigt/ausgeführt hat, und was noch manuell offen ist (z.B. Xcode-Erststart, Windows Developer Modus, AppleId). Für den eigentlichen Build/Start danach auf den Skill `run` bzw. die Confluence-Seite "HiDrive Next mit Visual Studio Code" verweisen — nicht selbst bauen oder die App starten.

## Abgrenzung

- **run**-Skill (falls vorhanden/aufgerufen): fürs tatsächliche Bauen/Starten der bereits eingerichteten App — dieser Skill endet, sobald `craft --install-deps` durchgelaufen ist.
- **Confluence "Setup: KI-gestützt oder manuell" / "Git Hooks" / "Craft Installation (Windows/macOS)" / "Linux-Build-Runbook"**: die kanonische, von Menschen gepflegte Doku. Dieser Skill ist eine ausführbare Kopie ihrer Kernschritte für den Alltag — bei Abweichung gewinnt Confluence, und dieser Skill muss nachgezogen werden (kein Live-Abruf aus Confluence zur Laufzeit, da der MCP-Zugang nicht in jeder Session verfügbar ist).

## Nicht in diesem Skill enthalten

- Keine automatischen Installationen von Systemsoftware (Visual Studio Build Tools, Xcode, Homebrew selbst, apt-Pakete) ohne vorherige Bestätigung des jeweiligen Schritts.
- Kein automatisches Aktivieren des Windows Developer Modus (Registry-/Admin-Eingriff) — nur Statusprüfung und Hinweis.
- Keine Linux-VM-Erstellung — setzt eine bereits existierende Linux-Maschine voraus.
- Kein automatisches Bauen oder Starten der App nach Abschluss des Setups (siehe Abgrenzung).
- Kein Live-Abgleich mit Confluence — die dort dokumentierten aktuellen Werte (z.B. geforderte macOS-SDK-Version, aktueller Blueprint-Branch) müssen bei Verdacht auf Drift manuell gegengeprüft werden.
