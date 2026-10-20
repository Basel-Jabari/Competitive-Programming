/*
Problem: build the min Cartesian tree of a[1..n].
Input: n then n integers. Output: root index, then parent,left,right for each
index 1..n. Zero means no parent/child. Equal values prefer the earlier index.
Limits: 1 <= n <= 200000, |a[i]| <= 10^9.

Sample input:
3
3 1 2
Sample output:
2
2 0 0
0 1 3
2 0 0

In-order traversal gives indices 1..n. Each parent has a smaller value,
or the same value and an earlier index, than its children.
Pop larger values from the current right boundary. The last popped subtree
becomes the new node's left child; the surviving top becomes its parent.
Each index enters/leaves the stack once: O(n) time and O(n) memory.
Useful reduction: RMQ(l,r) is the LCA of index l and index r in this tree.
Building the tree alone does not provide fast LCA queries.
A treap has a similar shape rule, but uses random priorities to stay balanced.
A Cartesian tree can be a chain; use iterative traversals afterwards.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n;
  cin >> n;
  vector<ll> a(n + 1);
  vector<int> parent(n + 1), left(n + 1), right(n + 1), stack;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    int last = 0;
    while (!stack.empty() and a[stack.back()] > a[i]) {
      last = stack.back();
      stack.pop_back();
    }
    if (!stack.empty()) {
      parent[i] = stack.back();
      right[stack.back()] = i;
    }
    if (last) {
      left[i] = last;
      parent[last] = i;
    }
    stack.push_back(i);
  }
  cout << stack.front() << endl;
  for (int i = 1; i <= n; ++i) {
    cout << parent[i] << ' ' << left[i] << ' ' << right[i] << endl;
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
