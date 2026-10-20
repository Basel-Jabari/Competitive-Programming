"""Small examples for the preserved original input/output contracts."""
EXAMPLES = {}


def add(ref, contract, data, answer):
    EXAMPLES[ref] = (contract, data.strip() + "\n", answer.strip() + "\n")


add("5", "Problem: Find shortest distances from vertex 1 in a connected undirected graph with nonnegative integer weights.\nInput: n m, followed by m lines x y w.\nOutput: Distances to vertices 2 through n.",
    "4 5\n1 2 7\n1 3 1\n3 2 2\n2 4 4\n3 4 10", "3 1 7")
add("6", "Problem: Find shortest distances from vertex 1 in a connected undirected graph. Every edge weight is 0 or 1.\nInput: n m, followed by m lines x y w.\nOutput: Distances to vertices 2 through n.",
    "4 4\n1 2 1\n1 3 0\n3 2 0\n2 4 1", "0 0 1")
add("7", "Problem: Find all shortest distances in a directed graph, or detect a negative cycle.\nInput: n m, followed by m lines x y w.\nOutput: IMPOSSIBLE if any negative cycle exists. Otherwise print n rows; row i lists destinations in vertex order, skipping i itself. Print INF for an unreachable destination.",
    "3 3\n1 2 5\n2 3 -2\n1 3 10", "5 3\nINF -2\nINF INF")
for ref in ["8", "9"]:
    add(ref, "Problem: Find the minimum spanning tree cost of a connected undirected graph.\nInput: n m, followed by m lines x y w.\nOutput: The total cost. Negative weights are allowed.",
        "4 5\n1 2 1\n2 3 2\n3 4 3\n1 3 8\n1 4 10", "6")
add("10", "Problem: Split a directed graph into strongly connected components.\nInput: n m, followed by m directed edges x y.\nOutput: First print the vertices of each component on a separate line. Then print n lines: vertex and its 0-based component ID.",
    "5 6\n1 2\n2 1\n2 3\n3 4\n4 3\n4 5", "1 2\n3 4\n5\n1 0\n2 0\n3 1\n4 1\n5 2")
add("11", "Problem: Split a directed graph into strongly connected components.\nInput: n m, followed by m directed edges u v.\nOutput: First print n lines: vertex --> component ID. Then print the vertices in each component. IDs start at 0 and can differ from Kosaraju's labels.",
    "5 6\n1 2\n2 1\n2 3\n3 4\n4 3\n4 5", "1 --> 2\n2 --> 2\n3 --> 1\n4 --> 1\n5 --> 0\n5\n4 3\n2 1")
add("12", "Problem: Send the maximum flow from vertex 1 to vertex n in a directed network.\nInput: n m, then m lines x y w, where w is a nonnegative capacity that fits int. Use n >= 2.\nOutput: The maximum total flow.",
    "4 5\n1 2 3\n1 3 2\n2 3 1\n2 4 2\n3 4 3", "5")
TREE = "1 2\n1 3\n2 4\n2 5\n"
for ref in ["13", "14"]:
    add(ref, "Problem: Answer lowest common ancestor queries on a tree rooted at vertex 1.\nInput: n q, then n-1 undirected edges, then q pairs a b.\nOutput: One lowest common ancestor per query.",
        "5 3\n" + TREE + "4 5\n4 3\n2 4", "2\n1\n2")
add("15", "Problem: Replace node values and query subtree sums in a tree rooted at vertex 1.\nInput: n q, n initial integer values, n-1 edges, then queries.\n1 s x: Set node s to x.\n2 s: Print the sum of all values in s's subtree, including s.",
    "5 4\n1 2 3 4 5\n" + TREE + "2 2\n1 4 10\n2 2\n2 1", "11\n17\n21")
add("16", "Problem: Count nodes in a given subtree at a given absolute depth. Root 1 has depth 0.\nInput: n q, n-1 edges, then q lines u l.\nOutput: The number of descendants of u at depth l, including u if its depth is l.",
    "5 4\n" + TREE + "1 2\n2 1\n3 2\n1 8", "2\n1\n0\n0")
add("18", "Problem: Answer sums of rectangles in a fixed square grid.\nInput: n q, an n by n integer grid, then q lines x1 y1 x2 y2.\nOutput: Each inclusive rectangle sum. Coordinates are 1-based.",
    "2 2\n1 2\n3 4\n1 1 2 2\n1 2 2 2", "10\n6")
for ref in ["19", "20"]:
    add(ref, "Problem: Replace array values and answer range sums.\nInput: n q, n integer values, then q queries. Input positions are 1-based.\n1 k u: Set a[k] to u.\n2 x y: Print the inclusive sum from x through y.",
        "5 3\n1 2 3 4 5\n2 2 4\n1 3 10\n2 2 4", "9\n16")
add("21", "Problem: Assign values to points or intervals and query sums.\nInput: n q, n integer values, then q queries. Input positions are 1-based.\n1 k u: Set a[k] to u.\n2 x y u: Set every position in [x,y] to u.\n3 x y: Print the inclusive sum from x through y.",
    "5 5\n1 2 3 4 5\n3 1 5\n2 2 4 0\n3 1 5\n1 3 7\n3 2 4", "15\n6\n7")
for ref in ["23", "24"]:
    add(ref, "Problem: Count how many times pattern p occurs in string s, including overlaps.\nInput: Two nonempty strings s and p. The hashing version requires lowercase English letters.\nOutput: The number of occurrences.",
        "abababa aba", "3")
add("39", "Problem: Find shortest distances from vertex 1 in a directed graph with possible negative weights.\nInput: n m, followed by m lines x y w.\nOutput: IMPOSSIBLE if a negative cycle is reachable from 1. Otherwise print distances to vertices 2 through n; an unreachable vertex is printed as LLONG_MAX.",
    "3 3\n1 2 4\n1 3 9\n2 3 -2", "4 2")
UNDIRECTED = "4 4\n1 2\n2 3\n3 1\n3 4"
add("40", "Problem: Find all bridges in an undirected graph. A bridge is an edge whose removal increases the number of connected components.\nInput: n m, then m endpoint pairs.\nOutput: One pair of bridge endpoints per line, in DFS completion order.",
    UNDIRECTED, "3 4")
add("41", "Problem: Find all articulation vertices in an undirected graph. Removing such a vertex and its edges increases the number of components.\nInput: n m, then m endpoint pairs.\nOutput: The articulation vertices in increasing order.",
    UNDIRECTED, "3")
add("43", "Problem: For every vertex, find the sum of its distances to all other vertices in an unweighted tree.\nInput: n, then n-1 undirected edges.\nOutput: n distance sums in vertex order. Each edge has length 1.",
    "5\n" + TREE, "6 5 9 8 8")
add("45", "Problem: Apply rectangle additions, then print the final grid.\nInput: n m q, the initial n by m integer grid, then q lines x1 y1 x2 y2 value.\nOutput: The grid after all additions. Rectangle endpoints are inclusive and 1-based.",
    "2 3 2\n1 2 3\n4 5 6\n1 1 2 2 10\n2 2 2 3 -3", "11 12 3\n14 12 3")
add("47", "Problem: For each array interval, count positive integers x that appear exactly x times in that interval.\nInput: n q with n,q >= 1, n positive integers, then q inclusive 1-based ranges l r.\nOutput: One count per range, in the original query order.",
    "6 3\n1 2 2 3 3 3\n1 6\n2 5\n4 6", "3\n1\n1")
add("50", "Problem: Find dictionary words in a lowercase-letter grid using up, down, left, and right moves. A cell cannot be reused within one word.\nInput: n m, the grid, w, then w nonempty words.\nOutput: Each found word once, in alphabetical order.",
    "2 2\nab\ncd\n5\nab\nac\nabd\naba\ndc", "ab\nabd\nac\ndc")
add("55", "Problem: Test whether one unsigned 64-bit integer is prime.\nInput: A single integer n, from 0 through 18446744073709551615.\nOutput: YES if prime, otherwise NO.",
    "97", "YES")
add("68", "Problem: Split a loop-free undirected graph into vertex-biconnected edge blocks. Parallel edges are allowed.\nInput: n m, then m endpoint pairs.\nOutput: One block per line; each edge is printed as u <--> v. A bridge forms a one-edge block. Isolated vertices produce no edge block.",
    UNDIRECTED, "3 <--> 4\n3 <--> 1 2 <--> 3 1 <--> 2")
