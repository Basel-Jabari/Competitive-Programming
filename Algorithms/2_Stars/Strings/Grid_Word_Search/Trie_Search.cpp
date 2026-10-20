/*
Problem: count DISTINCT dictionary words found in a lowercase grid.
Move up/down/left/right; do not reuse a cell in one word.
Input: rows columns w, grid rows, then w nonempty lowercase words.
Output the count. Repeated dictionary words count once.
Limits: 1 <= rows,columns <= 4, 1 <= w <= 1000, word length <= 10.

Sample input:
2 2 4
ab
cd
ab
ac
aba
ab
Sample output:
2

A trie lets many dictionary words share the same search.
Stop a search when its prefix is absent from the trie.
Clear a found terminal flag so later paths cannot count the same word again.
Mark the current cell before DFS and unmark it when returning.
With maximum word length L, worst-case time O(total word lengths +
rows*columns*4*3^(L-1)); trie pruning often saves much work.
Memory O(26*total word lengths + rows*columns + L). Recursion depth <= 10.
Grid coordinates are 1-based; character positions inside a word are 0-based.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int rows, columns, w;
  cin >> rows >> columns >> w;
  vector<string> grid(rows + 1);
  for (int i = 1; i <= rows; ++i) {
    cin >> grid[i];
    grid[i] = " " + grid[i];
  }
  struct Node {
    array<int, 26> next{};
    bool terminal = false;
  };
  vector<Node> trie(2);
  for (int i = 1; i <= w; ++i) {
    string word;
    cin >> word;
    int node = 1;
    for (int j = 0; j < (int)word.size(); ++j) {
      int letter = word[j] - 'a';
      if (!trie[node].next[letter]) {
        int child = trie.size();
        trie.push_back(Node());
        trie[node].next[letter] = child;
      }
      node = trie[node].next[letter];
    }
    trie[node].terminal = true;
  }
  vector<vector<bool>> used(rows + 1, vector<bool>(columns + 1));
  int answer = 0;
  int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1}; // dr = change in row; dc = change in column.
  function<void(int,int,int)> DFS = [&](int r, int c, int node) { // DFS = depth-first search.
    if (r < 1 or r > rows or c < 1 or c > columns or used[r][c]) return;
    node = trie[node].next[grid[r][c] - 'a'];
    if (!node) return;
    if (trie[node].terminal) {
      ++answer;
      trie[node].terminal = false;
    }
    used[r][c] = true;
    for (int i = 0; i < 4; ++i) DFS(r + dr[i], c + dc[i], node);
    used[r][c] = false;
  };
  for (int r = 1; r <= rows; ++r) {
    for (int c = 1; c <= columns; ++c) DFS(r, c, 1);
  }
  cout << answer << endl;
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
