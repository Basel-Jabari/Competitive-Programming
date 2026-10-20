/*
Problem: find the connected components of an undirected graph.
Input: n m, then m lines u v. Vertices are 1-based.
Output: the number of components, then each vertex's component number.
Components are numbered from 1, by their smallest vertex.
Limits: 1 <= n <= 200000; 0 <= m <= 300000.
Sample input:
5 3
1 2
2 3
4 5
Sample output:
2
1 1 1 2 2
Use DFS to visit reachable vertices and group connected vertices.
This stack version avoids a recursive call for each vertex in a long chain.
It finds components; it does not provide recursive DFS entry/exit times.
Time: O(n + m). Memory: O(n + m), including the graph.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> graph(n + 1);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  vector<int> component(n + 1, 0);
  int count = 0;
  for (int source = 1; source <= n; ++source) {
    if (component[source] != 0) {
      continue;
    }
    ++count;
    stack<int> st; // st = stack.
    st.push(source);
    component[source] = count;
    while (!st.empty()) {
      int u = st.top();
      st.pop();
      for (int index1 = 0; index1 < (int)graph[u].size(); ++index1) {
        int v = graph[u][index1];
        if (component[v] == 0) {
          /* Mark on push to avoid repeated stack entries. */
          component[v] = count;
          st.push(v);
        }
      }
    }
  }
  cout << count << endl;
  for (int i = 1; i <= n; ++i) {
    cout << component[i] << (i == n ? '\n' : ' ');
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
