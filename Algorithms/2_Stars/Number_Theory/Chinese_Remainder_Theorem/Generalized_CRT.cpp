/*
Problem: find x satisfying x = r[i] modulo m[i] for every equation.
Input: k, then k lines r m. Output smallest x>=0 and the combined modulus,
or NO SOLUTION. Limits: 1<=k<=100, 1<=m[i]<=10^9, |r[i]|<=10^18.
The LCM of all input moduli must be <=10^18.

Sample input:
2
2 6
5 9
Sample output:
14 18

A remainder condition describes times/positions repeating every m steps.
CRT combines such conditions. Pairwise-coprime moduli always have a solution.
For x=a modulo M and x=b modulo m, let g=gcd(M,m).
They are compatible exactly when g divides b-a.
Write x=a+M*t, divide by g, and invert M/g modulo m/g.
The new period is lcm(M,m)=M*(m/g), not always M*m.
Extended Euclid supplies the inverse even for composite reduced moduli.
Time O(k log(max modulus)); extra memory O(log(max modulus)).
GNU __int128 protects products BEFORE reduction. Normalize negative residues.
This one method covers both coprime and non-coprime CRT.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int k;
  cin >> k;

  function<ll(ll,ll,__int128&,__int128&)> Extended_GCD =
    [&](ll a, ll b, __int128 &x, __int128 &y) -> ll {
      if (b == 0) { x = 1; y = 0; return a; }
      __int128 x1, y1;
      ll g = Extended_GCD(b, a % b, x1, y1);
      x = y1;
      y = x1 - (__int128)(a / b) * y1;
      return g;
    };

  ll answer = 0, period = 1;
  bool possible = true;
  for (int i = 1; i <= k; ++i) {
    ll b, mod; // mod = modulus.
    cin >> b >> mod;
    if (!possible) continue;
    b = (b % mod + mod) % mod;
    __int128 x, y;
    ll g = Extended_GCD(period, mod, x, y);
    ll difference = b - answer;
    if (difference % g != 0) {
      possible = false;
      continue;
    }
    ll reduced = mod / g;
    ll step = (ll)(((__int128)(difference / g) * x % reduced + reduced) % reduced);
    ll nextPeriod = (ll)((__int128)period * reduced);
    answer = (ll)(((__int128)answer + (__int128)period * step) % nextPeriod);
    period = nextPeriod;
  }
  if (possible) cout << answer << ' ' << period << endl;
  else cout << "NO SOLUTION" << endl;
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
