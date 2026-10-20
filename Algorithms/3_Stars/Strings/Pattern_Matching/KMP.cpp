/*
Problem: Find every occurrence of pattern p in text s, including overlaps.
Input: Two nonempty lowercase strings: s, then p.
Output: The number of occurrences, then their 1-based start positions.
Print an empty second line when there is no match.
Bounds: 1 <= |s|, |p| <= 1000000.
Sample input:
aaaaa
aaa
Sample output:
3
1 2 3

KMP is exact pattern matching; it has no hash collisions.
pi[i] = length of the longest proper prefix of p[0..i] that is also
its suffix. "Proper" means shorter than the whole string.
Example: p = "ababa" gives pi = [0, 0, 1, 2, 3].
On a mismatch, j = pi[j - 1] keeps the next possible matched prefix.
Build + search: O(|p| + |s|). Extra memory: O(|p| + number of matches).
Pitfalls: After a match, fall back with pi to keep overlapping matches.
This input uses cin >>, so it excludes empty strings and spaces.
Internal indices are 0-based; returned positions below are 1-based.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

vector<int> Prefix_Function(const string &p) {
  vector<int> pi(p.size(), 0); // pi stores prefix-function values (the usual pi notation).
  for (int i = 1; i < (int)p.size(); ++i) {
    int j = pi[i - 1];
    while (j > 0 and p[i] != p[j]) {
      j = pi[j - 1];
    }
    if (p[i] == p[j]) {
      ++j;
    }
    pi[i] = j;
  }
  return pi;
}

vector<int> Find_Matches(const string &s, const string &p) {
  assert(!p.empty());
  vector<int> pi = Prefix_Function(p); // pi stores prefix-function values (the usual pi notation).
  vector<int> positions;
  int j = 0;
  for (int i = 0; i < (int)s.size(); ++i) {
    while (j > 0 and s[i] != p[j]) {
      j = pi[j - 1];
    }
    if (s[i] == p[j]) {
      ++j;
    }
    if (j == (int)p.size()) {
      positions.push_back(i - (int)p.size() + 2);
      j = pi[j - 1];
    }
  }
  return positions;
}

void Solve() {
  string s, p;
  cin >> s >> p;
  vector<int> positions = Find_Matches(s, p);
  cout << positions.size() << endl;
  for (int i = 0; i < (int)positions.size(); ++i) {
    if (i > 0) {
      cout << ' ';
    }
    cout << positions[i];
  }
  cout << endl;
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
