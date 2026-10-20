/*
Problem: a[1..U] starts at zero, but U can be huge.
Input: U q, then operations: 1 pos delta adds to a[pos]; 2 l r prints sum[l,r].
Limits: 1 <= U <= 10^18, 1 <= q <= 50000, |delta| <= 10^9.
All positions and inclusive ranges are valid and 1-based.

Sample input:
1000000000000000000 4
1 7 5
1 1000000000000000000 9
2 1 10
2 7 1000000000000000000
Sample output:
5
14

Allocate only nodes visited by an update. Missing nodes represent zero.
An ordinary tree needs O(U) memory; this tree needs O(q log U).
Each update/query O(log U). Queries do not allocate nodes.
Use integer node IDs: vector growth can invalidate references.
For known offline positions, coordinate compression may be simpler.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  ll upper;
  int q;
  cin >> upper >> q;
  struct Node { int left = 0, right = 0; ll sum = 0; };
  vector<Node> nodes(1);
  auto New_Node = [&]() {
    nodes.push_back(Node());
    return (int)nodes.size() - 1;
  };
  function<int(int,ll,ll,ll,ll)> Add = [&](int node, ll l, ll r, ll pos, ll delta) {
    if (!node) node = New_Node();
    nodes[node].sum += delta;
    if (l == r) return node;
    ll mid = l + (r - l) / 2;
    if (pos <= mid) {
      int child = Add(nodes[node].left, l, mid, pos, delta);
      nodes[node].left = child;
    } else {
      int child = Add(nodes[node].right, mid + 1, r, pos, delta);
      nodes[node].right = child;
    }
    return node;
  };
  function<ll(int,ll,ll,ll,ll)> Query = [&](int node, ll l, ll r, ll ql, ll qr) -> ll {
    if (!node or r < ql or qr < l) return 0;
    if (ql <= l and r <= qr) return nodes[node].sum;
    ll mid = l + (r - l) / 2;
    return Query(nodes[node].left, l, mid, ql, qr)
         + Query(nodes[node].right, mid + 1, r, ql, qr);
  };
  int root = 0;
  for (int i = 1; i <= q; ++i) {
    int type;
    ll a, b;
    cin >> type >> a >> b;
    if (type == 1) root = Add(root, 1, upper, a, b);
    else cout << Query(root, 1, upper, a, b) << endl;
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
