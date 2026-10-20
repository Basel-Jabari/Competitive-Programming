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

vector<vector<pair<int, int>>> g;
vector<bool> v;

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n + 1, {});
	v.assign(n + 1, false);

	for (int i = 0; i < m; ++i)
	{
		int x, y, w;
		cin >> x >> y >> w;

		g[x].push_back(make_pair(y, w));
		g[y].push_back(make_pair(x, w));
	}

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	q.push(make_pair(0, 1));

	ll ans = 0;
	while (q.size())
	{
		int current = q.top().first, node = q.top().second;
		q.pop();

		if (!v[node])
		{
			v[node] = true;
			ans += current;

			for (auto [c, w] : g[node])
				q.push(make_pair(w, c));
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
