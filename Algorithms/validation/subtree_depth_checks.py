"""Check the migrated subtree-at-depth example; writes only Algorithms/build."""
from pathlib import Path
from collections import deque
import os
import random
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "3_Stars/Trees/Subtree_Queries/Depth_Count.cpp"
BUILD = ROOT / "build"
BINARY = BUILD / "subtree_depth.exe"
RNG = random.Random(20261020)
RUNS = 0

def check(data, expected):
    global RUNS
    result = subprocess.run([str(BINARY)], input=data, text=True,
                            capture_output=True, timeout=20, check=True)
    RUNS += 1
    assert list(map(int, result.stdout.split())) == expected, (data[:1000], result.stdout[:1000], expected[:50])

def main():
    BUILD.mkdir(exist_ok=True)
    compiler = os.environ.get("CXX") or shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
    result = subprocess.run([compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
                             str(SOURCE), "-o", str(BINARY)], text=True, capture_output=True, timeout=120)
    assert result.returncode == 0 and not result.stderr.strip(), result.stderr
    text = SOURCE.read_text(encoding="utf-8")
    data, rest = text.split("Sample input:\n", 1)[1].split("Sample output:\n", 1)
    check(data, list(map(int, rest.split("\n\n", 1)[0].split())))
    for _ in range(80):
        n = RNG.randint(1, 70)
        root = RNG.randint(1, n)
        edges = [(i, RNG.randrange(1, i)) for i in range(2, n+1)]
        graph = [[] for _ in range(n+1)]
        for u, v in edges:
            graph[u].append(v)
            graph[v].append(u)
        parent = [0]*(n+1)
        depth = [0]*(n+1)
        queue = deque([root])
        while queue:
            u = queue.popleft()
            for v in graph[u]:
                if v != parent[u]:
                    parent[v] = u
                    depth[v] = depth[u]+1
                    queue.append(v)
        queries, expected = [], []
        for _ in range(100):
            u = RNG.randint(1, n)
            d = RNG.choice([0, 10**9, depth[u], RNG.randrange(n)])
            queries.append((u, d))
            count = 0
            for v in range(1, n+1):
                if depth[v] == d:
                    ancestor = v
                    while ancestor and ancestor != u:
                        ancestor = parent[ancestor]
                    count += ancestor == u
            expected.append(count)
        data = f"{n} {len(queries)} {root}\n" + "".join(f"{u} {v}\n" for u, v in edges+queries)
        check(data, expected)
    n = 200000
    data = f"{n} 5 {n}\n" + "".join(f"{i} {i+1}\n" for i in range(1, n))
    data += f"{n} 0\n{n} {n-1}\n1 {n-1}\n1 0\n{n} 1000000000\n"
    check(data, [1, 1, 1, 0, 0])
    print(f"PASS: 1 program; {RUNS} sample/random/chain checks; no compiler warnings.")

if __name__ == "__main__":
    main()
