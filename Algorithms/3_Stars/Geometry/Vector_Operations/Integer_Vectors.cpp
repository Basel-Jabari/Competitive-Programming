/*
Given points A, B, C and an integer k, print:
1. A+B as a vector.
2. B-A: the vector from A to B.
3. k*A.
4. dot(B-A, C-A) and cross(B-A, C-A).
5. LEFT, RIGHT, or ON: where C lies relative to the directed line A -> B.
A and B must be different. ON means on the infinite line, not the segment.
Input: ax ay bx by cx cy k. Integer coordinates, all absolute values <= 10^9.
Sample input:
1 1 4 1 2 3 2
Sample output:
5 2
3 0
2 2
3 6
LEFT

Use dot for angles and projection; cross for side tests and areas.
dot(u,v)>0: acute angle; <0: obtuse; =0: perpendicular for nonzero vectors.
cross(u,v)>0: counterclockwise; <0: clockwise; =0: parallel.
abs(cross(B-A,C-A))/2 is the triangle area.
Every operation O(1), extra memory O(1).
These bounds make both dot and cross fit in signed long long.
For larger coordinates, cast BEFORE multiplying and use GNU __int128.
Exact integer tests need no EPS. Floating-point rotation is a later file.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

struct Point {
  ll x, y;

  Point operator+(const Point &other) const {
    return {x + other.x, y + other.y};
  }

  Point operator-(const Point &other) const {
    return {x - other.x, y - other.y};
  }

  Point operator*(ll k) const {
    return {x * k, y * k};
  }
};

ll Dot(const Point &a, const Point &b) {
  return a.x * b.x + a.y * b.y;
}

ll Cross(const Point &a, const Point &b) {
  return a.x * b.y - a.y * b.x;
}

ll Orientation(const Point &a, const Point &b, const Point &c) {
  return Cross(b - a, c - a);
}

bool On_Segment(const Point &a, const Point &b, const Point &p) {
  return Orientation(a, b, p) == 0
      and min(a.x, b.x) <= p.x and p.x <= max(a.x, b.x)
      and min(a.y, b.y) <= p.y and p.y <= max(a.y, b.y);
}

void Solve() {
  Point a, b, c;
  ll k;
  cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> k;

  Point sum = a + b, difference = b - a, scaled = a * k;
  cout << sum.x << ' ' << sum.y << endl;
  cout << difference.x << ' ' << difference.y << endl;
  cout << scaled.x << ' ' << scaled.y << endl;
  ll side = Orientation(a, b, c);
  cout << Dot(b - a, c - a) << ' ' << side << endl;
  cout << (side > 0 ? "LEFT" : side < 0 ? "RIGHT" : "ON") << endl;
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
