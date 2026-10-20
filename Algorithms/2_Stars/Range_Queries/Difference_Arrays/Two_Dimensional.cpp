/*
Problem: start with an all-zero n*m grid and add x to rectangles.
Input: n m q, then q lines r1 c1 r2 c2 x. Corners are inclusive and 1-based.
Output the final n*m grid. Limits: n,m <= 1000, q <= 200000, |x| <= 10^9.

Sample input:
2 3 2
1 1 2 2 5
2 2 2 3 -2
Sample output:
5 5 0
5 3 -2

A rectangle update changes four corners, with signs +,-,-,+.
Two-dimensional prefix sums recover the values after all updates.
Time O(n*m+q); memory O(n*m). The extra row and column store r2+1,c2+1.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<ll>> difference(n + 2, vector<ll>(m + 2));
  for (int i = 1; i <= q; ++i) {
    int r1, c1, r2, c2;
    ll x;
    cin >> r1 >> c1 >> r2 >> c2 >> x;
    difference[r1][c1] += x;
    difference[r2 + 1][c1] -= x;
    difference[r1][c2 + 1] -= x;
    difference[r2 + 1][c2 + 1] += x;
  }
  for (int r = 1; r <= n; ++r) {
    for (int c = 1; c <= m; ++c) {
      difference[r][c] += difference[r - 1][c] + difference[r][c - 1] - difference[r - 1][c - 1];
      cout << difference[r][c] << (c == m ? '\n' : ' ');
    }
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
