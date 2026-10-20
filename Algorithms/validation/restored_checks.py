"""Compile restored programs and compare their original contracts with independent oracles."""
from concurrent.futures import ThreadPoolExecutor
from collections import Counter, deque
from pathlib import Path
import itertools
import json
import math
import os
import random
import re
import shutil
import subprocess

from original_sources import MAPPING, ROOT, main as check_originals
from original_examples import EXAMPLES

BUILD = ROOT / "build/restored"
COMPILER = shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
RNG = random.Random(20261020)
RUNS = 0


def lines(rows):
    return "\n".join(" ".join(map(str, row)) for row in rows) + "\n"


def run(ref, data):
    global RUNS
    result = subprocess.run([str(BUILD / (ref + ".exe"))], input=data, text=True,
                            capture_output=True, timeout=15)
    assert result.returncode == 0, (ref, data, result.stderr)
    RUNS += 1
    return result.stdout


def check(ref, data, expected):
    actual = run(ref, data).split()
    assert actual == str(expected).split(), (ref, data, actual, expected)


def compile_one(item):
    _, path, ref = item
    result = subprocess.run([COMPILER, "-std=c++17", "-O2", "-DONLINE_JUDGE",
                             "-Wall", "-Wextra", "-Wshadow", str(ROOT / path),
                             "-o", str(BUILD / (ref + ".exe"))],
                            capture_output=True, text=True, timeout=120)
    assert result.returncode == 0, (ref, result.stderr)
    (BUILD / (ref + ".warnings.txt")).write_text(result.stderr, encoding="utf-8")
    return ref, result.stderr.count("warning:")


def distances(n, edges, directed=False):
    inf = 10**100
    d = [[0 if u == v else inf for v in range(n)] for u in range(n)]
    for u, v, w in edges:
        d[u-1][v-1] = min(d[u-1][v-1], w)
        if not directed:
            d[v-1][u-1] = min(d[v-1][u-1], w)
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if d[i][k] < inf and d[k][j] < inf:
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j])
    return d, inf


def component_count(n, edges, removed_node=0, removed_edge=-1):
    seen = {removed_node}
    count = 0
    for start in range(1, n+1):
        if start in seen:
            continue
        count += 1
        pending = [start]
        seen.add(start)
        while pending:
            u = pending.pop()
            for i, (a, b) in enumerate(edges):
                if i == removed_edge:
                    continue
                v = b if a == u else a if b == u else 0
                if v not in seen:
                    seen.add(v)
                    pending.append(v)
    return count


def graph_checks():
    for _ in range(45):
        n = RNG.randint(2, 8)
        base = [(i, RNG.randrange(1, i), RNG.randint(0, 20)) for i in range(2, n+1)]
        base += [(RNG.randint(1, n), RNG.randint(1, n), RNG.randint(0, 20)) for _ in range(n)]
        d, _ = distances(n, base)
        check("5", f"{n} {len(base)}\n" + lines(base), lines([d[0][1:]]))
        binary = [(u, v, w % 2) for u, v, w in base]
        d, _ = distances(n, binary)
        check("6", f"{n} {len(binary)}\n" + lines(binary), lines([d[0][1:]]))
        # Independent cut-growing MST oracle.
        reached, cost = {1}, 0
        while len(reached) < n:
            w, u, v = min((w, u, v) for u, v, w in base if (u in reached) != (v in reached))
            reached.update([u, v])
            cost += w
        for ref in ["8", "9"]:
            check(ref, f"{n} {len(base)}\n" + lines(base), cost)
        directed = [(RNG.randint(1, n), RNG.randint(1, n), RNG.randint(-4, 9)) for _ in range(n*2)]
        d, inf = distances(n, directed, True)
        expected = "IMPOSSIBLE" if any(d[i][i] < 0 for i in range(n)) else lines([
            ["INF" if d[i][j] == inf else d[i][j] for j in range(n) if i != j] for i in range(n)])
        check("7", f"{n} {len(directed)}\n" + lines(directed), expected)
        bad = any(d[0][i] != inf and d[i][i] < 0 for i in range(n))
        expected = "IMPOSSIBLE" if bad else lines([[2**63-1 if x == inf else x for x in d[0][1:]]])
        check("39", f"{n} {len(directed)}\n" + lines(directed), expected)
        # Directed reachability gives the SCC equivalence relation.
        data = f"{n} {len(directed)}\n" + lines([(u, v) for u, v, _ in directed])
        reach, inf = distances(n, [(u, v, 1) for u, v, _ in directed], True)
        for ref in ["10", "11"]:
            output = run(ref, data).strip().splitlines()
            labels = [int(row.split()[-1]) for row in (output[-n:] if ref == "10" else output[:n])]
            assert all((labels[u] == labels[v]) == (reach[u][v] < inf and reach[v][u] < inf)
                       for u in range(n) for v in range(n)), (ref, data, output)
            groups = [set(map(int, row.split())) for row in (output[:-n] if ref == "10" else output[n:])]
            assert groups == [{u+1 for u in range(n) if labels[u] == k} for k in range(len(groups))]
        # Enumerate every source/sink cut as an independent maximum-flow oracle.
        capacity = [(u, v, abs(w)) for u, v, w in directed]
        best = min(sum(w for u, v, w in capacity if mask >> (u-1) & 1 and not(mask >> (v-1) & 1))
                   for mask in range(1 << n) if mask & 1 and not(mask >> (n-1) & 1))
        check("12", f"{n} {len(capacity)}\n" + lines(capacity), best)
        edges = [(u, v) for u, v, _ in base if u != v]
        data = f"{n} {len(edges)}\n" + lines(edges)
        count = component_count(n, edges)
        bridges = {tuple(sorted(e)) for i, e in enumerate(edges)
                   if component_count(n, edges, removed_edge=i) > count}
        actual = {tuple(sorted(map(int, row.split()))) for row in run("40", data).splitlines() if row.strip()}
        assert actual == bridges, (edges, actual, bridges)
        points = [v for v in range(1, n+1) if component_count(n, edges, removed_node=v) > count]
        check("41", data, " ".join(map(str, points)))
        # Edge blocks on small simple graphs: union edges lying on a common simple cycle.
        simple = sorted(set(tuple(sorted(e)) for e in edges))
        parent = list(range(len(simple)))
        def find(i):
            while i != parent[i]:
                i = parent[i]
            return i
        adj = [[] for _ in range(n+1)]
        for i, (u, v) in enumerate(simple):
            adj[u].append((v, i)); adj[v].append((u, i))
        def cycles(start, u, visited, used):
            for v, edge in adj[u]:
                if v == start and len(visited) >= 3:
                    for e in used:
                        parent[find(e)] = find(edge)
                elif v > start and v not in visited:
                    cycles(start, v, visited | {v}, used + [edge])
        for start in range(1, n+1):
            cycles(start, start, {start}, [])
        expected = {frozenset(simple[i] for i in range(len(simple)) if find(i) == k)
                    for k in set(find(i) for i in range(len(simple)))}
        output = run("68", f"{n} {len(simple)}\n" + lines(simple))
        actual = {frozenset(tuple(sorted(map(int, e))) for e in re.findall(r"(\d+) <--> (\d+)", row))
                  for row in output.splitlines() if row.strip()}
        assert actual == expected, (simple, actual, expected)
    for ref in ["5", "8", "9"]:
        check(ref, "3 2\n1 2 2000000000\n2 3 2000000000\n",
              "2000000000 4000000000" if ref == "5" else "4000000000")
    check("7", "2 3\n1 1 8\n1 2 2\n1 2 9\n", "2 INF")
    dense_negative = [(u, v, -2000000000) for u in range(1, 41) for v in range(1, 41)]
    check("7", f"40 {len(dense_negative)}\n" + lines(dense_negative), "IMPOSSIBLE")
    check("68", "2 2\n1 2\n1 2\n", "2 <--> 1 1 <--> 2")
    check("12", "2 2\n1 2 2147483647\n1 2 2147483647\n", "4294967294")
    print("Graphs: shortest paths, MST, SCC, cuts, removals, and cycle-based edge blocks passed.", flush=True)


def tree_checks():
    for _ in range(45):
        n = RNG.randint(1, 35)
        par = [0, 0] + [RNG.randrange(1, i) for i in range(2, n+1)]
        edges = [(i, par[i]) for i in range(2, n+1)]
        depth = [0]*(n+1)
        for i in range(2, n+1):
            depth[i] = depth[par[i]]+1
        def ancestors(v):
            result = []
            while v:
                result.append(v); v = par[v]
            return result
        def lca(u, v):
            av = set(ancestors(v))
            return next(x for x in ancestors(u) if x in av)
        queries = [(RNG.randint(1,n), RNG.randint(1,n)) for _ in range(25)]
        for ref in ["13", "14"]:
            check(ref, f"{n} 25\n" + lines(edges) + lines(queries),
                  " ".join(str(lca(u,v)) for u,v in queries))
        sums = [sum(depth[u]+depth[v]-2*depth[lca(u,v)] for v in range(1,n+1)) for u in range(1,n+1)]
        check("43", f"{n}\n" + lines(edges), " ".join(map(str,sums)))
        queries = [(RNG.randint(1,n), RNG.randint(0,n+3)) for _ in range(25)]
        check("16", f"{n} 25\n" + lines(edges) + lines(queries),
              " ".join(str(sum(u in ancestors(v) and depth[v] == level for v in range(1,n+1))) for u,level in queries))
        a = [0]+[RNG.randint(-100,100) for _ in range(n)]
        initial = a[:]; queries = []; expected = []
        for _ in range(40):
            u = RNG.randint(1,n)
            if RNG.randrange(2):
                x = RNG.randint(-2**31, 2**31-1); a[u]=x; queries.append((1,u,x))
            else:
                queries.append((2,u)); expected.append(sum(a[v] for v in range(1,n+1) if u in ancestors(v)))
        check("15", f"{n} 40\n" + lines([initial[1:]])+lines(edges)+lines(queries), " ".join(map(str,expected)))
    # A deep path below normal stack limits; a wide two-level tree exceeds int sums.
    n = 1000
    check("43", f"{n}\n"+lines((i-1,i) for i in range(2,n+1)),
          " ".join(str(i*(i-1)//2+(n-i)*(n-i+1)//2) for i in range(1,n+1)))
    n = 100000
    check("43", f"{n}\n"+lines((1,i) for i in range(2,n+1)),
          str(n-1)+" "+" ".join([str(2*n-3)]*(n-1)))
    print("Trees: naive ancestors, subtrees, depths, and distance sums passed.", flush=True)


def range_checks():
    for _ in range(60):
        n = RNG.randint(1,30)
        a = [RNG.randint(-2**31,2**31-1) for _ in range(n)]
        initial = a[:]; queries=[]; expected=[]
        for _ in range(40):
            l=RNG.randint(1,n); r=RNG.randint(l,n)
            if RNG.randrange(2):
                value=RNG.randint(-2**31,2**31-1); queries.append((1,l,value)); a[l-1]=value
            else:
                queries.append((2,l,r)); expected.append(sum(a[l-1:r]))
        data=f"{n} 40\n"+lines([initial])+lines(queries)
        for ref in ["19","20"]:
            check(ref,data," ".join(map(str,expected)))
        a=initial[:]; queries=[]; expected=[]
        for _ in range(40):
            l=RNG.randint(1,n); r=RNG.randint(l,n); value=RNG.randint(-100,100)
            op=RNG.randrange(1,4)
            if op==1:
                queries.append((1,l,value)); a[l-1]=value
            elif op==2:
                queries.append((2,l,r,value)); a[l-1:r]=[value]*(r-l+1)
            else:
                queries.append((3,l,r)); expected.append(sum(a[l-1:r]))
        check("21",f"{n} 40\n"+lines([initial])+lines(queries)," ".join(map(str,expected)))
        a=[RNG.randint(1,10) for _ in range(n)]
        queries=[sorted([RNG.randint(1,n),RNG.randint(1,n)]) for _ in range(30)]
        expected=[sum(k==v for k,v in Counter(a[l-1:r]).items()) for l,r in queries]
        check("47",f"{n} 30\n"+lines([a])+lines(queries)," ".join(map(str,expected)))
        n=RNG.randint(1,7); m=RNG.randint(1,7)
        a=[[RNG.randint(-2**31,2**31-1) for _ in range(n)] for _ in range(n)]
        queries=[]; expected=[]
        for _ in range(20):
            x,y=sorted([RNG.randint(1,n),RNG.randint(1,n)])
            l,r=sorted([RNG.randint(1,n),RNG.randint(1,n)])
            queries.append((x,l,y,r)); expected.append(sum(sum(row[l-1:r]) for row in a[x-1:y]))
        check("18",f"{n} 20\n"+lines(a)+lines(queries)," ".join(map(str,expected)))
        a=[[RNG.randint(-100,100) for _ in range(m)] for _ in range(n)]
        initial=[row[:] for row in a]; queries=[]
        for _ in range(20):
            x,y=sorted([RNG.randint(1,n),RNG.randint(1,n)])
            l,r=sorted([RNG.randint(1,m),RNG.randint(1,m)])
            v=RNG.randint(-100,100); queries.append((x,l,y,r,v))
            for i in range(x-1,y):
                for j in range(l-1,r): a[i][j]+=v
        check("45",f"{n} {m} 20\n"+lines(initial)+lines(queries),lines(a))
    print("Ranges: direct arrays and grids, assignments, and original Mo aggregate passed.", flush=True)


def string_math_checks():
    for _ in range(100):
        s="".join(RNG.choices("abc",k=RNG.randint(1,70)))
        p="".join(RNG.choices("abc",k=RNG.randint(1,12)))
        expected=sum(s.startswith(p,i) for i in range(len(s)))
        for ref in ["23","24"]: check(ref,s+" "+p,expected)
    for _ in range(35):
        n=m=3; grid=["".join(RNG.choices("abc",k=m)) for _ in range(n)]
        words=["".join(RNG.choices("abc",k=RNG.randint(1,6))) for _ in range(20)]
        def exists(word):
            def dfs(i,j,k,seen):
                if not(0<=i<n and 0<=j<m) or (i,j) in seen or grid[i][j]!=word[k]: return False
                if k==len(word)-1:return True
                return any(dfs(i+di,j+dj,k+1,seen|{(i,j)}) for di,dj in [(1,0),(-1,0),(0,1),(0,-1)])
            return any(dfs(i,j,0,set()) for i in range(n) for j in range(m))
        expected=sorted(w for w in set(words) if exists(w))
        check("50","3 3\n"+"\n".join(grid)+"\n20\n"+"\n".join(words),"\n".join(expected))
    for n in list(range(100))+[RNG.randrange(10**8) for _ in range(40)]:
        prime=n>=2 and all(n%d for d in range(2,math.isqrt(n)+1))
        check("55",str(n),"YES" if prime else "NO")
    for n,prime in [(18446744073709551557,True),(18446744073709551615,False),
                    (2**61-1,True),(341550071728321,False),(3825123056546413051,False)]:
        check("55",str(n),"YES" if prime else "NO")
    print("Strings and primality: direct matching, word search, trial division, and 64-bit cases passed.", flush=True)


def targeted_regressions():
    # Each modulus alone has a collision, but at a different position.
    # The old min(count1, count2) returned 1; intersecting positions returns 0.
    text, pattern = "nuzeaahpzzzzzzzzlwjmafby", "abcdefgh"
    hits = []
    for base, mod in [(911, 1000000007), (3571, 1000000009)]:
        def hashed(word):
            return sum((ord(c)-96)*pow(base,i+1,mod) for i,c in enumerate(word)) % mod
        hits.append([i for i in range(len(text)-len(pattern)+1)
                     if hashed(text[i:i+len(pattern)]) == hashed(pattern)])
    assert hits == [[0], [16]] and pattern not in text
    check("24", text+" "+pattern, 0)
    # Preserve recursive code; give this depth/overflow regression enough stack.
    if os.name == "nt":
        source = next(ROOT/path for _,path,ref in MAPPING if ref == "43")
        result = subprocess.run([COMPILER, "-std=c++17", "-O2", "-DONLINE_JUDGE",
                                 "-Wl,--stack,134217728", str(source), "-o",
                                 str(BUILD/"43_large_stack.exe")],
                                text=True, capture_output=True, timeout=120)
        assert result.returncode == 0, result.stderr
        n=70000
        check("43_large_stack",f"{n}\n"+lines((i-1,i) for i in range(2,n+1)),
              " ".join(str(i*(i-1)//2+(n-i)*(n-i+1)//2) for i in range(1,n+1)))
    print("Regression cases: different-position hash collisions and large reroot sums passed.", flush=True)


def main():
    check_originals()
    BUILD.mkdir(parents=True,exist_ok=True)
    with ThreadPoolExecutor(max_workers=2) as pool:
        warnings=dict(pool.map(compile_one,MAPPING))
    print("Compiled all 27 restored programs; preserved original warnings are recorded.",flush=True)
    for ref,(_,data,expected) in EXAMPLES.items():
        check(ref,data,expected)
    graph_checks(); tree_checks(); range_checks(); string_math_checks(); targeted_regressions()
    report={"programs":27,"executions":RUNS,"warnings":warnings,"seed":20261020}
    (BUILD/"results.json").write_text(json.dumps(report,indent=2),encoding="utf-8")
    print(f"PASS: {RUNS} complete-program checks across 27 restored sources.",flush=True)


if __name__=="__main__":
    main()
