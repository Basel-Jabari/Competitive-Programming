/*
Problem: Compare pairs of substrings of one fixed lowercase string.
Input: s, then q, then q lines: l1 r1 l2 r2.
Output: YES when the two substrings have equal length and both hashes
match; otherwise NO. This is a probabilistic equality test, not a proof.
Indices are 1-based; both ends are included.
Bounds: 1 <= |s|, q <= 200000; each interval is nonempty and inside s.
Sample input:
abacaba
4
1 3 5 7
1 2 2 3
2 2 6 6
1 3 1 4
Sample output:
YES
NO
YES
NO

Hash a string left to right: h = h * BASE + value, modulo MOD.
prefix[r] - prefix[l-1]*BASE^(r-l+1) gives the hash of [l,r].
Equal text has equal hashes at every position: no modular inverse needed.
Use both moduli for the SAME pair of substrings, and check lengths first.
Build: O(n). Query: O(1). Extra memory: O(n).
Keep the original (base, modulus) pairs: (911, 1000000007) and
(3571, 1000000009).
Pitfalls: Different strings can share even two hashes. These fixed bases
do not make the test exact.
If an exact answer is required, use an exact method (KMP for one pattern)
or verify matching text directly; verification can take O(length).
All residues/products fit ll for the moduli below. Text is fixed: no updates.
The class uses 1-based inclusive ranges [l, r].
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Double_Hash {
private:
  static constexpr ll MOD1 = 1000000007; // MOD1 = first modulus.
  static constexpr ll MOD2 = 1000000009; // MOD2 = second modulus.
  vector<ll> _prefix1, _prefix2, _power1, _power2;

public:
  /* Pass the SAME bases to every object if you compare different strings. */
  Double_Hash(const string &s, ll base1, ll base2) {
    int n = (int)s.size();
    _prefix1.assign(n + 1, 0);
    _prefix2.assign(n + 1, 0);
    _power1.assign(n + 1, 1);
    _power2.assign(n + 1, 1);
    for (int i = 0; i < n; ++i) {
      ll value = s[i] - 'a' + 1;
      _prefix1[i + 1] = (_prefix1[i] * base1 + value) % MOD1;
      _prefix2[i + 1] = (_prefix2[i] * base2 + value) % MOD2;
      _power1[i + 1] = _power1[i] * base1 % MOD1;
      _power2[i + 1] = _power2[i] * base2 % MOD2;
    }
  }

  pair<ll, ll> Query(int l, int r) const {
    ll h1 = (_prefix1[r] - _prefix1[l - 1] * _power1[r - l + 1] % MOD1 + MOD1) % MOD1;
    ll h2 = (_prefix2[r] - _prefix2[l - 1] * _power2[r - l + 1] % MOD2 + MOD2) % MOD2;
    return {h1, h2};
  }
};

void Solve() {
  string s;
  int q;
  cin >> s >> q;

  const ll base1 = 911, base2 = 3571;
  Double_Hash hash(s, base1, base2);

  for (int query = 1; query <= q; ++query) {
    int l1, r1, l2, r2;
    cin >> l1 >> r1 >> l2 >> r2;
    /* Query indices are already 1-based and inclusive. */
    bool equal = r1 - l1 == r2 - l2
                 and hash.Query(l1, r1) == hash.Query(l2, r2);
    cout << (equal ? "YES" : "NO") << endl;
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
