/*
Problem: count pairs (x,y), 1<=x<=n, 1<=y<=m, with gcd(x,y)=1.
Input: n m, with 1<=n,m<=10^7. Output the count.

Sample input:
3 4
Sample output:
9

mu(1)=1. mu(x)=0 if a squared prime divides x.
Otherwise mu(x)=(-1)^k, where k is the number of distinct prime factors.
Example: mu(6)=1, mu(30)=-1, mu(12)=0.
The sum of mu(d) over divisors d of g is 1 when g=1, otherwise 0.
This turns the coprime test into inclusion-exclusion over common divisors:
answer = sum mu(d)*floor(n/d)*floor(m/d).
The linear sieve computes mu while finding smallest prime factors.
Time O(min(n,m)); memory O(min(n,m)). Use ll for products and the count.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m;
  cin >> n >> m;
  int limit = min(n, m);
  vector<int> smallest(limit + 1), mu(limit + 1), primes;
  mu[1] = 1;
  for (int i = 2; i <= limit; ++i) {
    if (!smallest[i]) {
      smallest[i] = i;
      primes.push_back(i);
      mu[i] = -1;
    }
    for (int j = 0; j < (int)primes.size(); ++j) {
      int p = primes[j];
      if (p > limit / i) break;
      smallest[i * p] = p;
      if (i % p == 0) { mu[i * p] = 0; break; }
      mu[i * p] = -mu[i];
    }
  }
  ll answer = 0;
  for (int d = 1; d <= limit; ++d) answer += 1LL * mu[d] * (n / d) * (m / d);
  cout << answer << endl;
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
