/*
Problem: Keep a collection of lowercase words, including duplicates.
Input: q, then q operations:
1 word: Insert one copy.
2 word: Remove one copy if present; otherwise do nothing.
3 word: Print the number of copies of this exact word.
4 prefix: Print the number of stored words starting with this prefix.
All words/prefixes are nonempty and use only 'a'..'z'.
Bounds: 1 <= q <= 200000; sum of lengths in ALL operations <= 200000.
Sample input:
10
1 app
1 apple
1 app
3 app
4 app
2 app
3 app
2 ape
4 ap
3 apple
Sample output:
2
3
1
2
1

A trie follows one edge per letter. end counts exact copies; pass counts
all copies that visit the node, including words that end there.
Each operation: O(word length). Memory: O(26 * total inserted characters).
Nodes are kept after removal; memory depends on all inserts, not live words.
Pitfalls: Count a word before removing it, or missing words may make
counts negative. Prefix counts include duplicate words.
Do not keep a reference to a vector node across emplace_back: it may move.
No string indexing is given in input; internal letters map to 0..25.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Trie {
private:
  struct Node {
    array<int, 26> next;
    int pass = 0;
    int end = 0;

    Node() {
      next.fill(-1);
    }
  };

  vector<Node> _nodes;

  int Find(const string &word) const {
    int node = 0;
    for (int index1 = 0; index1 < (int)word.size(); ++index1) {
      char letter = word[index1];
      int c = letter - 'a';
      if (_nodes[node].next[c] == -1) {
        return -1;
      }
      node = _nodes[node].next[c];
    }
    return node;
  }

public:
  Trie() {
    _nodes.emplace_back();
  }

  void Insert(const string &word) {
    int node = 0;
    ++_nodes[node].pass;
    for (int index2 = 0; index2 < (int)word.size(); ++index2) {
      char letter = word[index2];
      int c = letter - 'a';
      if (_nodes[node].next[c] == -1) {
        int newNode = (int)_nodes.size();
        _nodes[node].next[c] = newNode;
        _nodes.emplace_back();
      }
      node = _nodes[node].next[c];
      ++_nodes[node].pass;
    }
    ++_nodes[node].end;
  }

  int Count(const string &word) const {
    int node = Find(word);
    return node == -1 ? 0 : _nodes[node].end;
  }

  int Count_Prefix(const string &prefix) const {
    int node = Find(prefix);
    return node == -1 ? 0 : _nodes[node].pass;
  }

  bool Remove(const string &word) {
    if (Count(word) == 0) {
      return false;
    }
    int node = 0;
    --_nodes[node].pass;
    for (int index3 = 0; index3 < (int)word.size(); ++index3) {
      char letter = word[index3];
      node = _nodes[node].next[letter - 'a'];
      --_nodes[node].pass;
    }
    --_nodes[node].end;
    return true;
  }
};

void Solve() {
  int q;
  cin >> q;
  Trie trie;
  for (int query = 1; query <= q; ++query) {
    int type;
    string word;
    cin >> type >> word;
    if (type == 1) {
      trie.Insert(word);
    } else if (type == 2) {
      trie.Remove(word);
    } else if (type == 3) {
      cout << trie.Count(word) << endl;
    } else {
      cout << trie.Count_Prefix(word) << endl;
    }
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
