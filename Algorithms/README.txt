NDP-Problem Competitive Programming Templates
Basel Al-Jabari

CURRENT STEP
All three levels have C++ templates and are ready for your review.
3_Stars: 29 topics, 38 files.
2_Stars: 27 topics, 30 files.
1_Star:   6 topics,  7 files.
Total:   62 topics, 75 standalone programs.
Use TOPICS.tsv to find a topic, then open a method's complete C++ program.
Preserve chosen numeric constants when updating templates. Double hashing
uses the original (base, modulus) pairs (911, 1000000007) and
(3571, 1000000009). Keep Miller-Rabin's original seven bases unchanged.

FOLDER ORDER
Algorithms / importance / domain / algorithm / method.cpp
Example: Algorithms/3_Stars/Graphs/Minimum_Spanning_Tree/Kruskal.cpp

3_Stars = highest preparation priority: widely useful tools or core building
          blocks, including some hard tools such as maximum flow.
2_Stars = useful next: narrower conditions or more specialized applications.
1_Star  = keep for later: niche tools or smaller benefit for this reference.

Stars measure preparation value for NDP-Problem, NOT difficulty.
This is a suggested ranking, not measured ICPC occurrence rates.
A one-star topic can still be the key to solving a hard contest problem.
Some domains have no three-star topic in the requested scope.
TOPICS.tsv lists every selected topic, its level, methods, reason, and path.
Open it as text or as a table in a spreadsheet application.

HOW THE RANKING WAS CHOSEN
We compared broad practice coverage, common prerequisites, reuse across
problems, and how useful a ready template is during a contest.
Sources checked on 2026-10-20:

1. CSES Problem Set: https://cses.fi/problemset
   Broad practice coverage across graphs, range queries, trees, mathematics,
   strings, and geometry. Its task list is not a frequency study of ICPC.
2. KTH ICPC reference: https://github.com/kth-competitive-programming/kactl
   A real ICPC team reference. Useful for judging what merits template space.
   It omits some simple common tools, so absence does not mean low importance.
3. USACO Guide: https://usaco.guide/gold
   https://usaco.guide/plat and https://usaco.guide/silver
   Useful for prerequisites and broad coverage. Its frequency labels refer to
   USACO divisions; they are not ICPC statistics and were not copied as stars.

METHOD CHOICES: AT MOST TWO FILES PER ALGORITHM FOLDER
Keep methods with a useful time bound for their stated conditions.
Equal time bounds are a reason to compare methods, not to store every variant.

- Minimum spanning tree: Kruskal for an edge list, dense Prim for a dense
  graph or an implicit complete graph. O(E log E) versus O(V^2).
- SCC: Kosaraju and Tarjan, both O(V + E). Two useful implementations.
- LCA: binary lifting for ancestor jumps, Euler walk + sparse table for O(1)
  LCA queries after O(N log N) work. No repeated copies of the same Euler code.
- Maximum flow: Dinic only. Ford-Fulkerson and Edmonds-Karp are omitted from
  the new set because their general time guarantees are weaker.
- Sieve: both requested methods. Linear sieve gives smallest prime factors
  in O(N); Eratosthenes is short and can use less memory, in O(N log log N).
- nCr/nPr: factorials with a suitable prime modulus; Pascal for nCr under
  any modulus and small N. Different modulus conditions justify both files.
- Shortest paths: unweighted, binary-weight, nonnegative-weight, all-pairs,
  and negative-weight problems have different conditions. They are separate
  tasks, not five competing methods for the same unrestricted problem.
- Segment tree: point operations and combined lazy set/add operations.
  No third file for a minor change to the same lazy tree.
- Mo: one array-range file and one tree-path file. The tree example solves
  Permutate Paths using an entry/exit Euler tour, LCA, and missing-value blocks.
- Pattern matching: exact KMP. The separate double-hash template supports
  substring equality; it covers the hashing idea behind Rabin-Karp. A second
  hash-based pattern-count program is not needed for the same task.
- Euler tours: the LCA and subtree files show the two useful visit orders.
  The subtree folder has sum queries and the old depth-count question.
  Repeated copies of the same visit order are not needed.
- Treap and Cartesian tree are range/data structures, not segment-tree
  variants. CHT and Li Chao belong to DP optimization, apart from polygon hulls.
- Matrix range products use a segment tree: multiplication is ordered, so
  always combine the left interval before the right interval.
- Ordinary Bernoulli numbers help with sums of powers. The second-kind file
  answers a separate coefficient question; it is not a second power-sum method.

TEMPLATE STYLE
Each .cpp is a complete GNU C++17 program:
- The question, input, output, limits, and a small example come first.
- Brief notes explain when to use it, complexity, and common mistakes.
- Code follows the recent repository style: two spaces, Solve(), main(),
  fast input/output, and the same author line.
- Longer explanations use /* ... */ blocks. Use // for the Messenger of
  Allah quote, the optional cin >> t, and short abbreviation notes beside
  definitions, such as spf = smallest prime factor.
- Local lambdas keep related state together when useful; reusable data
  structures can still use a class.
- Queries use counted for loops. Vector contents use indexed loops.
- Graph vertices, matrix rows/columns, sequence positions, and inclusive
  query intervals start at 1. Ordered-set helper ranks also start at 1.
- Natural exceptions: C++ string characters, bit masks, polynomial degrees,
  mathematical sequences such as F[0], and native PBDS find_by_order ranks.
  These start at 0; keep their formulas and library syntax as shown.
  Index 0 may also mean a missing child or parent.
- Hard topics receive extra comments. Familiar topics focus on code.
- Indices and valid inputs are stated. Run one file at a time.
- The cerr author line goes to stderr; it is not part of the required output.

GNU PBDS (ordered sets) and __int128 are compiler extensions. Use a GNU
C++17 toolchain. The files that need them say so.
PBDS API reference:
https://gcc.gnu.org/onlinedocs/gcc-13.2.0/libstdc%2B%2B/api/a12961.html

REVIEW NOTES
- "Moduis Function" is recorded as the Mobius function.
- Bernoulli coverage follows the confirmed scope: ordinary numbers, power
  sums, the B1 sign convention, and separate second-kind coefficients.
- "Counting Accumulation" means I+A+...+A^k, as confirmed.
- "RMQ on Matrix Multiplication" means A[l]*...*A[r], as confirmed;
  it is a range-product problem, not a minimum query.
- Replaced old folders are preserved in ../Archives/Legacy_Algorithms_2026-10-20.zip,
  including their five Word documents and the two existing local code edits.
  MIGRATION.txt maps the old topics to their replacements.
- Training remains outside the review, writing, and cleanup scope.

VALIDATION
validation/ contains reproducible checks. The scripts compile their assigned
templates and compare examples and small random cases with simpler solutions.
Build output goes to the ignored build/ folder. See VALIDATION.txt for results
and commands after the current batch has finished.
