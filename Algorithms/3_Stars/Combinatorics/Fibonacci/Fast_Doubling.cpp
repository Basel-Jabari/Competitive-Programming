/*
Problem: find F(n) modulo m for each query.
F(0) = 0, F(1) = 1, and F(n) = F(n-1) + F(n-2) for n >= 2.
Input: q, then q lines with n m. Output: F(n) modulo m.
Limits: 1 <= q <= 200000, 0 <= n <= 10^18, 2 <= m <= 10^9 + 7.
Sample input:
3
0 1000
1 1000
10 1000
Sample output:
0
1
55

Example use: ways to climb n stairs with steps of 1 or 2 = F(n+1).
Fast doubling returns both F(n) and F(n+1).
If a = F(k), b = F(k+1), then:
F(2k) = a * (2b-a), and F(2k+1) = a*a + b*b.
One recursive call on n/2 is enough; do not make two recursive calls.
Time per query: O(log(n + 1)); extra memory: O(log(n + 1)) call stack.
The modulus need not be prime. All products fit long long at these limits.
Syntax: normalize the subtraction before multiplying.
Fibonacci(n, m).first is F(n); .second is F(n+1).
For all values through a small n, a simple O(n) loop is often enough.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

pair<ll, ll> Fibonacci(ll n, ll mod) {
  if (n == 0) {
    return {0, 1};
  }
  auto [a, b] = Fibonacci(n / 2, mod);
  ll even = a * ((2 * b % mod - a + mod) % mod) % mod;
  ll odd = (a * a % mod + b * b % mod) % mod;
  if (n & 1) {
    return {odd, (even + odd) % mod};
  }
  return {even, odd};
}

void Solve() {
  int q;
  cin >> q;
  for (int query = 1; query <= q; ++query) {
    ll n, mod; // mod = modulus.
    cin >> n >> mod;
    cout << Fibonacci(n, mod).first << endl;
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
