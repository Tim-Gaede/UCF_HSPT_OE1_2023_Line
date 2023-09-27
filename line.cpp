#include <bits/stdc++.h>

int main() {
	using namespace std;
	int t;
	cin >> t;
	while (t--) {
		int n, m, T;
		cin >> n >> m >> T;
		n += 2;
		vector<vector<pair<int, int>>> g(n);
		for (int i = 0; i < m; i++) {
			int u, v, w;
			cin >> u >> v >> w;
			g[u].emplace_back(v, w);
			g[v].emplace_back(u, w);
		}

		int cost = -1;
		vector<bool> added(n);
		added[0] = added[1] = added[n - 1] = 1;
		for (int k = 2; k <= n; k++) {
			for (int i = 2; i < n; i++)
				added[i] = added[i] || i % k == 0;

			priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
			vector<long long> d(n, LLONG_MAX);
			pq.emplace(0, 0);
			d[0] = 0;
			while (!pq.empty()) {
				auto [du, u] = pq.top();
				pq.pop();

				if (du > d[u])
					continue;

				for (auto [v, w] : g[u]) {
					if (added[v] && d[u] + w < d[v]) {
						d[v] = d[u] + w;
						pq.emplace(d[v], v);
					}
				}
			}

			if (d[n - 1] <= T) {
				cost = k;
				break;
			}
		}

		if (cost != -1)
			cout << cost << '\n';
		else
			cout << "Love isn't always on time!\n";
	}
	return 0;
}
