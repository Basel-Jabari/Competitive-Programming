/*
Problem: multiply integer polynomials.
Input: n m, then n coefficients of A and m coefficients of B, lowest degree first.
Output n+m-1 coefficients of A*B in increasing degree.
Limits for this floating-point example: 1<=n,m<=10000, |coefficients|<=1000.

Sample input:
3 2
1 2 3
4 5
Sample output:
4 13 22 15

Convolution: C[k]=sum A[i]*B[j] over i+j=k.
FFT changes coefficients to values at roots of unity. Pointwise multiplication
then becomes polynomial multiplication. Inverse FFT returns coefficients.
Pad to a power of two >=n+m-1, otherwise coefficients wrap around.
The butterfly combines two half-size transforms. Bit reversal puts entries
in the order required by the iterative stages.
Time O(N log N); memory O(N), N=the padded length.
Round only AFTER the inverse transform and division by N.
Coefficient index is its degree, so it starts at 0. Keep that convention.
Floating-point rounding is not an exact arithmetic guarantee. These modest
bounds limit error; use NTT/CRT for large coefficients or strict modular results.
long double precision differs between platforms; avoid blindly raising limits.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  using Complex = complex<long double>;
  int n, m;
  cin >> n >> m;
  int size = 1;
  while (size < n + m - 1) size *= 2;
  vector<Complex> a(size), b(size);
  for (int i = 0; i < n; ++i) { ll x; cin >> x; a[i] = x; }
  for (int i = 0; i < m; ++i) { ll x; cin >> x; b[i] = x; }
  auto FFT = [&](vector<Complex> &values, bool inverse) { // FFT = fast Fourier transform.
    for (int i = 1, j = 0; i < size; ++i) {
      int bit = size >> 1;
      while (j & bit) { j ^= bit; bit >>= 1; }
      j ^= bit;
      if (i < j) swap(values[i], values[j]);
    }
    for (int length = 2; length <= size; length *= 2) {
      long double angle = 2 * acosl(-1.0L) / length * (inverse ? -1 : 1);
      Complex root(cosl(angle), sinl(angle));
      for (int start = 0; start < size; start += length) {
        Complex factor = 1;
        for (int j = 0; j < length / 2; ++j) {
          Complex u = values[start + j], v = values[start + j + length / 2] * factor;
          values[start + j] = u + v;
          values[start + j + length / 2] = u - v;
          factor *= root;
        }
      }
    }
    if (inverse) {
      for (int i = 0; i < size; ++i) values[i] /= size;
    }
  };
  FFT(a, false);
  FFT(b, false);
  for (int i = 0; i < size; ++i) a[i] *= b[i];
  FFT(a, true);
  for (int i = 0; i < n + m - 1; ++i) cout << llroundl(a[i].real()) << (i == n + m - 2 ? '\n' : ' ');
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
