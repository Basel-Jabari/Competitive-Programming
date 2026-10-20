/*
Problem: for each pair a, b, print their GCD and LCM.
Input: q, then q lines with a b. Output: gcd(a, b) lcm(a, b).
Limits: 1 <= q <= 200000, 1 <= a, b <= 10^9.
Sample input:
3
12 18
7 5
1 1
Sample output:
6 36
1 35
1 1

GCD is the largest common divisor. LCM is the smallest common multiple.
Use GCD to simplify a fraction or find a common step size.
Use LCM when two repeating events must happen together.
gcd(a, b) = gcd(b, a % b). Stop when b becomes zero.
Time per pair: O(log(min(a, b))). Extra memory: O(1).
Syntax: C++17 also has std::gcd(a, b) and std::lcm(a, b) in <numeric>.
Divide BEFORE multiplying: (a / gcd) * b. The LCM must fit in long long.
These limits guarantee LCM <= 10^18; larger inputs need another check.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

ll GCD(ll a, ll b) { // GCD = greatest common divisor.
  while (b != 0) {
    ll remainder = a % b;
    a = b;
    b = remainder;
  }
  return a;
}

ll LCM(ll a, ll b) { // LCM = least common multiple.
  return (a / GCD(a, b)) * b;
}

void Solve() {
  int q;
  cin >> q;
  for (int query = 1; query <= q; ++query) {
    ll a, b;
    cin >> a >> b;
    cout << GCD(a, b) << ' ' << LCM(a, b) << endl;
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
