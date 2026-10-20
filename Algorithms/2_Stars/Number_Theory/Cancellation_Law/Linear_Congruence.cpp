/*
Problem: solve a*x = b modulo m.
Input: q, then q lines a b m; 0<=a,b<=10^18, 1<=m<=10^18.
Output smallest solution x0, step, count, or NONE.
All solutions in [0,m-1] are x0 + j*step for j=0..count-1. q<=10000.

Sample input:
3
6 8 14
6 7 14
0 0 5
Sample output:
6 7 2
NONE
0 1 5

Ordinary cancellation can change the modulus.
If c*A = c*B modulo m, then A = B modulo m/gcd(c,m).
You may keep modulus m only when gcd(c,m)=1.
For a*x=b, let g=gcd(a,m). If g does not divide b, no solution exists.
Otherwise divide a,b,m by g; now a/g has an inverse modulo m/g.
Example: 6*x=8 mod14 becomes 3*x=4 mod7; x=6 or13 mod14.
Time O(log m) per query; recursive Euclid memory O(log m).
GNU __int128 avoids overflow. m=1 and a=0 are handled explicitly.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int q;
  cin >> q;

  function<ll(ll,ll,__int128&,__int128&)> Extended_GCD =
    [&](ll a, ll b, __int128 &x, __int128 &y) -> ll {
      if (b == 0) { x = 1; y = 0; return a; }
      __int128 x1, y1;
      ll g = Extended_GCD(b, a % b, x1, y1);
      x = y1;
      y = x1 - (__int128)(a / b) * y1;
      return g;
    };

  for (int i = 1; i <= q; ++i) {
    ll a, b, mod; // mod = modulus.
    cin >> a >> b >> mod;
    ll g = gcd(a, mod);
    if (b % g) {
      cout << "NONE" << endl;
      continue;
    }
    ll step = mod / g, first = 0;
    if (step > 1) {
      __int128 inverse, other;
      Extended_GCD(a / g, step, inverse, other);
      first = (ll)(((__int128)(b / g) * inverse % step + step) % step);
    }
    cout << first << ' ' << step << ' ' << g << endl;
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
