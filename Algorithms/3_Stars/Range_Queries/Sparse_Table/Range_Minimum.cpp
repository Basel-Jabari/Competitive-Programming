/*
Problem: Given n numbers and q queries, print the minimum in each [l, r].
Input: n q, then n numbers, then q lines: l r.
Output: One minimum per query. Input is 1-based; ends are included.
Bounds: 1 <= n, q <= 200000; |a[i]| <= 10^9; 1 <= l <= r <= n.
Sample input:
5 3
7 2 6 -1 4
1 3
2 5
3 3
Sample output:
2
-1
6

Use this for many minimum queries when the array does not change.
table[k][i] is the minimum of 2^k numbers starting at i.
For a query, take two blocks of length 2^floor(log2(length)).
They may overlap. min(x, x) = x, so counting the overlap twice is safe.
This O(1) query also works for max/gcd, but NOT for sum or XOR.
Build: O(n log n). Query: O(1). Extra memory: O(n log n).
Pitfalls: No fast updates. Compute logs with integers, not floating point.
The class uses 1-based inclusive ranges; n must be positive.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Sparse_Table {
private:
  vector<int> _log;
  vector<vector<ll>> _table;

public:
  Sparse_Table(const vector<ll> &a) {
    int n = (int)a.size() - 1;
    _log.assign(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
      _log[i] = _log[i / 2] + 1;
    }
    int levels = _log[n] + 1;
    _table.assign(levels, vector<ll>(n + 1));
    _table[0] = a;
    for (int k = 1; k < levels; ++k) {
      for (int i = 1; i + (1 << k) - 1 <= n; ++i) {
        _table[k][i] = min(_table[k - 1][i],
                           _table[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  ll Query(int l, int r) const {
    int k = _log[r - l + 1];
    return min(_table[k][l], _table[k][r - (1 << k) + 1]);
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

  Sparse_Table table(a);
  for (int query = 1; query <= q; ++query) {
    int l, r;
    cin >> l >> r;
    cout << table.Query(l, r) << endl;
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
