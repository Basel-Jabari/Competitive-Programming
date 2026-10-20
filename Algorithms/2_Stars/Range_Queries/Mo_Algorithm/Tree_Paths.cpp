/*
Problem: Permutate Paths (PCPC 2025, Problem A; statement supplied by Basel).
https://codeforces.com/group/Rilx5irOux/contest/640620/problem/A
For each path u..v, find the largest k such that all values 1..k occur.
Duplicates are allowed; both endpoints belong to the path.
Input: n q, values of vertices 1..n, n-1 undirected edges, then q pairs u v.
Output: one answer per query, in input order.
Limits: 2 <= n <= 100000, 1 <= q <= 100000, 1 <= a[i] <= 10^9.
The input is a connected tree. Values do not change. Root the tree at 1.

Sample input:
7 5
1 6 20 3 2 1 4
1 2
2 4
1 3
1 5
5 6
5 7
4 7
1 6
1 3
2 4
7 6
Sample output:
4
2
1
0
2

Mo handles queries OFFLINE: read them all, reorder, then restore output order.
Record each vertex twice: once on entry and once on exit. Euler length = 2*n.
This differs from the one-entry subtree order and the 2*n-1 LCA Euler walk.
Toggle a vertex whenever one of its two positions enters/leaves the interval.
A vertex seen twice cancels out; a vertex seen once stays in the answer.
After swapping endpoints so tin[u] <= tin[v], let w = LCA(u,v):
- w == u: interval [tin[u], tin[v]], with no extra vertex.
- otherwise: interval [tout[u], tin[v]], then add w temporarily.
Here tout[u] is the EXIT occurrence, not the last subtree entry time.
Remove the extra LCA again before moving the interval for the next query.
Toggle VERTICES, not values: two active vertices may share the same value.
This is for tree paths, not arbitrary paths in a graph with cycles.

Answer = first missing POSITIVE value - 1.
Ordinary MEX (minimum excluded value) starts at 0; this problem starts at 1.
A path has at most n vertices, so the answer is at most n. Ignore values > n.
Keep n+1 as an always-missing sentinel. DO NOT compress values: gaps matter.
For example, {1,20} has answer 1, not 2.
Split values 1..n+1 into blocks of size S=floor(sqrt(n+1)).
missing[b] counts values with frequency 0 in value block b.
A toggle changes missing[b] only on frequency transitions 0->1 or 1->0.
To answer, find the first block with a missing value, then scan that block.
Example path 4..7: values {3,6,1,2,4}; first missing = 5; answer = 4.

Let N=2*n and B=max(1,floor(N/sqrt(max(1,q)))).
Time O(n log n + q log n + q log(q+1) + N*N/B + q*B + N
       + q*(n/S + S)).
Each toggle is O(1); finding the answer is O(sqrt(n)).
Memory O(n log n + q). The statement allows 3 seconds and 256 MB;
Mo can make many pointer moves, so keep toggles cheap and test large inputs.
Tree traversal is iterative, safe for a chain.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; ++i) cin >> a[i];
  vector<vector<int>> tree(n + 1);
  for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    tree[u].push_back(v);
    tree[v].push_back(u);
  }

  int levels = 1;
  while ((1LL << levels) <= n) ++levels;
  vector<vector<int>> up(levels + 1, vector<int>(n + 1, 1));
  vector<int> depth(n + 1), next(n + 1), euler(2 * n + 1);
  vector<int> tin(n + 1), tout(n + 1); // tin = time in; tout = time out (exit occurrence).
  vector<int> stack = {1};
  int timer = 0;
  tin[1] = ++timer;
  euler[timer] = 1;
  while (!stack.empty()) {
    int u = stack.back();
    if (next[u] == (int)tree[u].size()) {
      tout[u] = ++timer;
      euler[timer] = u;
      stack.pop_back();
    } else {
      int v = tree[u][next[u]++];
      if (v == up[1][u]) continue;
      depth[v] = depth[u] + 1;
      up[1][v] = u;
      for (int j = 2; j <= levels; ++j) up[j][v] = up[j - 1][up[j - 1][v]];
      tin[v] = ++timer;
      euler[timer] = v;
      stack.push_back(v);
    }
  }
  /* up[j][u] is the ancestor 2^(j-1) edges above u. Levels start at 1. */
  auto LCA = [&](int u, int v) { // LCA = lowest common ancestor.
    if (depth[u] < depth[v]) swap(u, v);
    int difference = depth[u] - depth[v];
    for (int j = 1; j <= levels; ++j) {
      if (difference & (1LL << (j - 1))) u = up[j][u];
    }
    if (u == v) return u;
    for (int j = levels; j >= 1; --j) {
      if (up[j][u] != up[j][v]) {
        u = up[j][u];
        v = up[j][v];
      }
    }
    return up[1][u];
  };

  struct Query { int l, r, extra, id; }; // id = identifier (original query number).
  vector<Query> queries(q + 1);
  for (int i = 1; i <= q; ++i) {
    int u, v;
    cin >> u >> v;
    if (tin[u] > tin[v]) swap(u, v);
    int ancestor = LCA(u, v);
    if (ancestor == u) queries[i] = {tin[u], tin[v], 0, i};
    else queries[i] = {tout[u], tin[v], ancestor, i};
  }
  int blockSize = max(1, (int)(timer / sqrt(max(1, q))));
  sort(queries.begin() + 1, queries.end(), [&](const Query &first, const Query &second) {
    int leftA = (first.l - 1) / blockSize, leftB = (second.l - 1) / blockSize;
    if (leftA != leftB) return leftA < leftB;
    return leftA % 2 ? first.r > second.r : first.r < second.r;
  });

  int valueBlockSize = max(1, (int)sqrt(n + 1));
  int valueBlocks = (n + valueBlockSize) / valueBlockSize;
  vector<int> frequency(n + 2), valueBlock(n + 2), missing(valueBlocks + 1);
  for (int x = 1; x <= n + 1; ++x) {
    valueBlock[x] = (x - 1) / valueBlockSize + 1;
    ++missing[valueBlock[x]];
  }
  vector<int> answer(q + 1);
  vector<char> active(n + 1, false);
  int left = 1, right = 0;
  auto Toggle_Vertex = [&](int u) {
    int x = a[u];
    if (x <= n) {
      if (active[u]) {
        if (--frequency[x] == 0) ++missing[valueBlock[x]];
      } else {
        if (++frequency[x] == 1) --missing[valueBlock[x]];
      }
    }
    active[u] = !active[u];
  };
  auto Toggle_Position = [&](int pos) { Toggle_Vertex(euler[pos]); };
  auto Get_Answer = [&]() {
    for (int b = 1; b <= valueBlocks; ++b) {
      if (missing[b] == 0) continue;
      int end = min(n + 1, b * valueBlockSize);
      for (int x = (b - 1) * valueBlockSize + 1; x <= end; ++x) {
        if (frequency[x] == 0) return x - 1;
      }
    }
    return n; /* The always-missing value n+1 guarantees a return above. */
  };
  for (int i = 1; i <= q; ++i) {
    auto [l, r, extra, id] = queries[i];
    while (left > l) Toggle_Position(--left);
    while (right < r) Toggle_Position(++right);
    while (left < l) Toggle_Position(left++);
    while (right > r) Toggle_Position(right--);
    if (extra) Toggle_Vertex(extra);
    answer[id] = Get_Answer();
    if (extra) Toggle_Vertex(extra);
  }
  for (int i = 1; i <= q; ++i) cout << answer[i] << endl;
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
