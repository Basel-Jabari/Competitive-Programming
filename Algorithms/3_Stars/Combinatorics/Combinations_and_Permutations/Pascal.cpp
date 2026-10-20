/*
Problem: answer nCr and nPr queries modulo m, which need not be prime.
Input: N m q, then q lines with n r. Output: nCr nPr modulo m.
Limits: 0 <= N <= 2000, 2 <= m <= 10^9 + 7, 1 <= q <= 200000,
        0 <= n <= N, 0 <= r <= 10^9.
Sample input:
5 12 3
5 2
4 0
3 4
Sample output:
10 8
1 1
0 0

nCr counts groups of r distinct items; nPr also counts their order.
Example: choosing 2 of 5 gives 10 groups and 20 ordered lists.
C(n, r) = C(n-1, r-1) + C(n-1, r): take the last item, or leave it.
C(n, 0) = C(n, n) = 1. Also nPr = nCr * r!.
This recurrence only uses addition; it works for composite m too.
Preprocessing: O(N^2) time and memory; each query: O(1).
Factorials_Mod_Prime.cpp is better for large N < a prime modulus:
its preprocessing is linear, but inverse factorials need that condition.
Syntax: r > n must return 0 before accessing the table.
Do not use factorial division modulo a composite m.
This code uses long long, so addition and products fit the stated limits.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Combinatorics {
private:
  ll _mod; // mod = modulus.
  vector<vector<ll>> _choose;
  vector<ll> _factorial;

public:
  Combinatorics(int n, ll mod) : _mod(mod), _choose(n + 1),
                               _factorial(n + 1, 1) {
    for (int i = 0; i <= n; ++i) {
      _choose[i].assign(i + 1, 1);
      for (int r = 1; r < i; ++r) {
        _choose[i][r] = (_choose[i - 1][r - 1]
                        + _choose[i - 1][r]) % _mod;
      }
      if (i > 0) {
        _factorial[i] = _factorial[i - 1] * i % _mod;
      }
    }
  }

  ll nCr(int n, int r) const { // nCr = number of combinations of r items from n.
    if (r < 0 or r > n) {
      return 0;
    }
    return _choose[n][r];
  }

  ll nPr(int n, int r) const { // nPr = number of ordered selections of r items from n.
    if (r < 0 or r > n) {
      return 0;
    }
    return _choose[n][r] * _factorial[r] % _mod;
  }
};

void Solve() {
  int n, q;
  ll mod; // mod = modulus.
  cin >> n >> mod >> q;
  Combinatorics combinations(n, mod);
  for (int query = 1; query <= q; ++query) {
    int size, r;
    cin >> size >> r;
    cout << combinations.nCr(size, r) << ' '
         << combinations.nPr(size, r) << endl;
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
