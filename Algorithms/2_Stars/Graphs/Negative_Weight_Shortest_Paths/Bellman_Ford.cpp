/*
Problem: find distances from s in a directed graph with negative edge weights.
Input: n m s, then m lines u v w. Vertices are 1-based.
Output n tokens: distance, INF if unreachable, or -INF if arbitrarily small.
Limits: 1 <= n <= 2500, 0 <= m <= 5000, |w| <= 10^9.

Sample input:
4 4 1
1 2 2
2 3 -3
3 2 1
3 4 5
Sample output:
0 -INF -INF -INF

Use when edges can be negative. Dijkstra is not valid in that case.
Relax u->v when going through u gives a smaller distance to v.
Without a reachable negative cycle, n-1 rounds are enough.
A change on round n means a reachable negative cycle can affect that vertex.
Follow outgoing edges from these vertices to mark every -INF answer.
Unreachable negative cycles do not affect this source.
Time O(n*m + n + m); memory O(n+m). Values fit ll at these bounds.
For detecting a negative cycle anywhere, initialize all distances to zero.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m, source;
  cin >> n >> m >> source;
  struct Edge { int u, v; ll w; };
  vector<Edge> edges(m + 1);
  vector<vector<int>> graph(n + 1);
  for (int i = 1; i <= m; ++i) {
    cin >> edges[i].u >> edges[i].v >> edges[i].w;
    graph[edges[i].u].push_back(edges[i].v);
  }
  const ll INF = (1LL << 62); // INF = infinity (a large marker value).
  vector<ll> distance(n + 1, INF);
  vector<bool> bad(n + 1, false);
  queue<int> pending;
  distance[source] = 0;
  for (int round = 1; round <= n; ++round) {
    bool changed = false;
    for (int i = 1; i <= m; ++i) {
      auto [u, v, w] = edges[i];
      if (distance[u] != INF and distance[u] + w < distance[v]) {
        distance[v] = distance[u] + w;
        changed = true;
        if (round == n and !bad[v]) {
          bad[v] = true;
          pending.push(v);
        }
      }
    }
    if (!changed) break;
  }
  while (!pending.empty()) {
    int u = pending.front();
    pending.pop();
    for (int i = 0; i < (int)graph[u].size(); ++i) {
      int v = graph[u][i];
      if (!bad[v]) {
        bad[v] = true;
        pending.push(v);
      }
    }
  }
  for (int u = 1; u <= n; ++u) {
    if (u > 1) cout << ' ';
    if (bad[u]) cout << "-INF";
    else if (distance[u] == INF) cout << "INF";
    else cout << distance[u];
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
