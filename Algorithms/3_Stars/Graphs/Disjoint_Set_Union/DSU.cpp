/*
Problem: maintain groups of n people, initially one person per group.
Input: n q, then q queries. People are 1-based.
1 u v: merge the groups of u and v.
2 u v: print YES if they are in the same group, otherwise NO.
3 u: print the size of u's group.
Limits: 1 <= n, q <= 200000.
Sample input:
4 5
1 1 2
2 1 3
3 2
1 2 3
2 1 3
Sample output:
NO
2
YES
Use DSU for connectivity with added edges and for Kruskal's MST.
It cannot split a group or directly handle edge deletions.
Path compression shortens paths; union by size joins small under large.
Time: O(n + q * alpha(n)) amortized; alpha grows extremely slowly.
Memory: O(n).
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class DSU { // DSU = disjoint set union.
private:
  vector<int> _parent, _size;

public:
  DSU(int n) : _parent(n + 1), _size(n + 1, 1) {
    iota(_parent.begin(), _parent.end(), 0);
  }

  int Find(int u) {
    while (_parent[u] != u) {
      _parent[u] = _parent[_parent[u]];
      u = _parent[u];
    }
    return u;
  }

  bool Unite(int u, int v) {
    u = Find(u);
    v = Find(v);
    if (u == v) {
      return false;
    }
    if (_size[u] < _size[v]) {
      swap(u, v);
    }
    _parent[v] = u;
    _size[u] += _size[v];
    return true;
  }

  bool Same(int u, int v) {
    return Find(u) == Find(v);
  }

  int Size(int u) {
    return _size[Find(u)];
  }
};

void Solve() {
  int n, q;
  cin >> n >> q;
  DSU dsu(n); // DSU = disjoint set union.
  for (int query = 1; query <= q; ++query) {
    int type, u;
    cin >> type >> u;
    if (type == 3) {
      cout << dsu.Size(u) << endl;
    } else {
      int v;
      cin >> v;
      if (type == 1) {
        dsu.Unite(u, v);
      } else {
        cout << (dsu.Same(u, v) ? "YES" : "NO") << endl;
      }
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
