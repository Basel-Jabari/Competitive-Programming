/*
Problem: print vertices whose removal increases the number of components.
Input: n m, then m undirected edges u v. Vertex and edge IDs are 1-based.
Limits: 1 <= n <= 200000, 0 <= m <= 300000. Parallel edges allowed; no self-loops.
Output: count, then sorted IDs on the next line (empty if none).

Sample input:
4 4
1 2
2 3
3 1
3 4
Sample output:
1
3

tin[u] is the DFS entry time. low[u] is the earliest entry reachable
from u's subtree using tree edges and at most one back edge.
Skip the PARENT EDGE ID, not every edge to the parent: parallel edges matter.
A non-root p is an articulation point if some child has low[u] >= tin[p].
A DFS root is an articulation point only if it has at least two DFS children.
The explicit DFS stack avoids recursion overflow. Disconnected graphs work.
Time O(n+m); memory O(n+m).
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
  vector<int> next(n + 1), children(n + 1);
  vector<bool> articulation(n + 1, false);
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
          ++children[u];
          tin[v] = low[v] = ++timer;
          stack.push_back(v);
        } else if (tin[v] < tin[u]) {
          low[u] = min(low[u], tin[v]);
        }
      } else {
        stack.pop_back();
        int p = parent[u];
        if (p == 0) {
          articulation[u] = children[u] >= 2;
          continue;
        }
        low[p] = min(low[p], low[u]);
        if (parent[p] != 0 and low[u] >= tin[p]) articulation[p] = true;
      }
    }
  }
  cout << count(articulation.begin(), articulation.end(), true) << endl;
  bool first = true;
  for (int u = 1; u <= n; ++u) {
    if (articulation[u]) {
      if (!first) cout << ' ';
      cout << u;
      first = false;
    }
  }
  cout << endl;
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
