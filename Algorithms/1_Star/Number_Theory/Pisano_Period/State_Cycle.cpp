/*
Problem: find the Fibonacci period modulo m, then F(N) modulo m.
Input: m N, with 1<=m<=2000 and 0<=N<=10^18. F(0)=0,F(1)=1.
Output period and F(N)%m.

Sample input:
3 10
Sample output:
8 1

The state is the pair (F(i),F(i+1)) modulo m, not one Fibonacci value.
Stop when the initial pair (0,1) returns. The transition is invertible,
so this state sequence is a cycle from the start. There are at most m^2 states.
For m=1 the period is 1 and all remainders are 0.
Once period P is known, use index N%P. This is useful for repeated Fibonacci
queries with the SAME small modulus or a decimal index too large to store.
For one large-modulus query, fast doubling avoids finding the period.
Time O(P), P<=m^2 for this state bound; memory O(P).
Fibonacci indices naturally start at 0; array positions here equal those indices.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int mod; // mod = modulus.
  unsigned long long n;
  cin >> mod >> n;
  if (mod == 1) { cout << "1 0" << endl; return; }
  int a = 0, b = 1;
  vector<int> fibonacci;
  do {
    fibonacci.push_back(a);
    int next = (a + b) % mod;
    a = b;
    b = next;
  } while (a != 0 or b != 1);
  cout << fibonacci.size() << ' ' << fibonacci[n % fibonacci.size()] << endl;
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
