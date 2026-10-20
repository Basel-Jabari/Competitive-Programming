// Given a number n, output YES if it is prime, NO otherwise.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long
#define ull unsigned long long

void Solve() {
  ull n;
  cin >> n;

  function<ull(ull, ull, ull)> Mul = [&](ull num1, ull num2, ull MOD) -> ull {
    return (unsigned __int128)num1 * num2 % MOD;
  };

  function<ull(ull, ull, ull)> Power = [&](ull n, ull p, ull MOD) -> ull {
    ull ans = 1, temp = n;
    while (p) {
      if (p & 1) {
        ans = Mul(ans, temp, MOD);
      }

      temp = Mul(temp, temp, MOD);
      p >>= 1;
    }

    return ans;
  };

  vector<ull> a = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

  function<bool(ull)> Rabin_Miller = [&](ull n) -> bool {
    if (n < 2 or n % 6 % 4 != 1) {
      return n - 2 < 2;
    }

    ull trailingZeroes = __builtin_ctzll(n - 1);
    ull d = (n - 1) >> trailingZeroes;

    for (int i = 0; i < a.size(); ++i) {
      ull p = Power(a[i], d, n), j = trailingZeroes;

      while (p != 1 and p != n - 1 and a[i] % n and j--) {
        p = Mul(p, p, n);
      }

      if (p != n - 1 and j != trailingZeroes) {
        return false;
      }
    }

    return true;
  };

  cout << (Rabin_Miller(n) ? "YES" : "NO") << endl;
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