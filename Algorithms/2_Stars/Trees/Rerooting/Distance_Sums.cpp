/*
Problem: for every vertex, print the sum of weighted distances to all vertices.
Input: n, then n-1 lines u v w describing a connected tree. Vertices are 1-based.
Output n sums in vertex order. Limits: 1 <= n <= 200000, 0 <= w <= 10^6.

Sample input:
3
1 2 2
2 3 3
Sample output:
7 5 8

First root at 1. Compute subtree sizes and the distance sum for root 1.
Moving the root across edge u->v of weight w makes size[v] vertices w closer
and the other n-size[v] vertices w farther:
answer[v] = answer[u] + (n - 2*size[v])*w.
This example shows rerooting: reuse a parent's answer to get a child's.
Other tree DPs need their own merge/remove formulas; this is not universal.
Time O(n); memory O(n). Traversal is iterative. Sums fit ll under these limits.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n;
  cin >> n;
  vector<vector<pair<int, ll>>> tree(n + 1);
  for (int i = 1; i < n; ++i) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    tree[u].push_back({v, w});
    tree[v].push_back({u, w});
  }
  vector<int> parent(n + 1), size(n + 1, 1), order = {1};
  vector<ll> distance(n + 1), parentWeight(n + 1), answer(n + 1);
  for (int i = 0; i < (int)order.size(); ++i) {
    int u = order[i];
    for (int j = 0; j < (int)tree[u].size(); ++j) {
      auto [v, w] = tree[u][j];
      if (v == parent[u]) continue;
      parent[v] = u;
      parentWeight[v] = w;
      distance[v] = distance[u] + w;
      order.push_back(v);
    }
  }
  for (int u = 1; u <= n; ++u) answer[1] += distance[u];
  for (int i = n - 1; i >= 1; --i) {
    int u = order[i];
    size[parent[u]] += size[u];
  }
  for (int i = 1; i < n; ++i) {
    int v = order[i];
    answer[v] = answer[parent[v]] + (n - 2LL * size[v]) * parentWeight[v];
  }
  for (int u = 1; u <= n; ++u) cout << answer[u] << (u == n ? '\n' : ' ');
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
