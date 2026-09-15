# src/gui/SesComponents

This folder holds small, fork-specific UI/logic components used across the GUI, kept separate from upstream Nextcloud code so they survive upstream merges with minimal conflicts. "Ses" is this team's internal ticket/component prefix (BRICKMAKERS work on the IONOS/HiDrive Next/STRATO whitelabel fork). All six files here are fork-specific — there is no upstream equivalent folder.

## Key classes/components

- `SesErrorBox.qml` — reusable inline error banner (icon + bold "Error" title + wrapped message text), styled via `Style.ses*` tokens; used e.g. in `filedetails/ShareDetailsPage.qml` to show password errors.
- `SesTrayHeader.qml` — custom tray-popup header bar (account menu button, "Website" link, local/team-folder button, optional "featured app"/assistant button); embedded into `tray/MainWindow.qml` as `trayWindowHeaderBackground`, replacing/restyling the upstream Nextcloud tray header row for the whitelabel look.
- `SesCheckBox.qml` (SES-597) — themed `CheckBox` override: square 20×20 indicator (checked = `accentColor` fill, unchecked = `Style.sesTrayInputField` border) with a white checkmark glyph, `Style.sesTrayFontColor` label text. `accentColor` is a property (default `Style.sesCheckboxAccentColor`) rather than reaching up to a parent page, so call sites pass it explicitly. Extracted out of a local `component` block in `filedetails/ShareDetailsPage.qml` (its only current caller) to cut that page's structural drift from upstream `stable-33.0`.
- `SesInputBackground.qml` (SES-597) — trivial bordered-`Rectangle` background (`Style.sesTrayInputField` border, `Style.sesBackgroundColor` fill, `z: -1`, fills parent) used behind text-entry fields. Extracted from three copy-pasted instances in `filedetails/ShareDetailsPage.qml` (its only current caller: password, expiry-date, note fields).
- `syncdirvalidation.h` / `syncdirvalidation.cpp` — `SyncDirValidator` class: on Windows, checks that a chosen sync directory is not the app's own `%AppData%` roaming path (to avoid users pointing sync at the app's own data folder); on other OSes it's a no-op always returning valid. Used by `folderwizard.cpp` and `wizard/owncloudadvancedsetuppage.cpp` during local-folder selection.

## How it fits together

The QML components are drop-in replacements/extensions for tray and sharing UI, pulled in via relative QML imports (`import "../tray/"` etc.) and the shared `Style` singleton for whitelabel theming; `SyncDirValidator` is an unrelated plain C++ helper reused by folder-setup wizards to validate a picked path. All are additive fork logic, not modifications of upstream files. `SesCheckBox`/`SesInputBackground` are registered as QML types the same way as `SesErrorBox`/`SesTrayHeader` (via `ionos.qrc`).

## Fork-specific notes

- Entire folder is fork-only (BRICKMAKERS/IONOS-HiDrive Next fork), no upstream Nextcloud equivalent directory exists.
- `SesTrayHeader.qml` specifically substitutes for/duplicates parts of what would otherwise be inline header markup in upstream `tray/MainWindow.qml`.
- `SesCheckBox.qml`/`SesInputBackground.qml` are the newest additions (SES-597): both existed only as inline, duplicated markup inside `filedetails/ShareDetailsPage.qml` until extracted here specifically to reduce that page's ~99% line-level drift from upstream `stable-33.0` and ease future merges — a pattern worth repeating for other heavily-drifted pages.

*Quelle: src/gui/SesComponents — Stand 2026-09-10, automatisch erstellt, bitte gegenlesen.*
