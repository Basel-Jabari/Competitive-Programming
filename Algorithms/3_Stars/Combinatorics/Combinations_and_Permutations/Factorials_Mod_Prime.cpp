/*
Problem: choose r distinct items from n, with and without order.
Input: q, then q lines with n r. Output: nCr nPr modulo 1000000007.
Limits: 1 <= q <= 200000, 0 <= n <= 10^6, 0 <= r <= 10^9.
Sample input:
3
5 2
4 0
3 4
Sample output:
10 20
1 1
0 0

nCr counts groups: {A, B} and {B, A} are the same group.
nPr counts ordered lists: [A, B] and [B, A] are different lists.
No item is used twice. nCr = n!/(r!(n-r)!), nPr = n!/(n-r)!.
We define 0! = 1, so choosing or arranging zero items gives one way.
Build factorials and inverse factorials once; each query then takes O(1).
Total time: O(N + log(MOD) + q), where N is the largest input n.
Memory: O(N + q), including the stored queries.
Conditions: MOD is prime and N < MOD, so factorial[N] has an inverse.
Fermat: inverse(x) = x^(MOD-2) modulo MOD when x % MOD != 0.
Use Pascal.cpp for a small N and any modulus; it needs no inverse.
Pitfalls: use long long products, reject r > n before indexing,
and never replace modular division with the normal / operator.
For n >= MOD, this factorial method is invalid; use another method.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

const ll MOD = 1000000007; // MOD = modulus.

ll Power(ll base, ll exponent) {
  ll result = 1;
  while (exponent > 0) {
    if (exponent & 1) {
      result = result * base % MOD;
    }
    base = base * base % MOD;
    exponent >>= 1;
  }
  return result;
}

class Combinatorics {
private:
  vector<ll> _factorial, _inverse_factorial;

public:
  explicit Combinatorics(int n) : _factorial(n + 1, 1),
                                 _inverse_factorial(n + 1, 1) {
    for (int i = 1; i <= n; ++i) {
      _factorial[i] = _factorial[i - 1] * i % MOD;
    }
    _inverse_factorial[n] = Power(_factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
      _inverse_factorial[i - 1] = _inverse_factorial[i] * i % MOD;
    }
  }

  ll nCr(int n, int r) const { // nCr = number of combinations of r items from n.
    if (r < 0 or r > n) {
      return 0;
    }
    return _factorial[n] * _inverse_factorial[r] % MOD
           * _inverse_factorial[n - r] % MOD;
  }

  ll nPr(int n, int r) const { // nPr = number of ordered selections of r items from n.
    if (r < 0 or r > n) {
      return 0;
    }
    return _factorial[n] * _inverse_factorial[n - r] % MOD;
  }
};

void Solve() {
  int q;
  cin >> q;
  vector<pair<int, int>> queries(q);
  int maximum_n = 0;
  for (int index1 = 0; index1 < (int)queries.size(); ++index1) {
    auto &[n, r] = queries[index1];
    cin >> n >> r;
    maximum_n = max(maximum_n, n);
  }
  Combinatorics combinations(maximum_n);
  for (int index2 = 0; index2 < (int)queries.size(); ++index2) {
    auto [n, r] = queries[index2];
    cout << combinations.nCr(n, r) << ' '
         << combinations.nPr(n, r) << endl;
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
