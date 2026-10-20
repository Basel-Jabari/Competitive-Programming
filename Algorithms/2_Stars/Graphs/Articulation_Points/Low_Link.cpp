// Given an undirected graph with n nodes and m edges,
// print articulation points.

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define endl '\n'
#define ll long long

template<typename data_type>
using ordered_set = tree<data_type, null_type, less<data_type>, rb_tree_tag, tree_order_statistics_node_update>;

int dx[16] = { 0, 1, 0, -1, -1, 1, 1, -1, -1, -1, +1, +1, -2, -2, +2, +2 };
int dy[16] = { 1, 0, -1, 0, 1, 1, -1, -1, -2, +2, -2, +2, -1, +1, -1, +1 };

void Solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adjList(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;

        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    vector<bool> vis(n + 1);
    vector<int> discoveryTime(n + 1), lowestTime(n + 1);
    int curTime = 0;
    set<int> articulationPoints;

    function<void(int, int)> apDFS = [&](int curNode, int prevNode) -> void {
        vis[curNode] = true;
        discoveryTime[curNode] = lowestTime[curNode] = curTime++;

        int nextNodes = 0;
        for (int nextNode : adjList[curNode]) {
            if (!vis[nextNode]) {
                nextNodes++;

                apDFS(nextNode, curNode);

                lowestTime[curNode] = min(lowestTime[curNode], lowestTime[nextNode]);

                if (prevNode and discoveryTime[curNode] <= lowestTime[nextNode]) {
                    articulationPoints.insert(curNode);
                }
            }

            else if (nextNode != prevNode) {
                lowestTime[curNode] = min(lowestTime[curNode], discoveryTime[nextNode]);
            }
        }

        if (!prevNode) {
            if (nextNodes > 1) {
                articulationPoints.insert(curNode);
            }
        }
        };

    for (int i = 1; i <= n; ++i) {
        if (!vis[i]) {
            apDFS(i, 0);
        }
    }

    for (int node : articulationPoints) {
        cout << node << ' ';
    }

    cout << endl;
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int t = 1;
    // cin >> t;
    for (int i = 1; i <= t; ++i) {
        Solve();
    }

    cerr << "Basel Al-Jabari." << endl;

    return 0;
}
