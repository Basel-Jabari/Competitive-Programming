// Given a tree with n nodes, q queries, the tree root is node 1.
// The next n - 1 lines will give x, y (Edge between node x and node y)(1-base index).
// The next q lines will give a and b.
// For each query, output the lowest common ancestor of a and b.

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
vector<int> euler, eulerlvl, lvl, firstocc;
int timer;

void dfs(int node, int par)
{
	euler[timer] = node;
	eulerlvl[timer] = lvl[node];
	if (firstocc[node] == -1)
		firstocc[node] = timer;
	timer++;

	for (auto c : g[node])
		if (c != par)
		{
			lvl[c] = lvl[node] + 1;
			dfs(c, node);
			euler[timer] = node;
			eulerlvl[timer] = lvl[node];
			timer++;
		}
}

struct segment
{
	int n;
	vector<ll> st;

	segment(int n)
	{
		this->n = n;
		st.resize(n * 4, -1);
	}

	void build(int s, int e, int node)
	{
		if (s == e)
		{
			st[node] = s;
			return;
		}

		int mid = (s + e) >> 1;
		build(s, mid, 2 * node + 1);
		build(mid + 1, e, 2 * node + 2);

		st[node] = (eulerlvl[st[2 * node + 1]] <= eulerlvl[st[2 * node + 2]] ? st[2 * node + 1] : st[2 * node + 2]);
	}

	void build()
	{
		build(0, n - 1, 0);
	}

	int query(int s, int e, int node, int rs, int re)
	{
		if (s > re || e < rs)
			return -1;

		if (s >= rs and e <= re)
			return st[node];

		int mid = (s + e) >> 1;
		int q1 = query(s, mid, 2 * node + 1, rs, re);
		int q2 = query(mid + 1, e, 2 * node + 2, rs, re);

		if (q1 == -1)
			return q2;

		if (q2 == -1)
			return q1;

		return (eulerlvl[q1] <= eulerlvl[q2] ? q1 : q2);
	}

	int query(int rs, int re)
	{
		return query(0, n - 1, 0, rs, re);
	}
};

void Solve()
{
	int n, q;
	cin >> n >> q;

	g.assign(n + 1, {});
	lvl.assign(n + 1, 0);
	euler.assign(2 * n, 0);
	eulerlvl.assign(2 * n, 0);
	firstocc.assign(n + 1, -1);
	timer = 0;

	for (int i = 0; i < n - 1; ++i)
	{
		int x, y;
		cin >> x >> y;

		g[x].push_back(y);
		g[y].push_back(x);
	}
	dfs(1, 0);

	segment tree(2 * n);
	tree.build();

	while (q--)
	{
		int a, b;
		cin >> a >> b;

		if (firstocc[a] > firstocc[b])
			swap(a, b);

		cout << euler[tree.query(firstocc[a], firstocc[b])] << endl;
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
