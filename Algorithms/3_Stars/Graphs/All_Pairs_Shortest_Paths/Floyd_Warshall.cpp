/*
Problem: answer shortest-distance queries in a directed weighted graph.
Input: n m q, then m lines u v w, then q lines a b.
Vertices are 1-based. Output the minimum distance, or IMPOSSIBLE.
If ANY negative cycle exists, print only NEGATIVE CYCLE and stop.
Limits: 1 <= n <= 400; 0 <= m <= 200000; 0 <= q <= 200000;
-1e9 <= w <= 1e9. Negative edges and parallel edges are allowed.
Sample input:
3 3 2
1 2 5
2 3 -2
1 3 10
1 3
3 1
Sample output:
3
IMPOSSIBLE
Use for small n when many pairs are queried. k MUST be the outer loop.
In step k, allow vertex k as an extra middle vertex on each path.
distance[v][v] < 0 detects a negative cycle: no finite minimum on
paths that can enter that cycle and then reach their target.
This problem rejects the whole graph if it has such a cycle.
Time: O(n^3 + m + q). Memory: O(n^2).
Without negative cycles, simple path costs fit in +/- (n-1)*1e9.
Saturate very negative values only to avoid overflow on negative cycles.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

const ll INF = (1LL << 60); // INF = infinity (a large marker value).

void Solve() {
  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<ll>> distance(n + 1, vector<ll>(n + 1, INF));
  for (int i = 1; i <= n; ++i) {
    distance[i][i] = 0;
  }
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    ll weight;
    cin >> u >> v >> weight;
    /* min also preserves zero on a positive self-loop. */
    distance[u][v] = min(distance[u][v], weight);
  }

  for (int k = 1; k <= n; ++k) {
    for (int u = 1; u <= n; ++u) {
      if (distance[u][k] == INF) {
        continue;
      }
      for (int v = 1; v <= n; ++v) {
        if (distance[k][v] == INF) {
          continue; /* Never add the unreachable sentinel. */
        }
        ll candidate = max(-INF, distance[u][k] + distance[k][v]);
        distance[u][v] = min(distance[u][v], candidate);
      }
    }
  }
  bool negativeCycle = false;
  for (int i = 1; i <= n; ++i) {
    negativeCycle |= distance[i][i] < 0;
  }
  if (negativeCycle) {
    cout << "NEGATIVE CYCLE" << endl;
    /* Consume this case's queries so enabling multiple cases remains safe. */
    for (int query = 1; query <= q; ++query) {
      int u, v;
      cin >> u >> v;
    }
    return;
  }
  for (int query = 1; query <= q; ++query) {
    int u, v;
    cin >> u >> v;
    ll answer = distance[u][v];
    if (answer == INF) {
      cout << "IMPOSSIBLE" << endl;
    } else {
      cout << answer << endl;
    }
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
