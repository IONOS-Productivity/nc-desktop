---
name: translation-diff-merge
description: Sortiert und vergleicht eine oder mehrere Kandidaten-Übersetzungsdateien (z. B. Phrase-Export oder Downloads-Datei wie "de_DE(3).ts") gegen die zugehörige translation_scripts/<lang>.ts im Repo, liefert eine strukturierte Auswertung (neu/entfernt/geändert je Kontext+Source-Key) und führt den eigentlichen Merge in die Basis-Datei erst nach expliziter Bestätigung durch den Nutzer aus. Nutzt translation_scripts/diff_tools.py (sort/compare/merge). Trigger — "vergleiche diese Übersetzungsdiff-Datei", "sortiere und vergleiche de_DE(n).ts", "merge die Diff-Datei", "neue Übersetzungsdatei prüfen", "Diff-Datei gegen unsere de_DE.ts abgleichen", Nutzer legt eine oder mehrere heruntergeladene *.ts-Dateien vor und will wissen, was sich ändert bevor gemergt wird.
---

# Translation-Diff-Merge

Automatisiert den Review-Workflow für die hand-gepflegten Diff-Dateien unter `translation_scripts/<lang>.ts` (de_DE.ts, en.ts, en_GB.ts, es.ts, es_MX.ts, fr.ts, it.ts, nl.ts, sv.ts). Diese Dateien sind die Basis, die in Schritt 3 von `merge_translation.py` in die eigentlichen `translations/client_*.ts` gemergt wird (siehe [Readme.md](../../../translation_scripts/Readme.md)) — Fehler oder unbeabsichtigte Löschungen hier pflanzen sich dorthin fort.

Werkzeug ist ausschließlich [translation_scripts/diff_tools.py](../../../translation_scripts/diff_tools.py) (`sort` / `compare` / `merge`) — **nicht** `sort.py` direkt. `sort.py`s `repair_file()`-Schritt schreibt rohe `"`/`'` in `&quot;`/`&apos;` um (Stil von `lupdate`-Output); bei diesen handgepflegten Diff-Dateien erzeugt das nur unnötiges Escaping-Rauschen im Vergleich, siehe Docstring in `diff_tools.py`.

Zweistufiger Ablauf: **erst sortieren+vergleichen+auswerten, dann — nur nach ausdrücklicher Bestätigung — mergen.** Nie eigenständig von Stufe 1 zu Stufe 2 übergehen, auch wenn der Befund unauffällig aussieht.

## Stufe 1 — Sortieren & Vergleichen

Gilt pro Kandidaten-Datei; bei mehreren vom Nutzer übergebenen Dateien alle Dateien durchlaufen, bevor um Bestätigung gebeten wird.

1. **Kandidat lokalisieren.** Pfad kommt vom Nutzer (Chat-Text, IDE-Selection/offene Datei, Downloads-Ordner). Bei relativer/unklarer Angabe nachfragen statt zu raten.

2. **Sprache ableiten und Basis-Datei finden.** Aus dem Dateinamen den Sprachcode extrahieren — Windows-Downloads hängen oft `(1)`, `(2)`, ` (3)` etc. vor der Endung an (z. B. `de_DE(3).ts` → `de_DE`). Den Code gegen die vorhandenen Basisdateien in `translation_scripts/` matchen (`Glob` auf `translation_scripts/*.ts`). Kein eindeutiger Treffer → Nutzer fragen, welche Basis-Datei gemeint ist. **Nie eine Datei ohne passende Basis kommentarlos überspringen** — auch das gehört in die Auswertung.

3. **Kandidat sortieren** (in-place, kanonische Reihenfolge):
   ```
   python translation_scripts/diff_tools.py sort "<candidate_file>"
   ```

4. **Vergleichen:**
   ```
   python translation_scripts/diff_tools.py compare "translation_scripts/<lang>.ts" "<candidate_file>"
   ```
   Das Skript vergleicht per (Kontext, Source-Text)-Schlüssel, unabhängig von der Position in der Datei — reine Verschiebungen zwischen Kontext-Blöcken tauchen dadurch nicht fälschlich als Änderung auf (siehe frühere Analyse in diesem Chat, wo genau das den naiven `git diff` verrauscht hatte).

5. **Rohe Skript-Ausgabe nicht einfach durchreichen** — für den Nutzer aufbereiten:
   - **Neu**: Keys nur im Kandidaten → kommen beim Merge hinzu.
   - **Entfernt**: Keys, die im Kandidaten fehlen, in der Basis aber existieren → **würden beim Merge aus der Basis gelöscht** (siehe `merge`-Semantik unten). Das ist der risikoreichste Befund — immer explizit hervorheben, besonders wenn mehrere entfernte Keys denselben Kontextnamen teilen (deutet oft auf einen unvollständigen Export hin, nicht auf eine gewollte Löschung).
   - **Geändert**: gleicher Key, anderer Übersetzungstext — alt/neu gegenüberstellen. Wirkt ein "Geändert"-Eintrag wie ein reines Escaping-Artefakt (`"`/`'` vs. `&quot;`/`&apos;`, i. d. R. weil versehentlich `sort.py` statt `diff_tools.py sort` auf den Kandidaten gelaufen ist), das explizit als solches einordnen statt als inhaltliche Änderung zu präsentieren — im Zweifel `<candidate_file>` erneut mit `diff_tools.py sort` normalisieren und neu vergleichen.
   - **Unverändert**: nur die Anzahl nennen, nicht auflisten.
   - Bei mehreren Dateien: ein Abschnitt pro Sprache, am Ende eine knappe Gesamt-Zusammenfassung (wie viele Dateien komplett unauffällig, wie viele mit Neu/Entfernt/Geändert).

## Stufe 2 — Merge (nur nach expliziter Bestätigung)

1. Nach der Auswertung aus Stufe 1 **immer** explizit fragen, ob gemergt werden soll — für jede Datei mit nennenswertem Befund einzeln, es sei denn der Nutzer bestätigt pauschal für alle. Sagt der Nutzer nur "vergleichen"/"prüfen"/"auswerten", endet der Workflow hier.
2. Merge-Semantik vergegenwärtigen (steht auch im Docstring von `diff_tools.py`): die Kandidaten-Datei gewinnt vollständig — für jeden (Kontext, Source)-Key, den sie noch enthält, ersetzt ihre komplette `<message>` die der Basis; Keys, die die Basis hat und der Kandidat nicht mehr, werden aus der Basis **gelöscht**; fehlt ein ganzer Kontext im Kandidaten, wird der komplette Kontext-Block aus der Basis entfernt.
3. Ausführen:
   ```
   python translation_scripts/diff_tools.py merge "translation_scripts/<lang>.ts" "<candidate_file>"
   ```
   (Merged in-place in die Basis-Datei; sortiert Basis und Kandidat vor und nach dem Merge automatisch noch einmal.)
4. Ergebnis zeigen: `git diff --stat -- translation_scripts/<lang>.ts` (und bei Bedarf `git diff -- translation_scripts/<lang>.ts` für den vollen Diff). Die Änderung bleibt **ungestaged** — nicht committen, außer der Nutzer verlangt das ausdrücklich (globale Regel dieses Repos: nie ungefragt committen, und laut Nutzer-Memory ohnehin nie mit Co-Authored-By-Trailer).

## Bekannte Sprachcodes

`de_DE`, `en`, `en_GB`, `es`, `es_MX`, `fr`, `it`, `nl`, `sv` — bei einer neuen Sprache, die es unter `translation_scripts/` noch nicht gibt, den Nutzer fragen, ob eine neue Basis-Datei angelegt werden soll, statt das stillschweigend zu tun.
