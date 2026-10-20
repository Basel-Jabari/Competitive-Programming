"""Check newly added 1D prefix sums, sparse table, and trie.
Restored range/string programs are checked by restored_checks.py.
"""

import argparse
import collections
import concurrent.futures
import os
from pathlib import Path
import random
import shutil
import subprocess


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "range_strings"
SOURCES = {
    "prefix1": "3_Stars/Range_Queries/Prefix_Sums/One_Dimensional.cpp",
    "sparse": "3_Stars/Range_Queries/Sparse_Table/Range_Minimum.cpp",
    "trie": "3_Stars/Strings/Trie/Word_Count.cpp",
}
RNG = random.Random(20261020)
COUNTS = collections.Counter()
BINARIES = {}


def compile_one(item, compiler):
    name, relative = item
    binary = BUILD / (name + (".exe" if os.name == "nt" else ""))
    result = subprocess.run(
        [
            compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
            "-pedantic", str(ROOT / relative), "-o", str(binary),
        ],
        capture_output=True, text=True, timeout=90,
    )
    if result.returncode or result.stderr.strip():
        raise AssertionError(f"Compile failed or warned: {relative}\n{result.stderr}")
    return name, binary


def check(name, data, expected):
    result = subprocess.run(
        [str(BINARIES[name])], input=data, capture_output=True,
        text=True, timeout=15,
    )
    assert result.returncode == 0, (name, result.returncode, result.stderr)
    actual = result.stdout.split()
    wanted = str(expected).split()
    assert actual == wanted, (
        name, "input", data[:3000], "expected", wanted[:100], "got", actual[:100],
    )
    COUNTS[name] += 1


def check_samples():
    for name, relative in SOURCES.items():
        lines = (ROOT / relative).read_text(encoding="utf-8").splitlines()
        start = lines.index("Sample input:") + 1
        middle = lines.index("Sample output:")
        end = next(i for i in range(middle + 1, len(lines)) if not lines[i].strip())
        data = "\n".join(lines[start:middle]) + "\n"
        expected = "\n".join(lines[middle + 1:end])
        check(name, data, expected)


def interval(n):
    l = RNG.randrange(n)
    return l, RNG.randrange(l, n)


def array_input(a, queries):
    return (
        f"{len(a)} {len(queries)}\n"
        + " ".join(map(str, a)) + "\n"
        + "\n".join(queries) + "\n"
    )


def static_ranges():
    for case in range(28):
        n = [1, 2, 3, 7, 8, 9, 31, 32, 33][case % 9]
        a = [RNG.randint(-10**9, 10**9) for _ in range(n)]
        queries, sums, minima = [], [], []
        for l in range(n):
            for r in range(l, n):
                queries.append(f"{l + 1} {r + 1}")
                sums.append(sum(a[l:r + 1]))
                minima.append(min(a[l:r + 1]))
        data = array_input(a, queries)
        check("prefix1", data, " ".join(map(str, sums)))
        check("sparse", data, " ".join(map(str, minima)))

    a = [10**9] * 200000
    data = array_input(a, ["1 200000", "200000 200000", "17 150003"])
    check("prefix1", data, f"{sum(a)} {a[-1]} {sum(a[16:150003])}")
    check("sparse", data, "1000000000 1000000000 1000000000")












def trie_counts():
    for case in range(25):
        words = ["a", "ab", "abc", "abcd", "b", "z"]
        words += ["".join(RNG.choices("abcd", k=RNG.randint(1, 9)))
                  for _ in range(60)]
        counts, queries, expected = collections.Counter(), [], []
        for _ in range(700):
            op = RNG.randint(1, 4)
            word = RNG.choice(words)
            if op == 4:
                word = word[:RNG.randint(1, len(word))]
            queries.append(f"{op} {word}")
            if op == 1:
                counts[word] += 1
            elif op == 2:
                counts[word] = max(0, counts[word] - 1)
            elif op == 3:
                expected.append(counts[word])
            else:
                expected.append(sum(count for text, count in counts.items()
                                    if text.startswith(word)))
        data = f"{len(queries)}\n" + "\n".join(queries) + "\n"
        check("trie", data, " ".join(map(str, expected)))
    word = "a" * 199995
    check("trie", f"4\n1 {word}\n4 a\n3 a\n2 a\n", "1 0")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
    args = parser.parse_args()
    BUILD.mkdir(parents=True, exist_ok=True)
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
        for name, binary in pool.map(
            lambda item: compile_one(item, args.compiler), SOURCES.items()
        ):
            BINARIES[name] = binary
    print(f"Compiled {len(SOURCES)} GNU C++17 files without warnings.", flush=True)
    check_samples()
    static_ranges()
    trie_counts()
    for name in SOURCES:
        print(f"{name}: {COUNTS[name]} input cases passed")
    print(f"PASS: {sum(COUNTS.values())} input cases; seeded inputs = 20261020.")


if __name__ == "__main__":
    main()
