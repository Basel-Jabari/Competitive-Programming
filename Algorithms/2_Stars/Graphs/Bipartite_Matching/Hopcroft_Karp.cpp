/*
Problem: match as many left vertices to right vertices as possible.
Each vertex may be used once. Also find a minimum vertex cover.
Input: L R m, then m lines u v with 1 <= u <= L, 1 <= v <= R.
Output: matching size; that many pairs; cover size; left cover count and IDs;
then right cover count and IDs. Any optimum matching/cover is accepted.
Limits: 1 <= L,R <= 1000, 0 <= m <= 200000. Duplicate edges allowed.

Sample input:
2 2 2
1 1
2 2
Sample output:
2
1 1
2 2
2
2 1 2
0

A bipartite graph has two sides; every edge joins different sides.
An augmenting path alternates unused and matched edges, from a free left
vertex to a free right vertex. Flipping its edges increases matching by one.
BFS finds the shortest free-right layer. DFS uses only paths of that length.
Time O((L+R+m)*sqrt(L+R)); memory O(L+R+m).
Recursive DFS depth <= L <= 1000 here; use an explicit stack for larger depths.
For a minimum vertex cover, start at unmatched left vertices and follow
unmatched edges left->right, then matched edges right->left.
Take unvisited left vertices and visited right vertices. Its size equals
the maximum matching (Konig's theorem). It touches every graph edge.
This is for bipartite graphs only, not general graph matching.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

void Solve() {
  int leftSize, rightSize, m;
  cin >> leftSize >> rightSize >> m;
  vector<vector<int>> graph(leftSize + 1);
  for (int i = 1; i <= m; ++i) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
  }
  vector<int> leftMatch(leftSize + 1), rightMatch(rightSize + 1);
  vector<int> distance(leftSize + 1), next(leftSize + 1);
  const int INF = 1000000000; // INF = infinity (a large marker value).
  int shortest;
  auto BFS = [&]() { // BFS = breadth-first search.
    queue<int> q;
    fill(distance.begin(), distance.end(), INF);
    for (int u = 1; u <= leftSize; ++u) {
      if (!leftMatch[u]) {
        distance[u] = 0;
        q.push(u);
      }
    }
    shortest = INF;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      if (distance[u] >= shortest) continue;
      for (int i = 0; i < (int)graph[u].size(); ++i) {
        int v = graph[u][i], nextLeft = rightMatch[v];
        if (!nextLeft) shortest = min(shortest, distance[u] + 1);
        else if (distance[nextLeft] == INF) {
          distance[nextLeft] = distance[u] + 1;
          q.push(nextLeft);
        }
      }
    }
    return shortest != INF;
  };
  function<bool(int)> DFS = [&](int u) { // DFS = depth-first search.
    for (int &i = next[u]; i < (int)graph[u].size(); ++i) {
      int v = graph[u][i], nextLeft = rightMatch[v];
      bool freeEnd = !nextLeft and distance[u] + 1 == shortest;
      if (freeEnd or (nextLeft and distance[nextLeft] == distance[u] + 1 and DFS(nextLeft))) {
        leftMatch[u] = v;
        rightMatch[v] = u;
        return true;
      }
    }
    distance[u] = INF;
    return false;
  };
  int matching = 0;
  while (BFS()) {
    fill(next.begin(), next.end(), 0);
    for (int u = 1; u <= leftSize; ++u) {
      if (!leftMatch[u] and DFS(u)) ++matching;
    }
  }
  vector<bool> seenLeft(leftSize + 1), seenRight(rightSize + 1);
  queue<int> q;
  for (int u = 1; u <= leftSize; ++u) {
    if (!leftMatch[u]) {
      seenLeft[u] = true;
      q.push(u);
    }
  }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < (int)graph[u].size(); ++i) {
      int v = graph[u][i];
      if (v == leftMatch[u] or seenRight[v]) continue;
      seenRight[v] = true;
      int nextLeft = rightMatch[v];
      if (nextLeft and !seenLeft[nextLeft]) {
        seenLeft[nextLeft] = true;
        q.push(nextLeft);
      }
    }
  }
  vector<int> coverLeft, coverRight;
  for (int u = 1; u <= leftSize; ++u) if (!seenLeft[u]) coverLeft.push_back(u);
  for (int v = 1; v <= rightSize; ++v) if (seenRight[v]) coverRight.push_back(v);
  cout << matching << endl;
  for (int u = 1; u <= leftSize; ++u) if (leftMatch[u]) cout << u << ' ' << leftMatch[u] << endl;
  cout << coverLeft.size() + coverRight.size() << endl;
  cout << coverLeft.size();
  for (int i = 0; i < (int)coverLeft.size(); ++i) cout << ' ' << coverLeft[i];
  cout << endl << coverRight.size();
  for (int i = 0; i < (int)coverRight.size(); ++i) cout << ' ' << coverRight[i];
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
