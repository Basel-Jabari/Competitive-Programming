/*
Problem: find triangle area from its three side lengths.
Input: a b c as finite positive real numbers, each <=10^9.
Output area to 10 decimals, or INVALID if no triangle exists.
A degenerate triangle (sum of two sides equals the third) has area zero.

Sample input:
3 4 5
Sample output:
6.0000000000

Heron: area=sqrt(s*(s-a)*(s-b)*(s-c)), where s=(a+b+c)/2.
When coordinates are known, a cross product is usually simpler.
For thin triangles, use the rearranged formula below after sorting a>=b>=c.
The parentheses matter: they reduce cancellation compared with direct Heron.
Reject invalid side lengths before taking sqrt. Clamp a small roundoff
error in a valid product to zero. Time/memory O(1). Floating-point error remains.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  long double a, b, c;
  cin >> a >> b >> c;
  if (a < b) swap(a, b);
  if (b < c) swap(b, c);
  if (a < b) swap(a, b);
  if (c <= 0 or a - b > c) { cout << "INVALID" << endl; return; }
  long double product = (a + (b + c)) * (c - (a - b))
                      * (c + (a - b)) * (a + (b - c));
  cout << fixed << setprecision(10) << sqrtl(max(0.0L, product)) / 4 << endl;
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
