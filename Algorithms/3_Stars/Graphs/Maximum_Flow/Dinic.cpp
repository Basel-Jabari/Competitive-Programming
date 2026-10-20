// Given a directed weighted graph with n nodes and m edges.
// Output the maximum flow from the node 1 to the node n.

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
vector<array<int, 3>> edges;
vector<int> lvl, cur;
int src, dst;

bool bfs()
{
	fill(lvl.begin(), lvl.end(), -1);

	queue<int> q;
	q.push(src);
	lvl[src] = 0;

	while (q.size())
	{
		int node = q.front();
		q.pop();

		for (auto idx : g[node])
		{
			int x = edges[idx][0], y = edges[idx][1], w = edges[idx][2];
			if (w and lvl[y] == -1)
			{
				lvl[y] = lvl[x] + 1;
				q.push(y);
			}
		}
	}

	return lvl[dst] != -1;
}

int dfs(int node, int flow)
{
	if (node == dst)
		return flow;

	for (int &gidx = cur[node]; gidx < g[node].size(); ++gidx)
	{
		int idx = g[node][gidx];
		int x = edges[idx][0], y = edges[idx][1], w = edges[idx][2];

		if (lvl[y] == lvl[x] + 1 and w)
		{
			int bottleneck = dfs(y, min(flow, w));

			if (bottleneck)
			{
				edges[idx][2] -= bottleneck;
				edges[idx ^ 1][2] += bottleneck;
				return bottleneck;
			}
		}
	}

	return 0;
}

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n + 1, {});
	edges.clear();
	lvl.resize(n + 1);
	cur.resize(n + 1);
	src = 1;
	dst = n;

	int tracker = 0;
	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g[x].push_back(tracker++);
		edges.push_back({ x, y, w });

		g[y].push_back(tracker++);
		edges.push_back({ y, x, 0 });
	}

	ll ans = 0;
	while (bfs())
	{
		fill(cur.begin(), cur.end(), 0);
		while (int flow = dfs(src, INT_MAX))
			ans += flow;
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
