"""Compile tree Mo and compare with direct BFS paths; no tree-Mo oracle reuse."""
from pathlib import Path
from collections import deque
import random
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT/"2_Stars/Range_Queries/Mo_Algorithm/Tree_Paths.cpp"
BINARY = ROOT/"build/mo_tree.exe"
RNG = random.Random(20261022)
RUNS = 0

def run(data, expected):
    global RUNS
    result = subprocess.run([str(BINARY)], input=data, text=True, capture_output=True, timeout=30)
    RUNS += 1
    assert result.returncode == 0, result.stderr
    got = list(map(int, result.stdout.split()))
    assert got == expected, (data[:2000], expected[:100], got[:100])

def case(n, colors, edges, queries, expected=None):
    if expected is None:
        graph = [[] for _ in range(n+1)]
        for u, v in edges:
            graph[u].append(v)
            graph[v].append(u)
        expected = []
        for start, end in queries:
            parent = [-1]*(n+1)
            parent[start] = 0
            queue = deque([start])
            while queue:
                u = queue.popleft()
                if u == end:
                    break
                for v in graph[u]:
                    if parent[v] == -1:
                        parent[v] = u
                        queue.append(v)
            found = set()
            u = end
            while u:
                found.add(colors[u-1])
                u = parent[u]
            first_missing = 1
            while first_missing in found:
                first_missing += 1
            expected.append(first_missing - 1)
    data = f"{n} {len(queries)}\n" + " ".join(map(str, colors)) + "\n"
    data += "".join(f"{u} {v}\n" for u, v in edges)
    data += "".join(f"{u} {v}\n" for u, v in queries)
    run(data, expected)

def main():
    BINARY.parent.mkdir(exist_ok=True)
    compiler = shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
    result = subprocess.run([compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
                             str(SOURCE), "-o", str(BINARY)], capture_output=True, text=True, timeout=120)
    assert result.returncode == 0 and not result.stderr.strip(), result.stderr
    text = SOURCE.read_text(encoding="utf-8")
    data, rest = text.split("Sample input:\n", 1)[1].split("Sample output:\n", 1)
    run(data, list(map(int, rest.split("\n\n", 1)[0].split())))
    for _ in range(100):
        n = RNG.randint(1, 70)
        colors = [RNG.choice([1, 10**9, RNG.randint(1, n+1)]) for _ in range(n)]
        edges = [(i, RNG.randrange(1, i)) for i in range(2, n+1)]
        RNG.shuffle(edges)
        queries = [(RNG.randint(1, n), RNG.randint(1, n)) for _ in range(100)]
        case(n, colors, edges, queries)
    case(1, [10**9], [], [(1, 1)]*100)
    case(5, [7]*5, [(1, 2), (1, 3), (2, 4), (2, 5)],
         [(u, v) for u in range(1, 6) for v in range(1, 6)])
    case(3, [1, 2, 3], [(1, 2), (2, 3)], [])
    for n in [8, 9, 15, 16, 24, 25, 99, 100]:
        case(n, list(range(1, n+1)), [(i, i+1) for i in range(1, n)],
             [(1, i) for i in range(1, n+1)] + [(i, n) for i in range(1, n+1)])
    n = 100000
    queries = [(1, n), (n, 1), (1, 1), (n, n), (100, 1000)]
    case(n, list(range(1, n+1)), [(i, i+1) for i in range(1, n)],
         queries, [n, n, 1, 0, 0])
    case(n, [1] + [10**9]*(n-1), [(1, i) for i in range(2, n+1)],
         [(2, 3), (3, 2), (1, 3), (3, 3)], [1, 1, 1, 0])
    queries = [(RNG.randint(1, n), RNG.randint(1, n)) for _ in range(100000)]
    started = time.perf_counter()
    case(n, list(range(1, n+1)), [(i, i+1) for i in range(1, n)],
         queries, [max(u, v) if min(u, v) == 1 else 0 for u, v in queries])
    print(f"100000-node, 100000-query chain: {time.perf_counter()-started:.2f}s including input assembly.")
    print(f"PASS: Permutate Paths tree Mo; {RUNS} sample/random/boundary program checks; no compiler warnings.")

if __name__ == "__main__":
    main()
