/*
Problem: send as much flow as possible from source s to sink t.
Input: n m s t, then m directed edges u v capacity. Vertices are 1-based.
Output: maximum flow, then the number of vertices on the source side
of a minimum cut, then these vertices in increasing order.
If s == t, print INVALID: this template needs different endpoints.
Limits: 2 <= n <= 500; 0 <= m <= 5000; 0 <= capacity <= 1e9.
Parallel edges and self-loops are allowed. Total flow <= m*1e9.
Sample input:
4 5 1 4
1 2 3
1 3 2
2 3 1
2 4 2
3 4 3
Sample output:
5
1
1

Flow is an amount sent along edges. Capacity is an edge's maximum.
Except at s and t, the amount entering a vertex must equal the amount
leaving it. Applications include matching and resource assignment.
Residual capacity means how much MORE we can send on an edge.
When sending f from u to v, subtract f from that residual capacity
and add f to its reverse edge. The reverse edge lets us undo earlier
choices and send that flow along a better route later.

Dinic repeats:
1. BFS makes levels, using only edges with positive residual capacity.
2. DFS sends flow only from one level to the next.
3. Keep sending until no s-to-t path remains in these levels.
   This is a blocking flow. Then rebuild the levels with BFS.
ptr[u] skips edges already found useless in the current BFS phase.

After maximum flow, mark vertices reachable from s in the residual
graph. Original edges from marked to unmarked vertices form a minimum
cut: removing them separates s from t, with the smallest capacity sum.
That sum equals the maximum flow. Call Source_Side AFTER Max_Flow.
General time bound: O(n^2 * m). Memory: O(n + m).
Send() is recursive; the stated n <= 500 bounds its call depth.
For much larger/deeper graphs, use an iterative blocking-flow routine.
*/

// The Messenger of Allah (Peace and blessings be upon him) said: "Whoever is humble for the sake of Allah, Allah will raise him".

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long // ll = long long.

class Dinic {
private:
  struct Edge {
    int to;
    ll capacity; /* Remaining capacity, not the original capacity. */
  };

  vector<Edge> _edges;
  vector<vector<int>> _graph;
  vector<int> _level, _ptr; // ptr = pointer; the index of the next edge to try.

  bool Build_Levels(int source, int sink) {
    fill(_level.begin(), _level.end(), -1);
    queue<int> q;
    _level[source] = 0;
    q.push(source);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int index1 = 0; index1 < (int)_graph[u].size(); ++index1) {
        int id = _graph[u][index1];
        const Edge &edge = _edges[id];
        if (edge.capacity > 0 and _level[edge.to] == -1) {
          _level[edge.to] = _level[u] + 1;
          q.push(edge.to);
        }
      }
    }
    return _level[sink] != -1;
  }

  ll Send(int u, int sink, ll available) {
    if (u == sink) {
      return available;
    }
    for (int &i = _ptr[u]; i < (int)_graph[u].size(); ++i) {
      int id = _graph[u][i];
      Edge &edge = _edges[id];
      if (edge.capacity == 0 or _level[edge.to] != _level[u] + 1) {
        continue;
      }
      ll pushed = Send(edge.to, sink, min(available, edge.capacity));
      if (pushed > 0) {
        edge.capacity -= pushed;
        _edges[id ^ 1].capacity += pushed; /* Paired reverse edge. */
        return pushed;
      }
    }
    return 0;
  }

public:
  Dinic(int n) : _graph(n + 1), _level(n + 1), _ptr(n + 1) {}

  void Add_Edge(int u, int v, ll capacity) {
    int id = _edges.size();
    _edges.push_back({v, capacity});
    _edges.push_back({u, 0});
    _graph[u].push_back(id);
    _graph[v].push_back(id ^ 1);
    /*
    Consecutive IDs work even for parallel edges and self-loops.
    An undirected capacity edge needs a second Add_Edge(v, u, capacity).
    */
  }

  ll Max_Flow(int source, int sink) {
    if (source == sink) {
      throw invalid_argument("Source and sink must differ.");
    }
    ll answer = 0;
    const ll INF = (1LL << 62); // INF = infinity (a large marker value).
    while (Build_Levels(source, sink)) {
      fill(_ptr.begin(), _ptr.end(), 0);
      while (ll pushed = Send(source, sink, INF)) {
        answer += pushed;
      }
    }
    return answer; /* Additional flow; call once on a freshly built network. */
  }

  vector<bool> Source_Side(int source) const {
    vector<bool> visited(_graph.size(), false);
    queue<int> q;
    visited[source] = true;
    q.push(source);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int index2 = 0; index2 < (int)_graph[u].size(); ++index2) {
        int id = _graph[u][index2];
        const Edge &edge = _edges[id];
        if (edge.capacity > 0 and !visited[edge.to]) {
          visited[edge.to] = true;
          q.push(edge.to);
        }
      }
    }
    return visited;
  }
};

void Solve() {
  int n, m, source, sink;
  cin >> n >> m >> source >> sink;
  Dinic flow(n);
  for (int edgeIndex = 1; edgeIndex <= m; ++edgeIndex) {
    int u, v;
    ll capacity;
    cin >> u >> v >> capacity;
    flow.Add_Edge(u, v, capacity);
  }
  if (source == sink) {
    cout << "INVALID" << endl;
    return;
  }
  cout << flow.Max_Flow(source, sink) << endl;
  vector<bool> side = flow.Source_Side(source);
  cout << count(side.begin(), side.end(), true) << endl;
  bool first = true;
  for (int u = 1; u <= n; ++u) {
    if (side[u]) {
      if (!first) {
        cout << ' ';
      }
      cout << u;
      first = false;
    }
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
