/*
Problem: compute I+A+A^2+...+A^k modulo m.
Input: d k m, then a d*d matrix A. Output exactly d rows with d entries.
Limits: 1<=d<=60, 0<=k<=10^18, 1<=m<=10^9+7, entries in [0,m-1].

Sample input:
2 2 1000
1 1
0 1
Sample output:
3 3
0 3

For an adjacency matrix, the result counts walks of length AT MOST k.
I counts length-zero walks. For lengths 1..k only, subtract I modulo m.
Represent a block of t steps by P=A^t and S=I+...+A^(t-1).
Joining blocks (P1,S1),(P2,S2) gives (P1*P2, S1+P1*S2).
Binary exponentiation joins k+1 terms, without dividing by A-I.
All products have size (d*d)*(d*d)=(d*d); the answer is d*d.
Time O(d^3 log(k+2)); memory O(d^2). Matrix indices are 1-based.
Update the sum before overwriting the power used by that sum.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int d;
  ll k, mod; // mod = modulus.
  cin >> d >> k >> mod;

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
      for (int shared = 1; shared <= d; ++shared) {
        for (int j = 1; j <= d; ++j) {
          result[i][j] = (result[i][j] + a[i][shared] * b[shared][j]) % mod;
        }
      }
    }
    return result;
  };

  auto Add = [&](Matrix a, const Matrix &b) {
    for (int i = 1; i <= d; ++i) {
      for (int j = 1; j <= d; ++j) a[i][j] = (a[i][j] + b[i][j]) % mod;
    }
    return a;
  };
  Matrix basePower = Zero(), baseSum = Identity();
  for (int i = 1; i <= d; ++i) {
    for (int j = 1; j <= d; ++j) cin >> basePower[i][j];
  }
  Matrix resultPower = Identity(), resultSum = Zero();
  ll terms = k + 1;
  while (terms > 0) {
    if (terms & 1) {
      resultSum = Add(resultSum, Multiply(resultPower, baseSum));
      resultPower = Multiply(resultPower, basePower);
    }
    baseSum = Add(baseSum, Multiply(basePower, baseSum));
    basePower = Multiply(basePower, basePower);
    terms >>= 1;
  }
  for (int i = 1; i <= d; ++i) {
    for (int j = 1; j <= d; ++j) cout << resultSum[i][j] << (j == d ? '\n' : ' ');
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
