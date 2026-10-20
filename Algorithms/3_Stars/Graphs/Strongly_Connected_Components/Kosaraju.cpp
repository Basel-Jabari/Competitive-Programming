/*
Problem: group vertices that can reach each other in a directed graph.
Input: n m q, then m edges u v, then q pairs a b. Vertices are 1-based.
Output: the number of groups, then YES/NO for each pair in the same group.
Limits: 1 <= n <= 200000; 0 <= m <= 300000; 0 <= q <= 200000.
Sample input:
5 6 3
1 2
2 1
2 3
3 4
4 3
4 5
1 2
1 4
3 4
Sample output:
3
YES
NO
YES
A strongly connected component (SCC) is a maximal group in which
every vertex can reach every other vertex.
Replacing each SCC by one vertex gives a directed acyclic graph (DAG):
a directed graph without a directed cycle. This is useful before DP.
Kosaraju: finish DFS on original edges, then process reverse finish
order on reversed edges. Each new traversal finds one component.
This version uses explicit stacks, so a long chain is safe.
Kosaraju and Tarjan both take O(n + m); Kosaraju stores reversed edges.
Time: O(n + m + q). Memory: O(n + m).
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Kosaraju {
private:
  vector<int> _component;
  int _count = 0;

public:
  Kosaraju(const vector<vector<int>> &graph) {
    int n = (int)graph.size() - 1;
    vector<vector<int>> reversed(n + 1);
    for (int u = 1; u <= n; ++u) {
      for (int index1 = 0; index1 < (int)graph[u].size(); ++index1) {
        int v = graph[u][index1];
        reversed[v].push_back(u);
      }
    }
    vector<bool> visited(n + 1, false);
    vector<int> order;
    for (int source = 1; source <= n; ++source) {
      if (visited[source]) {
        continue;
      }
      /* A frame stores (vertex, next edge index): real DFS exit order. */
      vector<pair<int, int>> st = {{source, 0}}; // st = stack.
      visited[source] = true;
      while (!st.empty()) {
        int u = st.back().first;
        int &next = st.back().second;
        if (next == (int)graph[u].size()) {
          order.push_back(u);
          st.pop_back();
        } else {
          int v = graph[u][next++];
          if (!visited[v]) {
            visited[v] = true;
            st.push_back({v, 0});
          }
        }
      }
    }

    _component.assign(n + 1, -1);
    reverse(order.begin(), order.end());
    for (int index2 = 0; index2 < (int)order.size(); ++index2) {
      int source = order[index2];
      if (_component[source] != -1) {
        continue;
      }
      vector<int> st = {source}; // st = stack.
      _component[source] = _count + 1;
      while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int index3 = 0; index3 < (int)reversed[u].size(); ++index3) {
          int v = reversed[u][index3];
          if (_component[v] == -1) {
            _component[v] = _count + 1;
            st.push_back(v);
          }
        }
      }
      ++_count;
    }
  }

  int Count() const {
    return _count;
  }

  int Component(int u) const {
    return _component[u]; /* Component IDs and vertices are 1-based. */
  }
};

void Solve() {
  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<int>> graph(n + 1);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
  }
  Kosaraju scc(graph); // SCC = strongly connected components.
  cout << scc.Count() << endl;
  for (int query = 1; query <= q; ++query) {
    int u, v;
    cin >> u >> v;
    cout << (scc.Component(u) == scc.Component(v) ? "YES" : "NO") << endl;
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
