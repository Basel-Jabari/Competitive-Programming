/*
Problem: find the smallest SQUARED distance between two points.
Input: n then n integer points x y. Output the squared distance.
Limits: 2<=n<=200000, |x|,|y|<=10^9. Duplicate points are allowed.

Sample input:
4
0 0
3 0
1 1
10 10
Sample output:
2

Sort by x. Keep previous points whose horizontal distance could beat best.
Store active points by (y,x,id), then check only the nearby y interval.
A packing argument bounds candidate points per step, giving O(n log n)
time and O(n) memory. Duplicate points give zero immediately.
Keep distances squared for exact comparisons. sqrt only sets a search bound;
integer adjustment makes that bound safe from floating-point rounding.
Squared distances fit ll at the stated coordinate limits (at most 8*10^18).
Use the ID in the set key to preserve distinct points with equal coordinates.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n;
  cin >> n;
  vector<pair<ll,ll>> points(n + 1);
  for (int i = 1; i <= n; ++i) cin >> points[i].first >> points[i].second;
  sort(points.begin() + 1, points.end());
  for (int i = 2; i <= n; ++i) {
    if (points[i] == points[i - 1]) { cout << 0 << endl; return; }
  }
  auto Distance = [&](int i, int j) {
    ll dx = points[i].first - points[j].first, dy = points[i].second - points[j].second; // dx = change in x; dy = change in y.
    return dx * dx + dy * dy;
  };
  ll best = Distance(1, 2);
  set<tuple<ll,ll,int>> active;
  int left = 1;
  for (int i = 1; i <= n; ++i) {
    ll x = points[i].first, y = points[i].second;
    while (left < i) {
      ll dx = x - points[left].first; // dx = change in x.
      if (dx * dx <= best) break;
      active.erase({points[left].second, points[left].first, left});
      ++left;
    }
    ll radius = (ll)sqrtl((long double)best);
    while ((__int128)(radius + 1) * (radius + 1) <= best) ++radius;
    while ((__int128)radius * radius > best) --radius;
    auto it = active.lower_bound({y - radius, LLONG_MIN, INT_MIN});
    while (it != active.end() and get<0>(*it) <= y + radius) {
      best = min(best, Distance(i, get<2>(*it)));
      ++it;
    }
    active.insert({y, x, i});
  }
  cout << best << endl;
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
