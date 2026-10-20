/*
Problem: Given n numbers and q queries, print the sum from l to r.
Input: n q, then n numbers, then q lines: l r.
Output: One sum per query. Indices are 1-based; both ends are included.
Bounds: 1 <= n, q <= 200000, |a[i]| <= 10^9, 1 <= l <= r <= n.
Sample input:
5 3
2 -1 4 3 6
1 3
2 5
4 4
Sample output:
5
12
3

Use this when the array does not change.
prefix[i] is the sum of the first i numbers; prefix[0] = 0.
Sum(l, r) = prefix[r] - prefix[l - 1].
Build: O(n). Query: O(1). Extra memory: O(n).
Pitfalls: Keep the extra zero at index 0; use ll for sums.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;

  vector<ll> prefix(n + 1, 0);
  for (int i = 1; i <= n; ++i) {
    ll value;
    cin >> value;
    prefix[i] = prefix[i - 1] + value;
  }

  for (int query = 1; query <= q; ++query) {
    int l, r;
    cin >> l >> r;
    cout << prefix[r] - prefix[l - 1] << endl;
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
