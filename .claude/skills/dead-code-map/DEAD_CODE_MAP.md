# Dead-Code-Map

Landkarte für fork-eigenen, reposweit unreferenzierten Code (neue Dateien oder neue Symbole in bestehenden Nextcloud-Dateien). Siehe [SKILL.md](SKILL.md) für Scope-Abgrenzung und Ablauf. Referenz-Branch: siehe [stable-merge-check/reference-branch.txt](../stable-merge-check/reference-branch.txt).

**Einschätzung**: Löschkandidat (reposweit keine Referenz gefunden) / behalten (noch referenziert oder aus anderem Grund bewusst erhalten) / unklar (Referenzlage nicht eindeutig klärbar).

Eine erneute Prüfung überschreibt die bestehende Zeile des Symbols/der Datei — kein Runden-/Verlaufsprotokoll.

| Symbol/Datei | Art | Fundort | Reposweite Referenzen | Einschätzung | Zuletzt geprüft (Commit) | Bemerkung |
|---|---|---|---|---|---|---|
| `src/gui/SesComponents/SesErrorBox.qml` | neue Datei (fork-only) | SesComponents/ | `ShareDetailsPage.qml`, `ShareView.qml`, `ionos.qrc` | behalten | `74d5bba25` | |
| `src/gui/SesComponents/SesTrayHeader.qml` | neue Datei (fork-only) | SesComponents/ | `tray/MainWindow.qml:284`, `ionos.qrc` | behalten | `74d5bba25` | Ersetzt upstream `TrayWindowHeader.qml` (siehe shadow-component-watch) |
| `src/gui/SesComponents/SesCheckBox.qml` | neue Datei (fork-only) | SesComponents/ | 10× in `filedetails/ShareDetailsPage.qml`, `ionos.qrc` | behalten | `74d5bba25` | Fehlt noch in `.claude/context/gui/SesComponents/CLAUDE.md` (Doku veraltet) |
| `src/gui/SesComponents/SesInputBackground.qml` | neue Datei (fork-only) | SesComponents/ | 3× in `filedetails/ShareDetailsPage.qml`, `ionos.qrc` | behalten | `74d5bba25` | Fehlt noch in `.claude/context/gui/SesComponents/CLAUDE.md` (Doku veraltet) |
| `SyncDirValidator` (`src/gui/SesComponents/syncdirvalidation.{h,cpp}`) | neue Datei + Klasse (fork-only) | SesComponents/ | `folderwizard.cpp:108,151`, `wizard/owncloudadvancedsetuppage.cpp:520`, `CMakeLists.txt:308-309` | behalten | `74d5bba25` | |
