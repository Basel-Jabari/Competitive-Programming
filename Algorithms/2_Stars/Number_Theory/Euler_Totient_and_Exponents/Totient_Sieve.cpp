/*
Problem: answer phi(x) and the number of unordered coprime pairs in [1,x].
Input: N q, then q values x (1<=x<=N).
Output: phi(x) and pair count per query. Limits: N<=10^7, q<=200000.

Sample input:
5 3
1
4
5
Sample output:
1 0
2 5
4 9

Compute many totients with a sieve instead of factoring each number.
For each prime p, update all its multiples by phi[j]-=phi[j]/p.
For each larger endpoint b>=2, there are phi(b) smaller coprime endpoints.
Therefore unordered coprime pairs up to x = phi(2)+...+phi(x).
Build O(N log log N); query O(1); memory O(N), roughly 12*(N+1) bytes.
The sieve uses int for phi but ll for accumulated counts.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<int> phi(n + 1);
  iota(phi.begin(), phi.end(), 0);
  for (int p = 2; p <= n; ++p) {
    if (phi[p] == p) {
      for (int i = p; i <= n; i += p) phi[i] -= phi[i] / p;
    }
  }
  vector<ll> pairs(n + 1);
  for (int i = 2; i <= n; ++i) pairs[i] = pairs[i - 1] + phi[i];
  for (int i = 1; i <= q; ++i) {
    int x;
    cin >> x;
    cout << phi[x] << ' ' << pairs[x] << endl;
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
