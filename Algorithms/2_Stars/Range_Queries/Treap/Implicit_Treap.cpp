/*
Problem: edit a sequence of integers.
Input: n q, n values, then q operations:
1 pos x: insert x BEFORE pos (1..size+1).
2 pos: erase this position. 3 l r: reverse [l,r]. 4 l r: print sum[l,r].
All positions are 1-based and valid; an empty starting sequence is allowed.
Limits: 0 <= n <= 200000, 1 <= q <= 200000, |values| <= 10^9.

Sample input:
3 5
1 2 3
4 1 3
3 1 3
1 2 9
2 3
4 1 3
Sample output:
6
13

An implicit treap stores sequence order in its in-order traversal.
Random priorities keep expected height O(log n). Subtree size gives position.
Split(root,k) returns the first k values and the rest; k is a COUNT.
Merge(left,right) requires every left value to come before every right value.
To work on [l,r], split at l-1, then split the rest at r-l+1.
Push the reversal flag BEFORE descending; pull size/sum AFTER changing children.
Initial build O(n); each operation expected O(log(n+q)); memory O(n+q).
Worst-case height and operation time are linear; recursion shares that risk.
Erased nodes are retained in the node pool. Index 0 is the null node.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  struct Node {
    int left = 0, right = 0, size = 1;
    ll value = 0, sum = 0;
    uint64_t priority = 0;
    bool reverse = false;
  };
  vector<Node> nodes(1);
  nodes.reserve(n + q + 1);
  nodes[0].size = 0;
  mt19937_64 random(chrono::steady_clock::now().time_since_epoch().count());
  auto New_Node = [&](ll value) {
    Node node;
    node.value = node.sum = value;
    node.priority = random();
    nodes.push_back(node);
    return (int)nodes.size() - 1;
  };
  auto Pull = [&](int u) {
    nodes[u].size = 1 + nodes[nodes[u].left].size + nodes[nodes[u].right].size;
    nodes[u].sum = nodes[u].value + nodes[nodes[u].left].sum + nodes[nodes[u].right].sum;
  };
  auto Push = [&](int u) {
    if (!u or !nodes[u].reverse) return;
    swap(nodes[u].left, nodes[u].right);
    if (nodes[u].left) nodes[nodes[u].left].reverse ^= true;
    if (nodes[u].right) nodes[nodes[u].right].reverse ^= true;
    nodes[u].reverse = false;
  };
  function<pair<int,int>(int,int)> Split = [&](int u, int count) -> pair<int,int> {
    if (!u) return {0, 0};
    Push(u);
    int leftSize = nodes[nodes[u].left].size;
    if (count <= leftSize) {
      auto [a, b] = Split(nodes[u].left, count);
      nodes[u].left = b;
      Pull(u);
      return {a, u};
    }
    auto [a, b] = Split(nodes[u].right, count - leftSize - 1);
    nodes[u].right = a;
    Pull(u);
    return {u, b};
  };
  function<int(int,int)> Merge = [&](int a, int b) {
    if (!a or !b) return a ? a : b;
    if (nodes[a].priority >= nodes[b].priority) {
      Push(a);
      nodes[a].right = Merge(nodes[a].right, b);
      Pull(a);
      return a;
    }
    Push(b);
    nodes[b].left = Merge(a, nodes[b].left);
    Pull(b);
    return b;
  };
  vector<int> stack;
  for (int i = 1; i <= n; ++i) {
    ll value;
    cin >> value;
    int u = New_Node(value), last = 0;
    while (!stack.empty() and nodes[stack.back()].priority < nodes[u].priority) {
      last = stack.back();
      stack.pop_back();
    }
    nodes[u].left = last;
    if (!stack.empty()) nodes[stack.back()].right = u;
    stack.push_back(u);
  }
  int root = stack.empty() ? 0 : stack.front();
  vector<int> order;
  if (root) order.push_back(root);
  for (int i = 0; i < (int)order.size(); ++i) {
    int u = order[i];
    if (nodes[u].left) order.push_back(nodes[u].left);
    if (nodes[u].right) order.push_back(nodes[u].right);
  }
  for (int i = (int)order.size() - 1; i >= 0; --i) Pull(order[i]);
  for (int i = 1; i <= q; ++i) {
    int type, l;
    cin >> type >> l;
    auto [a, rest] = Split(root, l - 1);
    if (type == 1) {
      ll x;
      cin >> x;
      root = Merge(Merge(a, New_Node(x)), rest);
    } else {
      int r = l;
      if (type >= 3) cin >> r;
      auto [middle, b] = Split(rest, r - l + 1);
      if (type == 2) middle = 0;
      else if (type == 3) nodes[middle].reverse ^= true;
      else cout << nodes[middle].sum << endl;
      root = Merge(Merge(a, middle), b);
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
