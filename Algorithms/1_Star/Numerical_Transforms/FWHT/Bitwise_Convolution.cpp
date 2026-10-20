/*
Problem: C[mask]=sum A[i]*B[j] over pairs with (i OP j)=mask, modulo 998244353.
Input: OP k, then 2^k values of A and 2^k values of B.
OP is XOR, AND, or OR. 0<=k<=18, coefficients are in [0,998244352].
Output 2^k values in mask order. Masks ALWAYS start at 0.

Sample input:
XOR 1
1 2
3 4
Sample output:
11 10

Bitwise convolution replaces the i+j of ordinary convolution with XOR/AND/OR.
Example for XOR: C[0]=A[0]*B[0]+A[1]*B[1].
Transform both arrays, multiply corresponding values, then invert.
Butterflies for a pair (u,v):
XOR: (u+v,u-v); inverse uses the same butterfly, then divides all values by N.
AND: (u+v,v); inverse (u-v,v).
OR:  (u,u+v); inverse (u,v-u).
AND/OR are often called subset/superset zeta transforms; the code groups
them with FWHT because they solve the related bitwise-convolution tasks.
Time O(N log N); memory O(N), N=2^k.
Do not mix the inverses: only XOR uses the final inverse(N) scaling.
The odd prime modulus makes N invertible. Mask bits require 0-based positions.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  const ll MOD = 998244353; // MOD = modulus.
  string operation;
  int k;
  cin >> operation >> k;
  int n = 1 << k;
  vector<ll> a(n), b(n);
  for (int i = 0; i < n; ++i) cin >> a[i];
  for (int i = 0; i < n; ++i) cin >> b[i];
  auto Power = [&](ll base, ll exponent) {
    ll answer = 1;
    while (exponent) {
      if (exponent & 1) answer = answer * base % MOD;
      base = base * base % MOD;
      exponent >>= 1;
    }
    return answer;
  };
  auto Transform = [&](vector<ll> &values, bool inverse) {
    for (int half = 1; half < n; half *= 2) {
      for (int start = 0; start < n; start += 2 * half) {
        for (int j = 0; j < half; ++j) {
          ll u = values[start + j], v = values[start + j + half];
          if (operation == "XOR") {
            values[start + j] = (u + v) % MOD;
            values[start + j + half] = (u - v + MOD) % MOD;
          } else if (operation == "AND") {
            values[start + j] = (u + (inverse ? MOD - v : v)) % MOD;
          } else {
            values[start + j + half] = (v + (inverse ? MOD - u : u)) % MOD;
          }
        }
      }
    }
    if (inverse and operation == "XOR") {
      ll scale = Power(n, MOD - 2);
      for (int i = 0; i < n; ++i) values[i] = values[i] * scale % MOD;
    }
  };
  Transform(a, false);
  Transform(b, false);
  for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % MOD;
  Transform(a, true);
  for (int i = 0; i < n; ++i) cout << a[i] << (i == n - 1 ? '\n' : ' ');
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
