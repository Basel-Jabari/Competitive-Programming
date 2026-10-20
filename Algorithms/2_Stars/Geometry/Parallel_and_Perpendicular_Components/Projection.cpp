/*
Problem: split vector v into its parallel and perpendicular components
relative to nonzero direction u.
Input: vx vy ux uy. Output parallel x y, then perpendicular x y (10 decimals).
Finite real coordinates with absolute values <=10^6; norm(u)>=10^-6.

Sample input:
3 4 1 0
Sample output:
3.0000000000 0.0000000000
0.0000000000 4.0000000000

parallel=(dot(v,u)/dot(u,u))*u; perpendicular=v-parallel.
Their sum is v; dot(perpendicular,u)=0.
The scalar coefficient is not itself the parallel vector or its length.
For point-to-line distance, let v=point-line_origin. The length of the
perpendicular component is the distance to the INFINITE line.
For a segment, clamp the projection parameter to [0,1] first.
Time/memory O(1). Do not normalize u unnecessarily or accept a zero direction.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  long double x, y, ux, uy;
  cin >> x >> y >> ux >> uy;
  long double scale = (x * ux + y * uy) / (ux * ux + uy * uy);
  long double px = scale * ux, py = scale * uy;
  cout << fixed << setprecision(10);
  cout << px << ' ' << py << endl;
  cout << x - px << ' ' << y - py << endl;
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
