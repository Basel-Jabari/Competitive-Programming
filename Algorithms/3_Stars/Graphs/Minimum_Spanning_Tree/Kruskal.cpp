/*
Problem: connect all n cities with minimum total road cost.
Input: n m, then m undirected roads u v w. Vertices are 1-based.
Output: the minimum spanning tree cost, or IMPOSSIBLE if disconnected.
Limits: 1 <= n <= 200000; 0 <= m <= 300000; |w| <= 1e9.
Parallel edges, self-loops, and negative costs are allowed.
Sample input:
4 5
1 2 3
2 3 1
3 4 2
1 4 10
1 3 5
Sample output:
6
A spanning tree connects every vertex with n-1 edges and no cycles.
Kruskal tries edges from cheapest to most expensive.
Accept an edge only if its endpoints are in different DSU groups.
Prefer Kruskal for an edge list / sparse graph. Dense Prim is O(n^2)
and can be better for dense or implicit complete graphs.
Time: O(n + m * log(m + 2)). Memory: O(n + m).
Total cost may reach (n-1)*1e9 in absolute value: use long long.
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
};

struct Edge {
  int u, v;
  ll weight;
};

void Solve() {
  int n, m;
  cin >> n >> m;
  vector<Edge> edges(m);
  for (int index1 = 0; index1 < (int)edges.size(); ++index1) {
    Edge &edge = edges[index1];
    cin >> edge.u >> edge.v >> edge.weight;
  }
  sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
    return a.weight < b.weight;
  });

  DSU dsu(n); // DSU = disjoint set union.
  ll answer = 0;
  int used = 0;
  for (int index2 = 0; index2 < (int)edges.size(); ++index2) {
    const Edge &edge = edges[index2];
    if (dsu.Unite(edge.u, edge.v)) {
      answer += edge.weight;
      ++used;
      /* Save this edge here if the actual tree is required. */
    }
  }
  if (used != n - 1) {
    cout << "IMPOSSIBLE" << endl;
  } else {
    cout << answer << endl;
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
