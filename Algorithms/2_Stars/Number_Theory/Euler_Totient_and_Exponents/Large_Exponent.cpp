/*
Problem: print phi(m), then a^E modulo m. E is given as a decimal string.
Input: a E m. Limits: 0 <= a <= 10^9, 1 <= m <= 10^9,
E has 1..100000 decimal digits and may have leading zeros.

Sample input:
2 100 8
Sample output:
4
0

phi(m) counts integers from 1 to m that are coprime to m; phi(1)=1.
Example: phi(8)=4 because 1,3,5,7 are coprime to 8.
For each distinct prime p dividing m, replace result by result-result/p.
Uses: counting coprime choices, modular inverses, and reducing huge exponents.
Euler: if gcd(a,m)=1, a^phi(m)=1 modulo m, so reduce E modulo phi(m).
If gcd(a,m)!=1, reducing E alone is WRONG: 2^4 mod 8=0, but 2^0 mod 8=1.
A safe extension is: keep E if E<phi(m); otherwise use E%phi(m)+phi(m).
We track both the remainder and whether E reached phi(m), without storing E.
For a prime m, phi(m)=m-1. Inverses still require gcd(a,m)=1.
Time O(sqrt(m)+digits(E)+log m); memory O(digits(E)).
0^0 is treated as 1 before taking the remainder.
Reference: https://cp-algorithms.com/algebra/phi-function.html
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  ll a, mod; // mod = modulus.
  string exponent;
  cin >> a >> exponent >> mod;
  auto Phi = [&](ll value) {
    ll answer = value;
    for (ll p = 2; p <= value / p; ++p) {
      if (value % p == 0) {
        while (value % p == 0) value /= p;
        answer -= answer / p;
      }
    }
    if (value > 1) answer -= answer / value;
    return answer;
  };

  auto Power = [&](ll base, ll e, ll modulus) {
    ll result = 1 % modulus;
    base %= modulus;
    while (e > 0) {
      if (e & 1) result = result * base % modulus;
      base = base * base % modulus;
      e >>= 1;
    }
    return result;
  };

  ll phi = Phi(mod), remainder = 0, capped = 0;
  for (int i = 0; i < (int)exponent.size(); ++i) {
    int digit = exponent[i] - '0';
    remainder = (remainder * 10 + digit) % phi;
    capped = min(phi, capped * 10 + digit);
  }
  ll reduced = remainder + (capped == phi ? phi : 0);
  cout << phi << endl << Power(a, reduced, mod) << endl;
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
