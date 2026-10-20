"""Compile graph/tree templates and compare them with independent small solvers.

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


def bellman_ford(n, edges, source):
    distance = [None] * n
    distance[source] = 0
    for _ in range(n - 1):
        changed = False
        for u, v, weight in edges:
            if distance[u] is not None:
                candidate = distance[u] + weight
                if distance[v] is None or candidate < distance[v]:
                    distance[v] = candidate
                    changed = True
        if not changed:
            break
    return distance


def has_negative_cycle(n, edges):
    # A zero-cost super-source reaches every vertex.
    distance = [0] * n
    for step in range(n):
        changed = False
        for u, v, weight in edges:
            if distance[v] > distance[u] + weight:
                distance[v] = distance[u] + weight
                changed = True
        if not changed:
            return False
    return True


def check_shortest_paths():
    for name, max_weight in [("Dijkstra", 10**9), ("Zero_One_BFS", 1)]:
        for _ in range(80):
            n = RNG.randint(1, 10)
            source = RNG.randrange(n)
            edges = [
                (RNG.randrange(n), RNG.randrange(n), RNG.randint(0, max_weight))
                for _ in range(RNG.randrange(40))
            ]
            expected = bellman_ford(n, edges, source)
            data = f"{n} {len(edges)} {source + 1}\n"
            data += edge_lines([(u + 1, v + 1, w) for u, v, w in edges])
            check(name, data, " ".join(str(-1 if d is None else d) for d in expected))

    check("Dijkstra", "4 3 1\n1 2 1000000000\n2 3 1000000000\n3 4 1000000000\n",
          "0 1000000000 2000000000 3000000000")
    for _ in range(90):
        n = RNG.randint(1, 8)
        edges = [(RNG.randrange(n), RNG.randrange(n), RNG.randint(-8, 15))
                 for _ in range(RNG.randrange(25))]
        queries = [(u, v) for u in range(n) for v in range(n)]
        data = f"{n} {len(edges)} {len(queries)}\n"
        data += edge_lines([(u + 1, v + 1, w) for u, v, w in edges])
        data += edge_lines([(u + 1, v + 1) for u, v in queries])
        if has_negative_cycle(n, edges):
            expected = "NEGATIVE CYCLE"
        else:
            distances = [bellman_ford(n, edges, u) for u in range(n)]
            expected = "\n".join("IMPOSSIBLE" if distances[u][v] is None else str(distances[u][v])
                                 for u, v in queries)
        check("Floyd_Warshall", data, expected)
    # Dense negative cycles make unsaturated Floyd values overflow.
    n = 40
    edges = [(u, v, -10**9) for u in range(1, n + 1) for v in range(1, n + 1)]
    check("Floyd_Warshall", f"{n} {len(edges)} 1\n" + edge_lines(edges) + "1 1\n",
          "NEGATIVE CYCLE")
    check("Floyd_Warshall", "2 3 3\n1 1 9\n1 2 8\n1 2 3\n1 1\n1 2\n2 1\n",
          "0\n3\nIMPOSSIBLE")


def brute_mst(n, edges):
    answer = None
    for chosen in combinations(edges, n - 1):
        graph = [[] for _ in range(n)]
        cost = 0
        for u, v, weight in chosen:
            graph[u].append(v)
            graph[v].append(u)
            cost += weight
        seen = {0}
        q = [0]
        for u in q:
            for v in graph[u]:
                if v not in seen:
                    seen.add(v)
                    q.append(v)
        if len(seen) == n and (answer is None or cost < answer):
            answer = cost
    return "IMPOSSIBLE" if answer is None else str(answer)


def check_mst():
    for _ in range(80):
        n = RNG.randint(1, 6)
        edges = [(RNG.randrange(n), RNG.randrange(n), RNG.randint(-20, 20))
                 for _ in range(RNG.randrange(10))]
        data = f"{n} {len(edges)}\n"
        data += edge_lines([(u + 1, v + 1, w) for u, v, w in edges])
        expected = brute_mst(n, edges)
        for name in ["Kruskal", "Prim_Dense"]:
            check(name, data, expected)
    for name in ["Kruskal", "Prim_Dense"]:
        check(name, "4 3\n1 2 1000000000\n2 3 1000000000\n3 4 1000000000\n",
              "3000000000")


def check_scc():
    for _ in range(80):
        n = RNG.randint(1, 12)
        edges = [(RNG.randrange(n), RNG.randrange(n)) for _ in range(RNG.randrange(40))]
        reach = [[u == v for v in range(n)] for u in range(n)]
        for u, v in edges:
            reach[u][v] = True
        for k in range(n):
            for u in range(n):
                for v in range(n):
                    reach[u][v] |= reach[u][k] and reach[k][v]
        groups = {frozenset(v for v in range(n) if reach[u][v] and reach[v][u])
                  for u in range(n)}
        queries = [(u, v) for u in range(n) for v in range(n)]
        data = f"{n} {len(edges)} {len(queries)}\n"
        data += edge_lines([(u + 1, v + 1) for u, v in edges + queries])
        expected = str(len(groups)) + "\n"
        expected += "\n".join("YES" if reach[u][v] and reach[v][u] else "NO" for u, v in queries)
        for name in ["Kosaraju", "Tarjan"]:
            check(name, data, expected)


def check_flow():
    for _ in range(100):
        n = RNG.randint(2, 8)
        source, sink = RNG.sample(range(n), 2)
        edges = [(RNG.randrange(n), RNG.randrange(n), RNG.randint(0, 20))
                 for _ in range(RNG.randrange(35))]
        # Enumerate every source-side cut. This is independent of flow code.
        minimum = None
        for mask in range(1 << n):
            if not (mask & (1 << source)) or (mask & (1 << sink)):
                continue
            capacity = sum(c for u, v, c in edges if mask & (1 << u) and not mask & (1 << v))
            minimum = capacity if minimum is None else min(minimum, capacity)
        data = f"{n} {len(edges)} {source + 1} {sink + 1}\n"
        data += edge_lines([(u + 1, v + 1, c) for u, v, c in edges])
        output = list(map(int, run("Dinic", data).split()))
        flow, size, *vertices = output
        side = {v - 1 for v in vertices}
        assert flow == minimum, (data, flow, minimum)
        assert size == len(side) == len(vertices)
        assert vertices == sorted(vertices) and source in side and sink not in side
        assert sum(c for u, v, c in edges if u in side and v not in side) == flow
    check("Dinic", "2 1 1 1\n1 2 5\n", "INVALID")
    check("Dinic", "2 3 1 2\n1 2 1000000000\n1 2 1000000000\n1 2 1000000000\n",
          "3000000000\n1\n1")


def rooted_tree(n, edges, root):
    graph = [[] for _ in range(n)]
    for u, v in edges:
        graph[u].append(v)
        graph[v].append(u)
    parent, depth = [-1] * n, [0] * n
    order = [root]
    for u in order:
        for v in graph[u]:
            if v != parent[u]:
                parent[v] = u
                depth[v] = depth[u] + 1
                order.append(v)
    return parent, depth


def naive_lca(u, v, parent, depth):
    while depth[u] > depth[v]:
        u = parent[u]
    while depth[v] > depth[u]:
        v = parent[v]
    while u != v:
        u, v = parent[u], parent[v]
    return u


def check_trees():
    for _ in range(70):
        n = RNG.randint(1, 25)
        edges = [(v, RNG.randrange(v)) for v in range(1, n)]
        root = RNG.randrange(n)
        parent, depth = rooted_tree(n, edges, root)
        queries = [(RNG.randrange(n), RNG.randrange(n)) for _ in range(80)]
        data = f"{n} {len(queries)} {root + 1}\n"
        data += edge_lines([(u + 1, v + 1) for u, v in edges + queries])
        expected = []
        for u, v in queries:
            ancestor = naive_lca(u, v, parent, depth)
            expected.append(f"{ancestor + 1} {depth[u] + depth[v] - 2 * depth[ancestor]}")
        for name in ["Binary_Lifting", "Euler_RMQ"]:
            check(name, data, "\n".join(expected))

        values = [RNG.randint(-10**9, 10**9) for _ in range(n)]
        initial = values[:]
        queries, expected = [], []
        for _ in range(100):
            u = RNG.randrange(n)
            if RNG.randrange(2):
                value = RNG.randint(-10**9, 10**9)
                queries.append((1, u + 1, value))
                values[u] = value
            else:
                queries.append((2, u + 1))
                total = 0
                for v in range(n):
                    current = v
                    while current != -1 and current != u:
                        current = parent[current]
                    if current == u:
                        total += values[v]
                expected.append(str(total))
        data = f"{n} {len(queries)} {root + 1}\n" + " ".join(map(str, initial)) + "\n"
        data += edge_lines([(u + 1, v + 1) for u, v in edges])
        data += edge_lines(queries)
        check("Euler_Fenwick", data, "\n".join(expected))


def check_long_chains():
    # These exercise the explicit stacks at the documented n=200000 limit.
    n = 200000
    edges = "".join(f"{u} {u + 1}\n" for u in range(1, n))
    for name in ["Kosaraju", "Tarjan"]:
        check(name, f"{n} {n - 1} 2\n" + edges + f"1 {n}\n{n} {n}\n", f"{n}\nNO\nYES")
    check("BFS", f"{n} {n - 1} 1\n" + edges, " ".join(map(str, range(n))))
    check("DFS", f"{n} {n - 1}\n" + edges, "1\n" + " ".join(["1"] * n))
    for name in ["Binary_Lifting", "Euler_RMQ"]:
        check(name, f"{n} 2 1\n" + edges + f"1 {n}\n{n - 1} {n}\n",
              f"1 {n - 1}\n{n - 1} 1")
    values = " ".join(["1000000000"] * n) + "\n"
    check("Euler_Fenwick", f"{n} 3 1\n" + values + edges + f"2 1\n1 {n} -1000000000\n2 1\n",
          f"{n * 10**9}\n{(n - 2) * 10**9}")
    # Dinic uses bounded recursion: cover its declared maximum depth too.
    n = 500
    weighted = "".join(f"{u} {u + 1} 1000000000\n" for u in range(1, n))
    check("Dinic", f"{n} {n - 1} 1 {n}\n" + weighted, "1000000000\n1\n1")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--skip-compile", action="store_true")
    args = parser.parse_args()
    if args.skip_compile:
        sources = sorted((ROOT / "3_Stars" / "Graphs").rglob("*.cpp"))
        sources += sorted((ROOT / "3_Stars" / "Trees").rglob("*.cpp"))
    else:
        sources = compile_all()
    check_samples(sources)
    for group in [check_traversal, check_dsu, check_shortest_paths, check_mst,
                  check_scc, check_flow, check_trees, check_long_chains]:
        group()
        print(f"Passed: {group.__name__}")
    print(f"PASS: {len(sources)} templates; {RUNS} executable checks, including all samples.")


if __name__ == "__main__":
    main()
