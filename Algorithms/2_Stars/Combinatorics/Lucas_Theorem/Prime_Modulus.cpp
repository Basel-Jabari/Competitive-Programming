/*
Problem: compute C(n,r) modulo a small prime p for very large n.
Input: p q, then q lines n r; 2<=p<=10^6 is PRIME, q<=200000,
0<=n,r<=10^18. Output one remainder per query.

Sample input:
7 3
8 1
10 3
3 4
Sample output:
1
1
0

Write n and r in base p. Lucas says C(n,r)%p is the product of
C(n_digit,r_digit)%p over matching digit positions.
If any r_digit>n_digit, the answer is zero.
Example: C(8,1) mod7 = C(1,0)*C(1,1)=1.
Precompute factorials only for 0..p-1; a factorial at p would be zero.
Build O(p+log p); query O(log_p(n+1)); memory O(p).
This theorem requires prime p. It does not directly work modulo p^k or
a composite number; those need other methods and possibly CRT.
For n<p and many queries, the normal factorial template is enough.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int p, q;
  cin >> p >> q;
  auto Power = [&](ll a, ll e) {
    ll answer = 1;
    while (e) {
      if (e & 1) answer = answer * a % p;
      a = a * a % p;
      e >>= 1;
    }
    return answer;
  };
  vector<ll> factorial(p, 1), inverse(p, 1);
  for (int i = 1; i < p; ++i) factorial[i] = factorial[i - 1] * i % p;
  inverse[p - 1] = Power(factorial[p - 1], p - 2);
  for (int i = p - 1; i >= 1; --i) inverse[i - 1] = inverse[i] * i % p;
  auto Lucas = [&](ll n, ll r) {
    ll answer = 1;
    while (n > 0 or r > 0) {
      int a = n % p, b = r % p;
      if (b > a) return 0LL;
      answer = answer * factorial[a] % p * inverse[b] % p * inverse[a - b] % p;
      n /= p;
      r /= p;
    }
    return answer;
  };
  for (int i = 1; i <= q; ++i) {
    ll n, r;
    cin >> n >> r;
    cout << Lucas(n, r) << endl;
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
