"""Check template structure, style rules, catalog paths, and recovery contents."""
from pathlib import Path
from collections import Counter
from hashlib import sha256
import csv
import re
import subprocess
import zipfile
from original_sources import MAPPING, main as verify_originals

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parent

def main():
    tiers = ["3_Stars", "2_Stars", "1_Star"]
    sources = sorted(p for tier in tiers for p in (ROOT/tier).rglob("*.cpp"))
    with (ROOT/"TOPICS.tsv").open(encoding="utf-8", newline="") as f:
        topics = list(csv.DictReader(f, delimiter="\t"))
    assert len(sources) == 75 and len(topics) == 62
    restored = {ROOT/path for _, path, _ in MAPPING}
    verify_originals()
    assert {p.parent for p in sources} == {ROOT/e["Path"] for e in topics}
    for topic in topics:
        folder = ROOT/topic["Path"]
        assert folder == ROOT/topic["Priority"]/topic["Domain"]/topic["Algorithm"]
        assert topic["Stage"] == "ready_for_review"
        assert 1 <= len(list(folder.glob("*.cpp"))) <= 2
    for path in sources:
        text = path.read_text(encoding="utf-8")
        assert len(path.relative_to(ROOT).parts) == 4, path
        if path in restored:
            # The author's exact original code takes precedence over new style rules.
            continue
        for marker in ["Sample input:", "Sample output:", "void Solve()", "int32_t main()", "Basel Al-Jabari."]:
            assert marker in text, (path, marker)
        code = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        code = re.sub(r"//[^\n]*", "", code)
        assert not re.search(r"for\s*\([^;\n]*:\s*[^;\n]*\)", code), path
        assert not re.search(r"while\s*\(\s*(q|t)\s*--", code), path
        assert "freopen(" not in code, path
    ignored = subprocess.run(["git", "check-ignore", "--"] +
                             [p.relative_to(REPO).as_posix() for p in sources],
                             cwd=REPO, text=True, capture_output=True)
    assert ignored.returncode == 1 and not ignored.stdout.strip(), ignored.stdout + ignored.stderr
    migration = (ROOT/"MIGRATION.txt").read_text(encoding="utf-8")
    for path in re.findall(r"[123]_Stars?/[^\s]*\.cpp", migration):
        assert (ROOT/path).is_file(), path
    archive_path = REPO/"Archives/Legacy_Algorithms_2026-10-20.zip"
    with (REPO/"Archives/Legacy_Algorithms_2026-10-20.sha256.tsv").open(encoding="utf-8", newline="") as f:
        manifest = list(csv.DictReader(f, delimiter="\t"))
    with zipfile.ZipFile(archive_path) as archive:
        assert len(manifest) == 38
        assert set(archive.namelist()) == {e["Path"] for e in manifest}
        for entry in manifest:
            data = archive.read(entry["Path"])
            assert len(data) == int(entry["Bytes"])
            assert sha256(data).hexdigest() == entry["SHA256"]
            if entry["Path"].endswith(".cpp"):
                assert entry["Path"].split("/", 1)[1] in migration, entry["Path"]
        assert sum(name.endswith(".docx") for name in archive.namelist()) == 5
    for old in ["Graph_Theory", "Range_Queries", "String_Algorithms",
                "Temp_Hopcroft_Algorithm", "Temp_Rabin_Miller_Test"]:
        assert not (REPO/old).exists(), old
    for tier in tiers:
        assert not list((ROOT/tier).rglob(".gitkeep"))
        assert not list((ROOT/tier).rglob("STATUS.txt"))
    print("PASS: 75 files, 62 topics, style rules, Git visibility, migration paths, and recovery hashes.")
    print("Files per tier:", dict(Counter(p.relative_to(ROOT).parts[0] for p in sources)))

if __name__ == "__main__":
    main()
