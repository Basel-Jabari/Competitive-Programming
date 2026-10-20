/*
Problem: dp[1]=0, dp[i]=min(dp[j]+b[j]*a[i]) over 1<=j<i.
Input: n, then a[1..n], then b[1..n]. Output dp[n].
Limits: 1<=n<=200000; 0<=a[i],b[i]<=10^6.
a must be NONDECREASING and b must be NONINCREASING.

Sample input:
3
1 2 3
5 4 3
Sample output:
15

Each earlier j is a line y=b[j]*x+dp[j]. Query the minimum at x=a[i].
The direct DP is O(n^2). Ordered slopes and queries let a deque keep only
useful lines. Pop the front when the next line is no worse at the current x.
Because later x values do not decrease, a removed front is never needed again.
For three slopes, remove the middle line when its useful interval is empty.
Compare intersections by cross products, not floating-point division.
Equal slopes: keep only the smaller intercept.
Time O(n) total; memory O(n). Each line enters/leaves the deque once.
Do not sort the original DP to force these conditions: order is part of it.
Use Li Chao if slopes or query positions arrive in arbitrary order.
GNU __int128 protects intersection comparisons. dp values fit ll here.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n;
  cin >> n;
  vector<ll> a(n + 1), b(n + 1), dp(n + 1); // dp = dynamic programming; dp[i] stores the answer for state i.
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= n; ++i) cin >> b[i];
  struct Line { ll slope, intercept; };
  deque<Line> hull;
  auto Value = [&](const Line &line, ll x) { return line.slope * x + line.intercept; };
  auto Bad = [&](const Line &x, const Line &y, const Line &z) {
    return (__int128)(y.intercept - x.intercept) * (y.slope - z.slope)
        >= (__int128)(z.intercept - y.intercept) * (x.slope - y.slope);
  };
  auto Add = [&](Line line) {
    if (!hull.empty() and hull.back().slope == line.slope) {
      if (hull.back().intercept <= line.intercept) return;
      hull.pop_back();
    }
    while (hull.size() >= 2 and Bad(hull[hull.size() - 2], hull.back(), line)) hull.pop_back();
    hull.push_back(line);
  };
  Add({b[1], 0});
  for (int i = 2; i <= n; ++i) {
    while (hull.size() >= 2 and Value(hull[1], a[i]) <= Value(hull[0], a[i])) hull.pop_front();
    dp[i] = Value(hull.front(), a[i]);
    Add({b[i], dp[i]});
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
