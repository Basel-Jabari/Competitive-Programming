/*
Problem: write n as a product of prime powers.
Input: one integer n. Output: number of distinct prime factors,
then one line p exponent for each factor, in increasing order of p.
Limits: 1 <= n <= 10^12. For n = 1, output only 0.
Sample input:
360
Sample output:
3
2 3
3 2
5 1

Example: 360 = 2^3 * 3^2 * 5. Divide out every copy of a found factor.
After checking divisors up to sqrt(the remaining n), any remainder > 1
is prime. A composite remainder would have a smaller prime divisor.
Time: O(sqrt(original n)); extra memory: O(log(n + 1)) for the result.
Good for one moderate number. For many bounded numbers, use a sieve's spf.
This is not a fast factorization method for arbitrary 64-bit numbers.
Use p <= n / p instead of p*p <= n to avoid multiplication overflow.
The function changes its local copy of n, not the caller's value.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<pair<ll, int>> Factorize(ll n) {
  vector<pair<ll, int>> factors;
  for (ll p = 2; p <= n / p; p += (p == 2 ? 1 : 2)) {
    if (n % p != 0) {
      continue;
    }
    int exponent = 0;
    while (n % p == 0) {
      n /= p;
      ++exponent;
    }
    factors.push_back({p, exponent});
  }
  if (n > 1) {
    factors.push_back({n, 1});
  }
  return factors;
}

void Solve() {
  ll n;
  cin >> n;
  vector<pair<ll, int>> factors = Factorize(n);
  cout << factors.size() << endl;
  for (int index1 = 0; index1 < (int)factors.size(); ++index1) {
    auto [prime, exponent] = factors[index1];
    cout << prime << ' ' << exponent << endl;
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
