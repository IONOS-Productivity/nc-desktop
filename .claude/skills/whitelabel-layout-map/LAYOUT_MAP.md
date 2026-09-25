# Whitelabel-Sichtbarkeits-Landkarte (STRATO vs. IONOS)

Wird vom [whitelabel-layout-map](SKILL.md)-Skill befüllt. Eine Zeile pro Fundstelle, an der eine UI-Komponente markenabhängig aus-/eingeblendet oder unterschiedlich aufgebaut wird. Reine Inhalts-/Farb-/Icon-Unterschiede (erwartetes Whitelabeling) werden nur kurz gelistet, nicht tief bewertet — siehe `color-scheme-map` dafür.

*Zuletzt geprüfter Commit (repoweit): `74d5bba25` (2026-09-10, inkrementell — seit `efb0b59e1` keine Änderungen an relevanten Dateien; Hinweis: `develop_stable-33.0` wurde zwischenzeitlich versehentlich per `git reset --hard` von `3c766d46b` auf diesen Stand zurückgesetzt, siehe Session-Verlauf)*

## Ergebnis auf einen Blick

**Keine ungeklärte STRATO-vs-IONOS-Sichtbarkeits-Asymmetrie gefunden.** Alle 13 echten Marken-Unterschiede (`STRATO_WL_BUILD`/`IONOS_WL_BUILD`) sind reine Inhaltsunterschiede (Link-Ziel, Logo-Datei, Übersetzungstext) — die dahinterliegende Komponente existiert und ist in beiden Marken gleich sichtbar. Das ursprünglich als Beispiel vermutete `folderwizard.cpp`-Verhalten stellte sich beim genauen Lesen als der separate `IONOS_BUILD`-Dead-Branch-Fall heraus (siehe unten), nicht als Marken-Unterschied.

## A. Marken-Unterschiede (`STRATO_WL_BUILD`/`IONOS_WL_BUILD`) — alle ✅ Inhalt, keine Sichtbarkeits-Asymmetrie

| Komponente/Feature | Datei:Zeile | STRATO | IONOS | Status |
|---|---|---|---|---|
| WLTheme-Instanz (Farben/Icons global) | `whitelabeltheme.h:13` | `StratoTheme` | `IonosTheme` | ✅ Grundmechanismus, kein Einzelfall |
| "Expand memory"-Link | `accountsettings.cpp:1674` | `ExpandMemory-Link_STRATO` | `ExpandMemory-Link` | ✅ Inhalt |
| Login-URL-Override beim Start | `application.cpp:379` | `Login_URL_STRATO` | `Login_URL` | ✅ Inhalt (Block selbst nur unter fork-weitem `IONOS_BUILD`, siehe B) |
| "Usage documentation"-Link | `generalsettings.cpp:281` | `Help-Link_STRATO` | `Help-Link` | ✅ Inhalt |
| "More information"-Link | `generalsettings.cpp:388` | `MoreInformation-Link_STRATO` | `MoreInformation-Link` | ✅ Inhalt |
| "Legal notice"-Link | `generalsettings.cpp:397` | `LegalNotice-Link_STRATO` | `LegalNotice-Link` | ✅ Inhalt |
| "Open source"-Link | `generalsettings.cpp:406` | `OpenSource-Link_STRATO` | `OpenSource-Link` | ✅ Inhalt |
| "Privacy"-Link | `generalsettings.cpp:415` | `Privacy-Link_STRATO` | `Privacy-Link` | ✅ Inhalt |
| Tray-Menü "Help"-Link | `owncloudgui.cpp:758` | `Help-Link_STRATO` | `Help-Link` | ✅ Inhalt |
| Data-Protection-Seite: Logo | `wizard/dataprotectionpage.cpp:46-50` | `strato-data-protection-logo.png` | `ionos-data-protection-logo.png` | ✅ Inhalt — Seite selbst für beide Marken sichtbar (siehe B, `flow2authcredspage.cpp:104`) |
| Data-Protection-Seite: Text | `wizard/dataprotectionpage.cpp:53` | Text mit `_STRATO`-Suffix | Standardtext | ✅ Inhalt |
| Externer-Login-Widget: Logo | `wizard/flow2authwidget.cpp:54-57` | `strato/ses-external.svg` | `ses-external.svg` | ✅ Inhalt |
| Wizard-Slideshow: Logo | `wizard/welcomepage.cpp:52-55` | `STRATO_logo_w_suffix_frontend.png` | `IONOS_logo_w_suffix_frontend.png` | ✅ Inhalt |

**Kleiner Stil-Fund (kein Verhaltensfehler):** In `dataprotectionpage.cpp:48`, `flow2authwidget.cpp:56` und `welcomepage.cpp:54` steht `#else defined(STRATO_WL_BUILD)` statt `#else`. Der Präprozessor ignoriert den Ausdruck nach `#else` und behandelt es wie ein normales `#else` — funktional unauffällig, aber irreführend zu lesen (sieht aus wie eine Bedingung, ist keine).

## B. Fork-weites `IONOS_BUILD` (immer definiert, `CMakeLists.txt:9`) — kein Marken-Bezug

Kein Fall deutet auf eine echte STRATO/IONOS-Verwechslung hin (🔴 trat nicht auf). Zwei Unter-Kategorien:

### B1. `#ifdef IONOS_BUILD` — Zweig läuft für beide Marken gleich (🔲 symmetrisch, `#else` ist toter Code)

| Stelle | Was |
|---|---|
| `application.cpp:375-391` | Login-URL-Override aus Übersetzung (enthält den Marken-Unterschied aus A) |
| `application.cpp:473-486` | `sesStyle`-Basis-Style (OS-Versions-abhängig) statt `QProxyStyle(nullptr)` |
| `folderstatusmodel.cpp:292` | Folder-Status-Icon aus `WLTheme` statt Standard-Theme |
| `folderwizard.cpp:728-730` | `setupVirtualFilesCheckbox()` statt inline erzeugter `QCheckBox` (der `#else`-Zweig, ursprünglich als mögliches Beispiel vermutet, ist toter Code — betrifft nicht STRATO, sondern ist unerreichbar) |
| `generalsettings.cpp:923-928` | `monoIconsCheckBox`/`callNotificationsCheckBox`/`advancedGroupBox`/`updates_frame` ausgeblendet — Kommentar "SES-4 removed", für beide Marken gleich |
| `main.cpp:113-126` | QQuickStyle-Setup; bereits als Dead-Branch-Fall in Memory `project_ionos-build-dead-branch-pattern` dokumentiert (Style-Auswahl wurde deshalb nach `application.cpp` verlegt) |
| `owncloudgui.cpp:297-479` (5 Stellen) | Tray-Icons aus `WLTheme` statt Standard-`Theme`-Icons |
| `wizard/flow2authcredspage.cpp:104-107` | Wizard-Folgeseite `Page_DataProtection` statt `Page_AdvancedSetup` — Kommentar `//TODO SES-459 Check why actually removed` direkt daneben, ungeklärt aber nicht markenspezifisch |

### B2. `#ifndef IONOS_BUILD` — Zweig ist für **beide** Marken toter Code (🔲, bekannter Anti-Pattern, kein Handlungsbedarf durch diesen Skill)

| Stelle | Was wäre (ist aber inaktiv) |
|---|---|
| `accountsettings.cpp:1332-1339` | Weiß-auf-Rot-Fehlerstil für `connectLabel` |
| `generalsettings.cpp:457-460` | `remotePollIntervalSpinBox`-Wert setzen + `updatePollIntervalVisibility()` aufrufen |
| `generalsettings.cpp:964-966` | `horizontalLayoutWidget_remotePollInterval` je nach Push-Verfügbarkeit aus-/einblenden — **da diese Zeile nie läuft, bleibt die Zeile permanent auf ihrer `.ui`-Default-Sichtbarkeit stehen, unabhängig von Push-Verfügbarkeit** |
| `main.cpp:96-106` | Universal-Theme-Env-Var für Windows < 11 |
| `settingsdialog.cpp:193-199` | Spacer zwischen Toolbar-Icon-Gruppen |
| `socketapi/socketapi.cpp:1352-1356` | Explorer-Kontextmenü-Eintrag "Activity" |
| `wizard/owncloudsetuppage.cpp:88-90, 459-461` | `setLogo()` (Wizard-Logo) |
| `wizard/owncloudsetuppage.cpp:316-320` | Freundlicherer Fehlertext bei ungültiger Server-Adresse (`setErrorString` statt `setConnectionError`) |
| `wizard/owncloudsetuppage.cpp:360-...` | Dialog "wie mit fehlerhaftem HTTPS-Zertifikat umgehen" — Kommentar "SES-84: Should only be shown in the Managed Cloud Client" |
| `libsync/theme.cpp:452-454` | `APPLICATION_SERVER_URL`-Override |

**Auffällig, aber kein Marken-Fall:** `libsync/theme.cpp:286` — `#if !(defined(IONOS_BUILD) && defined(IONOS_BUILD))` prüft dasselbe Makro doppelt (wirkungsgleich zu `!defined(IONOS_BUILD)`, also ebenfalls dauerhaft tot). Sieht nach einem Copy-Paste-Rest aus, bei dem die zweite Bedingung ursprünglich etwas anderes prüfen sollte. Unabhängig von diesem Skill meldenswert.

**Ebenfalls auffällig:** `main.cpp:118` — `#elif` ohne Bedingung statt `#else`. Gehört zum bereits in Memory dokumentierten Dead-Branch-Fall an derselben Stelle.

## Format (für Folgeläufe)

| Komponente/Feature | Datei:Zeile | STRATO | IONOS | Status | Begründung/Referenz |
|---|---|---|---|---|---|

**Status-Werte:** ✅ symmetrisch/Inhalt · ℹ️ bewusst markenspezifisch (mit Referenz) · ⚠️ ungeklärte Asymmetrie · 🔲 fork-weit (`IONOS_BUILD`), kein Marken-Bezug · 🔴 Makro-Verwechslung (`IONOS_BUILD` statt `IONOS_WL_BUILD`)
