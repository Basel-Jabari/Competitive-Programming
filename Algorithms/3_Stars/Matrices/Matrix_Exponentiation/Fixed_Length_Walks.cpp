/*
Problem: count directed walks of exactly k edges between every pair of vertices.
A walk may repeat vertices and edges. Different parallel edges are distinct choices.
Input: n m k, then m directed edges u v, using vertices 1 through n.
Output: n rows of n counts modulo 1000000007.
Entry in row u, column v is the number of length-k walks from u to v.
Limits: 1 <= n <= 100, 0 <= m <= 200000, 0 <= k <= 10^18.
Self-loops and parallel edges are allowed.
Sample input:
3 4 2
1 2
1 2
2 3
1 3
Sample output:
0 0 2
0 0 0
0 0 0

A[u][v] counts edges from u to v. Then (A^k)[u][v] counts length-k walks.
In A*B, the shared index is the vertex where two parts of a walk meet.
Use binary exponentiation to apply the transition many times.
All matrices here are n*n: (n x n) * (n x n) = (n x n).
A^0 is the identity matrix: exactly one empty walk from a vertex to itself.
Time: O(m + n^3 log(k + 1) + n^2); extra memory: O(n^2).
Syntax: start result[i][i] = 1, not an all-ones or all-zero matrix.
Use += for parallel edges, not assignment to 1. Keep input vertices 1-based.
Multiply returns a new matrix, so input entries are not overwritten mid-product.
Reduce every added product modulo MOD; products fit long long at this MOD.
General use: a fixed linear transition can often be applied with matrix power.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

const ll MOD = 1000000007; // MOD = modulus.
using Matrix = vector<vector<ll>>;

Matrix Multiply(const Matrix &a, const Matrix &b) {
  int n = (static_cast<int>(a.size()) - 1);
  Matrix result(n + 1, vector<ll>(n + 1, 0));
  for (int i = 1; i <= n; ++i) {
    for (int t = 1; t <= n; ++t) {
      if (a[i][t] == 0) {
        continue;
      }
      for (int j = 1; j <= n; ++j) {
        result[i][j] = (result[i][j] + a[i][t] * b[t][j]) % MOD;
      }
    }
  }
  return result;
}

Matrix Power(Matrix base, ll exponent) {
  int n = (static_cast<int>(base.size()) - 1);
  Matrix result(n + 1, vector<ll>(n + 1, 0));
  for (int i = 1; i <= n; ++i) {
    result[i][i] = 1;
  }
  while (exponent > 0) {
    if (exponent & 1) {
      result = Multiply(result, base);
    }
    base = Multiply(base, base);
    exponent >>= 1;
  }
  return result;
}

void Solve() {
  int n, m;
  ll k;
  cin >> n >> m >> k;
  Matrix adjacency(n + 1, vector<ll>(n + 1, 0));
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adjacency[u][v] = (adjacency[u][v] + 1) % MOD;
  }
  Matrix answer = Power(adjacency, k);
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (j > 1) {
        cout << ' ';
      }
      cout << answer[i][j];
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
