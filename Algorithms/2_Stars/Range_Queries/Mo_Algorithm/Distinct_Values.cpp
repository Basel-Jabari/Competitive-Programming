/*
Problem: count distinct values in each static range [l,r].
Input: n q, n values, then q lines l r. Indices are 1-based.
Limits: 1 <= n,q <= 200000, |a[i]| <= 10^9.

Sample input:
5 3
1 2 1 3 2
1 3
2 5
4 4
Sample output:
2
3
1

Mo reorders offline queries so a moving interval changes slowly.
Add/remove one value and maintain its frequency and the distinct count.
Coordinate compression makes frequencies an array even for negative values.
Sort by left block, then alternate right-end direction between blocks.
With B about n/sqrt(q), pointer work is O(n^2/B + q*B + n).
Total time includes O(n log n + q log q) sorting; memory O(n+q).
Queries may not change the array. Results are printed in their original order.
Mo is a general range technique, not specifically a range-minimum algorithm.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  vector<int> a(n + 1), values;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    values.push_back(a[i]);
  }
  sort(values.begin(), values.end());
  values.erase(unique(values.begin(), values.end()), values.end());
  for (int i = 1; i <= n; ++i) {
    a[i] = (int)(lower_bound(values.begin(), values.end(), a[i]) - values.begin()) + 1;
  }
  int blockSize = max(1, (int)(n / sqrt(q)));
  struct Query { int l, r, id; };
  vector<Query> queries(q);
  for (int i = 0; i < q; ++i) {
    cin >> queries[i].l >> queries[i].r;
    queries[i].id = i + 1;
  }
  sort(queries.begin(), queries.end(), [&](const Query &first, const Query &second) {
    int leftA = (first.l - 1) / blockSize, leftB = (second.l - 1) / blockSize;
    if (leftA != leftB) return leftA < leftB;
    return leftA % 2 ? first.r > second.r : first.r < second.r;
  });
  vector<int> frequency(values.size() + 1), answer(q + 1);
  int distinct = 0, left = 1, right = 0;
  auto Add = [&](int pos) { if (++frequency[a[pos]] == 1) ++distinct; };
  auto Remove = [&](int pos) { if (--frequency[a[pos]] == 0) --distinct; };
  for (int i = 0; i < q; ++i) {
    auto [l, r, id] = queries[i];
    while (left > l) Add(--left);
    while (right < r) Add(++right);
    while (left < l) Remove(left++);
    while (right > r) Remove(right--);
    answer[id] = distinct;
  }
  for (int i = 1; i <= q; ++i) cout << answer[i] << endl;
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
