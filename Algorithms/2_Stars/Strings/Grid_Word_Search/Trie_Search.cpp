// Given n * m board of characters, and w words, output the number of words that can be constructed using the characters in the board.
// Each word must be constructed from letters of sequentially adjacent cells, where adjacent cells are horizontally or vertically neighboring.
// The same letter cell may not be used more than once in a word.

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

struct node
{
	node* arr[26];
	bool isTerminated;
	string word;

	node()
	{
		for (int i = 0; i < 26; ++i)
			arr[i] = nullptr;

		isTerminated = false;
		word = "";
	}
};

struct Trie
{
	node* head;

	Trie()
	{
		head = new node;
	}

	void addWord(string &word)
	{
		int n = word.size();
		node* temp = head;

		for (int i = 0; i < n; ++i)
		{
			if (temp->arr[word[i] - 'a'] == nullptr)
				temp->arr[word[i] - 'a'] = new node;

			temp = temp->arr[word[i] - 'a'];
			if (i == n - 1)
			{
				temp->word = word;
				temp->isTerminated = true;
			}
		}
	}
};

vector<vector<char>> g;
vector<vector<bool>> v;
set<string> answer;
void dfs(int x, int y, node *nd)
{
	if (nd->arr[g[x][y] - 'a'] == nullptr)
		return;

	v[x][y] = true;
	nd = nd->arr[g[x][y] - 'a'];
	if (nd->isTerminated == true and answer.find(nd->word) == answer.end())
		answer.insert(nd->word);

	int n = g.size(), m = g[0].size();
	for (int k = 0; k < 4; ++k)
	{
		int xx = x + dx[k];
		int yy = y + dy[k];

		if (xx >= 0 and yy >= 0 and xx < n and yy < m and !v[xx][yy])
			dfs(xx, yy, nd);
	}

	v[x][y] = false;
}

void Solve()
{
	int n, m;
	cin >> n >> m;

	g.assign(n, vector<char>(m, ' '));
	v.assign(n, vector<bool>(m, false));
	answer.clear();
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			cin >> g[i][j];

	int w;
	cin >> w;
	Trie tr;
	for (int i = 0; i < w; ++i)
	{
		string word;
		cin >> word;
		tr.addWord(word);
	}

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			dfs(i, j, tr.head);

	for (auto ans : answer)
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
