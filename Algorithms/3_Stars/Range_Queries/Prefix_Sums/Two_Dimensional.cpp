/*
Problem: Given an n by m grid and q queries, print each rectangle sum.
Input: n m q, then n rows of m numbers.
Each query: r1 c1 r2 c2 (top-left and bottom-right corners).
Output: One sum per query. Coordinates are 1-based; ends are included.
Bounds: 1 <= n, m <= 1000, 1 <= q <= 200000, |a[r][c]| <= 10^9.
Queries satisfy 1 <= r1 <= r2 <= n and 1 <= c1 <= c2 <= m.
Sample input:
2 3 3
1 2 3
4 5 6
1 1 2 3
1 2 2 3
2 1 2 1
Sample output:
21
16
4

Use this for rectangle sums when the grid does not change.
prefix[r][c] covers rows 1..r and columns 1..c.
Remove the area above and the area on the left; add their overlap back.
Build: O(n*m). Query: O(1). Extra memory: O(n*m).
Pitfalls: Add a zero row/column. Use ll for prefixes AND the answer.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, m, q;
  cin >> n >> m >> q;

  vector<vector<ll>> prefix(n + 1, vector<ll>(m + 1, 0));
  for (int r = 1; r <= n; ++r) {
    for (int c = 1; c <= m; ++c) {
      ll value;
      cin >> value;
      prefix[r][c] = value + prefix[r - 1][c] + prefix[r][c - 1]
                    - prefix[r - 1][c - 1];
    }
  }

  for (int query = 1; query <= q; ++query) {
    int r1, c1, r2, c2;
    cin >> r1 >> c1 >> r2 >> c2;
    ll answer = prefix[r2][c2] - prefix[r1 - 1][c2]
                - prefix[r2][c1 - 1] + prefix[r1 - 1][c1 - 1];
    cout << answer << endl;
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
