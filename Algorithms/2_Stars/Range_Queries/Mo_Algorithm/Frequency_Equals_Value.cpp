// https://codeforces.com/contest/221/problem/D

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long

class Query {
public:
  int l, r, block, index;

  bool operator<(Query &query) const {
    return make_pair(this->block, this->r) < make_pair(query.block, query.r);
  }
};

void Solve() {
  int n, q;
  cin >> n >> q;

  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  int blockCnt = sqrt(n) + 1;
  vector<Query> queries(q);
  for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;

    l--;
    r--;

    queries[i].index = i;
    queries[i].l = l;
    queries[i].r = r;
    queries[i].block = l / blockCnt;
  }

  sort(queries.begin(), queries.end());

  unordered_map<int, int> freq;
  int curAns = 0;

  function<void(int)> Add = [&](int index) -> void {
    int curNum = a[index];
    if (freq.count(curNum)) {
      if (freq[curNum] == curNum) {
        curAns--;
      }
    }

    freq[curNum]++;
    if (freq[curNum] == curNum) {
      curAns++;
    }
  };

  function<void(int)> Remove = [&](int index) -> void {
    int curNum = a[index];

    if (freq.count(curNum)) {
      if (freq[curNum] == curNum) {
        curAns--;
      }
    }

    freq[curNum]--;
    if (freq[curNum] == curNum) {
      curAns++;
    }
  };

  vector<int> ans(q);
  int curL = queries[0].l, curR = queries[0].l;
  for (int i = 0; i < q; ++i) {
    while (curL > queries[i].l) {
      curL--;
      Add(curL);
    }

    while (curR > queries[i].r + 1) {
      curR--;
      Remove(curR);
    }

    while (curL < queries[i].l) {
      Remove(curL);
      curL++;
    }

    while (curR < queries[i].r + 1) {
      Add(curR);
      curR++;
    }

    ans[queries[i].index] = curAns;
  }

  for (int i = 0; i < q; ++i) {
    cout << ans[i] << endl;
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