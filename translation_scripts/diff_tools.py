"""Sort, compare and merge the per-language diff-files (translation_scripts/<lang>.ts).

compare reports (context, source) keys that are new, removed or changed between two
diff-files, independent of on-disk order -- useful to review a candidate file before
merging it in, since merge's "new file wins" semantics (see below) can delete keys.

merge treats the "new" file as authoritative: for every (context, source) key
it still contains, its whole <message> (translation, comments, ...) replaces
whatever is in the base file; keys the new file no longer has are removed
from the base; keys only the new file has are added.

Unlike translations/client_*.ts, these diff-files are hand-maintained and
never touched by lupdate, so we deliberately skip sort.py's repair_file()
step: it rewrites literal quotes/apostrophes to &quot;/&apos; to match
lupdate's own output style, which only produces noisy, non-semantic diffs
here. Structural sort + ET.indent() plus normalizing the XML declaration and
line endings back to this repo's existing convention (CRLF, "1.0"/"UTF-8" in
double quotes) keeps sorting/merging a diff-file a clean, content-only diff.
"""

import sys
import copy
import xml.etree.ElementTree as ET

import sort as sort_tool

XML_DECLARATION = '<?xml version="1.0" encoding="UTF-8"?>'


def _normalize_diff_file(file_path):
    sort_tool.sort_contexts(file_path, file_path)
    sort_tool.sort_messages(file_path, file_path)

    tree = ET.parse(file_path)
    ET.indent(tree, space="  ")
    with open(file_path, "wb") as f:
        f.write((XML_DECLARATION + "\n").encode("utf-8"))
        tree.write(f, encoding="utf-8", xml_declaration=False)
        f.write(b"\n")  # ET.write() never adds one after the root's closing tag

    with open(file_path, "rb") as f:
        content = f.read()
    content = content.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
    with open(file_path, "wb") as f:
        f.write(content)


def sort_diff_file(file_path):
    _normalize_diff_file(file_path)


def _load_contexts(ts_file):
    """Return {context_name: {source_text: <message> element}} for a diff file."""
    tree = ET.parse(ts_file)
    root = tree.getroot()
    contexts = {}
    for context in root.findall("context"):
        name = context.find("name").text if context.find("name") is not None else ""
        messages = contexts.setdefault(name, {})
        for message in context.findall("message"):
            source = message.find("source")
            if source is not None and source.text:
                messages[source.text] = message
    return contexts


def compare_diff_files(base_file, new_file):
    """Compare two diff-files by (context, source) key, independent of on-disk order.

    Returns {"added": [...], "removed": [...], "changed": [...], "unchanged": int}.
    "removed" entries are keys merge_diff_files() would delete from base_file if new_file
    were merged in as-is -- includes both single missing keys within a shared context and
    keys belonging to a context absent from new_file entirely (that whole context would be
    dropped on merge).
    """
    base = _load_contexts(base_file)
    new = _load_contexts(new_file)

    def translation_of(message):
        t = message.find("translation")
        return t.text if t is not None and t.text else ""

    added, removed, changed = [], [], []
    unchanged = 0

    for name in sorted(set(base) | set(new), key=str.lower):
        base_msgs = base.get(name, {})
        new_msgs = new.get(name, {})
        for source in sorted(set(base_msgs) | set(new_msgs)):
            b = base_msgs.get(source)
            n = new_msgs.get(source)
            if b is not None and n is None:
                removed.append((name, source, translation_of(b)))
            elif n is not None and b is None:
                added.append((name, source, translation_of(n)))
            elif translation_of(b) != translation_of(n):
                changed.append((name, source, translation_of(b), translation_of(n)))
            else:
                unchanged += 1

    return {"added": added, "removed": removed, "changed": changed, "unchanged": unchanged}


def format_compare_report(base_file, new_file, result):
    lines = [f"Vergleich: {base_file}  <->  {new_file}", ""]

    lines.append(f"Neu ({len(result['added'])}):")
    for ctx, src, trans in result["added"]:
        lines.append(f"  [{ctx}] {src!r}")
        lines.append(f"      -> {trans!r}")

    lines.append("")
    lines.append(f"Entfernt -- fehlt in {new_file}, würde beim Merge aus der Basis gelöscht werden ({len(result['removed'])}):")
    for ctx, src, trans in result["removed"]:
        lines.append(f"  [{ctx}] {src!r}")
        lines.append(f"      war: {trans!r}")

    lines.append("")
    lines.append(f"Geändert ({len(result['changed'])}):")
    for ctx, src, old, new_t in result["changed"]:
        lines.append(f"  [{ctx}] {src!r}")
        lines.append(f"      alt: {old!r}")
        lines.append(f"      neu: {new_t!r}")

    lines.append("")
    lines.append(f"Unverändert: {result['unchanged']}")

    return "\n".join(lines)


def merge_diff_files(base_file, new_file, output_file=None, pre_sort=True, final_sort=True):
    if pre_sort:
        _normalize_diff_file(base_file)
        _normalize_diff_file(new_file)

    base_tree = ET.parse(base_file)
    base_root = base_tree.getroot()
    remaining = _load_contexts(new_file)

    for context in list(base_root.findall("context")):
        name = context.find("name").text if context.find("name") is not None else ""

        if name not in remaining:
            base_root.remove(context)
            continue

        new_messages = remaining.pop(name)
        for message in list(context.findall("message")):
            context.remove(message)
        for message in new_messages.values():
            context.append(copy.deepcopy(message))

    # contexts that only exist in the new file
    for name, messages in remaining.items():
        new_context = ET.SubElement(base_root, "context")
        name_elem = ET.SubElement(new_context, "name")
        name_elem.text = name
        for message in messages.values():
            new_context.append(copy.deepcopy(message))

    out_file = output_file or base_file
    base_tree.write(out_file, encoding="utf-8", xml_declaration=True)

    if final_sort:
        _normalize_diff_file(out_file)


def _usage():
    print("Usage:")
    print("  python diff_tools.py sort <file>")
    print("  python diff_tools.py compare <base_file> <new_file>")
    print("  python diff_tools.py merge <base_file> <new_file> [output_file] [--no-pre-sort] [--no-final-sort]")


if __name__ == "__main__":
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")

    args = sys.argv[1:]
    if not args:
        _usage()
        sys.exit(1)

    command = args[0]

    if command == "sort":
        if len(args) != 2:
            _usage()
            sys.exit(1)
        sort_diff_file(args[1])
        print(f"{args[1]} sorted.")

    elif command == "compare":
        if len(args) != 3:
            _usage()
            sys.exit(1)
        base_file, new_file = args[1], args[2]
        result = compare_diff_files(base_file, new_file)
        print(format_compare_report(base_file, new_file, result))

    elif command == "merge":
        flags = args[1:]
        pre_sort = "--no-pre-sort" not in flags
        final_sort = "--no-final-sort" not in flags
        positional = [a for a in flags if not a.startswith("--")]

        if len(positional) < 2:
            _usage()
            sys.exit(1)

        base_file, new_file = positional[0], positional[1]
        output_file = positional[2] if len(positional) > 2 else None

        merge_diff_files(base_file, new_file, output_file, pre_sort=pre_sort, final_sort=final_sort)
        print(f"Merged {new_file} into {output_file or base_file}.")

    else:
        _usage()
        sys.exit(1)
