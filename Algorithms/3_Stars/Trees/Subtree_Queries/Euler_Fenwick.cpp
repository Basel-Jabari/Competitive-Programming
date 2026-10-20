// Given a tree with n nodes, q queries, there's two types of queries:
// 1. Update the value of node s to value x.
// 2. Output the sum of the subtree of node s.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <chrono>

using namespace std;
using namespace __gnu_pbds;
using namespace chrono;

#define endl '\n'
#define ll long long

template<typename data_type>
using ordered_set = tree<data_type, null_type, less<data_type>, rb_tree_tag, tree_order_statistics_node_update>;

mt19937 rng(high_resolution_clock::now().time_since_epoch().count());

int dx[8] = { 0, 1, 0, -1, -1, 1, 1, -1 };
int dy[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };

vector<vector<int>> g;
vector<int> in, out, a;
int timer;

struct fenwick
{
	int n;
	vector<ll> ft;

	fenwick(int n)
	{
		this->n = n;
		ft.resize(n + 1);
	}

	void build()
	{
		for (int i = 1; i <= n; ++i)
		{
			int index = in[i];
			while (index <= n)
			{
				ft[index] += a[i];
				index += index & -index;
			}
		}
	}

	void set(int index, ll val)
	{
		while (index <= n)
		{
			ft[index] += val;
			index += index & -index;
		}
	}

	ll query(int index)
	{
		ll ans = 0;
		while (index)
		{
			ans += ft[index];
			index -= index & -index;
		}

		return ans;
	}
};

void dfs(int node, int par)
{
	in[node] = ++timer;

	for (auto c : g[node])
		if (c != par)
			dfs(c, node);

	out[node] = timer;
}

void Solve()
{
	int n, q;
	cin >> n >> q;

	g.assign(n + 1, {});
	in.assign(n + 1, 0);
	out.assign(n + 1, 0);
	a.assign(n + 1, 0);
	timer = 0;

	for (int i = 1; i <= n; ++i)
		cin >> a[i];

	for (int i = 0; i < n - 1; ++i)
	{
		int x, y;
		cin >> x >> y;

		g[x].push_back(y);
		g[y].push_back(x);
	}

	dfs(1, 0);
	fenwick tree(n);
	tree.build();

	while (q--)
	{
		int type;
		cin >> type;

		if (type == 1)
		{
			int s, x;
			cin >> s >> x;

			ll delta = (ll)x - a[s];
			a[s] = x;
			tree.set(in[s], delta);
		}

		else
		{
			int s;
			cin >> s;
			cout << tree.query(out[s]) - tree.query(in[s] - 1) << endl;
		}
	}
}

int32_t main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

#ifndef ONLINE_JUDGE
	freopen("C:\\Sublime\\input.txt", "r", stdin);
	freopen("C:\\Sublime\\output.txt", "w", stdout);
#endif

	Solve();

	cerr << "Basel Al-Jabari." << endl;

	return 0;
}

