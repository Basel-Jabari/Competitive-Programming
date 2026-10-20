/*
Problem: an undirected graph has n vertices and m edges.
Find the fewest edges from source s to each vertex. Print -1 if unreachable.
Input: n m s, then m lines u v. Vertices are 1-based.
Limits: 1 <= n <= 200000; 0 <= m <= 300000.
Sample input:
5 3 1
1 2
2 3
1 4
Sample output:
0 1 2 1 -1
Use BFS for shortest paths when every edge has the same cost.
A queue visits distance 0, then distance 1, then distance 2, and so on.
Time: O(n + m). Memory: O(n + m), including the graph.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<int> BFS(const vector<vector<int>> &graph, int source) { // BFS = breadth-first search.
  vector<int> distance(graph.size(), -1);
  queue<int> q;
  distance[source] = 0;
  q.push(source);

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int index1 = 0; index1 < (int)graph[u].size(); ++index1) {
      int v = graph[u][index1];
      if (distance[v] != -1) {
        continue;
      }
      /* Mark when pushing, so each vertex enters the queue once. */
      distance[v] = distance[u] + 1;
      q.push(v);
    }
  }
  return distance;
}

void Solve() {
  int n, m, s;
  cin >> n >> m >> s;
  vector<vector<int>> graph(n + 1);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u); /* Remove this line for a directed graph. */
  }

  vector<int> distance = BFS(graph, s);
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
