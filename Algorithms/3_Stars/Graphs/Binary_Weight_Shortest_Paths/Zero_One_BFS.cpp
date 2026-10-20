/*
Problem: find minimum distances from s when each directed edge costs 0 or 1.
Input: n m s, then m lines u v w. Vertices are 1-based.
Output: n distances in vertex order; -1 means unreachable.
Limits: 1 <= n <= 200000; 0 <= m <= 300000; w is 0 or 1.
Sample input:
4 4 1
1 2 1
1 3 0
3 2 0
2 4 1
Sample output:
0 0 0 1
Use 0-1 BFS instead of a heap when weights are only 0 and 1.
A zero-cost move goes to the FRONT; a one-cost move goes to the BACK.
The deque processes states in distance order. Skip outdated states.
Time: O(n + m). Memory: O(n + m), including graph and pending states.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<int> Zero_One_BFS(const vector<vector<pair<int, int>>> &graph, int source) {
  const int INF = 1000000000; // INF = infinity (a large marker value).
  vector<int> distance(graph.size(), INF);
  deque<pair<int, int>> dq; // dq = double-ended queue.
  distance[source] = 0;
  dq.push_front({0, source});

  while (!dq.empty()) {
    auto [current, u] = dq.front();
    dq.pop_front();
    if (current != distance[u]) {
      continue;
    }
    for (int index1 = 0; index1 < (int)graph[u].size(); ++index1) {
      auto [v, weight] = graph[u][index1];
      if (distance[v] > current + weight) {
        distance[v] = current + weight;
        if (weight == 0) {
          dq.push_front({distance[v], v});
        } else {
          dq.push_back({distance[v], v});
        }
      }
    }
  }
  for (int index2 = 0; index2 < (int)distance.size(); ++index2) {
    int &value = distance[index2];
    if (value == INF) {
      value = -1;
    }
  }
  return distance;
}

void Solve() {
  int n, m, s;
  cin >> n >> m >> s;
  vector<vector<pair<int, int>>> graph(n + 1);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v, weight;
    cin >> u >> v >> weight;
    graph[u].push_back({v, weight});
  }
  vector<int> distance = Zero_One_BFS(graph, s);
  for (int i = 1; i <= n; ++i) {
    cout << distance[i] << (i == n ? '\n' : ' ');
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
