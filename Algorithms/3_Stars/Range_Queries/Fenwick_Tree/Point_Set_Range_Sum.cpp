// Given n elements, and q queries, there are two types of operations:
// 1. Update k-th index to u.
// 2. Output the sum from x to y.
// x, y, k are 1-base.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long

class Fenwick_Tree {
private:
  int _n;
  vector<ll> _fenwickTree;

public:
  Fenwick_Tree(const vector<int> &a) {
    _n = a.size() - 1;
    _fenwickTree.assign(_n + 1, 0);
    Build(a);
  }

  void Merge(int requiredIndex, ll curVal) {
    _fenwickTree[requiredIndex] += curVal;
  }

  void Build(const vector<int> &a) {
    for (int i = 1; i <= _n; ++i) {
      int tempIndex = i;
      while (tempIndex <= _n) {
        Merge(tempIndex, a[i]);
        tempIndex += tempIndex & -tempIndex;
      }
    }
  }

  void Update(int requiredIndex, int newVal, vector<int> &a) {
    ll delta = (ll)newVal - a[requiredIndex];
    a[requiredIndex] = newVal;

    while (requiredIndex <= _n) {
      Merge(requiredIndex, delta);
      requiredIndex += requiredIndex & -requiredIndex;
    }
  }

  ll Get(int requiredIndex) {
    ll ans = 0;

    while (requiredIndex) {
      ans += _fenwickTree[requiredIndex];
      requiredIndex -= requiredIndex & -requiredIndex;
    }

    return ans;
  }
};

void Solve() {
  int n, q;
  cin >> n >> q;

  vector<int> a(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }

  Fenwick_Tree fenwickTree(a);
  for (int i = 1; i <= q; ++i) {
    int type;
    cin >> type;

    if (type == 1) {
      int k, u;
      cin >> k >> u;
      fenwickTree.Update(k, u, a);
    } else if (type == 2) {
      int x, y;
      cin >> x >> y;
      cout << fenwickTree.Get(y) - fenwickTree.Get(x - 1) << endl;
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