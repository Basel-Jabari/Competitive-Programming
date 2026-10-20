/*
Problem: maintain n square matrices and query A[l]*A[l+1]*...*A[r] modulo m.
Input: n q d m, then n matrices of d rows and d columns.
Operation 1 l r: print the product as exactly d rows of d entries.
Operation 2 pos: read d*d entries and replace A[pos].
Limits: 1<=n,q<=20000, 1<=d<=5, 1<=m<=10^9+7, entries in [0,m-1].
Matrix positions, row indices, and column indices are all 1-based.

Sample input:
2 2 2 1000
1 1
0 1
2 0
0 3
1 1 2
1 2 2
Sample output:
2 3
0 3
2 0
0 3

Here RMQ means a range matrix-product query, not a range minimum.
Multiplication is associative, so a segment tree can combine adjacent ranges.
Order matters: combine LEFT*RIGHT. Do not sort or swap the factors.
Return the identity for an empty intersection. An all-zero matrix is wrong.
(d*d)*(d*d) returns d*d; every query has the same exact output size.
Build O(n*d^3); update/query O(d^3 log n); memory O(n*d^2).
This is useful for a sequence of transitions with point changes.
For static matrices, other range-product structures are possible, but a
normal overlapping sparse table is invalid: multiplication is not idempotent.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int n, q, d;
  ll mod; // mod = modulus.
  cin >> n >> q >> d >> mod;

  using Matrix = vector<vector<ll>>;
  auto Zero = [&]() { return Matrix(d + 1, vector<ll>(d + 1)); };
  auto Identity = [&]() {
    Matrix a = Zero();
    for (int i = 1; i <= d; ++i) a[i][i] = 1 % mod;
    return a;
  };
  auto Multiply = [&](const Matrix &a, const Matrix &b) {
    Matrix result = Zero();
    for (int i = 1; i <= d; ++i) {
      for (int k = 1; k <= d; ++k) {
        for (int j = 1; j <= d; ++j) {
          result[i][j] = (result[i][j] + a[i][k] * b[k][j]) % mod;
        }
      }
    }
    return result;
  };

  auto Read = [&]() {
    Matrix a = Zero();
    for (int i = 1; i <= d; ++i) {
      for (int j = 1; j <= d; ++j) cin >> a[i][j];
    }
    return a;
  };
  vector<Matrix> tree(4 * n + 4);
  function<void(int,int,int)> Build = [&](int node, int l, int r) {
    if (l == r) { tree[node] = Read(); return; }
    int mid = (l + r) / 2;
    Build(node * 2, l, mid);
    Build(node * 2 + 1, mid + 1, r);
    tree[node] = Multiply(tree[node * 2], tree[node * 2 + 1]);
  };
  function<void(int,int,int,int,const Matrix&)> Update =
    [&](int node, int l, int r, int pos, const Matrix &value) {
      if (l == r) { tree[node] = value; return; }
      int mid = (l + r) / 2;
      if (pos <= mid) Update(node * 2, l, mid, pos, value);
      else Update(node * 2 + 1, mid + 1, r, pos, value);
      tree[node] = Multiply(tree[node * 2], tree[node * 2 + 1]);
    };
  function<Matrix(int,int,int,int,int)> Query =
    [&](int node, int l, int r, int ql, int qr) -> Matrix {
      if (qr < l or r < ql) return Identity();
      if (ql <= l and r <= qr) return tree[node];
      int mid = (l + r) / 2;
      return Multiply(Query(node * 2, l, mid, ql, qr),
                      Query(node * 2 + 1, mid + 1, r, ql, qr));
    };
  Build(1, 1, n);
  for (int query = 1; query <= q; ++query) {
    int type, l;
    cin >> type >> l;
    if (type == 2) Update(1, 1, n, l, Read());
    else {
      int r;
      cin >> r;
      Matrix answer = Query(1, 1, n, l, r);
      for (int i = 1; i <= d; ++i) {
        for (int j = 1; j <= d; ++j) cout << answer[i][j] << (j == d ? '\n' : ' ');
      }
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
