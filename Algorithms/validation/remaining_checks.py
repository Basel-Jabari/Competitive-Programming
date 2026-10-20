"""Check newly added 2-star/1-star programs, including retained Hopcroft-Karp.
Restored originals are checked separately by restored_checks.py.
Run: python Algorithms/validation/remaining_checks.py [--skip-compile]
Generated files stay in Algorithms/build/remaining.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
from collections import deque
from fractions import Fraction
import argparse
import bisect
import itertools
import math
import random
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "remaining"
RNG = random.Random(20261021)
SOURCES = sorted(list((ROOT / "2_Stars").rglob("*.cpp")) + list((ROOT / "1_Star").rglob("*.cpp")))
# Tree paths have their own BFS-path oracle in mo_tree_checks.py.
SOURCES = [p for p in SOURCES if p != ROOT / "2_Stars/Range_Queries/Mo_Algorithm/Tree_Paths.cpp"]
from original_sources import MAPPING
RESTORED = {ROOT / path for _, path, _ in MAPPING}
SOURCES = [p for p in SOURCES if p not in RESTORED]
FILES = {p.parent.name + "/" + p.stem: p for p in SOURCES}
RUNS = 0

def exe(key):
    return BUILD / (key.replace("/", "_") + ".exe")

def compile_all():
    BUILD.mkdir(parents=True, exist_ok=True)
    compiler = shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
    def one(item):
        key, source = item
        p = subprocess.run([compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
                            str(source), "-o", str(exe(key))], capture_output=True, text=True, timeout=120)
        assert p.returncode == 0 and not p.stderr.strip(), (key, p.stderr)
    with ThreadPoolExecutor(max_workers=2) as pool:
        list(pool.map(one, FILES.items()))
    print(f"Compiled {len(FILES)} programs without warnings.", flush=True)

def run(key, data):
    global RUNS
    p = subprocess.run([str(exe(key))], input=data, capture_output=True, text=True, timeout=20)
    RUNS += 1
    assert p.returncode == 0, (key, data[:500], p.stderr, p.returncode)
    return p.stdout.split()

def check(key, data, expected, floating=False):
    got = run(key, data)
    wanted = str(expected).split()
    if floating:
        assert len(got) == len(wanted) and all(
            math.isclose(float(a), float(b), rel_tol=1e-8, abs_tol=1e-8)
            for a, b in zip(got, wanted)), (key, data, wanted, got)
    else:
        assert got == wanted, (key, data[:2000], wanted[:150], got[:150])

def rows(a):
    return "\n".join(" ".join(map(str, r)) for r in a) + "\n"

def flat(a):
    return " ".join(map(str, a))

def samples():
    for key, path in FILES.items():
        text = path.read_text(encoding="utf-8")
        data, rest = text.split("Sample input:\n", 1)[1].split("Sample output:\n", 1)
        expected = rest.split("\n\n", 1)[0]
        check(key, data, expected, any(s in key for s in ("Reflection", "Components/Projection", "Heron")))
    print("All published samples passed.", flush=True)


def graph_checks():
    for _ in range(50):
        l, r = RNG.randint(1,6), RNG.randint(1,6)
        edges = [(u,v) for u in range(l) for v in range(r) if RNG.randrange(3) == 0]
        adjacency = [set() for _ in range(l)]
        for u,v in edges: adjacency[u].add(v)
        def brute(u, used):
            if u == l: return 0
            return max([brute(u+1,used)]+[1+brute(u+1,used|{v}) for v in adjacency[u]-used])
        expected = brute(0,set())
        out = list(map(int, run("Bipartite_Matching/Hopcroft_Karp",
                               f"{l} {r} {len(edges)}\n"+rows([(u+1,v+1) for u,v in edges]))))
        assert out[0] == expected
        pairs = [(out[1+2*i]-1,out[2+2*i]-1) for i in range(expected)]
        assert len(set(u for u,v in pairs)) == len(set(v for u,v in pairs)) == expected
        assert all(pair in edges for pair in pairs)
        pos = 1+2*expected
        assert out[pos] == expected
        size = out[pos+1]; left = set(v-1 for v in out[pos+2:pos+2+size]); pos += 2+size
        size = out[pos]; right = set(v-1 for v in out[pos+1:pos+1+size])
        assert len(left)+len(right) == expected and all(u in left or v in right for u,v in edges)
    print("Hopcroft-Karp: brute matching and minimum vertex cover passed.", flush=True)


def range_checks():
    for _ in range(35):
        n=RNG.randint(1,30); a=[RNG.randint(-10,10) for _ in range(n)]
        operations=[]; b=a[:]
        for _ in range(50):
            l=RNG.randrange(n); r=RNG.randrange(l,n); x=RNG.randint(-10,10)
            operations.append((l+1,r+1,x))
            for j in range(l,r+1): b[j]+=x
        check("Difference_Arrays/One_Dimensional", f"{n} 50\n"+flat(a)+"\n"+rows(operations),flat(b))
        ops=[]; b=a[:]; expected=[]
        for _ in range(60):
            if RNG.randrange(2):
                pos=RNG.randrange(n); x=RNG.randint(-30,30); b[pos]=x; ops.append((1,pos+1,x))
            else:
                l=RNG.randrange(n); r=RNG.randrange(l,n); ops.append((2,l+1,r+1)); expected.append(sum(b[l:r+1]))
        check("Sqrt_Decomposition/Point_Set_Range_Sum", f"{n} 60\n"+flat(a)+"\n"+rows(ops),flat(expected))
        upper=RNG.choice([1,100,10**18]); values={};ops=[];expected=[]
        for _ in range(80):
            if RNG.randrange(2):
                pos=RNG.choice([1,upper,RNG.randint(1,upper)]); x=RNG.randint(-10**9,10**9)
                values[pos]=values.get(pos,0)+x;ops.append((1,pos,x))
            else:
                l=RNG.randint(1,upper);r=RNG.randint(l,upper);ops.append((2,l,r))
                expected.append(sum(x for p,x in values.items() if l<=p<=r))
        check("Dynamic_Segment_Tree/Point_Add_Range_Sum", f"{upper} 80\n"+rows(ops),flat(expected))
        a=[RNG.randint(-20,20) for _ in range(RNG.randrange(20))]; b=a[:];ops=[];expected=[]
        for _ in range(120):
            kind=RNG.randrange(1,5) if b else 1
            if kind==1:
                pos=RNG.randint(1,len(b)+1);x=RNG.randint(-20,20);b.insert(pos-1,x);ops.append((1,pos,x))
            elif kind==2:
                pos=RNG.randint(1,len(b));b.pop(pos-1);ops.append((2,pos))
            else:
                l=RNG.randrange(len(b));r=RNG.randrange(l,len(b));ops.append((kind,l+1,r+1))
                if kind==3: b[l:r+1]=b[l:r+1][::-1]
                else: expected.append(sum(b[l:r+1]))
        check("Treap/Implicit_Treap",f"{len(a)} 120\n"+flat(a)+"\n"+rows(ops),flat(expected))
        a=[RNG.randint(-5,5) for _ in range(n)]
        out=list(map(int,run("Cartesian_Tree/Monotonic_Stack",f"{n}\n"+flat(a)+"\n")))
        parent=[0]*(n+1);left=[0]*(n+1);right=[0]*(n+1)
        def build(l,r,p):
            if l>r:return 0
            u=min(range(l,r+1),key=lambda i:(a[i-1],i));parent[u]=p
            left[u]=build(l,u-1,u);right[u]=build(u+1,r,u);return u
        root=build(1,n,0)
        assert out == [root]+[x for i in range(1,n+1) for x in (parent[i],left[i],right[i])]
    print("Range structures: naive array/dictionary/sequence oracles passed.",flush=True)


def phi(n):
    return sum(math.gcd(i,n)==1 for i in range(1,n+1))

def math_checks():
    for _ in range(70):
        a=RNG.randrange(1000);m=RNG.randint(1,150)
        e=RNG.choice([RNG.randrange(100),10**RNG.randint(1,50)+RNG.randrange(100)])
        check("Euler_Totient_and_Exponents/Large_Exponent",f"{a} 000{e} {m}\n",f"{phi(m)} {pow(a,e,m)}")
    for n in [1,2,20,100]:
        expected=[(phi(x),sum(math.gcd(a,b)==1 for a in range(1,x+1) for b in range(a+1,x+1))) for x in range(1,n+1)]
        check("Euler_Totient_and_Exponents/Totient_Sieve",f"{n} {n}\n"+"\n".join(map(str,range(1,n+1)))+"\n",rows(expected))
    for _ in range(65):
        equations=[(RNG.randint(-30,30),RNG.randint(1,12)) for _ in range(RNG.randint(1,4))]
        period=math.lcm(*(m for r,m in equations))
        valid=next((x for x in range(period) if all(x%m==r%m for r,m in equations)),None)
        check("Chinese_Remainder_Theorem/Generalized_CRT",f"{len(equations)}\n"+rows(equations),
              "NO SOLUTION" if valid is None else f"{valid} {period}")
    equations=[(RNG.randrange(100),RNG.randrange(100),RNG.randint(1,100)) for _ in range(400)]
    expected=[]
    for a,b,m in equations:
        sol=[x for x in range(m) if (a*x-b)%m==0]
        expected.append("NONE" if not sol else f"{sol[0]} {m//len(sol)} {len(sol)}")
    check("Cancellation_Law/Linear_Congruence",f"{len(equations)}\n"+rows(equations),"\n".join(expected))
    for _ in range(30):
        n,m=RNG.randint(1,100),RNG.randint(1,100)
        check("Mobius_Function/Coprime_Pairs",f"{n} {m}\n",sum(math.gcd(i,j)==1 for i in range(1,n+1) for j in range(1,m+1)))
    for m in range(1,45):
        n=RNG.randrange(500);a,b=0,1
        for _ in range(n):a,b=b,(a+b)
        state=(0,1%m);p=0
        while True:
            state=(state[1],sum(state)%m);p+=1
            if state==(0,1%m):break
        check("Pisano_Period/State_Cycle",f"{m} {n}\n",f"{p} {a%m}")
    mod=1000000007
    ns=list(range(100))
    check("Catalan_Numbers/Balanced_Brackets",f"{len(ns)}\n"+"\n".join(map(str,ns))+"\n",
          flat(math.comb(2*n,n)//(n+1)%mod for n in ns))
    for p in [2,3,5,7,13,101]:
        queries=[(RNG.randrange(500),RNG.randrange(550)) for _ in range(100)]
        check("Lucas_Theorem/Prime_Modulus",f"{p} {len(queries)}\n"+rows(queries),
              flat(math.comb(n,r)%p if r<=n else 0 for n,r in queries))
    queries=[(RNG.randrange(100),RNG.randrange(21)) for _ in range(150)]
    check("Bernoulli_Numbers/Ordinary_Power_Sums",f"20 {len(queries)}\n"+rows(queries),
          flat(sum(pow(i,k,mod) for i in range(1,n+1))%mod for n,k in queries))
    # Exact rational formal-series inversion, separate from modular recurrence.
    b=[Fraction(1)]
    for n in range(1,21):b.append(-sum(Fraction((-1)**j,j+1)*b[n-j] for j in range(1,n+1)))
    check("Bernoulli_Numbers/Second_Kind_Coefficients","20\n",
          flat(x.numerator*pow(x.denominator,-1,mod)%mod for x in b))
    print("Number theory/combinatorics: direct integer and rational references passed.",flush=True)

def multiply(a,b,mod):
    return [[sum(a[i][k]*b[k][j] for k in range(len(b)))%mod for j in range(len(b))] for i in range(len(a))]

def matrix_checks():
    for _ in range(35):
        d=RNG.randint(1,4);mod=RNG.randint(1,1000);k=RNG.randrange(12)
        a=[[RNG.randrange(mod) for _ in range(d)] for _ in range(d)]
        identity=[[int(i==j)%mod for j in range(d)] for i in range(d)]
        total=[row[:] for row in identity];p=identity
        for j in range(k):
            p=multiply(p,a,mod);total=[[(x+y)%mod for x,y in zip(row1,row2)] for row1,row2 in zip(total,p)]
        check("Counting_Accumulation/Power_Sum",f"{d} {k} {mod}\n"+rows(a),rows(total))
        n=RNG.randint(1,9);q=30
        matrices=[[[RNG.randrange(mod) for _ in range(d)] for _ in range(d)] for _ in range(n)]
        data=f"{n} {q} {d} {mod}\n"+"".join(rows(x) for x in matrices);expected=[]
        for j in range(q):
            if RNG.randrange(3)==0:
                pos=RNG.randrange(n);value=[[RNG.randrange(mod) for _ in range(d)] for _ in range(d)]
                matrices[pos]=value;data+=f"2 {pos+1}\n"+rows(value)
            else:
                l=RNG.randrange(n);r=RNG.randrange(l,n);data+=f"1 {l+1} {r+1}\n";p=identity
                for pos in range(l,r+1):p=multiply(p,matrices[pos],mod)
                expected+=p
        check("RMQ_on_Matrix_Multiplication/Range_Product",data,rows(expected))
    print("Matrix sums/range products: repeated multiplication oracle passed.",flush=True)

def geometry_dp_checks():
    for _ in range(60):
        n=RNG.randint(2,35);points=[(RNG.randint(-10**9,10**9),RNG.randint(-10**9,10**9)) for _ in range(n)]
        if RNG.randrange(5)==0:points[1]=points[0]
        best=min((x-u)**2+(y-v)**2 for i,(x,y) in enumerate(points) for u,v in points[i+1:])
        check("Closest_Pair/Sweep_Line",f"{n}\n"+rows(points),best)
    for _ in range(30):
        x,y,ux,uy=[RNG.randint(-100,100) for _ in range(4)]
        if ux==uy==0:ux=1
        scale=(x*ux+y*uy)/(ux*ux+uy*uy);px,py=scale*ux,scale*uy
        check("Parallel_and_Perpendicular_Components/Projection",f"{x} {y} {ux} {uy}\n",
              f"{px} {py} {x-px} {y-py}",True)
        degrees=RNG.randint(-360,360);angle=math.radians(degrees)
        check("Reflection_and_Rotation/Transform_Vector",f"{x} {y} {ux} {uy} {degrees}\n",
              f"{x*math.cos(angle)-y*math.sin(angle)} {x*math.sin(angle)+y*math.cos(angle)} {2*px-x} {2*py-y}",True)
    for sides in [(3,4,5),(2,2,2),(1,2,3),(1,2,4),(5,12,13)]:
        a,b,c=sides;s=sum(sides)/2
        expected="INVALID" if max(sides)>s else str(math.sqrt(max(0,s*(s-a)*(s-b)*(s-c))))
        check("Heron_Formula/Triangle_Area",flat(sides)+"\n",expected,expected!="INVALID")
    for _ in range(60):
        n=RNG.randint(1,50)
        for ordered,key in [(True,"Convex_Hull_Trick/Monotone_DP"),(False,"Li_Chao_Tree/Discrete_DP")]:
            a=[RNG.randint(0 if ordered else -10**6,10**6) for _ in range(n)]
            b=[RNG.randint(0 if ordered else -10**6,10**6) for _ in range(n)]
            if ordered:a.sort();b.sort(reverse=True)
            dp=[0]*n
            for i in range(1,n):dp[i]=min(dp[j]+b[j]*a[i] for j in range(i))
            check(key,f"{n}\n"+flat(a)+"\n"+flat(b)+"\n",dp[-1])
    print("Geometry/DP: pairwise distance and quadratic DP oracles passed.",flush=True)

def transform_checks():
    primes=[(998244353,3,23),(167772161,3,25),(469762049,3,26),
            (754974721,11,24),(1224736769,3,24),(2013265921,31,27)]
    for p,g,power in primes:
        assert all(p%d for d in range(2,math.isqrt(p)+1))
        remaining=p-1;factors=[];d=2
        while d*d<=remaining:
            if remaining%d==0:
                factors.append(d)
                while remaining%d==0:remaining//=d
            d+=1
        if remaining>1:factors.append(remaining)
        assert all(pow(g,(p-1)//q,p)!=1 for q in factors)
        assert (p-1)%(1<<power)==0 and (p-1)%(1<<(power+1))!=0
        w=pow(g,(p-1)//(1<<power),p)
        assert pow(w,1<<power,p)==1 and pow(w,1<<(power-1),p)!=1
        for _ in range(12):
            n,m=RNG.randint(1,40),RNG.randint(1,40)
            a=[RNG.randrange(p) for _ in range(n)];b=[RNG.randrange(p) for _ in range(m)];c=[0]*(n+m-1)
            for i in range(n):
                for j in range(m):c[i+j]=(c[i+j]+a[i]*b[j])%p
            check("NTT/Modular_Convolution",f"{p} {n} {m}\n"+flat(a)+"\n"+flat(b)+"\n",flat(c))
    for _ in range(45):
        n,m=RNG.randint(1,80),RNG.randint(1,80)
        a=[RNG.randint(-1000,1000) for _ in range(n)];b=[RNG.randint(-1000,1000) for _ in range(m)];c=[0]*(n+m-1)
        for i in range(n):
            for j in range(m):c[i+j]+=a[i]*b[j]
        check("FFT/Integer_Convolution",f"{n} {m}\n"+flat(a)+"\n"+flat(b)+"\n",flat(c))
    n=10000
    check("FFT/Integer_Convolution",f"{n} {n}\n"+flat([1000]*n)+"\n"+flat([-1000]*n)+"\n",
          flat(-1000000*min(i+1,2*n-1-i,n) for i in range(2*n-1)))
    mod=998244353
    for kind in ["XOR","AND","OR"]:
        for _ in range(30):
            k=RNG.randrange(6);n=1<<k;a=[RNG.randrange(mod) for _ in range(n)];b=[RNG.randrange(mod) for _ in range(n)];c=[0]*n
            for i in range(n):
                for j in range(n):
                    index=(i^j) if kind=="XOR" else (i&j) if kind=="AND" else (i|j)
                    c[index]=(c[index]+a[i]*b[j])%mod
            check("FWHT/Bitwise_Convolution",f"{kind} {k}\n"+flat(a)+"\n"+flat(b)+"\n",flat(c))
    print("Transforms: convolution oracles, FFT bound, and every NTT prime/root/length passed.",flush=True)

def large_structure_checks():
    # Check the claimed depth/storage limits, which small random cases cannot.
    n = 200000
    total = n*(n+1)//2
    check("Treap/Implicit_Treap", f"{n} 7\n" + flat(range(1, n+1)) +
          f"\n4 1 {n}\n3 1 {n}\n4 1 3\n1 1 -1000000000\n2 {n+1}\n4 1 {n}\n4 {n} {n}\n",
          flat([total, 3*n-3, total-1-1000000000, 2]))
    q = 50000
    check("Dynamic_Segment_Tree/Point_Add_Range_Sum",
          f"1000000000000000000 {q}\n" +
          rows((1, i*10**13, 10**9) for i in range(1, q)) +
          "2 1 1000000000000000000\n", (q-1)*10**9)
    print("Large structures: 200000-value treap and 50000 sparse-tree operations passed.",
          flush=True)

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--skip-compile",action="store_true")
    args=parser.parse_args()
    if not args.skip_compile:compile_all()
    samples()
    for fn in (graph_checks,range_checks,math_checks,matrix_checks,geometry_dp_checks,transform_checks,large_structure_checks):fn()
    print(f"PASS: {len(FILES)} programs; {RUNS} full-program checks; seed 20261021.",flush=True)

if __name__=="__main__":main()
