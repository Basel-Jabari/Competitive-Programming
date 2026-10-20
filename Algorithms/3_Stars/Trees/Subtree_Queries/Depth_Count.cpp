/*
Problem: count vertices at depth d inside the subtree of u, including u.
Depth means distance in edges from the tree root, not from u.
Input: n q r, then n-1 undirected edges, then q queries u d.
Output: one count per query. The graph must be a connected tree.
Limits: 1 <= n,q <= 200000; vertices and root r are 1-based; 0 <= d <= 10^9.

Sample input:
5 4 1
1 2
1 3
2 4
2 5
1 1
2 2
2 1
3 2
Sample output:
2
2
1
0

Enter each vertex once. Its subtree is the inclusive interval [tin[u],tout[u]].
For each depth, keep the entry times of vertices at that depth.
Times are appended in increasing order, so each depth list is already sorted.
lower_bound finds the first time >= tin[u]; upper_bound finds the first > tout[u].
Their iterator difference counts the times inside the subtree interval.
Depth starts at 0 for the root; vertex IDs and entry times start at 1.
A depth outside [0,n-1] has no vertices. A leaf's tin and tout are equal.
Use this for a fixed tree and fixed depths; no Fenwick tree is needed.
Build O(n); query O(log n); memory O(n). Iterative DFS handles a long chain.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q, root;
  cin >> n >> q >> root;
  vector<vector<int>> graph(n + 1), byDepth(n);
  for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }
  vector<int> tin(n + 1), tout(n + 1), depth(n + 1), parent(n + 1), next(n + 1); // tin = time in (DFS entry time). tout = time out (last entry time in the subtree).
  vector<int> stack = {root};
  int timer = 0;
  tin[root] = ++timer;
  byDepth[0].push_back(tin[root]);
  while (!stack.empty()) {
    int u = stack.back();
    if (next[u] == (int)graph[u].size()) {
      tout[u] = timer;
      stack.pop_back();
    } else {
      int v = graph[u][next[u]++];
      if (v == parent[u]) continue;
      parent[v] = u;
      depth[v] = depth[u] + 1;
      tin[v] = ++timer;
      byDepth[depth[v]].push_back(tin[v]);
      stack.push_back(v);
    }
  }
  auto Count_At_Depth = [&](int u, int d) {
    if (d >= n) return 0;
    const vector<int> &times = byDepth[d];
    auto first = lower_bound(times.begin(), times.end(), tin[u]);
    auto after = upper_bound(times.begin(), times.end(), tout[u]);
    return (int)(after - first);
  };
  for (int i = 1; i <= q; ++i) {
    int u, d;
    cin >> u >> d;
    cout << Count_At_Depth(u, d) << endl;
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
