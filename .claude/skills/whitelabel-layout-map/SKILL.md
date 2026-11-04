---
name: whitelabel-layout-map
description: Pflegt eine Landkarte der STRATO-vs-IONOS-Sichtbarkeitsunterschiede für UI-Komponenten in LAYOUT_MAP.md. Findet `#ifdef IONOS_WL_BUILD`/`STRATO_WL_BUILD`-Stellen, an denen eine Komponente nur für eine Marke aus-/eingeblendet oder ganz anders aufgebaut wird, und markiert nicht dokumentierte Asymmetrien als klärungsbedürftig — Default-Annahme: eine ausgeblendete Komponente ist für beide Marken ausgeblendet, es sei denn es gibt eine erkennbare Begründung (Ticket-Kommentar, DECISIONS.md, bestätigte Produktentscheidung). Erkennt zusätzlich die bekannte Verwechslung von `IONOS_BUILD` (fork-weit, immer definiert, siehe CMakeLists.txt:9) mit `IONOS_WL_BUILD` (markenspezifisch) als eigene Fehlerklasse. Nur manuell auslösen. Trigger — "prüfe die Whitelabel-Layouts", "sind Strato und IONOS im UI gleich aufgebaut", "fehlt bei IONOS/Strato eine Komponente", "Whitelabel-Sichtbarkeits-Check", "ist dieses Ausblenden für beide Marken gedacht", nach Änderungen an `#ifdef IONOS_WL_BUILD`/`STRATO_WL_BUILD`-Blöcken.
---

# Whitelabel-Layout-Map

Der Client wird für zwei Marken gebaut — STRATO und IONOS (`IONOS.cmake`, Compile-Definitions `STRATO_WL_BUILD`/`IONOS_WL_BUILD`, ausgewertet u. a. in `whitelabeltheme.h`). Markenspezifische Unterschiede im Code sind meist gewollt (Logo, Farben, Link-Ziele) — aber grundsätzlich könnte ein `#ifdef STRATO_WL_BUILD`/`IONOS_WL_BUILD`-Block auch eine ganze Komponente nur für eine Marke aus-/einblenden. Ob so eine Asymmetrie eine bewusste Produktentscheidung oder ein Versehen ist, war bisher nie systematisch geprüft (Stand erster Vollscan 2026-09-10: siehe `LAYOUT_MAP.md` — aktuell keine gefunden, alle echten Marken-Unterschiede sind Inhalt/Logo/Link, keine Sichtbarkeit). Separat davon gibt es das bekannte, fork-weite `IONOS_BUILD`-Makro (immer definiert, siehe Makro-Familien unten) — Verwechslungen der beiden Familien sind die eigentliche Risikoquelle.

**Default-Regel (vom Nutzer festgelegt):** Eine ausgeblendete/entfernte Komponente gilt als für **beide** Marken ausgeblendet, sofern nichts anderes dokumentiert ist. Jede Stelle, an der eine Komponente nur für STRATO oder nur für IONOS sichtbar/aufgebaut ist, wird als klärungsbedürftig gemeldet — es sei denn ein Kommentar, `DECISIONS.md` oder eine vom Nutzer bestätigte Produktentscheidung belegt die Absicht.

## Drei Makro-Familien nicht verwechseln

1. **`STRATO_WL_BUILD` / `IONOS_WL_BUILD`** — die eigentliche Marken-Weiche, gegenseitig exklusiv gesetzt in `IONOS.cmake` je nach `APPLICATION_NAME`. Das ist die einzige Familie, die für diesen Skill "Marke A vs. Marke B" bedeutet.
2. **`IONOS_BUILD`** (ohne `_WL_`) — fork-weites Flag, in `CMakeLists.txt:9` bedingungslos für **beide** Marken gesetzt (`add_compile_definitions(IONOS_BUILD)`). Unterscheidet Fork vs. Upstream-Nextcloud, nicht Marke vs. Marke. Bekannte Fehlerklasse (siehe Memory `project_ionos-build-dead-branch-pattern`): der `#else`/`#elif`-Zweig von `#ifdef IONOS_BUILD` ist toter Code, weil das Flag nie fehlt. Wird dieses Flag fälschlich benutzt, um STRATO von IONOS zu unterscheiden, ist das Verhalten kaputt, nicht nur "Dead Code" — daher eigener Status unten.
3. **`APPLICATION_SERVER_URL_ENFORCE` u. ä. Compile-Konstanten** — reine Werte, keine Sichtbarkeits-Schalter, nicht Gegenstand dieses Skills.

## Datei in diesem Skill

- [LAYOUT_MAP.md](LAYOUT_MAP.md) — eine Zeile pro gefundener sichtbarkeits-/aufbaurelevanter `#ifdef`-Stelle: Komponente/Feature, Datei:Zeile, STRATO-Verhalten, IONOS-Verhalten, Status, Begründung/Referenz. Flache Tabelle, kein Verlauf über mehrere Prüfrunden — nur der aktuell festgestellte Zustand zählt (analog zu `color-scheme-map`/`merge-drift-map`). Zusätzlich "zuletzt geprüfter Commit", damit Folgeläufe nur die Differenz seit dem letzten Scan bewerten müssen.

## Ablauf

1. **Scope bestimmen.** Ohne genauere Nutzerangabe: Vollscan von `src/` nach den drei Makros (überschaubare Menge, ca. 30–50 Fundstellen repoweit, kein Grund für einen komponentenweisen Teilscan wie bei `color-scheme-map`). Bei Angabe eines Bereichs/einer Komponente durch den Nutzer nur dort suchen.
   - `grep -rn "IONOS_WL_BUILD\|STRATO_WL_BUILD" --include=*.cpp --include=*.h --include=*.cmake`
   - `grep -rn "\bIONOS_BUILD\b" --include=*.cpp --include=*.h` (separat, wegen Fehlerklasse 2 oben)
   - QML hat keinen Präprozessor — Marken-Unterschiede laufen dort ausschließlich über `WLTheme`/`Theme`-Properties (Farben/Icons), das ist Gegenstand von `color-scheme-map`, nicht dieses Skills. Nur prüfen, ob eine QML-Komponente über ein C++-exponiertes Property komplett aus-/eingeblendet wird (`visible: SomeExposedFlag`), falls ein solches Property im C++-Scan auffällt.

2. **Jede Fundstelle im Kontext lesen** (nicht nur die Grep-Zeile — mindestens die ganze `#ifdef`/`#else`/`#endif`-Struktur plus umgebende Funktion).

3. **Klassifizieren:**
   - **Reiner Inhaltsunterschied** (anderer Link/Text-Key/Icon-Pfad/Logo, Komponente existiert und ist in beiden Zweigen sichtbar) → **✅ Inhalt, kein Sichtbarkeits-Unterschied** — nicht weiter verfolgen, das ist normales Whitelabeling.
   - **Sichtbarkeits-/Existenz-Unterschied** (`hide()`, `setVisible(false)`, `->show()` nur in einem Zweig, unterschiedlicher Widget-Aufbau, Menüeintrag/Checkbox nur in einem Zweig erzeugt):
     - Beide Zweige blenden die Komponente aus bzw. keiner blendet sie ein → **✅ symmetrisch ausgeblendet**.
     - Beide Zweige zeigen sie (nur der Aufbau unterscheidet sich kosmetisch) → **✅ symmetrisch sichtbar**.
     - Nur eine Marke zeigt/hat die Komponente, die andere nicht:
       - Ticket-Kommentar (`// SES-...`), Eintrag in `../decision-log/DECISIONS.md` oder in `../shadow-component-watch/registry.md`, oder vom Nutzer im Gespräch bereits bestätigte Produktentscheidung vorhanden → **ℹ️ bewusst markenspezifisch**, mit Referenz in der Begründungs-Spalte.
       - Keine erkennbare Begründung → **⚠️ ungeklärte Asymmetrie** — das ist der Kernfall, den dieser Skill aufdecken soll.
   - **Verwendet bare `IONOS_BUILD`** (nicht `_WL_`):
     - Zweig/Kommentar spricht klar von Fork-vs-Upstream (z. B. Push-Notifications ersetzen Polling) → **🔲 fork-weit, kein Marken-Bezug** — informativ vermerken, nicht als Asymmetrie werten.
     - Zweig/Kommentar suggeriert, der Autor meinte tatsächlich "nur für IONOS" (z. B. Variablennamen, Kommentartext, oder die Logik ergibt nur Sinn, wenn sie markenspezifisch wäre) → **🔴 Makro-Verwechslung** (IONOS_BUILD statt IONOS_WL_BUILD) — hohe Priorität, das ist vermutlich ein echter Bug, kein Whitelabeling.

4. **`LAYOUT_MAP.md` schreiben/ergänzen.** Pro Fundstelle eine Zeile mit Status aus Schritt 3. "Zuletzt geprüfter Commit" auf aktuellen `HEAD` setzen.

5. **Ergebnis mit dem Nutzer besprechen — explizit alle ⚠️ und 🔴 Zeilen nennen**, das ist der eigentliche Zweck der Karte. 🔲- und ℹ️-Zeilen nur auf Nachfrage im Detail erläutern.

6. **Nicht automatisch fixen.** Nur melden/in der Karte vermerken; ein Fix (z. B. Sichtbarkeit angleichen, Makro korrigieren) erst nach Bestätigung durch den Nutzer als eigener Schritt.

7. **Bei Folgeläufen inkrementell vorgehen:** `git log <letzter-geprüfter-commit>..HEAD -- <betroffene Dateien>` prüfen, ob sich an den bekannten Fundstellen etwas geändert hat, plus neuen Grep-Lauf, um neu hinzugekommene `#ifdef`-Stellen zu erfassen.

## Abgrenzung

- **color-scheme-map**: Farb-/Icon-Unterschiede zwischen den Marken sind erwartet und dessen Gegenstand — dieser Skill interessiert sich nur dafür, ob eine Komponente als Ganzes nur für eine Marke existiert/sichtbar ist.
- **shadow-component-watch**: Fork-vs-Upstream-Ersatz (Nextcloud stable vs. eigene Entwicklung), unabhängig von der Marke — eine dort dokumentierte "bewusst entfernt"-Begründung (z. B. SES-4/SES-50) gilt für beide Marken gleichermaßen und ist kein Whitelabel-Fall, kann aber als Beleg für Schritt 3 herangezogen werden, falls sie zufällig denselben Code betrifft.
- **decision-log**: allgemeines Entscheidungsprotokoll — erste Anlaufstelle, um zu prüfen, ob eine gefundene Asymmetrie bereits dokumentiert ist, bevor sie als ⚠️ gemeldet wird.
- **component-context**: bündelt vorhandenen Kontext zu einer Komponente vor einer Änderung, liest aber nur — dieser Skill ist die einzige Quelle, die die Whitelabel-Sichtbarkeits-Landkarte selbst schreibt/pflegt.

## Nicht in diesem Skill enthalten

- Kein externes Lastenheft "diese Komponenten müssen vorhanden sein" — es existiert keine solche Spezifikation im Repo. "Erforderlich" wird operational über Symmetrie definiert: was für die eine Marke existiert, muss (mangels Gegenbeleg) auch für die andere existieren.
- Keine Bewertung von reinen Inhalts-/Text-/Icon-Unterschieden (das ist normales, erwartetes Whitelabeling) — nur Sichtbarkeit/Existenz einer Komponente als Ganzes.
- Kein automatisches Fixen gefundener Asymmetrien oder Makro-Verwechslungen ohne Bestätigung.
- Kein periodischer/automatischer Trigger — der Nutzer entscheidet, wann ein (Teil-)Scan läuft.
