// Given n elements, and q queries, there's two types of operations:
// 1. Update k-th index to val.
// 2. Output the sum from a to b.
// a, b, k are 1-base.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long

class Segment_Tree {
private:
  int _n;
  vector<ll> _segmentTree;

  void Merge(int curNode) {
    _segmentTree[curNode] = _segmentTree[2 * curNode + 1] + _segmentTree[2 * curNode + 2];
  }

  ll Operation(ll Q1, ll Q2) {
    return Q1 + Q2;
  }

  void Build(int curL, int curR, int curNode, const vector<int> &a) {
    if (curL == curR) {
      _segmentTree[curNode] = a[curL];
      return;
    }

    int curMid = (curL + curR) / 2;
    Build(curL, curMid, 2 * curNode + 1, a);
    Build(curMid + 1, curR, 2 * curNode + 2, a);

    Merge(curNode);
  }

  ll Query(int curL, int curR, int curNode, int requiredL, int requiredR) {
    if (curL > requiredR or curR < requiredL) {
      return 0;
    }

    if (curL >= requiredL and curR <= requiredR) {
      return _segmentTree[curNode];
    }

    int curMid = (curL + curR) / 2;
    ll Q1 = Query(curL, curMid, 2 * curNode + 1, requiredL, requiredR);
    ll Q2 = Query(curMid + 1, curR, 2 * curNode + 2, requiredL, requiredR);
    return Q1 + Q2;
  }

  void Update(int curL, int curR, int curNode, int requiredLeaf, int newVal) {
    if (curL == requiredLeaf and curR == requiredLeaf) {
      _segmentTree[curNode] = newVal;
      return;
    }

    int curMid = (curL + curR) / 2;
    if (requiredLeaf <= curMid) {
      Update(curL, curMid, 2 * curNode + 1, requiredLeaf, newVal);
    } else {
      Update(curMid + 1, curR, 2 * curNode + 2, requiredLeaf, newVal);
    }

    _segmentTree[curNode] = _segmentTree[2 * curNode + 1] + _segmentTree[2 * curNode + 2];
  }

public:
  Segment_Tree(const vector<int> &a) {
    _n = a.size();
    _segmentTree.resize(_n * 4, 0);
    Build(0, _n - 1, 0, a);
  }

  ll Query(int requiredL, int requiredR) {
    return Query(0, _n - 1, 0, requiredL, requiredR);
  }

  void Update(int requiredIndex, int newVal) {
    Update(0, _n - 1, 0, requiredIndex, newVal);
  }
};

void Solve() {
  int n, q;
  cin >> n >> q;

  vector<int> a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];

  Segment_Tree _segmentTree(a);
  for (int i = 1; i <= q; ++i) {
    int type;
    cin >> type;

    if (type == 1) {
      int k, val;
      cin >> k >> val;
      _segmentTree.Update(k - 1, val);
    } else {
      int a, b;
      cin >> a >> b;
      cout << _segmentTree.Query(a - 1, b - 1) << endl;
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
