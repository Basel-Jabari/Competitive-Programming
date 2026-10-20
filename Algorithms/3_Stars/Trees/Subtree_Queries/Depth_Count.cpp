// Given a tree with n nodes, q queries, the tree root is node 1.
// The next n - 1 lines will give x, y (Edge between node x and node y)(1-base index).
// The next q lines will give u and l.
// For each query, output the number of nodes that are in the subtree of the node u, and the distance between the root and the node equals to l.

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

vector<vector<int>> g, lvl;
vector<int> tin, tout, degree;
int timer;

void dfs(int node, int par)
{
	tin[node] = timer++;
	lvl[degree[node]].push_back(tin[node]);

	for (auto c : g[node])
		if (c != par)
		{
			degree[c] = degree[node] + 1;
			dfs(c, node);
		}

	tout[node] = timer++;
}

void Solve()
{
	int n, q;
	cin >> n >> q;

	g.assign(n + 1, {});
	lvl.assign(n + 1, {});
	tin.assign(n + 1, 0);
	tout.assign(n + 1, 0);
	degree.assign(n + 1, 0);
	timer = 0;

	for (int i = 0; i < n - 1; ++i)
	{
		int x, y;
		cin >> x >> y;

		g[x].push_back(y);
		g[y].push_back(x);
	}

	dfs(1, 0);
	while (q--)
	{
		int u, l;
		cin >> u >> l;

		if (l < 0 or l > n)
		{
			cout << 0 << endl;
			continue;
		}

		auto &temp = lvl[l];
		auto left = lower_bound(temp.begin(), temp.end(), tin[u]);
		auto right = lower_bound(temp.begin(), temp.end(), tout[u]);
		cout << right - left << endl;
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
