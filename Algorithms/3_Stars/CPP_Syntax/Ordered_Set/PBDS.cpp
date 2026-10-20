/*
Keep a collection of integers. Equal values are allowed.
Input: q, followed by q operations:
1 x: insert x. 2 x: erase ONE copy of x, if it exists.
3 x: print the counts < x, <= x, > x, >= x, in this order.
4 k: print the k-th smallest value. 5 k: print the k-th largest value.
k is 1-based. Print NONE when k is outside [1, size].
Limits: 1 <= q <= 200000; -10^18 <= x <= 10^18; |k| <= 10^18.
Sample input:
10
1 5
1 2
1 5
3 5
4 2
5 3
2 5
3 5
4 3
5 1
Sample output:
1 3 0 2
5
2
1 2 0 1
NONE
5

Use: online rank and k-th queries. Each operation O(log(q)); memory O(q).
Requires GNU PBDS. These two ext/pb_ds headers are needed.
less<T> sorts small to large; greater<T> sorts large to small.
NEVER use less_equal or greater_equal as the comparator.
A tree needs a strict order: compare(x, x) must be false.
For duplicates, store {value, unique_id}; do not break that rule.

Exact syntax for UNIQUE values:
ordered_set<ll> a;
ordered_set<ll, greater<ll>> b;
a.insert(7); a.erase(7);
a.order_of_key(x);
b.order_of_key(x);
a.find_by_order(0);
b.find_by_order(0);
a uses less<ll> by default. A repeated insert(7) does nothing.
order_of_key(x) counts values < x in a, and values > x in b.
find_by_order(0) returns the smallest in a and the largest in b.
PBDS ranks start at 0. The helper methods below take k starting at 1.
Always check iterator != tree.end() before reading *iterator.

Unique-value bound examples, using {2, 5, 9}:
a.lower_bound(5) -> 5 (first >= 5); a.upper_bound(5) -> 9 (first > 5).
b.lower_bound(5) -> 5 (first <= 5); b.upper_bound(5) -> 2 (first < 5).
The bounds follow the comparator; descending order changes their meaning.
For unique a: count(<=x) = a.order_of_key(x) + (a.find(x) != a.end()).
For unique b: count(>=x) = b.order_of_key(x) + (b.find(x) != b.end()).
Below, pair sentinels include/exclude all equal values without using x+1.
We keep two trees to show both orders. One ascending tree is enough if
largest queries use find_by_order(size-k) and > counts use complements.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds; // PBDS = policy-based data structures.

#define endl '\n'
#define ll long long // ll = long long.

template<typename T, typename Compare = less<T>>
using ordered_set = tree<T, null_type, Compare, rb_tree_tag,
                         tree_order_statistics_node_update>;

class Ordered_Multiset {
private:
  ordered_set<pair<ll, int>> _ascending;
  ordered_set<pair<ll, int>, greater<pair<ll, int>>> _descending;
  int _nextId = 0;

public:
  int Size() const {
    return (int) _ascending.size();
  }

  void Insert(ll x) {
    pair<ll, int> key = {x, _nextId++};
    _ascending.insert(key);
    _descending.insert(key);
  }

  void Erase_One(ll x) {
    auto it = _ascending.lower_bound({x, INT_MIN});
    if (it == _ascending.end() or it->first != x) {
      return;
    }
    pair<ll, int> key = *it;
    _ascending.erase(key);
    _descending.erase(key);
  }

  int Count_Less(ll x) const {
    return (int) _ascending.order_of_key({x, INT_MIN});
  }

  int Count_Less_Equal(ll x) const {
    return (int) _ascending.order_of_key({x, INT_MAX});
  }

  int Count_Greater(ll x) const {
    return (int) _descending.order_of_key({x, INT_MAX});
  }

  int Count_Greater_Equal(ll x) const {
    return (int) _descending.order_of_key({x, INT_MIN});
  }

  /* k is 1-based; the caller must check 1 <= k <= Size(). */
  ll Kth_Smallest(int k) const {
    return _ascending.find_by_order(k - 1)->first;
  }

  ll Kth_Largest(int k) const {
    return _descending.find_by_order(k - 1)->first;
  }
};

void Solve() {
  int q;
  cin >> q;
  Ordered_Multiset values;

  for (int query = 1; query <= q; ++query) {
    int type;
    ll x;
    cin >> type >> x;
    if (type == 1) {
      values.Insert(x);
    } else if (type == 2) {
      values.Erase_One(x);
    } else if (type == 3) {
      cout << values.Count_Less(x) << ' ' << values.Count_Less_Equal(x) << ' '
           << values.Count_Greater(x) << ' ' << values.Count_Greater_Equal(x) << endl;
    } else if (x < 1 or x > values.Size()) {
      cout << "NONE" << endl;
    } else if (type == 4) {
      cout << values.Kth_Smallest((int) x) << endl;
    } else {
      cout << values.Kth_Largest((int) x) << endl;
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
