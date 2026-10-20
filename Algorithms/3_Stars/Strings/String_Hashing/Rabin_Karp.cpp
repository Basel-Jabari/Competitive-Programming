// Given string s and pattern p, output the number of times p occures in s.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long

void Solve() {
  string s, p;
  cin >> s >> p;

  int n = s.size(), m = p.size();
  if (m > n) {
    cout << 0 << endl;
  }

  else {
    vector<bool> matched(n - m + 1, true);
    function<ll(int, int)> Rabin_Karp = [&](int BASE, int MOD) -> int {
      function<ll(char)> Convert = [&](char toConvert) -> ll {
        return toConvert - 'a' + 1;
      };

      function<int(int, int)> Power = [&](int number, int power) -> int {
        ll numberPower = 1, temp = number;
        while (power) {
          if (power & 1) {
            numberPower = (numberPower * temp) % MOD;
          }

          temp = (temp * temp) % MOD;
          power >>= 1;
        }

        return numberPower;
      };

      function<int(string &, int, int)> Rolling_Hash = [&](string &originalString, int fromIndex, int toIndex) -> int {
        ll hashedString = 0, basePower = BASE;

        for (int i = fromIndex; i <= toIndex; ++i) {
          hashedString = (hashedString + (Convert(originalString[i]) * basePower)) % MOD;
          basePower = (basePower * BASE) % MOD;
        }

        return hashedString;
      };

      ll stringHash = Rolling_Hash(s, 0, m - 1), patternHash = Rolling_Hash(p, 0, m - 1), ans = 0;
      matched[0] = matched[0] and stringHash == patternHash;
      if (matched[0]) {
        ans++;
      }

      int baseInverse = Power(BASE, MOD - 2), basePower = Power(BASE, m);
      for (int i = 1; i + m - 1 < n; ++i) {
        stringHash = (stringHash - Convert(s[i - 1]) * BASE + MOD) % MOD;
        stringHash = (stringHash * baseInverse) % MOD;
        stringHash = (stringHash + Convert(s[i + m - 1]) * basePower) % MOD;

        matched[i] = matched[i] and stringHash == patternHash;
        if (matched[i]) {
          ans++;
        }
      }

      return ans;
    };

    Rabin_Karp(911, 1e9 + 7);
    cout << Rabin_Karp(3571, 1e9 + 9) << endl;
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