/*
Problem: Given n numbers, answer q operations:
1 l r value: Set every a[i] in [l, r] to value.
2 l r delta: Add delta to every a[i] in [l, r].
3 l r: Print the sum of a[l..r].
Input: n q, then n numbers, then q operations.
Input indices are 1-based; both ends are included.
Bounds: 1 <= n, q <= 200000; |initial values|, |value|, |delta| <= 10^8.
Under these bounds every sum fits ll, even after q whole-range adds.
Sample input:
4 6
1 2 3 4
3 1 4
1 2 4 0
2 3 4 -2
3 1 4
1 1 3 5
3 2 4
Sample output:
10
-3
8

Lazy propagation: update a node now and save work for its children.
A set replaces every older set/add on the same node.
An add after a set changes the saved set value; otherwise save an add.
Push saved work to both children before reading/changing a smaller range.
Build: O(n). Each operation: O(log n). Extra memory: O(n).
Pitfalls: A separate bool marks a pending set: setting to 0 is valid.
A range add changes sum by delta * LENGTH, not just delta.
The class uses 1-based inclusive ranges. Do not use this class for n = 0.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Segment_Tree {
private:
  int _n;
  vector<ll> _sum, _set, _add;
  vector<bool> _hasSet;

  void Merge(int node) {
    _sum[node] = _sum[2 * node] + _sum[2 * node + 1];
  }

  void Build(int node, int l, int r, const vector<ll> &a) {
    if (l == r) {
      _sum[node] = a[l];
      return;
    }
    int mid = l + (r - l) / 2;
    Build(2 * node, l, mid, a);
    Build(2 * node + 1, mid + 1, r, a);
    Merge(node);
  }

  void Apply_Set(int node, int length, ll value) {
    _sum[node] = value * length;
    _hasSet[node] = true;
    _set[node] = value;
    _add[node] = 0;
  }

  void Apply_Add(int node, int length, ll delta) {
    _sum[node] += delta * length;
    if (_hasSet[node]) {
      _set[node] += delta;
    } else {
      _add[node] += delta;
    }
  }

  void Push(int node, int l, int r) {
    if (l == r) {
      return;
    }
    int mid = l + (r - l) / 2;
    int leftLength = mid - l + 1, rightLength = r - mid;
    if (_hasSet[node]) {
      Apply_Set(2 * node, leftLength, _set[node]);
      Apply_Set(2 * node + 1, rightLength, _set[node]);
      _hasSet[node] = false;
    }
    if (_add[node] != 0) {
      Apply_Add(2 * node, leftLength, _add[node]);
      Apply_Add(2 * node + 1, rightLength, _add[node]);
      _add[node] = 0;
    }
  }

  void Update(int node, int l, int r, int ql, int qr, ll value, bool isSet) {
    if (qr < l or r < ql) {
      return;
    }
    if (ql <= l and r <= qr) {
      if (isSet) {
        Apply_Set(node, r - l + 1, value);
      } else {
        Apply_Add(node, r - l + 1, value);
      }
      return;
    }
    Push(node, l, r);
    int mid = l + (r - l) / 2;
    Update(2 * node, l, mid, ql, qr, value, isSet);
    Update(2 * node + 1, mid + 1, r, ql, qr, value, isSet);
    Merge(node);
  }

  ll Query(int node, int l, int r, int ql, int qr) {
    if (qr < l or r < ql) {
      return 0;
    }
    if (ql <= l and r <= qr) {
      return _sum[node];
    }
    Push(node, l, r);
    int mid = l + (r - l) / 2;
    return Query(2 * node, l, mid, ql, qr)
           + Query(2 * node + 1, mid + 1, r, ql, qr);
  }

public:
  Segment_Tree(const vector<ll> &a) {
    _n = (int)a.size() - 1;
    _sum.assign(4 * _n, 0);
    _set.assign(4 * _n, 0);
    _add.assign(4 * _n, 0);
    _hasSet.assign(4 * _n, false);
    Build(1, 1, _n, a);
  }

  void Set(int l, int r, ll value) {
    Update(1, 1, _n, l, r, value, true);
  }

  void Add(int l, int r, ll delta) {
    Update(1, 1, _n, l, r, delta, false);
  }

  ll Query(int l, int r) {
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
    int type, l, r;
    cin >> type >> l >> r;
    if (type == 3) {
      cout << tree.Query(l, r) << endl;
    } else {
      ll value;
      cin >> value;
      if (type == 1) {
        tree.Set(l, r, value);
      } else {
        tree.Add(l, r, value);
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
