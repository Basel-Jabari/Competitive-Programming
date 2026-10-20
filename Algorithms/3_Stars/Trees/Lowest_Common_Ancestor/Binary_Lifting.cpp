/*
Problem: answer LCA and path-length queries on a tree rooted at r.
Input: n q r, then n-1 undirected edges, then q lines u v.
Output for each query: the 1-based LCA, then the number of path edges.
Vertices are 1-based. The input must be a connected tree.
Limits: 1 <= n <= 200000; 0 <= q <= 200000.
Sample input:
5 3 1
1 2
1 3
2 4
2 5
4 5
3 4
2 2
Sample output:
2 2
1 3
2 0
The lowest common ancestor is the deepest vertex above both u and v.
up[j][u] is u's ancestor 2^j edges above it.
First lift the deeper vertex. Then lift both while their ancestors
differ; their next parent is the LCA.
Binary lifting also supports the k-th ancestor. Euler + sparse RMQ
gives faster LCA queries, but does not directly give k-th ancestors.
Build: O(n log n). LCA / k-th ancestor: O(log n). Memory: O(n log n).
Preprocessing is iterative, so chains do not overflow the call stack.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Binary_Lifting {
private:
  int _log = 1;
  vector<int> _depth;
  vector<vector<int>> _up;

public:
  Binary_Lifting(const vector<vector<int>> &tree, int root) {
    int n = (int)tree.size() - 1;
    while ((1LL << _log) <= n) {
      ++_log;
    }
    _depth.assign(n + 1, 0);
    _up.assign(_log, vector<int>(n + 1, root));
    vector<int> order = {root};
    for (int i = 0; i < (int)order.size(); ++i) {
      int u = order[i];
      for (int index1 = 0; index1 < (int)tree[u].size(); ++index1) {
        int v = tree[u][index1];
        if (v == _up[0][u]) {
          continue;
        }
        _up[0][v] = u;
        _depth[v] = _depth[u] + 1;
        order.push_back(v);
      }
    }
    for (int j = 1; j < _log; ++j) {
      for (int u = 1; u <= n; ++u) {
        _up[j][u] = _up[j - 1][_up[j - 1][u]];
      }
    }
  }

  int Kth_Ancestor(int u, int k) const {
    /* Vertices are 1-based. k=0 returns u; too high returns -1. */
    if (k < 0 or k > _depth[u]) {
      return -1;
    }
    for (int j = 0; j < _log; ++j) {
      if (k & (1LL << j)) {
        u = _up[j][u];
      }
    }
    return u;
  }

  int LCA(int u, int v) const { // LCA = lowest common ancestor.
    if (_depth[u] < _depth[v]) {
      swap(u, v);
    }
    u = Kth_Ancestor(u, _depth[u] - _depth[v]);
    if (u == v) {
      return u;
    }
    for (int j = _log - 1; j >= 0; --j) {
      if (_up[j][u] != _up[j][v]) {
        u = _up[j][u];
        v = _up[j][v];
      }
    }
    return _up[0][u];
  }

  int Distance(int u, int v) const {
    return _depth[u] + _depth[v] - 2 * _depth[LCA(u, v)];
  }
};

void Solve() {
  int n, q, root;
  cin >> n >> q >> root;
  vector<vector<int>> tree(n + 1);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    tree[u].push_back(v);
    tree[v].push_back(u);
  }
  Binary_Lifting lifting(tree, root);
  for (int query = 1; query <= q; ++query) {
    int u, v;
    cin >> u >> v;
    cout << lifting.LCA(u, v) << ' ' << lifting.Distance(u, v) << endl;
    /* lifting.Kth_Ancestor(u, 3): 1-based ancestor 3 edges above u, or -1. */
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
