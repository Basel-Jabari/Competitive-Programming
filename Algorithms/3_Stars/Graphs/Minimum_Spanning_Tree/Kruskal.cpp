// Given a weighted connected undirected graph with n nodes, and m edges (1-base index).
// Output the sum of the minimum spanning tree.

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
vector<int> sz, par;

bool cmp(const vector<int> &a, const vector<int> &b)
{
	return a[2] < b[2];
}

int find(int node)
{
	if (par[node] == -1)
		return node;

	return par[node] = find(par[node]);
}

void Union(int s1, int s2)
{
	if (sz[s1] > sz[s2])
		swap(s1, s2);

	par[s1] = s2;
	sz[s2] += sz[s1];
	sz[s1] = 0;
}

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(m, {});
	sz.assign(n + 1, 1);
	par.assign(n + 1, -1);

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g[i] = { x, y, w };
	}

	sort(g.begin(), g.end(), cmp);

	ll ans = 0;
	for (int i = 0; i < m; ++i)
	{
		int x = g[i][0], y = g[i][1], w = g[i][2];

		int s1 = find(x), s2 = find(y);
		if (s1 != s2)
		{
			ans += w;
			Union(s1, s2);
		}
	}

	cout << ans << endl;
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
