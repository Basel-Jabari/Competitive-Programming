/*
Problem: add x to each element in [l,r] for q operations, then print the final array.
Input: n q, n initial values, then q lines l r x. All indices are 1-based.
Limits: 1 <= n <= 200000, 0 <= q <= 200000, |values|,|x| <= 10^9.

Sample input:
4 2
1 2 3 4
1 3 5
2 4 -2
Sample output:
6 5 6 2

For an addition on [l,r], add x at diff[l] and subtract x at diff[r+1].
One prefix pass recovers every position's total addition.
Use this when answers are needed AFTER all updates. Online queries need a tree.
Time O(n+q); memory O(n). Allocate n+2 cells: r+1 can equal n+1.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<ll> a(n + 1), difference(n + 2);
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= q; ++i) {
    int l, r;
    ll x;
    cin >> l >> r >> x;
    difference[l] += x;
    difference[r + 1] -= x;
  }
  ll add = 0;
  for (int i = 1; i <= n; ++i) {
    add += difference[i];
    cout << a[i] + add << (i == n ? '\n' : ' ');
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
