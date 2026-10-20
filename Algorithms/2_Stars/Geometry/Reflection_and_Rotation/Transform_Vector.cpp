/*
Problem: rotate vector v counterclockwise by theta degrees, and reflect v
across the line through the origin with nonzero direction u.
Input: vx vy ux uy theta. Output rotated x y, then reflected x y (10 decimals).
Finite real inputs; |coordinates|<=10^6, norm(u)>=10^-6, |theta|<=360000.

Sample input:
1 0 1 1 90
Sample output:
0.0000000000 1.0000000000
0.0000000000 1.0000000000

C++ sin/cos use RADIANS: radians=degrees*acos(-1)/180.
Rotation: (x*cos(t)-y*sin(t), x*sin(t)+y*cos(t)).
Clockwise rotation uses a negative angle; 90-degree CCW is exactly (-y,x).
Reflect across direction u: 2*projection_on_u(v)-v.
For a line through point P, subtract P before transforming, then add P back.
Time/memory O(1). Use floating point for arbitrary angles; allow small error.
Never divide by dot(u,u) when u is the zero vector.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  long double x, y, ux, uy, degrees;
  cin >> x >> y >> ux >> uy >> degrees;
  long double angle = degrees * acosl(-1.0L) / 180;
  long double rx = x * cosl(angle) - y * sinl(angle);
  long double ry = x * sinl(angle) + y * cosl(angle);
  long double scale = (x * ux + y * uy) / (ux * ux + uy * uy);
  auto Clean = [&](long double value) { return fabsl(value) < 1e-12L ? 0.0L : value; };
  cout << fixed << setprecision(10);
  cout << Clean(rx) << ' ' << Clean(ry) << endl;
  cout << Clean(2 * scale * ux - x) << ' ' << Clean(2 * scale * uy - y) << endl;
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
