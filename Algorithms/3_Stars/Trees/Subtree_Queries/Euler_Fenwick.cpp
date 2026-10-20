/*
Problem: maintain values on a fixed tree rooted at r.
Input: n q r, n values, n-1 undirected edges, then q queries.
1 u x: set vertex u's value to x.
2 u: print the sum in u's subtree, including u.
Vertices are 1-based. The input must be a connected tree.
Limits: 1 <= n, q <= 200000; every initial/new value is in [-1e9, 1e9].
Sample input:
5 4 1
1 2 3 4 5
1 2
1 3
2 4
2 5
2 2
1 4 10
2 2
2 1
Sample output:
11
17
21
Enter each vertex once in DFS order. A subtree becomes one interval.
tin[u] is its first position; tout[u] is its last position (inclusive).
Subtree u = [tin[u], tout[u]]. Unlike LCA's Euler walk, no vertex repeats.
A Fenwick tree supports point addition and prefix sums.
To SET a value, add (newValue - oldValue), not newValue itself.
Build: O(n). Update/query: O(log n). Memory: O(n).
Sums can reach n*1e9 in absolute value: use long long.
Preprocessing uses iterative frames, safe for a long chain.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Fenwick_Tree {
private:
  int _n;
  vector<ll> _bit; // BIT = binary indexed tree (Fenwick tree).

public:
  Fenwick_Tree(const vector<ll> &a) : _n((int)a.size() - 1), _bit(_n + 1, 0) {
    /* Linear build: each completed Fenwick block contributes to its parent. */
    for (int i = 1; i <= _n; ++i) {
      _bit[i] += a[i];
      int parent = i + (i & -i);
      if (parent <= _n) {
        _bit[parent] += _bit[i];
      }
    }
  }

  void Add(int index, ll delta) {
    /* Both public indices and the Fenwick array are 1-based. */
    for (int i = index; i <= _n; i += i & -i) {
      _bit[i] += delta;
    }
  }

  ll Prefix(int end) const {
    /* Sum of [1, end]; Prefix(0) = 0 and Prefix(n) is the total. */
    ll answer = 0;
    for (int i = end; i > 0; i -= i & -i) {
      answer += _bit[i];
    }
    return answer;
  }

  ll Query(int left, int right) const {
    return Prefix(right) - Prefix(left - 1); /* [left, right]. */
  }
};

void Solve() {
  int n, q, root;
  cin >> n >> q >> root;
  vector<ll> value(n + 1);
  for (int index1 = 1; index1 < (int)value.size(); ++index1) {
    ll &x = value[index1];
    cin >> x;
  }
  vector<vector<int>> tree(n + 1);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    tree[u].push_back(v);
    tree[v].push_back(u);
  }

  struct Frame {
    int u, parent, next;
  };
  vector<int> tin(n + 1), tout(n + 1); // tin = time in (DFS entry time). tout = time out (last entry time in the subtree).
  vector<Frame> st = {{root, -1, 0}}; // st = stack.
  int timer = 0;
  tin[root] = ++timer;
  while (!st.empty()) {
    int u = st.back().u;
    if (st.back().next == (int)tree[u].size()) {
      tout[u] = timer;
      st.pop_back();
    } else {
      int v = tree[u][st.back().next++];
      if (v == st.back().parent) {
        continue;
      }
      tin[v] = ++timer;
      st.push_back({v, u, 0});
    }
  }

  vector<ll> flattened(n + 1);
  for (int u = 1; u <= n; ++u) {
    flattened[tin[u]] = value[u];
  }
  Fenwick_Tree fenwick(flattened);
  for (int query = 1; query <= q; ++query) {
    int type, u;
    cin >> type >> u;
    if (type == 1) {
      ll x;
      cin >> x;
      fenwick.Add(tin[u], x - value[u]);
      value[u] = x;
    } else {
      cout << fenwick.Query(tin[u], tout[u]) << endl;
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
