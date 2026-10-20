/*
Problem: count valid bracket strings with n opening and n closing brackets.
Input: q then q values n, 0<=n<=10^6, q<=200000.
Output each count modulo 1000000007.

Sample input:
4
0
1
2
3
Sample output:
1
1
2
5

Catalan(n)=C(2n,n)/(n+1). For n=2 the strings are ()() and (()).
The same numbers count full binary tree shapes with n internal nodes and
triangulations of a convex polygon with n+2 vertices (for n>=1).
A valid prefix never has more ')' than '('; counting all C(2n,n) arrangements
also includes invalid prefixes, so we need the Catalan formula.
Here use C(2n,n)-C(2n,n+1), which avoids a separate division by n+1.
Factorial inverses still need prime MOD and 2*max(n)<MOD.
Preprocess O(N+log MOD), N=2*max(n); query O(1); memory O(N+q).
The empty bracket string counts as one.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {

  const ll MOD = 1000000007; // MOD = modulus.
  auto Power = [&](ll a, ll e) {
    ll answer = 1;
    while (e) {
      if (e & 1) answer = answer * a % MOD;
      a = a * a % MOD;
      e >>= 1;
    }
    return answer;
  };

  int q, maximum = 0;
  cin >> q;
  vector<int> queries(q + 1);
  for (int i = 1; i <= q; ++i) {
    cin >> queries[i];
    maximum = max(maximum, 2 * queries[i]);
  }
  vector<ll> factorial(maximum + 1, 1), inverse(maximum + 1, 1);
  for (int i = 1; i <= maximum; ++i) factorial[i] = factorial[i - 1] * i % MOD;
  inverse[maximum] = Power(factorial[maximum], MOD - 2);
  for (int i = maximum; i >= 1; --i) inverse[i - 1] = inverse[i] * i % MOD;
  auto Choose = [&](int n, int r) -> ll {
    if (r > n) return 0;
    return factorial[n] * inverse[r] % MOD * inverse[n - r] % MOD;
  };
  for (int i = 1; i <= q; ++i) {
    int n = queries[i];
    cout << (Choose(2 * n, n) - Choose(2 * n, n + 1) + MOD) % MOD << endl;
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
