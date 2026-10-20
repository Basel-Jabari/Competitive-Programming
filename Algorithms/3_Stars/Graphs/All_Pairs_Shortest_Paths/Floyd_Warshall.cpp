// Given a weighted connected directed graph with n nodes, and m edges (1-base index).
// If there's a negative cycle, output IMPOSSIBLE.
// Else, output the shortest path from each node to the other nodes.
// If there's no path between two nodes, output INF.

// Note: If there's any negative weight in an undirected graph, automatically the answer will be IMPOSSIBLE (No solution).
// But if there's a negative weight in a directed graph, then if there's a negative cycle then the answer will be -IMPOSSIBLE (No solution).
// Else we use Bellman Ford's Algorithm, or Floyd Warshall's Algorithm to find the answer.

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

vector<vector<ll>> dist;
void Solve()
{
	int n, m;
	cin >> n >> m;

	dist.assign(n + 1, vector<ll> (n + 1, LLONG_MAX));
	for (int i = 1; i <= n; ++i)
		dist[i][i] = 0;

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		dist[x][y] = min(dist[x][y], (ll)w);
	}

	for (int k = 1; k <= n; ++k)
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= n; ++j)
				if (dist[i][k] != LLONG_MAX and dist[k][j] != LLONG_MAX)
					dist[i][j] = min(dist[i][j], (ll)max((__int128)LLONG_MIN, (__int128)dist[i][k] + dist[k][j]));

	bool neg_cycle = false;
	for (int i = 1; i <= n; ++i)
		if (dist[i][i] < 0)
			neg_cycle = true;

	if (neg_cycle)
		cout << "IMPOSSIBLE" << endl;

	else
	{
		for (int i = 1; i <= n; ++i)
		{
			for (int j = 1; j <= n; ++j)
				if (i != j)
					if (dist[i][j] == LLONG_MAX)
						cout << "INF" << ' ';
					else
						cout << dist[i][j] << ' ';
			cout << endl;
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

