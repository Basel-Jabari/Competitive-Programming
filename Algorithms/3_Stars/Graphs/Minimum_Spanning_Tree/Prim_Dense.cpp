/*
Problem: connect all n cities with minimum total road cost.
Input: n m, then m undirected roads u v w. Vertices are 1-based.
Output: the minimum spanning tree cost, or IMPOSSIBLE if disconnected.
Limits: 1 <= n <= 2000; 0 <= m <= 2000000; |w| <= 1e9.
Parallel edges, self-loops, and negative costs are allowed.
Sample input:
4 5
1 2 3
2 3 1
3 4 2
1 4 10
1 3 5
Sample output:
6
Prim grows one tree. best[v] is the cheapest edge from this tree to v;
it is NOT a shortest-path distance. Add the unchosen vertex with
minimum best[v], then use its edges to update the remaining choices.
Use for dense graphs; Kruskal is usually simpler for sparse edge lists.
For a complete graph with a formula for cost(u,v), replace the matrix
lookup by that formula. This reduces extra memory to O(n).
Time: O(n^2 + m). Memory here: O(n^2).
Total cost fits in long long under these limits.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

const ll INF = (1LL << 62); // INF = infinity (a large marker value).

void Solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<ll>> weight(n + 1, vector<ll>(n + 1, INF));
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    weight[u][v] = weight[v][u] = min(weight[u][v], w);
  }

  vector<ll> best(n + 1, INF);
  vector<int> parent(n + 1, -1);
  vector<bool> chosen(n + 1, false);
  best[1] = 0; /* The first vertex requires no edge. */
  ll answer = 0;
  for (int step = 0; step < n; ++step) {
    int u = -1;
    for (int v = 1; v <= n; ++v) {
      if (!chosen[v] and (u == -1 or best[v] < best[u])) {
        u = v;
      }
    }
    if (u == -1 or best[u] == INF) {
      cout << "IMPOSSIBLE" << endl;
      return;
    }
    chosen[u] = true;
    answer += best[u];
    /* If parent[u] != -1, edge (parent[u], u) belongs to the MST. */
    for (int v = 1; v <= n; ++v) {
      if (!chosen[v] and weight[u][v] < best[v]) {
        best[v] = weight[u][v];
        parent[v] = u;
      }
    }
  }
  cout << answer << endl;
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
