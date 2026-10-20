// Given undirected graph with n nodes and m edges,
// and print the edges of each component.

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
    vector<int> discoveryTime(n + 1), lowestTime(n + 1), nodeToComponent(n + 1);
    stack<array<int, 2>> curEdges;
    int curTime = 0;
    vector<vector<array<int, 2>>> componentToEdge;

    function<void(int, int)> sccDFS = [&](int curNode, int prevNode) -> void {
        vis[curNode] = 1;
        discoveryTime[curNode] = lowestTime[curNode] = curTime++;

        bool skippedParent = false;
        for (int nextNode : adjList[curNode]) {
            if (nextNode == prevNode and !skippedParent) {
                skippedParent = true;
                continue;
            }

            if (!vis[nextNode]) {
                curEdges.push({ curNode, nextNode });
                sccDFS(nextNode, curNode);

                lowestTime[curNode] = min(lowestTime[curNode], lowestTime[nextNode]);

                if (discoveryTime[curNode] <= lowestTime[nextNode]) {
                    componentToEdge.push_back({});

                    int u, v;
                    do {
                        u = curEdges.top()[0];
                        v = curEdges.top()[1];
                        curEdges.pop();

                        componentToEdge.back().push_back({ u, v });
                    } while (make_pair(u, v) != make_pair(curNode, nextNode));
                }
            }

            else if (discoveryTime[nextNode] < discoveryTime[curNode]) {
                curEdges.push({ curNode, nextNode });
                lowestTime[curNode] = min(lowestTime[curNode], discoveryTime[nextNode]);
            }
        }
        };

    for (int i = 1; i <= n; ++i) {
        if (!vis[i]) {
            sccDFS(i, 0);
        }
    }

    if (curEdges.size()) {
        componentToEdge.push_back({});

        while (curEdges.size()) {
            componentToEdge.back().push_back(curEdges.top());
            curEdges.pop();     
        }
    }

    for (vector<array<int, 2>> &component : componentToEdge) {
        for (auto [u, v] : component) {
            cout << u << " <--> " << v << ' ';
        }

        cout << endl;
    }
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
