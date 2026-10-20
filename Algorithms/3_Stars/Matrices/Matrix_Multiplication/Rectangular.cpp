/*
Problem: multiply A of size r*k by B of size k*c, modulo m.
Input: r k c m, then r rows of k values for A, then k rows of c values for B.
Output: exactly r rows, each with c values, for C = A*B modulo m.
Limits: 1 <= r, k, c <= 200, 2 <= m <= 10^9 + 7,
        every matrix entry is in [0, m-1].
Sample input:
3 4 2 1000
1 0 0 1
0 1 1 0
1 1 1 1
1 2
3 4
5 6
7 8
Sample output:
8 10
8 10
16 20

Size rule: (r*k) * (k*c) = (r*c). The inner sizes must match.
Here: (3x4) * (4x2) = (3x2), so the result has 6 entries.
C[i][j] = sum of A[i][t] * B[t][j] over t = 1, ..., k.
Time: O(r*k*c). Extra space for the result: O(r*c).
Total storage including input: O(r*k + k*c + r*c).
Syntax: vector<vector<ll>>(rows + 1, vector<ll>(columns + 1, 0)).
Logical size is (A.size()-1) by (A[1].size()-1); storage includes padding.
The matrices must be nonempty and rectangular. Indices here start at 1; row/column 0 are unused.
Allocate C with r rows and c columns, not k rows or k columns.
Reduce each added product modulo m: accumulating the whole sum can overflow.
Products fit long long only under the stated modulus bound.
Matrix order matters: Multiply(A, B, m) usually differs from Multiply(B, A, m).
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

using Matrix = vector<vector<ll>>;

Matrix Multiply(const Matrix &a, const Matrix &b, ll mod) {
  int rows = (static_cast<int>(a.size()) - 1);
  int shared = (static_cast<int>(a[1].size()) - 1);
  int columns = (static_cast<int>(b[1].size()) - 1);
  assert(shared == (static_cast<int>(b.size()) - 1));
  Matrix result(rows + 1, vector<ll>(columns + 1, 0));
  for (int i = 1; i <= rows; ++i) {
    for (int t = 1; t <= shared; ++t) {
      if (a[i][t] == 0) {
        continue;
      }
      for (int j = 1; j <= columns; ++j) {
        result[i][j] = (result[i][j] + a[i][t] * b[t][j]) % mod;
      }
    }
  }
  return result;
}

void Solve() {
  int r, k, c;
  ll mod; // mod = modulus.
  cin >> r >> k >> c >> mod;
  Matrix a(r + 1, vector<ll>(k + 1));
  Matrix b(k + 1, vector<ll>(c + 1));
  for (int i = 1; i <= r; ++i) {
    for (int j = 1; j <= k; ++j) {
      cin >> a[i][j];
    }
  }
  for (int i = 1; i <= k; ++i) {
    for (int j = 1; j <= c; ++j) {
      cin >> b[i][j];
    }
  }
  Matrix result = Multiply(a, b, mod);
  for (int i = 1; i <= r; ++i) {
    for (int j = 1; j <= c; ++j) {
      if (j > 1) {
        cout << ' ';
      }
      cout << result[i][j];
    }
    cout << endl;
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
