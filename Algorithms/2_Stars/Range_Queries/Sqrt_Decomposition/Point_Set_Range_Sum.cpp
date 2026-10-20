/*
Problem: maintain an array. Operation 1 k x sets a[k]=x; 2 l r prints sum[l,r].
Input: n q, n values, then q operations. Indices are 1-based.
Limits: 1 <= n,q <= 200000, |initial/new values| <= 10^9.

Sample input:
4 3
1 2 3 4
2 2 4
1 3 10
2 1 3
Sample output:
9
13

Split the array into blocks of about sqrt(n) values and store each block sum.
Read complete blocks at once; read only individual values at the ends.
Build O(n); point set O(1); sum query O(sqrt(n)); memory O(n).
Fenwick is usually better for this exact task. Blocks are a flexible starting
point when a more unusual operation is easy inside one block.
block(i)=(i-1)/B+1 keeps array and block IDs 1-based.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q;
  cin >> n >> q;
  int blockSize = max(1, (int)sqrt(n));
  auto Block = [&](int i) { return (i - 1) / blockSize + 1; };
  vector<ll> a(n + 1), sum(Block(n) + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    sum[Block(i)] += a[i];
  }
  auto Query = [&](int l, int r) {
    ll answer = 0;
    while (l <= r) {
      if ((l - 1) % blockSize == 0 and l + blockSize - 1 <= r) {
        answer += sum[Block(l)];
        l += blockSize;
      } else {
        answer += a[l];
        ++l;
      }
    }
    return answer;
  };
  for (int i = 1; i <= q; ++i) {
    int type, l;
    ll x;
    cin >> type >> l >> x;
    if (type == 1) {
      sum[Block(l)] += x - a[l];
      a[l] = x;
    } else cout << Query(l, (int)x) << endl;
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
