/*
Problem: for each a, e, m, print a^e modulo m and the inverse of a modulo m.
Print -1 for the inverse if it does not exist.
Input: q, then q lines with a e m. Output: power inverse on each line.
Limits: 1 <= q <= 10000, 0 <= a, e <= 10^18, 2 <= m <= 10^18.
Sample input:
3
2 10 1000
3 4 11
0 0 7
Sample output:
24 -1
4 4
1 -1

Fast power processes the binary digits of e: square the base at each step.
We use a^0 = 1, including 0^0, as the empty-product convention.
An inverse x satisfies a*x % m == 1. It exists exactly when gcd(a, m) == 1.
Division modulo m means multiplying by this inverse, not normal division.
Example: 8 / 3 modulo 11 means 8 * 4 % 11 = 10.
Extended Euclid finds x, y with a*x + m*y = gcd(a, m).
If the GCD is 1, reduce x modulo m to get the inverse.
Fermat shortcut: a^(m-2) % m works only when m is prime and a % m != 0.
Time per query: O(log(e + 1) + log(m)); extra memory: O(log(m)).
GNU C++17 __int128 prevents multiplication overflow here.
The cast must happen BEFORE multiplication. Do not use floating-point pow().
Negative remainders need normalization: (x % m + m) % m.
__int128 is a GNU extension; this file needs a compiler that supports it.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

ll MultiplyMod(ll a, ll b, ll mod) { // mod = modulus.
  return static_cast<ll>(static_cast<__int128>(a) * b % mod);
}

ll PowerMod(ll base, ll exponent, ll mod) { // mod = modulus.
  ll result = 1;
  base %= mod;
  while (exponent > 0) {
    if (exponent & 1) {
      result = MultiplyMod(result, base, mod);
    }
    base = MultiplyMod(base, base, mod);
    exponent >>= 1;
  }
  return result;
}

ll ExtendedGCD(ll a, ll b, __int128 &x, __int128 &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  __int128 x1, y1;
  ll gcd = ExtendedGCD(b, a % b, x1, y1);
  x = y1;
  y = x1 - static_cast<__int128>(a / b) * y1;
  return gcd;
}

ll InverseMod(ll a, ll mod) { // mod = modulus.
  a %= mod;
  __int128 x, y;
  if (ExtendedGCD(a, mod, x, y) != 1) {
    return -1;
  }
  return static_cast<ll>((x % mod + mod) % mod);
}

void Solve() {
  int q;
  cin >> q;
  for (int query = 1; query <= q; ++query) {
    ll a, e, m;
    cin >> a >> e >> m;
    cout << PowerMod(a, e, m) << ' ' << InverseMod(a, m) << endl;
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
