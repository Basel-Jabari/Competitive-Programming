/*
Problem: test whether each unsigned 64-bit integer is prime.
Input: q then q values n in [0,18446744073709551615]. q<=10000.
Output YES or NO per value.

Sample input:
5
0
2
341
1000000007
18446744073709551615
Sample output:
NO
YES
NO
YES
NO

Write n-1=d*2^s with d odd. For a prime, each chosen base a must satisfy
a^d=1, or repeated squaring must reach -1 modulo n.
A failed base proves composite. This exact seven-base set is deterministic
for ALL unsigned 64-bit inputs; arbitrary few small bases are not equivalent.
Time O(log n) for this fixed base set; extra memory O(1).
Use unsigned __int128 for multiplication: signed __int128 may overflow near
2^64. Small numbers and even composites need separate handling.
This tests primality; it does not factor a composite number.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  using ull = unsigned long long; // ull = unsigned long long.
  auto Multiply = [&](ull a, ull b, ull mod) { // mod = modulus.
    return (ull)((unsigned __int128)a * b % mod);
  };
  auto Power = [&](ull a, ull e, ull mod) { // mod = modulus.
    ull answer = 1;
    while (e) {
      if (e & 1) answer = Multiply(answer, a, mod);
      a = Multiply(a, a, mod);
      e >>= 1;
    }
    return answer;
  };
  auto Is_Prime = [&](ull n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    ull d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }
    ull bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (int i = 0; i < 7; ++i) {
      ull a = bases[i] % n;
      if (a == 0) continue;
      ull x = Power(a, d, n);
      if (x == 1 or x == n - 1) continue;
      bool passed = false;
      for (int j = 1; j < s; ++j) {
        x = Multiply(x, x, n);
        if (x == n - 1) { passed = true; break; }
      }
      if (!passed) return false;
    }
    return true;
  };
  int q;
  cin >> q;
  for (int i = 1; i <= q; ++i) {
    ull n;
    cin >> n;
    cout << (Is_Prime(n) ? "YES" : "NO") << endl;
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
