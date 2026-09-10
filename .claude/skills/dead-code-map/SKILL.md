---
name: dead-code-map
description: Pflegt eine Landkarte in DEAD_CODE_MAP.md für unbenutzten Code, den dieser Fork selbst hinzugefügt hat (neue Dateien oder neue Symbole in bestehenden Dateien) — reposweit auf verbleibende Referenzen geprüft, mit Lösch-Empfehlung. Rührt bewusst nicht an Code, den Nextcloud pflegt (auch nicht an fork-seitigen Änderungen innerhalb bestehender Nextcloud-Funktionen). Wird bewusst Datei/Komponente für Komponente befüllt, kein Vollscan. Nur manuell auslösen. Trigger — "suche toten Code im Fork", "welche fork-eigenen Funktionen werden nicht mehr genutzt", "dead code map aktualisieren", "unused code check für <Datei/Komponente>", "ist <Funktion/Klasse/Datei> noch in Benutzung", "Codebasis aufräumen", "ungenutzten fork-Code finden".
---

# Dead-Code-Map

Dieses Repo ist ein Whitelabel-Fork von Nextcloud Desktop. Ziel dieses Skills: **fork-eigenen** toten Code finden und dauerhaft in einer Landkarte festhalten — ohne dabei an Code zu rühren, den Nextcloud pflegt. "Fork-eigen" ist dabei enger gefasst als "alles, was im Diff zu stable auftaucht":

- **Im Scope**: komplett neue Dateien (existieren nicht auf `origin/<branch>`) sowie komplett neue Symbole (Funktionen, Methoden, Klassen, QML-Typen, CMake-Targets), die der Fork zu einer bestehenden Nextcloud-Datei hinzugefügt hat.
- **Nicht im Scope**: Änderungen *innerhalb* einer bereits von Nextcloud existierenden Funktion/Methode. Auch wenn eine solche Änderung im Diff zu stable auftaucht, ist der Funktionskörper Nextcloud-Code — ob er "tot" wirkt, ist hier nicht zu bewerten, da eine Entfernung dort faktisch Nextcloud-Logik anfasst.

Diese Abgrenzung ist der Kern des Skills — im Zweifel (unklar, ob ein Symbol komplett neu oder nur modifiziert ist) lieber als "kein Kandidat" einstufen statt zu raten.

## Referenz-Branch

Wie [stable-merge-check](../stable-merge-check/SKILL.md): Branch aus dessen [reference-branch.txt](../stable-merge-check/reference-branch.txt) lesen, dem Nutzer nennen, vorher `git fetch origin <branch>` (rein lesend). Nicht duplizieren, nicht selbst raten.

## Datei

[DEAD_CODE_MAP.md](DEAD_CODE_MAP.md) — Legende einmal am Anfang, danach eine einzige, flache Tabelle: Symbol/Datei | Art (neue Datei / neues Symbol in bestehender Datei) | Fundort (Datei:Zeile) | Reposweite Referenzen | Einschätzung (Löschkandidat / behalten / unklar) | zuletzt geprüfter Commit | Bemerkung. **Kein Verlaufs-/Rundenprotokoll** — eine erneute Prüfung überschreibt die bestehende Zeile, kein "Runde 1/2/3" (wie [merge-drift-map](../merge-drift-map/SKILL.md)/[color-scheme-map](../color-scheme-map/SKILL.md)).

## Ablauf

1. **Referenz-Branch ansagen** (s.o.).

2. **Scope identifizieren.** Vom Nutzer übernehmen (Datei, Komponente, Verzeichnis). Bei vager Beschreibung wie bei [component-context](../component-context/SKILL.md) über `.claude/context/gui/**/CLAUDE.md`/`COMPONENTS.md` auflösen. Ohne Vorgabe: nicht das ganze Repo auf einmal scannen, sondern nachfragen, welcher Bereich zuerst dran ist.

3. **Prüfen, ob schon erfasst** (`Grep <Symbol/Datei> DEAD_CODE_MAP.md`) — vorhanden mit "zuletzt geprüft"-Commit → nur inkrementell nachziehen (`git log <commit>..HEAD -- <datei>`), keine neuen Commits → nichts tun. Noch nicht erfasst → Vollerfassung (Schritte 4–7).

4. **Kandidaten pro Datei klassifizieren:**
   - `git cat-file -e origin/<branch>:<datei>` schlägt fehl → ganze Datei ist fork-only, alle darin enthaltenen Symbole sind grundsätzlich Kandidaten.
   - Datei existiert auf stable → `git diff origin/<branch> -- <datei>` lesen und pro Hunk unterscheiden: komplett neue Funktion/Methode/Klasse (nur `+`-Zeilen, kein Bezug zu bestehendem Funktionskörper) vs. Änderung innerhalb einer auf stable existierenden Funktion (raus aus dem Scope, s.o.).
   - Bekannte fork-spezifische Muster mitprüfen, die oft tot sind: `#ifdef IONOS_BUILD`-Zweige (IONOS_BUILD ist projektweit immer definiert, siehe `CMakeLists.txt` — der `#else`/`#elif`-Zweig ist damit permanent unerreichbar; bereits zweimal in `settingsdialog.cpp` und `main.cpp` gefunden), Whitelabel-Asymmetrien aus [whitelabel-layout-map](../whitelabel-layout-map/SKILL.md).

5. **Reposweit auf Referenzen prüfen** (nicht nur im Diff oder in der einen Datei!): Grep nach Symbolname/Klassenname/QML-Typname über das ganze Repo, inkl.:
   - `CMakeLists.txt`/`.pro`/Ressourcendateien (Datei überhaupt noch eingebunden?)
   - `.ui`-Dateien, QML-Dateien (Typ noch instanziiert?)
   - String-basierte Qt-Verbindungen (`SIGNAL(...)`, `SLOT(...)`, `QMetaObject::invokeMethod` mit String-Namen, `Q_INVOKABLE`) — diese tauchen bei reiner Grep-nach-Funktionsaufruf-Suche leicht unter dem Radar durch.
   - Virtuelle Overrides (Funktion kann "unbenutzt" wirken, wird aber über Basisklassen-Zeiger aufgerufen).
   - Bei Unsicherheit, ob eine Referenz zählt: lieber "unklar" einstufen als fälschlich als Löschkandidat markieren.

6. **Löschkandidat nur, wenn zusätzlich:**
   - Reposweit keine Referenz gefunden (Schritt 5), UND
   - es sich um ein fork-eigenes Symbol/Datei handelt (Schritt 4), UND
   - bei GUI-Code: die Kriterien aus der Removal-Safety-Praxis erfüllt sind (kein von stable-x.y noch geführter struktureller Container, sondern ein Leaf-Widget/-Symbol ohne Weiterverwendung).
   - Sonst: "behalten" oder "unklar" mit kurzer Begründung.

7. **`DEAD_CODE_MAP.md` schreiben/aktualisieren** — neue Zeile anlegen oder bestehende überschreiben (nicht duplizieren), "zuletzt geprüft"-Commit aktualisieren.

8. **Ergebnis zusammenfassen, nicht selbst löschen.** Löschkandidaten dem Nutzer auflisten (nummeriert), jeweils mit der reposweiten Referenzprüfung als Beleg. Löschung erst nach expliziter Bestätigung durchführen — und dann Datei für Datei/Symbol für Symbol, nicht alle Kandidaten in einem Rutsch.

## Optionale ergänzende Tools

Statische Analyse (`cppcheck --enable=unusedFunction`, `clazy`) kann zusätzliche Hinweise liefern, ist in diesem Repo aber nicht eingerichtet (keine Konfiguration im Repo gefunden) und ersetzt nicht Schritt 5 — beide Tools kennen keine Qt-String-basierten Verbindungen und keine QML-Referenzen zuverlässig und produzieren dafür False Positives. Nur als Zusatzhinweis nutzen, nie als alleinige Grundlage für eine Löschempfehlung.

## Abgrenzung

- **shadow-component-watch**: beobachtet den umgekehrten Fall — eine *Nextcloud-Originaldatei*, die durch eine fork-eigene Ersatzkomponente funktional verwaist ist. Diese Originaldateien bleiben Nextcloud-Code und sind **kein** Kandidat für diesen Skill (nicht löschen, nur beobachten, ob sich stable dort noch weiterentwickelt).
- **whitelabel-layout-map**: liefert einen der Eingabe-Kandidatentypen (Whitelabel-Asymmetrien), pflegt aber keine Löschkandidaten-Landkarte.
- **merge-drift-map**: andere Dimension (Merge-Konfliktrisiko vs. toter Code), kein Zusammenhang.
- **stable-merge-check**: liefert den Referenz-Branch-Mechanismus und die Grep-vor-Löschung-Methodik, dieser Skill wendet sie speziell auf fork-eigene, unbenutzte Symbole an.

## Nicht in diesem Skill enthalten

- Keine Bewertung von Änderungen innerhalb bestehender Nextcloud-Funktionskörper (s. Scope oben).
- Kein automatisches Löschen — nur Kandidatenliste mit Beleg, Entfernung erst nach Bestätigung, dann einzeln.
- Kein Vollscan des gesamten Repos in einem Lauf.
- Keine Verlaufs-/Rundenhistorie in `DEAD_CODE_MAP.md` — nur der Ist-Zustand pro Symbol/Datei.
- Kein periodischer/automatischer Trigger.
