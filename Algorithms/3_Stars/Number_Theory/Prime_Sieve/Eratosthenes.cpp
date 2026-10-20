/*
Problem: count primes in [0, n], then answer whether each query is prime.
Input: n q, then q integers x. Output: prime count, then YES or NO per query.
Limits: 1 <= n <= 10^7, 1 <= q <= 200000, 0 <= x <= n.
Sample input:
10 4
0 1 2 9
Sample output:
4
NO
NO
YES
NO

Mark multiples of each prime. Start at p*p: smaller multiples are marked.
0 and 1 are not prime. A prime has exactly two positive divisors.
Preprocessing: O(n log log n); each query: O(1).
Memory: O(n) bits for vector<bool>, which packs Boolean values.
Choose this simple, compact sieve for primality queries.
Linear_Sieve.cpp takes O(n) preprocessing and O(n) integers of memory.
It also stores the smallest prime factor, useful for many factorizations.
Lower asymptotic time does not always mean a faster sieve in practice.
Syntax: allocate n + 1 values because n itself is a valid index.
Use 1LL * p * p to avoid int overflow when adapting the limits.
vector<bool> uses proxy objects; do not take bool& references to its items.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<bool> PrimeSieve(int n) {
  vector<bool> is_prime(n + 1, true);
  is_prime[0] = false;
  if (n >= 1) {
    is_prime[1] = false;
  }
  for (int p = 2; 1LL * p * p <= n; ++p) {
    if (!is_prime[p]) {
      continue;
    }
    for (ll multiple = 1LL * p * p; multiple <= n; multiple += p) {
      is_prime[multiple] = false;
    }
  }
  return is_prime;
}

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<bool> is_prime = PrimeSieve(n);
  cout << count(is_prime.begin(), is_prime.end(), true) << endl;
  for (int query = 1; query <= q; ++query) {
    int x;
    cin >> x;
    cout << (is_prime[x] ? "YES" : "NO") << endl;
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
