/*
Problem: find coefficients b[0..N] of t/log(1+t), modulo 1000000007.
Input: N, 0<=N<=2000. Output N+1 coefficients in increasing degree.

Sample input:
3
Sample output:
1 500000004 916666673 41666667

We use the ORDINARY series t/log(1+t)=sum b[n]*t^n:
b[0]=1,b[1]=1/2,b[2]=-1/12,b[3]=1/24.
Some sources instead use coefficients c[n]/n!, so c[n]=n!*b[n].
State the definition before using a formula: these are second-kind Bernoulli
numbers, not ordinary Bernoulli numbers with the sign of B[1] changed.
Use this niche sequence for a formal-series coefficient problem involving
logarithms or for numerical-integration correction formulas. Most contest
power-sum problems need the ORDINARY file instead.
Here a formal series is just an array of coefficients; no real logarithm
is evaluated. Multiplying by log(1+t)/t must give 1.
Since log(1+t)/t=sum (-1)^j*t^j/(j+1), the coefficient equation gives
b[n]=-sum((-1)^j*b[n-j]/(j+1),j=1..n).
Time O(N^2); memory O(N). Fractions use inverses under prime MOD, N+1<MOD.
Reference and convention: https://arxiv.org/abs/0709.2947
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  const ll MOD = 1000000007; // MOD = modulus.
  int n;
  cin >> n;
  vector<ll> inverse(n + 2), b(n + 1);
  inverse[1] = 1;
  for (int i = 2; i <= n + 1; ++i) inverse[i] = MOD - MOD / i * inverse[MOD % i] % MOD;
  b[0] = 1;
  for (int i = 1; i <= n; ++i) {
    ll sum = 0;
    for (int j = 1; j <= i; ++j) {
      ll term = b[i - j] * inverse[j + 1] % MOD;
      sum = (sum + (j % 2 ? MOD - term : term)) % MOD;
    }
    b[i] = (MOD - sum) % MOD;
  }
  for (int i = 0; i <= n; ++i) cout << b[i] << (i == n ? '\n' : ' ');
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
