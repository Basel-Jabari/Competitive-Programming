"""Compile the nine range/string templates and check them with simple oracles.

Run: python Algorithms/validation/range_string_checks.py [--compiler g++]
Only Algorithms/build/range_strings is used for generated binaries.
Random test INPUTS use a fixed seed. The double-hash template uses its own
random bases; these tests cannot prove absence of hash collisions.
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
    "prefix2": "3_Stars/Range_Queries/Prefix_Sums/Two_Dimensional.cpp",
    "fenwick": "3_Stars/Range_Queries/Fenwick_Tree/Point_Add_Range_Sum.cpp",
    "segment": "3_Stars/Range_Queries/Segment_Tree/Point_Set_Range_Sum.cpp",
    "lazy": "3_Stars/Range_Queries/Segment_Tree/Lazy_Set_Add_Range_Sum.cpp",
    "sparse": "3_Stars/Range_Queries/Sparse_Table/Range_Minimum.cpp",
    "kmp": "3_Stars/Strings/Pattern_Matching/KMP.cpp",
    "hash": "3_Stars/Strings/String_Hashing/Double_Hash.cpp",
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


def rectangles():
    for n, m in [(1, 1), (1, 19), (17, 1), (4, 7), (9, 11)] * 4:
        a = [[RNG.randint(-10**9, 10**9) for _ in range(m)] for _ in range(n)]
        queries, expected = [], []
        ranges = [(0, n - 1, 0, m - 1)]
        ranges += [(*interval(n), *interval(m)) for _ in range(100)]
        for r1, r2, c1, c2 in ranges:
            queries.append(f"{r1 + 1} {c1 + 1} {r2 + 1} {c2 + 1}")
            expected.append(sum(a[r][c] for r in range(r1, r2 + 1)
                                for c in range(c1, c2 + 1)))
        data = f"{n} {m} {len(queries)}\n"
        data += "\n".join(" ".join(map(str, row)) for row in a) + "\n"
        data += "\n".join(queries) + "\n"
        check("prefix2", data, " ".join(map(str, expected)))
    data = "1000 1000 1\n" + ("1000000000 " * 1000 + "\n") * 1000
    check("prefix2", data + "1 1 1000 1000\n", 10**15)


def point_updates(name, add):
    for case in range(25):
        n = 1 if case == 0 else RNG.randint(2, 65)
        initial = [RNG.randint(-10**9, 10**9) for _ in range(n)]
        a, queries, expected = initial.copy(), [], []
        for step in range(300):
            if step % 3:
                index = RNG.choice([0, n - 1, RNG.randrange(n)])
                value = RNG.randint(-10**9, 10**9)
                queries.append(f"1 {index + 1} {value}")
                a[index] = a[index] + value if add else value
            else:
                l, r = interval(n)
                queries.append(f"2 {l + 1} {r + 1}")
                expected.append(sum(a[l:r + 1]))
        queries.append(f"2 1 {n}")
        expected.append(sum(a))
        check(name, array_input(initial, queries), " ".join(map(str, expected)))

    a = [10**9] * 200000
    value = 2 * 10**9 if add else 10**9
    queries = ["1 200000 1000000000", "2 1 200000", "2 200000 200000"]
    check(name, array_input(a, queries), f"{199999 * 10**9 + value} {value}")


def lazy_ranges():
    for case in range(32):
        n = 1 if case == 0 else RNG.randint(2, 80)
        initial = [RNG.randint(-10**8, 10**8) for _ in range(n)]
        a, queries, expected = initial.copy(), [], []
        # Repeated whole/partial updates force tags to compose and then push.
        for step in range(450):
            l, r = (0, n - 1) if step % 4 == 0 else interval(n)
            op = RNG.choice([1, 1, 2, 2, 3])
            if op == 3:
                queries.append(f"3 {l + 1} {r + 1}")
                expected.append(sum(a[l:r + 1]))
            else:
                value = RNG.choice([0, -10**8, 10**8, RNG.randint(-100, 100)])
                queries.append(f"{op} {l + 1} {r + 1} {value}")
                for i in range(l, r + 1):
                    a[i] = value if op == 1 else a[i] + value
        for i in range(n):
            queries.append(f"3 {i + 1} {i + 1}")
            expected.append(a[i])
        queries.append(f"3 1 {n}")
        expected.append(sum(a))
        check("lazy", array_input(initial, queries), " ".join(map(str, expected)))

    initial = [10**8] * 200000
    queries = ["2 1 200000 100000000"] * 199990
    queries += ["3 1 200000", "1 2 199999 0", "3 1 200000",
                "2 1 200000 -100000000", "3 1 200000"]
    per_item = 199991 * 10**8
    expected = [200000 * per_item, 2 * per_item,
                2 * per_item - 200000 * 10**8]
    check("lazy", array_input(initial, queries), " ".join(map(str, expected)))


def kmp_matches():
    cases = [("a", "a"), ("a", "b"), ("abc", "abcdef"),
             ("aaaaaa", "aaa"), ("ababababa", "ababa"), ("abcabc", "abc")]
    for _ in range(45):
        s = "".join(RNG.choices("abca", k=RNG.randint(1, 130)))
        p = "".join(RNG.choices("abc", k=RNG.randint(1, 20)))
        cases.append((s, p))
    for s, p in cases:
        positions = [i + 1 for i in range(len(s)) if s.startswith(p, i)]
        check("kmp", s + "\n" + p + "\n",
              str(len(positions)) + " " + " ".join(map(str, positions)))
    s, p = "a" * 200000, "a" * 100000
    check("kmp", s + "\n" + p + "\n",
          "100001 " + " ".join(map(str, range(1, 100002))))


def hashes():
    for case in range(30):
        s = "a" if case == 0 else "".join(RNG.choices("abcd", k=RNG.randint(2, 160)))
        queries, expected = [], []
        for step in range(300):
            l1, r1 = interval(len(s))
            if step % 4 == 0:
                l2, r2 = l1, r1
            elif step % 4 == 1:
                length = r1 - l1 + 1
                l2 = RNG.randrange(len(s) - length + 1)
                r2 = l2 + length - 1
            else:
                l2, r2 = interval(len(s))
            queries.append(f"{l1 + 1} {r1 + 1} {l2 + 1} {r2 + 1}")
            expected.append("YES" if s[l1:r1 + 1] == s[l2:r2 + 1] else "NO")
        data = s + f"\n{len(queries)}\n" + "\n".join(queries) + "\n"
        check("hash", data, " ".join(expected))
    s = "ab" * 100000
    check("hash", s + "\n4\n1 199998 3 200000\n1 1 2 2\n"
          "1 200000 1 200000\n1 1 1 2\n", "YES NO YES NO")


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
    rectangles()
    point_updates("fenwick", add=True)
    point_updates("segment", add=False)
    lazy_ranges()
    kmp_matches()
    hashes()
    trie_counts()
    for name in SOURCES:
        print(f"{name}: {COUNTS[name]} input cases passed")
    print(f"PASS: {sum(COUNTS.values())} input cases; seeded inputs = 20261020.")


if __name__ == "__main__":
    main()
