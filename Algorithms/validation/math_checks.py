"""Compile and test the three-star math templates with independent references.

Run: python Algorithms/validation/math_checks.py --compiler /path/to/g++
The compiler defaults to CXX or g++ on PATH. Only Algorithms/build/math is written.
"""

from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor
import math
import os
from pathlib import Path
import random
import shutil
import subprocess


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "math"
MOD = 1_000_000_007
RNG = random.Random(20261020)
FILES = {
    "gcd": "3_Stars/Number_Theory/GCD_and_LCM/Euclid.cpp",
    "mod": "3_Stars/Number_Theory/Modular_Arithmetic/Power_and_Inverse.cpp",
    "sieve": "3_Stars/Number_Theory/Prime_Sieve/Eratosthenes.cpp",
    "linear": "3_Stars/Number_Theory/Prime_Sieve/Linear_Sieve.cpp",
    "factor": "3_Stars/Number_Theory/Prime_Factorization/Trial_Division.cpp",
    "comb": "3_Stars/Combinatorics/Combinations_and_Permutations/Factorials_Mod_Prime.cpp",
    "pascal": "3_Stars/Combinatorics/Combinations_and_Permutations/Pascal.cpp",
    "stars": "3_Stars/Combinatorics/Stars_and_Bars/Nonnegative_Solutions.cpp",
    "fib": "3_Stars/Combinatorics/Fibonacci/Fast_Doubling.cpp",
    "matrix": "3_Stars/Matrices/Matrix_Multiplication/Rectangular.cpp",
    "walks": "3_Stars/Matrices/Matrix_Exponentiation/Fixed_Length_Walks.cpp",
}


class Checks:
    def __init__(self, compiler: str) -> None:
        self.compiler = compiler
        self.executables: dict[str, Path] = {}
        self.runs = 0
        self.tokens = 0

    def compile_one(self, item: tuple[str, str]) -> tuple[str, Path]:
        name, relative = item
        target = BUILD / (name + (".exe" if os.name == "nt" else ""))
        result = subprocess.run(
            [
                self.compiler, "-std=c++17", "-O2", "-Wall", "-Wextra",
                "-Wshadow", str(ROOT / relative), "-o", str(target),
            ],
            text=True, capture_output=True, timeout=120,
        )
        if result.returncode != 0 or result.stderr.strip():
            raise AssertionError(
                f"Compilation failed or warned: {relative}\n{result.stderr}"
            )
        return name, target

    def compile_all(self) -> None:
        BUILD.mkdir(parents=True, exist_ok=True)
        with ThreadPoolExecutor(max_workers=2) as pool:
            self.executables = dict(pool.map(self.compile_one, FILES.items()))
        print(f"Compiled {len(self.executables)} files without warnings.", flush=True)

    def check(self, name: str, data: str, expected: str) -> None:
        result = subprocess.run(
            [str(self.executables[name])], input=data, text=True,
            capture_output=True, timeout=20,
        )
        if result.returncode != 0:
            raise AssertionError(f"{name} exited {result.returncode}: {result.stderr}")
        actual_tokens, expected_tokens = result.stdout.split(), expected.split()
        if actual_tokens != expected_tokens:
            raise AssertionError(
                f"{name} mismatch\nInput:\n{data[:3000]}\n"
                f"Expected:\n{expected[:3000]}\nActual:\n{result.stdout[:3000]}"
            )
        self.runs += 1
        self.tokens += len(expected_tokens)

    def samples(self) -> None:
        for name, relative in FILES.items():
            source = (ROOT / relative).read_text(encoding="utf-8")
            sample = source.split("Sample input:\n", 1)[1]
            sample_input, sample = sample.split("Sample output:\n", 1)
            sample_output = sample.split("\n\n", 1)[0]
            unquote = lambda text: "\n".join(
                line.removeprefix("// ") for line in text.splitlines()
            ) + "\n"
            self.check(name, unquote(sample_input), unquote(sample_output))
        print("All 11 published samples passed.", flush=True)


def lines(rows) -> str:
    return "\n".join(" ".join(map(str, row)) for row in rows) + "\n"


def prime_factors(n: int) -> list[tuple[int, int]]:
    answer = []
    divisor = 2
    while divisor * divisor <= n:
        exponent = 0
        while n % divisor == 0:
            exponent += 1
            n //= divisor
        if exponent:
            answer.append((divisor, exponent))
        divisor += 1
    if n > 1:
        answer.append((n, 1))
    return answer


def number_theory(checks: Checks) -> None:
    pairs = [(1, 1), (10**9, 10**9), (999999937, 999999929)]
    pairs += [(RNG.randint(1, 10**9), RNG.randint(1, 10**9)) for _ in range(600)]
    checks.check(
        "gcd", f"{len(pairs)}\n" + lines(pairs),
        lines((math.gcd(a, b), math.lcm(a, b)) for a, b in pairs),
    )
    queries = [
        (0, 0, 2), (0, 10**18, 10**18), (1, 0, 10**18),
        (10**18, 10**18, 10**18), (10**18 - 1, 10**18, 10**18),
        (10**18, 10**18, 999999999999999989), (12, 4, 18),
    ]
    queries += [
        (RNG.randint(0, 10**18), RNG.randint(0, 10**18), RNG.randint(2, 10**18))
        for _ in range(700)
    ]
    answers = [
        (pow(a, e, m), pow(a, -1, m) if math.gcd(a, m) == 1 else -1)
        for a, e, m in queries
    ]
    checks.check("mod", f"{len(queries)}\n" + lines(queries), lines(answers))

    for n in [1, 2, 3, 17, 100, 301, 1000]:
        factors = [prime_factors(x) for x in range(2, n + 1)]
        spf = [0, 0] + [f[0][0] for f in factors]
        primes = [False, False] + [spf[x] == x for x in range(2, n + 1)]
        count = sum(primes)
        checks.check(
            "sieve", f"{n} {n + 1}\n" + " ".join(map(str, range(n + 1))),
            f"{count}\n" + "\n".join("YES" if p else "NO" for p in primes),
        )
        checks.check(
            "linear", f"{n} {n}\n" + " ".join(map(str, range(1, n + 1))),
            f"{count}\n" + "\n".join(map(str, spf[1:])),
        )
    checks.check("sieve", "10000000 5\n0 1 2 9999991 10000000\n",
                 "664579\nNO\nNO\nYES\nYES\nNO\n")
    checks.check("linear", "10000000 5\n1 2 9999991 9999999 10000000\n",
                 "664579\n0\n2\n9999991\n3\n2\n")

    numbers = [1, 2, 3, 4, 360, 999983, 999983**2, 10**12, 9999999967]
    numbers += [RNG.randint(1, 10**7) for _ in range(90)]
    for n in numbers:
        expected = prime_factors(n)
        checks.check("factor", f"{n}\n", f"{len(expected)}\n" + lines(expected))
    print("Number theory: random cases and maximum-size sieve passed.", flush=True)


def choose(n: int, r: int) -> int:
    return math.comb(n, r) if 0 <= r <= n else 0


def permute(n: int, r: int) -> int:
    return math.perm(n, r) if 0 <= r <= n else 0


def distributions(n: int, k: int, positive: bool) -> int:
    # Independent counting DP: append one box at a time.
    ways = [1] + [0] * n
    minimum = int(positive)
    for _ in range(k):
        updated = [0] * (n + 1)
        for used in range(n + 1):
            for take in range(minimum, n - used + 1):
                updated[used + take] += ways[used]
        ways = updated
    return ways[n]


def fibonacci_reference(n: int, mod: int) -> int:
    # Independent 2x2 matrix power, rather than the doubling identities.
    def multiply(a, b):
        return [
            [sum(a[i][t] * b[t][j] for t in range(2)) % mod for j in range(2)]
            for i in range(2)
        ]
    answer = [[1, 0], [0, 1]]
    base = [[1, 1], [1, 0]]
    while n:
        if n & 1:
            answer = multiply(answer, base)
        base = multiply(base, base)
        n //= 2
    return answer[0][1]


def combinatorics(checks: Checks) -> None:
    queries = [(0, 0), (0, 1), (1, 10**9), (10**6, 2), (10**6, 0)]
    queries += [(RNG.randrange(151), RNG.randrange(200)) for _ in range(500)]
    answers = [(choose(n, r) % MOD, permute(n, r) % MOD) for n, r in queries]
    checks.check("comb", f"{len(queries)}\n" + lines(queries), lines(answers))
    checks.check("comb", "1\n0 0\n", "1 1\n")
    for maximum, mod in [(0, 2), (60, 2), (60, 6), (60, 97), (60, MOD), (2000, 10**9)]:
        queries = [(0, 0), (maximum, maximum), (maximum, 10**9)]
        queries += [
            (RNG.randint(0, maximum), RNG.randint(0, maximum + 3))
            for _ in range(100)
        ]
        answers = [(choose(n, r) % mod, permute(n, r) % mod) for n, r in queries]
        checks.check(
            "pascal", f"{maximum} {mod} {len(queries)}\n" + lines(queries),
            lines(answers),
        )

    queries = [(n, k) for n in range(13) for k in range(1, 8)]
    answers = [
        (distributions(n, k, False), distributions(n, k, True)) for n, k in queries
    ]
    checks.check("stars", f"{len(queries)}\n" + lines(queries), lines(answers))
    checks.check("stars", "1\n0 1\n", "1 0\n")
    checks.check("stars", "4\n0 1000001\n999999 2\n1000000 1\n1 1000000\n",
                 "1 0\n1000000 999998\n1 1\n1000000 0\n")

    queries = [(0, 2), (1, 2), (2, 2), (93, 1000), (10**18, MOD)]
    queries += [(RNG.randint(0, 10**18), RNG.randint(2, MOD)) for _ in range(350)]
    checks.check(
        "fib", f"{len(queries)}\n" + lines(queries),
        "\n".join(str(fibonacci_reference(n, m)) for n, m in queries),
    )
    print("Combinatorics: exact integer, counting DP, and matrix references passed.",
          flush=True)


def matrices(checks: Checks) -> None:
    dimensions = [(1, 1, 1), (3, 4, 2), (1, 7, 2), (9, 1, 4), (200, 200, 200)]
    dimensions += [
        (RNG.randint(1, 8), RNG.randint(1, 8), RNG.randint(1, 8)) for _ in range(100)
    ]
    for r, k, c in dimensions:
        mod = RNG.choice([2, 6, 97, MOD])
        a = [[RNG.randrange(mod) for _ in range(k)] for _ in range(r)]
        b = [[RNG.randrange(mod) for _ in range(c)] for _ in range(k)]
        expected = [
            [sum(x * y for x, y in zip(a[i], column)) % mod for column in zip(*b)]
            for i in range(r)
        ]
        checks.check("matrix", f"{r} {k} {c} {mod}\n" + lines(a) + lines(b),
                     lines(expected))
    checks.check("matrix", f"1 3 1 {MOD}\n{MOD-1} {MOD-1} {MOD-1}\n"
                 f"{MOD-1}\n{MOD-1}\n{MOD-1}\n", "3\n")

    for _ in range(130):
        n = RNG.randint(1, 7)
        m = RNG.randint(0, 30)
        k = RNG.randint(0, 9)
        edges = [(RNG.randrange(n), RNG.randrange(n)) for _ in range(m)]
        # Direct step-by-step walk counting over edges, preserving duplicates.
        expected = []
        for source in range(n):
            ways = [int(v == source) for v in range(n)]
            for _ in range(k):
                updated = [0] * n
                for u, v in edges:
                    updated[v] = (updated[v] + ways[u]) % MOD
                ways = updated
            expected.append(ways)
        checks.check(
            "walks", f"{n} {m} {k}\n" + lines((u + 1, v + 1) for u, v in edges),
            lines(expected),
        )
    checks.check("walks", "3 0 0\n", "1 0 0\n0 1 0\n0 0 1\n")
    checks.check("walks", "3 0 1000000000000000000\n", "0 0 0\n0 0 0\n0 0 0\n")
    checks.check("walks", "1 3 1000000000000000000\n1 1\n1 1\n1 1\n",
                 f"{pow(3, 10**18, MOD)}\n")
    print("Matrices: rectangular products and edge-by-edge walk counts passed.",
          flush=True)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()
    compiler = shutil.which(args.compiler)
    if compiler is None:
        raise SystemExit(f"Compiler not found: {args.compiler}")
    checks = Checks(compiler)
    checks.compile_all()
    checks.samples()
    number_theory(checks)
    combinatorics(checks)
    matrices(checks)
    print(
        f"PASS: {len(FILES)} templates; {checks.runs} program runs; "
        f"{checks.tokens} expected output tokens checked; seed 20261020."
    )


if __name__ == "__main__":
    main()
