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

  vector<int> LPS(m);
  function<void()> calculateLPS = [&]() -> void {
    int l = 0, r = 1;
    while (r < m) {
      if (p[r] == p[l]) {
        LPS[r] = l + 1;

        l++;
        r++;
      } else {
        if (l) {
          l = LPS[l - 1];
        } else {
          r++;
        }
      }
    }
  };

  function<int()> KMP = [&]() -> int {
    int ans = 0, i = 0, j = 0;
    while (i < n) {
      if (s[i] == p[j]) {
        i++;
        j++;

        if (j == m) {
          ans++;
          j = LPS[j - 1];
        }
      } else {
        if (j) {
          j = LPS[j - 1];
        } else {
          i++;
        }
      }
    }

    return ans;
  };

  calculateLPS();
  cout << KMP() << endl;
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