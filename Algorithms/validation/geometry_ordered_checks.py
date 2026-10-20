"""Compile and check the three-star geometry/PBDS programs.

Run from any folder with Python 3. Set CXX to select a GNU C++17 compiler.
Only Algorithms/build is used for generated files.
"""

from pathlib import Path
import bisect
import os
import random
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
CXX = os.environ.get("CXX") or shutil.which("g++") or r"C:\MinGW\bin\g++.exe"
RNG = random.Random(20261020)
RUNS = 0


def compile_file(relative):
    source = ROOT / relative
    binary = BUILD / (source.stem + "_geometry.exe")
    subprocess.run(
        [CXX, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
         str(source), "-o", str(binary)],
        check=True, timeout=90,
    )
    return binary


def run(binary, data):
    global RUNS
    result = subprocess.run(
        [str(binary)], input=data, text=True, capture_output=True,
        check=True, timeout=10,
    )
    RUNS += 1
    return result.stdout.strip().splitlines()


def check_ordered(binary):
    sample = "10\n1 5\n1 2\n1 5\n3 5\n4 2\n5 3\n2 5\n3 5\n4 3\n5 1\n"
    assert run(binary, sample) == ["1 3 0 2", "5", "2", "1 2 0 1", "NONE", "5"]
    for _ in range(45):
        values, ops, expected = [], [], []
        for _ in range(250):
            kind = RNG.randrange(1, 6)
            x = RNG.choice([-10**18, 10**18, RNG.randrange(-8, 9)])
            if kind in (4, 5):
                x = RNG.choice([-1, 0, 1, len(values), len(values) + 1,
                                RNG.randrange(1, len(values) + 3), 10**18])
            ops.append(f"{kind} {x}")
            if kind == 1:
                bisect.insort(values, x)
            elif kind == 2:
                if x in values:
                    values.remove(x)
            elif kind == 3:
                expected.append(" ".join(map(str, (
                    sum(v < x for v in values), sum(v <= x for v in values),
                    sum(v > x for v in values), sum(v >= x for v in values),
                ))))
            else:
                expected.append(
                    "NONE" if not 1 <= x <= len(values)
                    else str(values[x - 1] if kind == 4 else values[-x])
                )
        assert run(binary, f"{len(ops)}\n" + "\n".join(ops) + "\n") == expected
    # The final ID and many identical values must not break rank or erase.
    ops = ["1 7"] * 199995 + ["3 7", "4 199995", "5 199996", "2 7", "3 7"]
    assert run(binary, "200000\n" + "\n".join(ops) + "\n") == [
        "0 199995 0 199995", "7", "NONE", "0 199994 0 199994"
    ]


def cross(a, b, c):
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])


def check_vectors(binary):
    assert run(binary, "1 1 4 1 2 3 2\n") == [
        "5 2", "3 0", "2 2", "3 6", "LEFT"
    ]
    cases = [
        ((-10**9, -10**9), (10**9, 10**9), (10**9, 10**9), 10**9),
        ((-10**9, -10**9), (10**9, -10**9), (-10**9, 10**9), -10**9),
        ((0, 0), (2, 2), (-1, -1), 0),
    ]
    for _ in range(150):
        points = [tuple(RNG.randrange(-10**9, 10**9 + 1) for _ in range(2))
                  for _ in range(3)]
        if points[0] != points[1]:
            cases.append((*points, RNG.randrange(-10**9, 10**9 + 1)))
    for a, b, c, k in cases:
        side = cross(a, b, c)
        dot = sum((b[i] - a[i]) * (c[i] - a[i]) for i in range(2))
        expected = [
            f"{a[0] + b[0]} {a[1] + b[1]}",
            f"{b[0] - a[0]} {b[1] - a[1]}",
            f"{a[0] * k} {a[1] * k}",
            f"{dot} {side}",
            "LEFT" if side > 0 else "RIGHT" if side < 0 else "ON",
        ]
        data = " ".join(map(str, (*a, *b, *c, k))) + "\n"
        assert run(binary, data) == expected


def gift_wrap(points):
    # Independent O(n*h) oracle: choose a supporting edge at each vertex.
    points = sorted(set(points))
    if len(points) <= 1:
        return points
    hull, current = [], points[0]
    while True:
        hull.append(current)
        nxt = next(p for p in points if p != current)
        for p in points:
            side = cross(current, nxt, p)
            distance = lambda q: sum((q[i] - current[i]) ** 2 for i in range(2))
            if side < 0 or (side == 0 and distance(p) > distance(nxt)):
                nxt = p
        current = nxt
        if current == points[0]:
            return hull
        assert len(hull) <= len(points)


def check_hull(binary):
    cases = [
        [(0, 0)], [(1, 1)] * 7, [(2, 3), (-1, 0)],
        [(i, 2 * i) for i in range(-6, 7)],
        [(0, 0), (2, 0), (2, 2), (0, 2), (1, 1), (1, 0)],
        [(x, y) for x in (-10**9, 10**9) for y in (-10**9, 10**9)],
    ]
    for _ in range(250):
        cases.append([(RNG.randrange(-12, 13), RNG.randrange(-12, 13))
                      for _ in range(RNG.randrange(1, 35))])
    for points in cases:
        data = f"{len(points)}\n" + "\n".join(f"{x} {y}" for x, y in points) + "\n"
        out = run(binary, data)
        actual = [tuple(map(int, line.split())) for line in out[1:]]
        assert int(out[0]) == len(actual)
        assert actual == gift_wrap(points), (points, actual, gift_wrap(points))
    points = [(i, i) for i in range(200000)]
    out = run(binary, "200000\n" + "\n".join(f"{x} {y}" for x, y in points) + "\n")
    assert out == ["2", "0 0", "199999 199999"]


if __name__ == "__main__":
    BUILD.mkdir(exist_ok=True)
    programs = {
        "ordered": compile_file("3_Stars/CPP_Syntax/Ordered_Set/PBDS.cpp"),
        "vectors": compile_file("3_Stars/Geometry/Vector_Operations/Integer_Vectors.cpp"),
        "hull": compile_file("3_Stars/Geometry/Convex_Hull/Monotone_Chain.cpp"),
    }
    check_ordered(programs["ordered"])
    check_vectors(programs["vectors"])
    check_hull(programs["hull"])
    print(f"PASS: 3 programs, {RUNS} executions; examples, random oracles, bounds.")
