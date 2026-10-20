# Competitive Programming Templates

Practical **GNU C++17** templates for **NDP-Problem**, personal practice, and ICPC preparation. Each file starts with a problem, input/output format, and sample, then gives a complete solution with usage notes and complexity.

**Basel Al-Jabari**

[Start here](#start-here) · [Topics](#topics) · [Template style](#template-style) · [Validation](#validation)

## Find a template

The collection has **75 standalone programs across 62 topics**. Start with three stars, then add the tools your team needs.

| Priority | Focus | Topics | C++ files |
| --- | --- | ---: | ---: |
| [★★★ Three stars](Algorithms/3_Stars/) | Core tools and common building blocks | 29 | 38 |
| [★★ Two stars](Algorithms/2_Stars/) | Useful tools for more specific problems | 27 | 30 |
| [★ One star](Algorithms/1_Star/) | Specialized topics to study later | 6 | 7 |
| **Total** | | **62** | **75** |

Stars mean **preparation priority**, not difficulty. A three-star topic can be hard, and a one-star topic can be essential for a particular problem. The ranking is a team study guide, not a count of past ICPC appearances.

Each topic has at most two implementations, with different use cases or useful performance tradeoffs. See the [complete topic catalog](Algorithms/TOPICS.tsv) for every path and the [folder guide](Algorithms/README.txt) for method choices and ranking sources.

```text
Algorithms/
├── 3_Stars/
├── 2_Stars/
├── 1_Star/
│   └── Domain/Algorithm/Method.cpp
├── validation/       # Reproducible checks
├── TOPICS.tsv        # Full topic list, priorities, and paths
├── README.txt        # Method choices and style guide
├── VALIDATION.txt    # Recorded results and commands
└── MIGRATION.txt     # Old paths and their replacements
```

For example: `Algorithms/3_Stars/Graphs/Minimum_Spanning_Tree/Kruskal.cpp`.

## Topics

| Domain | Included topics |
| --- | --- |
| C++ syntax | [Ordered sets with GNU PBDS](Algorithms/3_Stars/CPP_Syntax/Ordered_Set/PBDS.cpp): includes, ranks, bounds, ascending/descending order, and duplicates |
| Graphs | Traversal, disjoint set union, shortest paths, minimum spanning trees, strongly connected components, bridges, articulation points, biconnected components, [maximum flow and minimum cut](Algorithms/3_Stars/Graphs/Maximum_Flow/Dinic.cpp), and bipartite matching |
| Trees | [Lowest common ancestor](Algorithms/3_Stars/Trees/Lowest_Common_Ancestor/), subtree sums, descendant counts by depth, and rerooting |
| Range queries | Prefix sums, difference arrays, Fenwick and segment trees, sparse tables, sqrt decomposition, dynamic segment trees, treaps, Cartesian trees, and [Mo on arrays and tree paths](Algorithms/2_Stars/Range_Queries/Mo_Algorithm/) |
| Strings | KMP, double hashing, tries, and grid word search |
| Number theory | GCD/LCM, modular arithmetic, sieves, factorization, [Euler's totient and large exponents](Algorithms/2_Stars/Number_Theory/Euler_Totient_and_Exponents/), Chinese remainder theorem, cancellation, Miller-Rabin, Mobius function, and Pisano periods |
| Combinatorics | nCr, nPr, stars and bars, Fibonacci, Catalan numbers, Lucas's theorem, and Bernoulli numbers |
| Matrices | Rectangular multiplication, exponentiation, sums of powers, and ordered range products |
| Geometry | Vector operations, rotation, reflection, projection, convex hull, closest pair, and Heron's formula |
| DP optimization | [Convex hull trick](Algorithms/2_Stars/DP_Optimization/Convex_Hull_Trick/) and [Li Chao tree](Algorithms/2_Stars/DP_Optimization/Li_Chao_Tree/) |
| Numerical transforms | FFT, NTT with a modulus/root table, and FWHT for XOR, AND, and OR convolution |

## Start here

Use a GNU C++17 compiler. Some templates use GNU extensions such as `bits/stdc++.h`, policy-based data structures (PBDS), or `__int128`; their comments state the requirements.

1. Find the topic in the [catalog](Algorithms/TOPICS.tsv).
2. Read the problem comment, especially the limits, indexing, and required conditions.
3. Compile **one file at a time**. Every template has its own `main()`.
4. Run its sample, then adapt the example to your problem.

For example, compile Dijkstra's algorithm:

```bash
git clone https://github.com/Basel-3mk/Competitive-Programming.git
cd Competitive-Programming
g++ -std=c++17 -O2 -Wall -Wextra Algorithms/3_Stars/Graphs/Nonnegative_Shortest_Paths/Dijkstra.cpp -o solution
```

Run `./solution` on Linux/macOS, or `.\solution.exe` in PowerShell, and enter the sample input shown at the top of the file.

Check each algorithm's conditions before reusing it. For example, Dijkstra requires nonnegative edge weights, and the tree-path Mo template assumes a fixed tree and fixed node values.

## Template style

- **Problem first:** a concrete task, input/output, limits, and a sample before the implementation.
- **Complete solutions:** required includes, `Solve()`, fast input/output, and `main()` in each file.
- **Practical explanations:** brief notes for familiar ideas; more detail for harder algorithms.
- **Consistent indexing:** 1-based problem positions where suitable. Natural exceptions, such as string offsets and native PBDS ranks, are explained.
- **Local state:** lambdas where useful, indexed loops, and counted query loops.
- **Readable comments:** `/* ... */` for longer notes; `//` for the Messenger of Allah quote, optional `cin >> t;`, and acronym meanings beside definitions.

The author line uses `cerr`; only the required answer is written to `cout`.

## Validation

The [recorded validation](Algorithms/VALIDATION.txt) covers **75 warning-free builds** and **3,719 program executions** using samples, independent small reference solutions, random cases, and boundary inputs. Large cases include long trees and large range-query inputs.

The tree-path Mo example uses **Permutate Paths**, including its sample and checks against direct path searches. These are local checks; no online-judge acceptance is claimed.

To run the structure/style checks or the tree-path Mo checks, use Python 3 and GNU `g++` on your `PATH`:

```bash
python Algorithms/validation/catalog_checks.py
python Algorithms/validation/mo_tree_checks.py
```

The [validation guide](Algorithms/VALIDATION.txt) lists all suites and their results. Generated binaries and temporary files stay in the ignored `Algorithms/build/` folder. Algorithm-specific limits still matter: hashing can collide, treap balance is randomized, and FFT uses floating-point arithmetic.

## Earlier versions

The old algorithm folders were replaced by the importance-first structure. Their original code and Word notes are preserved in the [recovery archive](Archives/), together with a SHA-256 manifest. Use the [migration guide](Algorithms/MIGRATION.txt) to find each replacement.

## License

See [LICENSE](LICENSE) for the repository's Unlicense terms.
