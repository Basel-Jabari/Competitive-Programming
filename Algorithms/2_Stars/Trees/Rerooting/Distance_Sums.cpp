// Given a tree with n nodes, and n - 1 edges (1-base index).
// Find the sum of distances from any root to the other nodes.
// https://leetcode.com/problems/sum-of-distances-in-tree/submissions/1461129850/

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
vector<ll> sum, ans;
vector<int> sz;

// sum[current] represents the sum of all possible subtrees that are in the subtree of current.
// sz[current] represents the size of the subtree of node current.
void dfs1(int node, int par)
{
	for (auto c : g[node])
		if (c != par)
		{
			dfs1(c, node);
			sz[node] += sz[c];
			sum[node] += sum[c];
		}

	sum[node] += sz[node];
}

// In this question, the distance between the root node and any other node (current) equals:
// The number of all subtrees that current node is in it - 1
// (To exclude the subtree that is rooted at the current root node).
void dfs2(int node, int par)
{
	// sum[node] - sz[node] will return the sum of all the appearance times of each node
	// in every possible subtree that are in the subtree of the current root node
	// except the main subtree that is rooted at the current root node.
	ans[node] = sum[node] - sz[node];

	for (auto c : g[node])
		if (c != par)
		{
			// Removed c from the subtree of node (as it is the old root).
			sum[node] -= sum[c];
			sum[node] -= sz[c];
			sz[node] -= sz[c];

			// Added node to the subtree of c (as it is the new root).
			sum[c] += sum[node];
			sum[c] += sz[node];
			sz[c] += sz[node];

			dfs2(c, node);

			// Remove node from the subtree of c (as it is the old root).
			sum[c] -= sum[node];
			sum[c] -= sz[node];
			sz[c] -= sz[node];

			// Added c to the subtree of node (as it is the new root).
			sum[node] += sum[c];
			sum[node] += sz[c];
			sz[node] += sz[c];
		}
}

void Solve()
{
	int n;
	cin >> n;

	g.assign(n + 1, {});
	sum.assign(n + 1, 0);
	sz.assign(n + 1, 1);
	ans.assign(n + 1, 0);

	for (int i = 0; i < n - 1; ++i)
	{
		int x, y;
		cin >> x >> y;

		g[x].push_back(y);
		g[y].push_back(x);
	}

	dfs1(1, 0);
	dfs2(1, 0);

	for (int i = 1; i <= n; ++i)
		cout << ans[i] << ' ';
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
