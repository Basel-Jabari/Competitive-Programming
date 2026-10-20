// Given a weighted connected undirected graph with n nodes, and m edges (1-base index).
// Output the shortest path from the node 1 and other nodes.
// Each weight is either 0 or 1.

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

vector<vector<pair<int, bool>>> g;
vector<int> dist;

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n + 1, {});
	dist.assign(n + 1, INT_MAX);

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g[x].push_back(make_pair(y, w));
		g[y].push_back(make_pair(x, w));
	}

	deque<int> dijk;
	dijk.push_back(1); // Weight 0, Node 1.
	dist[1] = 0;
	while (dijk.size())
	{
		int node = dijk.front();
		dijk.pop_front();

		for (auto [c, w] : g[node])
			if (dist[node] + w < dist[c])
			{
				dist[c] = dist[node] + w;
				if (w)
					dijk.push_back(c);
				else
					dijk.push_front(c);
			}
	}

	// The shortest path between node 1 and the other nodes.
	for (int i = 2; i <= n; ++i)
		cout << dist[i] << ' ';
	cout << endl;
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
