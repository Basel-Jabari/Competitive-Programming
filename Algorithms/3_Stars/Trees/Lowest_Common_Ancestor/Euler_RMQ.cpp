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
The LCA is the deepest common ancestor.
Record a vertex when DFS enters it AND whenever DFS returns to it.
This Euler walk has exactly 2*n-1 entries. It differs from the
one-entry-per-vertex order used for subtree sums.
Between the first appearances of u and v, the shallowest vertex is
their LCA. A sparse table answers this range minimum query (RMQ).
Two overlapping power-of-two ranges are safe because min(x,x)=x.
Build: O(n log n). Query: O(1). Memory: O(n log n).
Prefer this for many LCA queries; binary lifting also supports jumps.
The tree and root must remain fixed. All DFS frames are iterative.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Euler_RMQ { // RMQ = range minimum query.
private:
  struct Frame {
    int u, parent, next;
  };

  vector<int> _depth, _first, _log;
  vector<vector<int>> _table;

  int Shallower(int u, int v) const {
    return _depth[u] <= _depth[v] ? u : v;
  }

public:
  Euler_RMQ(const vector<vector<int>> &tree, int root) {
    int n = (int)tree.size() - 1;
    _depth.assign(n + 1, 0);
    _first.assign(n + 1, -1);
    vector<int> euler = {root};
    _first[root] = 0;
    vector<Frame> st = {{root, -1, 0}}; // st = stack.
    while (!st.empty()) {
      int u = st.back().u;
      if (st.back().next == (int)tree[u].size()) {
        st.pop_back();
        if (!st.empty()) {
          euler.push_back(st.back().u); /* Return to the parent. */
        }
      } else {
        int v = tree[u][st.back().next++];
        if (v == st.back().parent) {
          continue;
        }
        _depth[v] = _depth[u] + 1;
        _first[v] = euler.size();
        euler.push_back(v);
        st.push_back({v, u, 0});
      }
    }

    int length = euler.size();
    _log.assign(length + 1, 0);
    for (int i = 2; i <= length; ++i) {
      _log[i] = _log[i / 2] + 1;
    }
    _table.assign(_log[length] + 1, vector<int>(length));
    _table[0] = move(euler);
    for (int j = 1; j < (int)_table.size(); ++j) {
      for (int i = 0; i + (1 << j) <= length; ++i) {
        _table[j][i] = Shallower(_table[j - 1][i],
                                 _table[j - 1][i + (1 << (j - 1))]);
      }
    }
  }

  int LCA(int u, int v) const { // LCA = lowest common ancestor.
    int left = _first[u], right = _first[v];
    if (left > right) {
      swap(left, right);
    }
    int j = _log[right - left + 1];
    return Shallower(_table[j][left], _table[j][right - (1 << j) + 1]);
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
  Euler_RMQ rmq(tree, root); // RMQ = range minimum query.
  for (int query = 1; query <= q; ++query) {
    int u, v;
    cin >> u >> v;
    cout << rmq.LCA(u, v) << ' ' << rmq.Distance(u, v) << endl;
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
