/*
Problem: split an undirected graph into vertex-biconnected edge blocks.
Input: n m, then m undirected edges u v. Vertex and edge IDs are 1-based.
Limits: 1 <= n <= 200000, 0 <= m <= 300000. Parallel edges allowed; no self-loops.
Output: block count, then size and sorted edge IDs per block; blocks sorted lexicographically.

Sample input:
4 4
1 2
2 3
3 1
3 4
Sample output:
2
3 1 2 3
1 4

A block is a maximal connected part with no articulation vertex inside it:
removing one vertex does not split its remaining vertices into separate parts.
An articulation vertex is a vertex whose removal increases the component count.
tin[u] is the DFS entry time. low[u] is the earliest entry reachable
from u's subtree using tree edges and at most one back edge.
Skip the PARENT EDGE ID, not every edge to the parent: parallel edges matter.
The explicit DFS stack avoids recursion overflow. Disconnected graphs work.
Keep visited tree/back edges on a separate stack.
When low[child] >= tin[parent], pop through that tree edge to finish a block.
Vertices may belong to several blocks, but each edge belongs to one.
A bridge forms a one-edge block. Isolated vertices have no edge block here.
Useful for block-cut trees and paths constrained by articulation points.
DFS O(n+m); sorting the printed blocks O(m log(m+2)); memory O(n+m).
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> graph(n + 1);
  for (int id = 1; id <= m; ++id) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back({v, id});
    graph[v].push_back({u, id});
  }
  vector<int> tin(n + 1), low(n + 1), parent(n + 1), parentEdge(n + 1); // tin = time in (DFS entry time).
  vector<int> next(n + 1), edgeStack;
  vector<vector<int>> blocks;
  int timer = 0;
  for (int root = 1; root <= n; ++root) {
    if (tin[root]) continue;
    vector<int> stack = {root};
    tin[root] = low[root] = ++timer;
    while (!stack.empty()) {
      int u = stack.back();
      if (next[u] < (int)graph[u].size()) {
        auto [v, id] = graph[u][next[u]++];
        if (id == parentEdge[u]) continue;
        if (!tin[v]) {
          parent[v] = u;
          parentEdge[v] = id;
          edgeStack.push_back(id);
          tin[v] = low[v] = ++timer;
          stack.push_back(v);
        } else if (tin[v] < tin[u]) {
          low[u] = min(low[u], tin[v]);
          edgeStack.push_back(id);
        }
      } else {
        stack.pop_back();
        int p = parent[u];
        if (p == 0) continue;
        low[p] = min(low[p], low[u]);
        if (low[u] >= tin[p]) {
          vector<int> block;
          while (true) {
            int id = edgeStack.back();
            edgeStack.pop_back();
            block.push_back(id);
            if (id == parentEdge[u]) break;
          }
          blocks.push_back(block);
        }
      }
    }
  }
  for (int i = 0; i < (int)blocks.size(); ++i) {
    sort(blocks[i].begin(), blocks[i].end());
  }
  sort(blocks.begin(), blocks.end());
  cout << blocks.size() << endl;
  for (int i = 0; i < (int)blocks.size(); ++i) {
    cout << blocks[i].size();
    for (int j = 0; j < (int)blocks[i].size(); ++j) cout << ' ' << blocks[i][j];
    cout << endl;
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int t = 1;
  // cin >> t;
  for (int i = 1; i <= t; ++i) {
    Solve();
  }

  cerr << "Basel Al-Jabari." << endl;
  return 0;
}
