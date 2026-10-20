/*
Given n integer points, print the vertices of their convex hull.
The hull is the smallest convex boundary containing every input point.
Output only corner points; omit points inside a straight hull edge.
Start at the lexicographically smallest (x,y), then go counterclockwise.
Do not repeat the first point at the end. Duplicates are allowed in input.
For collinear points output the two endpoints; for one distinct point output it.
Input: n, followed by n lines x y.
Output: hull size, followed by one vertex per line.
Limits: 1 <= n <= 200000; |x|, |y| <= 10^9.
Sample input:
6
0 0
2 0
2 2
0 2
1 1
1 0
Sample output:
4
0 0
2 0
2 2
0 2

Method: sort points, then build the lower and upper boundaries.
Pop the last point while the next edge fails to make a LEFT turn.
Each point is pushed/popped O(1) times after sorting.
Time O(n log n); memory O(n).
<= 0 removes collinear boundary points. Keeping all boundary points needs
an extra all-collinear case; do not change this test without handling it.
This polygon hull is different from the convex hull trick used in DP.
The stated coordinate bounds keep cross products in signed long long.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

struct Point {
  ll x, y;

  bool operator<(const Point &other) const {
    return tie(x, y) < tie(other.x, other.y);
  }

  bool operator==(const Point &other) const {
    return x == other.x and y == other.y;
  }
};

ll Cross(const Point &a, const Point &b, const Point &c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

vector<Point> Convex_Hull(vector<Point> points) {
  sort(points.begin(), points.end());
  points.erase(unique(points.begin(), points.end()), points.end());
  if (points.size() <= 1) {
    return points;
  }

  vector<Point> lower, upper;
  for (int index1 = 0; index1 < (int)points.size(); ++index1) {
    const Point &p = points[index1];
    while (lower.size() >= 2 and Cross(lower[lower.size() - 2], lower.back(), p) <= 0) {
      lower.pop_back();
    }
    lower.push_back(p);
  }
  for (int i = (int) points.size() - 1; i >= 0; --i) {
    const Point &p = points[i];
    while (upper.size() >= 2 and Cross(upper[upper.size() - 2], upper.back(), p) <= 0) {
      upper.pop_back();
    }
    upper.push_back(p);
  }

  /* Both chains contain the two endpoints. Keep each endpoint only once. */
  lower.pop_back();
  upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());
  return lower;
}

void Solve() {
  int n;
  cin >> n;
  vector<Point> points(n);
  for (int index2 = 0; index2 < (int)points.size(); ++index2) {
    Point &p = points[index2];
    cin >> p.x >> p.y;
  }

  vector<Point> hull = Convex_Hull(points);
  cout << hull.size() << endl;
  for (int index3 = 0; index3 < (int)hull.size(); ++index3) {
    const Point &p = hull[index3];
    cout << p.x << ' ' << p.y << endl;
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
