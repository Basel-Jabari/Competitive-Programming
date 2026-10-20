/*
Problem: Given n numbers, answer q operations:
1 k delta: Add delta to a[k].
2 l r: Print the sum from l to r, including both ends.
Input: n q, then n numbers, then q operations. Indices are 1-based.
Bounds: 1 <= n, q <= 200000; initial |a[i]|, |delta| <= 10^9.
Sample input:
4 4
2 1 -3 5
2 1 4
1 3 7
2 2 3
2 3 4
Sample output:
5
5
9

A Fenwick tree keeps sums for blocks ending at each index.
i & -i is the size of that block (the lowest set bit).
Add: move up with i += i & -i. Prefix sum: move down with i -= i & -i.
Build: O(n). Add/query: O(log n). Extra memory: O(n).
Pitfalls: Add changes by a DELTA, not a new value. For set(k, x),
keep the current a[k], call Add(k, x - a[k]), then set a[k] = x.
Never call Add(0, ...): it will not advance. Use ll for all sums.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Fenwick_Tree {
private:
  int _n;
  vector<ll> _tree;

public:
  /* a and public operations are 1-based; a[0] is unused. */
  Fenwick_Tree(const vector<ll> &a) {
    _n = (int)a.size() - 1;
    _tree.assign(_n + 1, 0);
    for (int i = 1; i <= _n; ++i) {
      _tree[i] += a[i];
      int parent = i + (i & -i);
      if (parent <= _n) {
        _tree[parent] += _tree[i];
      }
    }
  }

  void Add(int index, ll delta) {
    assert(1 <= index and index <= _n);
    for (int i = index; i <= _n; i += i & -i) {
      _tree[i] += delta;
    }
  }

  ll Prefix_Sum(int index) const {
    ll answer = 0;
    for (int i = index; i > 0; i -= i & -i) {
      answer += _tree[i];
    }
    return answer;
  }

  ll Query(int l, int r) const {
    return Prefix_Sum(r) - Prefix_Sum(l - 1);
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

  Fenwick_Tree fenwick(a);
  for (int query = 1; query <= q; ++query) {
    int type;
    cin >> type;
    if (type == 1) {
      int k;
      ll delta;
      cin >> k >> delta;
      fenwick.Add(k, delta);
    } else {
      int l, r;
      cin >> l >> r;
      cout << fenwick.Query(l, r) << endl;
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
