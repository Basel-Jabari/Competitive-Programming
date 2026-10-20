/*
Problem: put n identical balls into k different boxes.
Print two counts: empty boxes allowed, then every box nonempty.
Counts are modulo 1000000007.
Input: q, then q lines with n k. Output: nonnegative_count positive_count.
Limits: 1 <= q <= 200000, n >= 0, k >= 1, n + k - 1 <= 10^6.
Sample input:
3
5 3
0 3
4 1
Sample output:
21 6
1 0
1 1

We count ordered vectors x1, ..., xk with x1 + ... + xk = n.
For xi >= 0: line up n stars and k-1 bars. Each bar starts the next box.
Example: **| |*** means (2, 0, 3). Choose the k-1 bar positions:
ways = C(n + k - 1, k - 1).
For xi >= 1: first put one ball in each box, leaving n-k balls.
ways = C(n - 1, k - 1) when n >= k, otherwise 0.
If n = 0 and k >= 1, only (0, ..., 0) is allowed in the first count.
For general lower bounds xi >= Li, first subtract sum(Li) from n.
If the result is negative, there are no solutions.
These formulas do not directly handle upper bounds or different balls.
Total time: O(N + log(MOD) + q); memory: O(N + q),
where N is the largest n+k-1. Each count after preprocessing is O(1).
Factorial inverses require prime MOD and N < MOD.
Syntax: k must be at least 1 here; check n < k before using n-1.
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

class StarsAndBars {
private:
  vector<ll> _factorial, _inverse_factorial;

  ll Choose(int n, int r) const {
    if (r < 0 or r > n) {
      return 0;
    }
    return _factorial[n] * _inverse_factorial[r] % MOD
           * _inverse_factorial[n - r] % MOD;
  }

public:
  explicit StarsAndBars(int maximum) : _factorial(maximum + 1, 1),
                                      _inverse_factorial(maximum + 1, 1) {
    for (int i = 1; i <= maximum; ++i) {
      _factorial[i] = _factorial[i - 1] * i % MOD;
    }
    _inverse_factorial[maximum] = Power(_factorial[maximum], MOD - 2);
    for (int i = maximum; i >= 1; --i) {
      _inverse_factorial[i - 1] = _inverse_factorial[i] * i % MOD;
    }
  }

  ll Nonnegative(int n, int k) const {
    return Choose(n + k - 1, k - 1);
  }

  ll Positive(int n, int k) const {
    if (n < k) {
      return 0;
    }
    return Choose(n - 1, k - 1);
  }
};

void Solve() {
  int q;
  cin >> q;
  vector<pair<int, int>> queries(q);
  int maximum = 0;
  for (int index1 = 0; index1 < (int)queries.size(); ++index1) {
    auto &[n, k] = queries[index1];
    cin >> n >> k;
    maximum = max(maximum, n + k - 1);
  }
  StarsAndBars counter(maximum);
  for (int index2 = 0; index2 < (int)queries.size(); ++index2) {
    auto [n, k] = queries[index2];
    cout << counter.Nonnegative(n, k) << ' '
         << counter.Positive(n, k) << endl;
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
