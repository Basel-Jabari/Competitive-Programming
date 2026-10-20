// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long

void Solve() {
  int n, m, q;
  cin >> n >> m >> q;

  vector<vector<int>> a(n + 1, vector<int>(m + 1));
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      cin >> a[i][j];
    }
  }

  vector<vector<ll>> preSum(n + 2, vector<ll>(m + 2));
  for (int i = 1; i <= q; ++i) {
    array<int, 2> row, column;
    for (int j = 0; j < 2; ++j) {
      cin >> row[j] >> column[j];
    }

    int val;
    cin >> val;

    preSum[row[0]][column[0]] += val;
    preSum[row[1] + 1][column[1] + 1] += val;

    preSum[row[0]][column[1] + 1] -= val;
    preSum[row[1] + 1][column[0]] -= val;
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      preSum[i][j] += preSum[i - 1][j] + preSum[i][j - 1] - preSum[i - 1][j - 1];
    }
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      cout << a[i][j] + preSum[i][j] << ' ';
    }

    cout << endl;
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