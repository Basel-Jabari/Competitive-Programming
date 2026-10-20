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
An SCC is a maximal group of vertices that can all reach each other.
Tarjan finds SCCs during one DFS. It needs no reversed graph.
discovery[u] is the DFS visit number. low[u] tracks the earliest
active visit reached through u's DFS subtree and an edge to the stack.
Keep unfinished vertices on an active stack, separate from DFS frames.
If low[u] == discovery[u], u starts a complete SCC: pop through u.
For an edge to an active visited vertex v, use discovery[v], not low[v].
Ignore edges to vertices whose SCC has already been removed.
The iterative frames below avoid stack overflow on long chains.
Kosaraju is another O(n+m) method; it is often easier to remember.
Time: O(n + m + q). Memory: O(n + m).
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Tarjan {
private:
  vector<int> _component;
  int _count = 0;

public:
  Tarjan(const vector<vector<int>> &graph) {
    int n = (int)graph.size() - 1;
    vector<int> discovery(n + 1, -1), low(n + 1), parent(n + 1, -1), next(n + 1, 0);
    vector<int> active, dfs;
    vector<bool> onStack(n + 1, false);
    _component.assign(n + 1, -1);
    int timer = 0;

    auto Enter = [&](int u) {
      discovery[u] = low[u] = timer++;
      onStack[u] = true;
      active.push_back(u);
      dfs.push_back(u);
    };

    for (int source = 1; source <= n; ++source) {
      if (discovery[source] != -1) {
        continue;
      }
      Enter(source);
      while (!dfs.empty()) {
        int u = dfs.back();
        if (next[u] < (int)graph[u].size()) {
          int v = graph[u][next[u]++];
          if (discovery[v] == -1) {
            parent[v] = u;
            Enter(v);
          } else if (onStack[v]) {
            low[u] = min(low[u], discovery[v]);
          }
          continue;
        }

        /* All children of u have finished: simulate returning from DFS. */
        dfs.pop_back();
        if (parent[u] != -1) {
          low[parent[u]] = min(low[parent[u]], low[u]);
        }
        if (low[u] == discovery[u]) {
          while (true) {
            int v = active.back();
            active.pop_back();
            onStack[v] = false;
            _component[v] = _count + 1;
            if (v == u) {
              break;
            }
          }
          ++_count;
        }
      }
    }
  }

  int Count() const {
    return _count;
  }

  int Component(int u) const {
    return _component[u]; /* Component IDs are arbitrary and 1-based. */
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
  Tarjan scc(graph); // SCC = strongly connected components.
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
