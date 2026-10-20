/*
Problem: compute 1^k+2^k+...+n^k modulo 1000000007.
Input: K q, then q lines n k; 0<=K<=500, q<=2000,
0<=n<=10^18, 0<=k<=K. Output one sum per query.

Sample input:
3 3
3 2
4 3
10 0
Sample output:
14
100
10

Use ordinary Bernoulli numbers when n is huge and the power k is small.
They are coefficients defined by t/(exp(t)-1)=sum B[j]*t^j/j!.
Our convention is B[0]=1,B[1]=-1/2,B[2]=1/6,B[3]=0,B[4]=-1/30.
Some references use B[1]=+1/2; then their sums-of-powers formula changes sign.
These two conventions are NOT the separate second-kind sequence.
Recurrence: B[m]=-sum(C(m+1,j)*B[j],j=0..m-1)/(m+1).
Sum(1..n,k)=sum((-1)^j*C(k+1,j)*B[j]*n^(k+1-j),j=0..k)/(k+1).
Fractions are modular inverses, not floating-point values or integer division.
MOD is prime and K+1<MOD, so all needed denominators have inverses.
Build O(K^2); query O(k+1); memory O(K^2).
For just k=1,2,3 the familiar direct formulas are shorter.
Reference: https://dlmf.nist.gov/24.4#iii
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  const ll MOD = 1000000007; // MOD = modulus.
  int maximum, q;
  cin >> maximum >> q;
  vector<vector<ll>> choose(maximum + 2, vector<ll>(maximum + 2));
  for (int n = 0; n <= maximum + 1; ++n) {
    choose[n][0] = choose[n][n] = 1;
    for (int j = 1; j < n; ++j) choose[n][j] = (choose[n - 1][j - 1] + choose[n - 1][j]) % MOD;
  }
  vector<ll> inverse(maximum + 2), bernoulli(maximum + 1);
  inverse[1] = 1;
  for (int i = 2; i <= maximum + 1; ++i) inverse[i] = MOD - MOD / i * inverse[MOD % i] % MOD;
  bernoulli[0] = 1;
  for (int i = 1; i <= maximum; ++i) {
    ll sum = 0;
    for (int j = 0; j < i; ++j) sum = (sum + choose[i + 1][j] * bernoulli[j]) % MOD;
    bernoulli[i] = (MOD - sum) * inverse[i + 1] % MOD;
  }
  for (int query = 1; query <= q; ++query) {
    ll n;
    int k;
    cin >> n >> k;
    vector<ll> powers(k + 2, 1);
    for (int i = 1; i <= k + 1; ++i) powers[i] = powers[i - 1] * (n % MOD) % MOD;
    ll answer = 0;
    for (int j = 0; j <= k; ++j) {
      ll term = choose[k + 1][j] * bernoulli[j] % MOD * powers[k + 1 - j] % MOD;
      answer = (answer + (j % 2 ? MOD - term : term)) % MOD;
    }
    cout << answer * inverse[k + 1] % MOD << endl;
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
