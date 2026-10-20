/*
Problem: count primes up to n, then find the smallest prime factor of x.
Print 0 for x = 1, because 1 has no prime factor.
Input: n q, then q integers x. Output: prime count, then one factor per query.
Limits: 1 <= n <= 10^7, 1 <= q <= 200000, 1 <= x <= n.
Sample input:
10 4
1 2 8 9
Sample output:
4
0
2
2
3

spf[x] is the smallest prime factor of x; spf[p] == p for a prime p.
Generate i*p only while p <= spf[i]. Each composite is generated once.
Preprocessing: O(n); smallest-factor query: O(1); memory: O(n) integers.
For many factorizations, repeatedly divide x by spf[x] while x > 1.
Example: 12 -> 6 -> 3 -> 1 gives factors 2, 2, 3 in O(log(x)) steps.
Eratosthenes.cpp uses O(n log log n) time and a compact Boolean table.
Use Eratosthenes if you only need prime checks; use this for factor data.
Syntax: 1LL * i * p is essential; int multiplication can overflow.
Never access spf[0] or spf[1] as a divisor. Both are stored as 0.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<int> LinearSieve(int n, vector<int> &primes) {
  vector<int> spf(n + 1); // spf = smallest prime factor.
  primes.clear();
  for (int i = 2; i <= n; ++i) {
    if (spf[i] == 0) {
      spf[i] = i;
      primes.push_back(i);
    }
    for (int index1 = 0; index1 < (int)primes.size(); ++index1) {
      int p = primes[index1];
      if (p > spf[i] or 1LL * i * p > n) {
        break;
      }
      spf[i * p] = p;
    }
  }
  return spf;
}

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<int> primes;
  vector<int> spf = LinearSieve(n, primes); // spf = smallest prime factor.
  cout << primes.size() << endl;
  for (int query = 1; query <= q; ++query) {
    int x;
    cin >> x;
    cout << spf[x] << endl;
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
