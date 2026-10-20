"""Check the newly added BFS, DFS, and DSU templates.
Restored graph/tree implementations are checked by restored_checks.py.

Run: python Algorithms/validation/graph_tree_checks.py
Use --skip-compile to reuse the executables in Algorithms/build.
Only Graphs/3_Stars and Trees/3_Stars are read; no older templates are touched.
"""

from collections import deque
from concurrent.futures import ThreadPoolExecutor
from itertools import combinations
from pathlib import Path
import argparse
import random
import shutil
import subprocess


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
RNG = random.Random(20261020)
RUNS = 0


def compile_all():
    compiler = shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
    sources = sorted((ROOT / "3_Stars" / "Graphs").rglob("*.cpp"))
    sources += sorted((ROOT / "3_Stars" / "Trees").rglob("*.cpp"))
    sources = [p for p in sources if p.stem in {"BFS", "DFS", "DSU"}]
    BUILD.mkdir(exist_ok=True)

    def compile_one(source):
        result = subprocess.run(
            [compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
             str(source), "-o", str(BUILD / (source.stem + "_graph.exe"))],
            capture_output=True, text=True, timeout=90,
        )
        if result.returncode:
            raise AssertionError(f"Compilation failed: {source}\n{result.stderr}")
        if result.stderr.strip():
            print(result.stderr)
        return source

    with ThreadPoolExecutor(max_workers=2) as pool:
        compiled = list(pool.map(compile_one, sources))
    print(f"Compiled {len(compiled)} graph/tree templates.")
    return sources


def run(name, data):
    global RUNS
    result = subprocess.run(
        [str(BUILD / (name + "_graph.exe"))], input=data,
        capture_output=True, text=True, timeout=12,
    )
    assert result.returncode == 0, (name, result.returncode, result.stderr, data[:1000])
    RUNS += 1
    return result.stdout.strip()


def check(name, data, expected):
    actual = run(name, data)
    assert actual.split() == str(expected).split(), (name, data, expected, actual)


def edge_lines(edges):
    return "".join(" ".join(map(str, edge)) + "\n" for edge in edges)


def check_samples(sources):
    for source in sources:
        lines = ["// " + line for line in source.read_text(encoding="utf-8").splitlines()]
        start = lines.index("// Sample input:") + 1
        middle = lines.index("// Sample output:")
        sample = "\n".join(line[3:] for line in lines[start:middle]) + "\n"
        expected = []
        for line in lines[middle + 1:]:
            # All supplied sample output lines contain numbers / YES / NO.
            value = line[3:] if line.startswith("// ") else ""
            if not value or any(c.islower() for c in value):
                break
            expected.append(value)
        check(source.stem, sample, "\n".join(expected))


def check_traversal():
    for _ in range(60):
        n = RNG.randint(1, 12)
        edges = [(RNG.randrange(n), RNG.randrange(n)) for _ in range(RNG.randrange(30))]
        graph = [[] for _ in range(n)]
        for u, v in edges:
            graph[u].append(v)
            graph[v].append(u)
        source = RNG.randrange(n)
        distance = [-1] * n
        distance[source] = 0
        q = deque([source])
        while q:
            u = q.popleft()
            for v in graph[u]:
                if distance[v] == -1:
                    distance[v] = distance[u] + 1
                    q.append(v)
        data = edge_lines([(u + 1, v + 1) for u, v in edges])
        check("BFS", f"{n} {len(edges)} {source + 1}\n" + data, " ".join(map(str, distance)))
        labels = [0] * n
        count = 0
        for start in range(n):
            if labels[start]:
                continue
            count += 1
            labels[start] = count
            q = deque([start])
            while q:
                for v in graph[q.popleft()]:
                    if labels[v] == 0:
                        labels[v] = count
                        q.append(v)
        check("DFS", f"{n} {len(edges)}\n" + data, f"{count}\n" + " ".join(map(str, labels)))


def check_dsu():
    for _ in range(40):
        n = RNG.randint(1, 30)
        labels = list(range(n))
        queries, answers = [], []
        for _ in range(100):
            kind = RNG.randint(1, 3)
            u, v = RNG.randrange(n), RNG.randrange(n)
            if kind == 1:
                a, b = labels[u], labels[v]
                labels = [a if label == b else label for label in labels]
                queries.append((1, u + 1, v + 1))
            elif kind == 2:
                answers.append("YES" if labels[u] == labels[v] else "NO")
                queries.append((2, u + 1, v + 1))
            else:
                answers.append(str(labels.count(labels[u])))
                queries.append((3, u + 1))
        check("DSU", f"{n} {len(queries)}\n" + edge_lines(queries), "\n".join(answers))
























def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--skip-compile", action="store_true")
    args = parser.parse_args()
    if args.skip_compile:
        sources = sorted((ROOT / "3_Stars" / "Graphs").rglob("*.cpp"))
        sources += sorted((ROOT / "3_Stars" / "Trees").rglob("*.cpp"))
    else:
        sources = compile_all()
    sources = [p for p in sources if p.stem in {"BFS", "DFS", "DSU"}]
    check_samples(sources)
    for group in [check_traversal, check_dsu]:
        group()
        print(f"Passed: {group.__name__}")
    print(f"PASS: {len(sources)} templates; {RUNS} executable checks, including all samples.")


if __name__ == "__main__":
    main()
