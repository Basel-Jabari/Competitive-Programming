// Given a weighted connected directed graph with n nodes, and m edges (1-base index).
// If there's a negative cycle, output IMPOSSIBLE.
// Else, output the shortest path from the node 1 to the other nodes.

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

vector<vector<int>> g;
vector<ll> dist;

void Solve()
{
	int n, m;
	cin >> n >> m;

	// Edge List.
	g.clear();
	dist.assign(n + 1, LLONG_MAX);

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g.push_back({ x, y, w });
	}

	// It should be 0 not INT_MIN, because we will add the dist of it to the dist of it's childs.
	dist[1] = 0;

	// If the edges of the source node are the first in the edge list.
	// Then the edges of the childs of the source node and so on.
	// Then it is sorted (No need to relax the edges).
	// But if it is not, we have to relax the edges, by iterating for n - 1 times.
	// Because each dist of a node depends on the dist of an other node.
	// If the dist of the other node is not calculated, then we cannot find out the dist of the current node.
	// That's why we need to relax the edges.
	for (int i = 0; i < n; ++i)
		for (auto edge : g)
		{
			int x = edge[0], y = edge[1], w = edge[2];

			// dist[x] != LLONG_MAX to prevent overflow in dist[x] + w.
			// We can fix it also by making dist long long instead of int.
			if (dist[x] != LLONG_MAX and dist[x] + w < dist[y])
				dist[y] = dist[x] + w;
		}

	bool neg_cycle = false;
	for (int i = 0; i < n; ++i)
		for (auto edge : g)
		{
			int x = edge[0], y = edge[1], w = edge[2];

			if (dist[x] != LLONG_MAX and dist[x] + w < dist[y])
				neg_cycle = true;
		}

	if (neg_cycle)
		cout << "IMPOSSIBLE" << endl;

	else
	{
		for (int i = 2; i <= n; ++i)
			cout << dist[i] << ' ';
		cout << endl;
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
