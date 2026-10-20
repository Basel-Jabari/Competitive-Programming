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

vector<vector<int>> g, rg, scc;
vector<int> topo, sccn;
vector<bool> v;
int cnt;

void topo_dfs(int node)
{
	v[node] = true;

	for (auto c : g[node])
		if (!v[c])
			topo_dfs(c);

	topo.push_back(node);
}

void scc_dfs(int node)
{
	scc[cnt].push_back(node);
	sccn[node] = cnt;

	for (auto c : rg[node])
		if (sccn[c] == -1)
			scc_dfs(c);
}

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n + 1, {});
	rg.assign(n + 1, {});
	v.assign(n + 1, false);
	scc.clear();
	sccn.assign(n + 1, -1);
	topo.clear();
	cnt = 0;

	for (int i = 0; i < m; ++i)
	{
		int x, y;
		cin >> x >> y;

		g[x].push_back(y);
		rg[y].push_back(x);
	}

	for (int i = 1; i <= n; ++i)
		if (!v[i])
			topo_dfs(i);

	reverse(topo.begin(), topo.end());
	for (int i = 0; i < n; ++i)
		if (sccn[topo[i]] == -1)
		{
			scc.push_back({});
			scc_dfs(topo[i]);
			cnt++;
		}

	for (auto d1 : scc)
	{
		for (auto d2 : d1)
			cout << d2 << ' ';
		cout << endl;
	}

	for(int i = 1; i <= n; ++i)
		cout << i << ' ' << sccn[i] << endl;
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
