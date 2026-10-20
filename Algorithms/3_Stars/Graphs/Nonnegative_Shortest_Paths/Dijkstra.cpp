// Given a weighted connected undirected graph with n nodes, and m edges (1-base index).
// Output the shortest path from the node 1 to the other nodes.
// There's no negative weights.

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

vector<vector<pair<int, int>>> g;
vector<ll> dist;

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n + 1, {});
	dist.assign(n + 1, LLONG_MAX);

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g[x].push_back(make_pair(y, w));
		g[y].push_back(make_pair(x, w));
	}

	// Start from node 1.
	// Why set instead of priority queue?
	// Because the set gives us the opportunity to erase the unneeded edges.
	set<pair<ll, int>> dijk;
	dijk.insert(make_pair(0, 1)); // Weight 0, Node 1.
	dist[1] = 0;
	while (dijk.size())
	{
		ll current = dijk.begin()->first;
		int node = dijk.begin()->second;
		dijk.erase(dijk.begin());

		for (auto [c, w] : g[node])
			if (current + w < dist[c])
			{
				auto temp = dijk.find(make_pair(dist[c], c));
				if (temp != dijk.end())
					dijk.erase(temp);

				dist[c] = current + w;
				dijk.insert(make_pair(dist[c], c));
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
