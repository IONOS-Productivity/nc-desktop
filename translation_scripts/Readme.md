# Lokalisierung des HiDrive Next Clients auf Basis einer Diff-Datei

Diese Anleitung beschreibt den Prozess zur Lokalisierung des HiDrive Next Clients mithilfe einer von uns erstellten Diff-Datei. Die Lokalisierung erfolgt auf Basis von `.ts`-Dateien, welche die Ressourcen der Anwendung in XML-Form enthalten.

## Voraussetzungen

- Nextcloud Stable Client Quellcode
- Python 3
- Python-Skripte `merge_translation.py` (die eigentliche Pipeline) und `diff_tools.py` (Pflege der sprachabhängigen Diff-Dateien)
- Qt Linguist Tools (insbesondere `lupdate`)

# Vorgehen bei Release (obsolet)

> **Obsolet, siehe [Automatisierung: Merge-Treiber + post-merge Hook](#automatisierung-merge-treiber--post-merge-hook).** Der separate `translations_<version source>`-Branch samt PR zurück in Richtung NC-Basisversion wird nicht mehr gebraucht: der Abgleich mit dem jeweils aktuellen NC-Basisstand passiert seitdem inplace bei jedem Merge eines `stable-x.y`-Branches, automatisch über den post-merge-Hook. Der folgende Ablauf bleibt hier nur als Referenz/Historie stehen.

Um in einem Release zu erstellen und einen valider PR zur Übersetzung zu haben ist folgendes Vorgehen notwendig:

0. (Optional) Einbeziehen unserer Änderungen aus Phrase. Dieser Schritt ist optional, da die Änderungen in der Regel schon in der Diff-Datei enthalten sind. Sollte es dennoch notwendig sein, können die Änderungen aus Phrase in die `.ts`-Diff-Dateien gemerged werden – dafür gibt es inzwischen `diff_tools.py`, siehe [Diff-Dateien aus PhraseApp aktualisieren](#diff-dateien-aus-phraseapp-aktualisieren-diff_toolspy).
1. Es wird ein neuer `translations_<version source>` branch erstellt. Abgeleitet vom entsprechenden `develop_<version source>`. (z.b. translations_stable-3.16)
2. Durchlaufen der untenstehenden Schritte 0–5
3. Erstellen eines "approved" PRs von `translations_<version source>` nach `<version source>`, also in Richtung der eigentlichen Basisversion von nc (z.B. [stable-3.16] Translations)
4. Der PR wird dann vom Brander gemerged

Das Rebasedn der Translation-Branches lohnt sich eigentlich nicht, weil der nextcloud master sich relativ häufig ändert, was zu vielen Konflikten führen würde.

## Automatisierung: Merge-Treiber + post-merge Hook

Beim Mergen eines `stable-x.y`-Branches (neue NC-Basisversion) in einen Feature-/Entwicklungs-Branch (oder z.B. auch `develop` in einen Feature-Branch) müssen die `client_*.ts`-Dateien gegen die neue Basis abgeglichen werden. Das übernehmen zwei zusammenspielende Mechanismen:

### 1. Merge-Treiber für `translations/client_*.ts`

Diese Dateien sind maschinell generierte, stark umsortierte XML-Dateien. Ein normaler zeilenbasierter 3-way-Merge (Git-Standardverhalten) kann Message-Blöcke falsch ausrichten und Konflikte erzeugen, deren Auflösung die Datei inhaltlich beschädigt, ohne dass es auffällt (die Validierung im Skript prüft nur Struktur/Konsistenz, nicht ob eine Übersetzung noch zum richtigen Source-Text gehört). Deshalb wird für diese Dateien gar kein inhaltlicher Merge mehr versucht: `.gitattributes` markiert sie mit einem eigenen Treiber, der bei jedem Merge **immer die eingehende Seite** (das, was gerade reingemerged wird) 1:1 übernimmt – unabhängig von der Richtung. Das garantiert z.B. auch, dass ein Merge von `develop` in einen Feature-Branch dort den korrekten, aktuellen Stand ankommen lässt.

**Einmalige Einrichtung pro Clone** (der Treiber-Name in `.gitattributes` ist bereits eingetragen, nur das Kommando dahinter muss lokal registriert werden – analog zu `core.hooksPath`):

```
git config merge.nc-take-incoming.driver "cp -- '%B' '%A'"
```

Ohne diesen Schritt versucht Git weiterhin einen normalen Inhalts-Merge auf den `.ts`-Dateien, mit dem oben beschriebenen Korruptionsrisiko. Der Schritt ist unabhängig von `core.hooksPath` und muss zusätzlich ausgeführt werden.

Der Eintrag steht bewusst **am Anfang** von `.gitattributes` und sollte dort bleiben: die IONOS-Branding-Pipeline (Brander) hängt beim Branden eines `stable-x.y`-Builds ihre eigene Zeile immer direkt hinter die letzte Zeile der unveränderten Upstream-Datei. Stand unser Block ebenfalls dort, treffen beide Ergänzungen dieselbe Ankerzeile ohne trennenden Kontext – ein Konflikt, an dem der automatisierte Brander-Merge schon gescheitert ist.

### 2. `post-merge`/`post-commit` Hook

- Läuft automatisch nach jedem lokalen `git merge`/`git pull` (`post-merge`) bzw. nach einem Merge-Commit, der Konflikte manuell aufgelöst hat (`post-commit`).
- Erkennt anhand der Merge-Commit-Message, ob ein `stable-x.y`-Branch gemerged wurde (z.B. `Merge branch 'stable-33.0' into ...`).
- Führt in diesem Fall automatisch `merge_translation.py auto --auto-commit` aus (**ohne** Branch-Argument).
- Da mit `--auto-commit` gelaufen wird, liegen die Änderungen an `translations/client_*.ts` danach **nicht** ungestaged im Working Directory, sondern bereits als eigene Commits pro Schritt (`Step 0` … `Step 5`) auf dem Branch. Sie sind vor dem Push zu prüfen; Schritte ohne Änderung erzeugen keinen Commit.
- Die komplette Hook-Ausgabe wird zusätzlich nach `.githooks/post-merge.log` (lokal, nicht versioniert) gespiegelt – erste Anlaufstelle bei Problemen, weil GUI-Clients wie GitKraken die Hook-Ausgabe oft nirgends sichtbar anzeigen.
- Squash-Merges werden übersprungen (kein Merge-Commit, dessen Message sich auswerten ließe).

`auto` durchläuft dabei Schritt 0 (jetzt nur noch Normalisieren/Sortieren der frisch eingemergten Datei, siehe unten) und Schritte 1–5 wie gewohnt.

**Einmalige Aktivierung pro Clone:**

```
git config core.hooksPath .githooks
```

## Diff-Dateien aus PhraseApp aktualisieren (`diff_tools.py`)

Die Dateien `translation_scripts/<lang>.ts` (`de_DE.ts`, `en.ts`, `en_GB.ts`, `es.ts`, `es_MX.ts`, `fr.ts`, `it.ts`, `nl.ts`, `sv.ts`) sind die hand-gepflegte Quelle unserer eigenen Keys und Übersetzungen – sie werden in Schritt 3 der Pipeline in die eigentlichen `translations/client_*.ts` gemergt. Ihre Ursprungsquelle ist PhraseApp: dort werden die Übersetzungen beauftragt und anschließend als `.ts`-Datei exportiert (Windows hängt beim Download häufig `(1)`, `(2)` … an den Dateinamen an, z.B. `de_DE(3).ts`).

Ein solcher Export darf nicht einfach über die Basis-Datei kopiert werden: er ist anders sortiert und kann – etwa bei einem unvollständigen Export – Keys nicht mehr enthalten, die bei uns noch gebraucht werden. Für das Prüfen und Übernehmen gibt es deshalb `diff_tools.py`:

```
python diff_tools.py sort <file>
python diff_tools.py compare <base_file> <new_file>
python diff_tools.py merge <base_file> <new_file> [output_file] [--no-pre-sort] [--no-final-sort]
```

- `sort` bringt eine Datei in unsere kanonische Reihenfolge und Formatierung (Kontexte und Messages sortiert, `ET.indent()`, XML-Deklaration und Zeilenenden auf die Repo-Konvention normalisiert) – inhaltsneutral.
- `compare` vergleicht Basis- und Kandidaten-Datei pro (Kontext, Source-Text)-Schlüssel und damit unabhängig von der Position in der Datei. Reine Umsortierungen tauchen dadurch nicht fälschlich als Änderung auf, wie es ein naiver `git diff` tut. Ausgegeben werden neue, entfernte, geänderte und die Anzahl unveränderter Einträge.
- `merge` behandelt den Kandidaten als maßgeblich: für jeden (Kontext, Source)-Key, den er noch enthält, ersetzt seine komplette `<message>` die der Basis; Keys, die nur die Basis hat, werden **aus der Basis gelöscht**; fehlt ein ganzer Kontext im Kandidaten, entfällt der komplette Kontext-Block. Basis und Kandidat werden dabei vor und nach dem Merge automatisch sortiert.

Wegen dieser Lösch-Semantik gilt: **erst `sort` + `compare`, den Befund prüfen – insbesondere die "Entfernt"-Einträge – und nur dann `merge`.** Mehrere entfernte Keys im selben Kontext deuten in der Regel auf einen unvollständigen Export hin, nicht auf eine gewollte Löschung.

`diff_tools.py` ist ausdrücklich **nicht** durch `sort.py` ersetzbar, auch wenn es dessen Sortierfunktionen benutzt: `sort.py` führt zusätzlich `repair_file()` aus und schreibt rohe `"`/`'` in `&quot;`/`&apos;` um, um den Ausgabestil von `lupdate` nachzubilden. Für die maschinell erzeugten `translations/client_*.ts` ist das richtig, bei den handgepflegten Diff-Dateien erzeugt es nur Escaping-Rauschen im Vergleich. Läuft versehentlich `sort.py` auf einem Kandidaten, erscheinen solche Artefakte in `compare` als "Geändert" – dann erneut mit `diff_tools.py sort` normalisieren und neu vergleichen.

Für den KI-gestützten Ablauf desselben Workflows (Sprache aus dem Dateinamen ableiten, passende Basis-Datei finden, Befund aufbereiten, Merge erst nach ausdrücklicher Bestätigung) gibt es den Claude-Code-Skill [`translation-diff-merge`](../.claude/skills/translation-diff-merge/SKILL.md).

## Allgemeines

Die Lokalisierung erfolgt in mehreren Schritten. 
Die Qt-Translation Files (`.ts`-Dateien) enthalten zu jeder Resource die entsprechnde Datei und die Zeilennummer. Diese Informationen entfernen wir für eine bessere Vergleichabrkeit.
*Die `.ts`-Dateien müssen vor jedem Merge-Schritt sortiert werden.* Dies geschieht in der Regel durch das Skript selbst.

## Schritte

### Automatischer Durchlauf

Alle Schritte (0–5) können mit einem einzigen Befehl ausgeführt werden. Mit `--auto-commit` wird nach jedem Schritt automatisch committet:

```
python3 merge_translation.py all --auto-commit
```

### Einzelne Schritte

Die Schritte können auch einzeln ausgeführt werden. Mit `--auto-commit` entfällt das manuelle Committen:

```
python3 merge_translation.py 1 --auto-commit
```

### Schritt 0: Eingemergte Übersetzungen normalisieren

- Dank des Merge-Treibers (siehe oben) enthält `translations/client_*.ts` an dieser Stelle bereits 1:1 den Stand der Seite, die gerade eingemergt wurde – es muss also nichts mehr rekonstruiert werden, kein Branch-Argument nötig.
- Das Skript sortiert die Dateien nur in unsere kanonische Reihenfolge/Formatierung und entfernt die `<location>`-Tags (die erst Schritt 5 wieder setzt), damit der Diff von Schritt 1 nachher nur echte `lupdate`-Änderungen zeigt statt Sortier- und Location-Rauschen:

```
python3 merge_translation.py 0
```

- Committen (STEP 0) — oder `--auto-commit` verwenden.

### Schritt 1: lupdate, obsolete Keys behalten

- Verwenden des Python-Skripts `merge_translation.py` mit Parameter `1`:

```
python3 merge_translation.py 1
```

- Führt ein `lupdate` auf dem **HiDrive Next Client** aus.
- Obsolete Einträge werden **nicht gelöscht**.
- Neue Keys werden hinzugefügt.
- Obsolete-Markierungen werden entfernt und die Datei wird sortiert.
- Committen (STEP 1) — oder `--auto-commit` verwenden.

### Schritt 2: lupdate, obsolete Keys entfernen

- Verwenden des Skripts mit Parameter `2`:

```
python3 merge_translation.py 2
```

- Führt ein `lupdate` aus.
- **Obsolete Keys werden entfernt**.
- Die Datei enthält jetzt nur die aktuellen Keys (unsere Keys ohne Übersetzungen).
- Committen (STEP 2) — oder `--auto-commit` verwenden.

### Schritt 3: Diff-Dateien einmergen

- Verwenden des Skripts mit Parameter `3`:

```
python3 merge_translation.py 3
```

- Sprachabhängige Diff-Dateien werden in die `.ts`Dateien gemergt.
- Leere Keys werden mit Übersetzungen gefüllt.
- Obsolete Keys aus der Diff-Datei werden gegebenenfalls eingefügt.
- Committen (STEP 3) — oder `--auto-commit` verwenden.

### Schritt 4: lupdate, doppelte Keys füllen

- Verwenden des Skripts mit Parameter `4`:

```
python3 merge_translation.py 4
```

- Ein weiteres `lupdate` wird ausgeführt.
- Heuristische Füllung von doppelten Keys mit unseren Übersetzungen.
- Committen (STEP 4) — oder `--auto-commit` verwenden.

### Schritt 5: obsolete Keys endgültig entfernen, Locations setzen

- Verwenden des Skripts mit Parameter `5`:

```
python3 merge_translation.py 5
```

- **Obsolete Keys werden endgültig entfernt**.
- Dieser Lauf setzt die `<location>`-Angaben (Datei + Zeilennummer) wieder in die Dateien.
- Committen (STEP 5) — oder `--auto-commit` verwenden.

## Abschluss

Nach dem letzten Schritt sind die `.ts`-Dateien vollständig lokalisiert, enthalten unsere Keys und Übersetzungen und sind frei von obsolete Keys.

## Historie: entfernte Mechanismen

Zwei frühere Mechanismen zum Abgleich mit dem NC-Basisstand wurden entfernt, weil der neue Merge-Treiber (siehe oben) das eigentliche Problem, das sie lösen sollten, gar nicht mehr entstehen lässt:

- **Ursprünglich (manuell):** den `stable-x`-Branch auschecken, den `lupdate`-Befehl von Hand gegen den dort ausgecheckten Quellcode laufen lassen, die Änderung stashen, zurück auf den eigenen Branch wechseln und den Stash anwenden.
- **Danach (automatisiert, `git worktree`):** Step 0 hat denselben Ablauf automatisiert – über ein temporäres `git worktree`-Checkout von `nc_branch` wurde `lupdate` gegen dessen Quellcode ausgeführt, mit `-ts` aber direkt auf die eigenen `client_*.ts`-Dateien zeigend.

Beide Varianten haben ausschließlich die *Menge* der Source-Strings synchronisiert (neu/entfernt/Location) – `lupdate` liest nie den Übersetzungstext einer anderen `.ts`-Datei, auch nicht den von `stable-x`. Der eigentliche NC-Übersetzungsstand kam bisher nur über den normalen Git-Merge von `client_*.ts` selbst ins Repo – und genau der war durch Zeilen-basiertes 3-way-Merging auf dieser stark umsortierten, maschinengenerierten XML-Struktur nicht robust (siehe Merge-Treiber-Abschnitt oben). Mit dem Merge-Treiber übernimmt der Git-Merge selbst zuverlässig den kompletten, korrekten Stand der eingehenden Seite; Step 0 muss diesen Stand daher nicht mehr aus dem Quellcode rekonstruieren und macht seitdem nur noch die Normalisierung/Sortierung.