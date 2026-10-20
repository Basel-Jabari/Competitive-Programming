/*
Problem: dp[1]=0, dp[i]=min(dp[j]+b[j]*a[i]) over 1<=j<i.
Input: n, then a[1..n], then b[1..n]. Output dp[n].
Limits: 1<=n<=200000; |a[i]|,|b[i]|<=10^6. No ordering is required.

Sample input:
3
1 2 3
5 4 3
Sample output:
15

Again each earlier state is a line with slope b[j], intercept dp[j].
A Li Chao node keeps the line that wins at its interval's middle x.
Two lines cross at most once. The losing line can matter on only one side;
pass it to that child. A query checks all lines on one root-to-leaf path.
Compress the known query x values, retaining their real values for evaluation.
Sorting THESE query coordinates is safe; DP states still run in original order.
Build compression O(n log n); each insertion/query O(log n); memory O(n).
This discrete version needs query coordinates in advance. It supports any
line order and avoids division. dp and line values fit ll at the stated bounds.
For monotone slopes/queries, the deque CHT is faster: O(n) total.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n;
  cin >> n;
  vector<ll> a(n + 1), b(n + 1), dp(n + 1), coordinates; // dp = dynamic programming; dp[i] stores the answer for state i.
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    coordinates.push_back(a[i]);
  }
  for (int i = 1; i <= n; ++i) cin >> b[i];
  sort(coordinates.begin(), coordinates.end());
  coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
  coordinates.insert(coordinates.begin(), 0);
  int size = (int)coordinates.size() - 1;
  const ll INF = (1LL << 62); // INF = infinity (a large marker value).
  struct Line { ll slope = 0, intercept = (1LL << 62); };
  vector<Line> tree(4 * size + 4);
  auto Value = [&](const Line &line, ll x) { return line.slope * x + line.intercept; };
  function<void(int,int,int,Line)> Add = [&](int node, int l, int r, Line line) {
    int mid = (l + r) / 2;
    bool betterLeft = Value(line, coordinates[l]) < Value(tree[node], coordinates[l]);
    bool betterMid = Value(line, coordinates[mid]) < Value(tree[node], coordinates[mid]);
    if (betterMid) swap(line, tree[node]);
    if (l == r) return;
    if (betterLeft != betterMid) Add(node * 2, l, mid, line);
    else Add(node * 2 + 1, mid + 1, r, line);
  };
  auto Query = [&](int pos) {
    int node = 1, l = 1, r = size;
    ll answer = INF;
    while (true) {
      answer = min(answer, Value(tree[node], coordinates[pos]));
      if (l == r) break;
      int mid = (l + r) / 2;
      if (pos <= mid) { node *= 2; r = mid; }
      else { node = node * 2 + 1; l = mid + 1; }
    }
    return answer;
  };
  Add(1, 1, size, {b[1], 0});
  for (int i = 2; i <= n; ++i) {
    int pos = lower_bound(coordinates.begin() + 1, coordinates.end(), a[i]) - coordinates.begin();
    dp[i] = Query(pos);
    Add(1, 1, size, {b[i], dp[i]});
  }
  cout << dp[n] << endl;
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
