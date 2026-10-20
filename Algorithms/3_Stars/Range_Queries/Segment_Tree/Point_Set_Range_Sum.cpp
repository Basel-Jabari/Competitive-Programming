/*
Problem: Given n numbers, answer q operations:
1 k value: Set a[k] = value.
2 l r: Print the sum from l to r, including both ends.
Input: n q, then n numbers, then q operations. Input is 1-based.
Bounds: 1 <= n, q <= 200000, |initial and assigned values| <= 10^9.
Sample input:
4 4
2 1 -3 5
2 1 4
1 3 7
2 2 3
2 3 4
Sample output:
5
8
12

Each node stores the sum of its interval; Merge adds its two children.
Change one leaf, then rebuild the sums on its path to the root.
Build: O(n). Update/query: O(log n). Extra memory: O(n).
Pitfalls: This update SETS a value; it does not add. Use ll for sums.
Input indices are 1-based; the class below uses 1-based inclusive ranges.
For point changes and sums only, Fenwick is smaller. A segment tree is
easier to adapt when each node must store more information.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Segment_Tree {
private:
  int _n;
  vector<ll> _tree;

  void Merge(int node) {
    _tree[node] = _tree[2 * node] + _tree[2 * node + 1];
  }

  void Build(int node, int l, int r, const vector<ll> &a) {
    if (l == r) {
      _tree[node] = a[l];
      return;
    }
    int mid = l + (r - l) / 2;
    Build(2 * node, l, mid, a);
    Build(2 * node + 1, mid + 1, r, a);
    Merge(node);
  }

  void Update(int node, int l, int r, int index, ll value) {
    if (l == r) {
      _tree[node] = value;
      return;
    }
    int mid = l + (r - l) / 2;
    if (index <= mid) {
      Update(2 * node, l, mid, index, value);
    } else {
      Update(2 * node + 1, mid + 1, r, index, value);
    }
    Merge(node);
  }

  ll Query(int node, int l, int r, int ql, int qr) const {
    if (qr < l or r < ql) {
      return 0;
    }
    if (ql <= l and r <= qr) {
      return _tree[node];
    }
    int mid = l + (r - l) / 2;
    return Query(2 * node, l, mid, ql, qr)
           + Query(2 * node + 1, mid + 1, r, ql, qr);
  }

public:
  Segment_Tree(const vector<ll> &a) {
    _n = (int)a.size() - 1;
    _tree.assign(4 * _n, 0);
    Build(1, 1, _n, a);
  }

  void Update(int index, ll value) {
    Update(1, 1, _n, index, value);
  }

  ll Query(int l, int r) const {
    return Query(1, 1, _n, l, r);
  }
};

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<ll> a(n + 1);
  for (int index1 = 1; index1 < (int)a.size(); ++index1) {
    ll &value = a[index1];
    cin >> value;
  }

  Segment_Tree tree(a);
  for (int query = 1; query <= q; ++query) {
    int type;
    cin >> type;
    if (type == 1) {
      int k;
      ll value;
      cin >> k >> value;
      tree.Update(k, value);
    } else {
      int l, r;
      cin >> l >> r;
      cout << tree.Query(l, r) << endl;
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
