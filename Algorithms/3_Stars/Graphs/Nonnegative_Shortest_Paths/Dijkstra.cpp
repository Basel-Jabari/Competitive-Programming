/*
Problem: find minimum distances from s in a directed weighted graph.
Input: n m s, then m lines u v w (edge u -> v). Vertices are 1-based.
Output: n distances in vertex order; -1 means unreachable.
Limits: 1 <= n <= 200000; 0 <= m <= 300000; 0 <= w <= 1e9.
Sample input:
4 4 1
1 2 7
1 3 2
3 2 1
2 4 4
Sample output:
0 3 2 7
Use Dijkstra only when ALL edge weights are nonnegative.
The min-heap chooses the smallest pending distance.
An older heap entry may remain after a better path is found: skip it.
Time: O(n + m * log(m + 2)) with this lazy heap (parallel edges allowed).
Memory: O(n + m). A simple shortest path costs at most (n-1)*1e9.
Both heap distances and answers must be long long.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

const ll INF = (1LL << 62); // INF = infinity (a large marker value).

vector<ll> Dijkstra(const vector<vector<pair<int, ll>>> &graph, int source) {
  vector<ll> distance(graph.size(), INF);
  priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq; // pq = priority queue.
  distance[source] = 0;
  pq.push({0, source});

  while (!pq.empty()) {
    auto [current, u] = pq.top();
    pq.pop();
    if (current != distance[u]) {
      continue;
    }
    for (int index1 = 0; index1 < (int)graph[u].size(); ++index1) {
      auto [v, weight] = graph[u][index1];
      if (distance[v] > current + weight) {
        distance[v] = current + weight;
        pq.push({distance[v], v});
      }
    }
  }
  return distance;
}

void Solve() {
  int n, m, s;
  cin >> n >> m >> s;
  vector<vector<pair<int, ll>>> graph(n + 1);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    ll weight;
    cin >> u >> v >> weight;
    graph[u].push_back({v, weight});
    /* For an undirected graph, also add graph[v].push_back({u, weight}). */
  }
  vector<ll> distance = Dijkstra(graph, s);
  for (int i = 1; i <= n; ++i) {
    cout << (distance[i] == INF ? -1 : distance[i]);
    cout << (i == n ? '\n' : ' ');
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
