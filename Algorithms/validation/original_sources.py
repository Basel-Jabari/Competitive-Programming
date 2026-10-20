"""Verify original implementations against the recovery archive plus explicit bug fixes.

Run with --restore only when deliberately restoring the archived implementations.
Normal execution is read-only and checks the exact normalized source text.
"""
from pathlib import Path
import difflib
import json
import re
import sys
import zipfile

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parent
MAPPING = [
    ("Graph_Theory/Shortest_Path/Dijkstra_Algorithm.cpp", "3_Stars/Graphs/Nonnegative_Shortest_Paths/Dijkstra.cpp", "5"),
    ("Graph_Theory/Shortest_Path/0-1_BFS.cpp", "3_Stars/Graphs/Binary_Weight_Shortest_Paths/Zero_One_BFS.cpp", "6"),
    ("Graph_Theory/Shortest_Path/Floyd_Warshall_Algorithm.cpp", "3_Stars/Graphs/All_Pairs_Shortest_Paths/Floyd_Warshall.cpp", "7"),
    ("Graph_Theory/Spanning_Trees/Kruskal_Algorithm.cpp", "3_Stars/Graphs/Minimum_Spanning_Tree/Kruskal.cpp", "8"),
    ("Graph_Theory/Spanning_Trees/Prim_Algorithm.cpp", "3_Stars/Graphs/Minimum_Spanning_Tree/Prim.cpp", "9"),
    ("Graph_Theory/Strongly_Connected_Components/Kosaraju_Algorithm.cpp", "3_Stars/Graphs/Strongly_Connected_Components/Kosaraju.cpp", "10"),
    ("Graph_Theory/Strongly_Connected_Components/Tarjan_Algorithm.cpp", "3_Stars/Graphs/Strongly_Connected_Components/Tarjan.cpp", "11"),
    ("Graph_Theory/Flow/Dinic_Algorithm.cpp", "3_Stars/Graphs/Maximum_Flow/Dinic.cpp", "12"),
    ("Graph_Theory/Lowest_Common_Ancestor/Binary_Lifting.cpp", "3_Stars/Trees/Lowest_Common_Ancestor/Binary_Lifting.cpp", "13"),
    ("Graph_Theory/Lowest_Common_Ancestor/Euler_Tour_2.cpp", "3_Stars/Trees/Lowest_Common_Ancestor/Euler_RMQ.cpp", "14"),
    ("Graph_Theory/Euler_Tour_3/Fenwick_Tree_On_Tree.cpp", "3_Stars/Trees/Subtree_Queries/Euler_Fenwick.cpp", "15"),
    ("Graph_Theory/Euler_Tour_1/Example.cpp", "3_Stars/Trees/Subtree_Queries/Depth_Count.cpp", "16"),
    ("Range_Queries/No_Data_Structure/2D_Prefix_Sum.cpp", "3_Stars/Range_Queries/Prefix_Sums/Two_Dimensional.cpp", "18"),
    ("Range_Queries/Fenwick_Tree/1D.cpp", "3_Stars/Range_Queries/Fenwick_Tree/Point_Set_Range_Sum.cpp", "19"),
    ("Range_Queries/Segment_Tree/1D_Without_Lazy_Propagation.cpp", "3_Stars/Range_Queries/Segment_Tree/Point_Set_Range_Sum.cpp", "20"),
    ("Range_Queries/Segment_Tree/1D_With_Lazy_Propagation.cpp", "3_Stars/Range_Queries/Segment_Tree/Lazy_Set_Range_Sum.cpp", "21"),
    ("String_Algorithms/Knuth_Morris_Pratt_Algorithm/Example.cpp", "3_Stars/Strings/Pattern_Matching/KMP.cpp", "23"),
    ("String_Algorithms/Rabin_Karp/Example.cpp", "3_Stars/Strings/String_Hashing/Rabin_Karp.cpp", "24"),
    ("Graph_Theory/Shortest_Path/Bellman_Ford_Algorithm.cpp", "2_Stars/Graphs/Negative_Weight_Shortest_Paths/Bellman_Ford.cpp", "39"),
    ("Graph_Theory/Bridges/Tarjan_Algorithm.cpp", "2_Stars/Graphs/Bridges/Low_Link.cpp", "40"),
    ("Graph_Theory/Articulation_Points/Tarjan_Algorithm.cpp", "2_Stars/Graphs/Articulation_Points/Low_Link.cpp", "41"),
    ("Graph_Theory/Re-Rooting_Tree/Example.cpp", "2_Stars/Trees/Rerooting/Distance_Sums.cpp", "43"),
    ("Range_Queries/No_Data_Structure/2D_Range_Sum.cpp", "2_Stars/Range_Queries/Difference_Arrays/Two_Dimensional.cpp", "45"),
    ("Range_Queries/Mo_Algorithm/1D.cpp", "2_Stars/Range_Queries/Mo_Algorithm/Frequency_Equals_Value.cpp", "47"),
    ("Graph_Theory/Graph_With_Other_Data_Structures/Graph_And_Trie.cpp", "2_Stars/Strings/Grid_Word_Search/Trie_Search.cpp", "50"),
    ("Temp_Rabin_Miller_Test/Rabin_Miller_Test.cpp", "2_Stars/Number_Theory/Miller_Rabin/Unsigned_64_Bit.cpp", "55"),
    ("Graph_Theory/Biconnected_Components/Tarjan_Algorithm.cpp", "1_Star/Graphs/Biconnected_Components/Edge_Blocks.cpp", "68"),
]
RENAMES = {
    "3_Stars/Graphs/Minimum_Spanning_Tree/Prim_Dense.cpp": MAPPING[4][1],
    "3_Stars/Range_Queries/Fenwick_Tree/Point_Add_Range_Sum.cpp": MAPPING[13][1],
    "3_Stars/Range_Queries/Segment_Tree/Lazy_Set_Add_Range_Sum.cpp": MAPPING[15][1],
    "3_Stars/Strings/String_Hashing/Double_Hash.cpp": MAPPING[17][1],
    "2_Stars/Range_Queries/Mo_Algorithm/Distinct_Values.cpp": MAPPING[23][1],
}


def normalize(data):
    return re.sub(r"\r+\n", "\n", data.decode("utf-8-sig"))


def fixed(source, ref):
    def replace(old, new):
        nonlocal source
        assert source.count(old) == 1, (ref, old, source.count(old))
        source = source.replace(old, new)

    if ref == "5":
        replace("set<pair<int, int>> dijk;", "set<pair<ll, int>> dijk;")
        replace("dist[1] = INT_MIN;", "dist[1] = 0;")
        replace("int current = dijk.begin()->first, node = dijk.begin()->second;",
                "ll current = dijk.begin()->first;\n\t\tint node = dijk.begin()->second;")
    elif ref == "7":
        replace("dist[x][y] = w;", "dist[x][y] = min(dist[x][y], (ll)w);")
        replace("dist[i][k] + dist[k][j]);",
                "(ll)max((__int128)LLONG_MIN, (__int128)dist[i][k] + dist[k][j]));")
        replace("if (dist[i][i])", "if (dist[i][i] < 0)")
    elif ref in {"8", "9"}:
        replace("int ans = 0;", "ll ans = 0;")
    elif ref == "13":
        start = source.index("\t// Why from 31 not from 0?")
        end = source.index("\tfor (int i = 31;", start)
        source = source[:start] + (
            "\t// For this fixed delta, either bit order works.\n"
            "\t// The second loop below uses descending jumps to stay below the LCA.\n"
        ) + source[end:]
    elif ref == "15":
        replace("void set(int index, int val)", "void set(int index, ll val)")
        replace("int delta = x - a[s];", "ll delta = (ll)x - a[s];")
    elif ref == "16":
        replace("\t\tauto &temp = lvl[l];",
                "\t\tif (l < 0 or l > n)\n\t\t{\n\t\t\tcout << 0 << endl;\n\t\t\tcontinue;\n\t\t}\n\n\t\tauto &temp = lvl[l];")
    elif ref == "18":
        replace("int ans = preSum", "ll ans = preSum")
    elif ref == "19":
        replace("// x, y, k are 0-base.", "// x, y, k are 1-base.")
        replace("_n = a.size();", "_n = a.size() - 1;")
        replace("void Merge(int requiredIndex, int curVal)", "void Merge(int requiredIndex, ll curVal)")
        replace("int delta = newVal - a[requiredIndex];", "ll delta = (ll)newVal - a[requiredIndex];")
        replace("    requiredIndex++;\n", "")
    elif ref == "24":
        replace("  else {\n", "  else {\n    vector<bool> matched(n - m + 1, true);\n")
        replace("\n      if (stringHash == patternHash) {", "\n      matched[0] = matched[0] and stringHash == patternHash;\n      if (matched[0]) {")
        replace("\n        if (stringHash == patternHash) {", "\n        matched[i] = matched[i] and stringHash == patternHash;\n        if (matched[i]) {")
        replace("    cout << min(Rabin_Karp(911, 1e9 + 7), Rabin_Karp(3571, 1e9 + 9)) << endl;",
                "    Rabin_Karp(911, 1e9 + 7);\n    cout << Rabin_Karp(3571, 1e9 + 9) << endl;")
    elif ref in {"40", "68"}:
        replace("        for (int nextNode : adjList[curNode]) {",
                "        bool skippedParent = false;\n        for (int nextNode : adjList[curNode]) {\n"
                "            if (nextNode == prevNode and !skippedParent) {\n"
                "                skippedParent = true;\n                continue;\n            }\n")
        if ref == "40":
            replace("else if (nextNode != prevNode) {", "else {")
        else:
            replace("nextNode != prevNode and discoveryTime[nextNode] < discoveryTime[curNode]",
                    "discoveryTime[nextNode] < discoveryTime[curNode]")
    elif ref == "43":
        replace("vector<int> sum, sz, ans;", "vector<ll> sum, ans;\nvector<int> sz;")
    elif ref == "55":
        replace("(__int128)num1", "(unsigned __int128)num1")
    return source


def main():
    restore = sys.argv[1:] == ["--restore"]
    diffs = []
    with zipfile.ZipFile(REPO / "Archives/Legacy_Algorithms_2026-10-20.zip") as archive:
        for old, new, ref in MAPPING:
            original = normalize(archive.read(old))
            expected = fixed(original, ref)
            destination = ROOT / new
            if restore:
                destination.write_text(expected, encoding="utf-8", newline="\n")
            assert destination.read_text(encoding="utf-8") == expected, new
            diffs.extend(difflib.unified_diff(original.splitlines(True), expected.splitlines(True),
                                            fromfile="archive/" + old, tofile="Algorithms/" + new))
    if restore:
        for old in RENAMES:
            target = (ROOT / old).resolve()
            assert target.is_relative_to(ROOT.resolve())
            if target.exists():
                target.unlink()
        (ROOT / "RESTORATION_BUG_FIXES.patch").write_text("".join(diffs), encoding="utf-8", newline="\n")
    print(f"PASS: {len(MAPPING)} original implementations match the archive plus the documented bug fixes.")


if __name__ == "__main__":
    main()
