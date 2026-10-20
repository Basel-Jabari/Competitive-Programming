/*
Problem: multiply polynomials modulo a supported prime p.
Input: p n m, n coefficients of A, m coefficients of B, lowest degree first.
Output n+m-1 coefficients. 1<=n,m<=200000; coefficients are in [0,p-1].
Choose p from the table below. The padded power-of-two length must divide p-1.

Sample input:
998244353 3 2
1 2 3
4 5
Sample output:
4 13 22 15

NTT is FFT over remainders. It is exact modulo p; there is no rounding.
Prime p       Primitive root g    Largest power-of-two transform length
998244353     3                   2^23
167772161     3                   2^25
469762049     3                   2^26
754974721     11                  2^24
1224736769    3                   2^24
2013265921    31                  2^27
These are mathematical length limits, not promises of affordable memory.
For stage length L, use g^((p-1)/L), NOT g directly.
For inverse stages use its modular inverse; finally multiply by inverse(N).
A primitive root generates all nonzero residues; the stage root has order L.
Pad N>=n+m-1. Time O(N log N + log(N)*log p); memory O(N).
1e9+7 does not support long power-of-two NTTs. Use suitable primes and CRT
only with a justified bound when reconstructing an integer result.
Use ll for sums/products: some primes here overflow int when doubled.
Positions start at 0 because they represent polynomial degrees.
Background: https://github.com/atcoder/ac-library/blob/master/atcoder/convolution.hpp
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  ll mod; // mod = modulus.
  int n, m;
  cin >> mod >> n >> m;
  ll primes[6] = {998244353, 167772161, 469762049, 754974721, 1224736769, 2013265921};
  ll roots[6] = {3, 3, 3, 11, 3, 31};
  ll primitive = 0;
  for (int i = 0; i < 6; ++i) if (mod == primes[i]) primitive = roots[i];
  int size = 1;
  while (size < n + m - 1) size *= 2;
  if (!primitive or (mod - 1) % size) { cout << "UNSUPPORTED" << endl; return; }
  vector<ll> a(size), b(size);
  for (int i = 0; i < n; ++i) cin >> a[i];
  for (int i = 0; i < m; ++i) cin >> b[i];
  auto Power = [&](ll base, ll exponent) {
    ll answer = 1;
    while (exponent) {
      if (exponent & 1) answer = answer * base % mod;
      base = base * base % mod;
      exponent >>= 1;
    }
    return answer;
  };
  auto NTT = [&](vector<ll> &values, bool inverse) { // NTT = number theoretic transform.
    for (int i = 1, j = 0; i < size; ++i) {
      int bit = size >> 1;
      while (j & bit) { j ^= bit; bit >>= 1; }
      j ^= bit;
      if (i < j) swap(values[i], values[j]);
    }
    for (int length = 2; length <= size; length *= 2) {
      ll root = Power(primitive, (mod - 1) / length);
      if (inverse) root = Power(root, mod - 2);
      for (int start = 0; start < size; start += length) {
        ll factor = 1;
        for (int j = 0; j < length / 2; ++j) {
          ll u = values[start + j], v = values[start + j + length / 2] * factor % mod;
          values[start + j] = (u + v) % mod;
          values[start + j + length / 2] = (u - v + mod) % mod;
          factor = factor * root % mod;
        }
      }
    }
    if (inverse) {
      ll scale = Power(size, mod - 2);
      for (int i = 0; i < size; ++i) values[i] = values[i] * scale % mod;
    }
  };
  NTT(a, false);
  NTT(b, false);
  for (int i = 0; i < size; ++i) a[i] = a[i] * b[i] % mod;
  NTT(a, true);
  for (int i = 0; i < n + m - 1; ++i) cout << a[i] << (i == n + m - 2 ? '\n' : ' ');
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
