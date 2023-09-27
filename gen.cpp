#include <bits/stdc++.h>
using namespace std;

struct DSU {
	vector<int> s;
	DSU(int n): s(n, -1) {}
	int find(int i) { return s[i] < 0 ? i : s[i] = find(s[i]); }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (s[a] > s[b]) swap(a, b);
		s[a] += s[b], s[b] = a;
		return true;
	}
	int size(int i) { return -s[find(i)]; }
	bool same(int a, int b) { return find(a) == find(b); }
};

template <class T>
pair<vector<bool>, vector<T>> dijkstra(
		const vector<vector<pair<int, T>>>& adj, int s) {
	using ST = pair<T, int>; /// state type
	priority_queue<ST, vector<ST>, greater<ST>> q;
	vector<T> dist(adj.size());
	vector<bool> seen(adj.size());
	q.emplace(T{}, s);
	seen[s] = 1;
	while (!q.empty()) {
		auto [d, u] = q.top();
		q.pop();
		if (dist[u] < d) continue;
		for (auto [v, w] : adj[u])
			if (!seen[v] || d + w < dist[v]) {
				q.emplace(dist[v] = d + w, v);
				seen[v] = 1;
			}
	}
	return {seen, dist};
}

struct Case {
	int n, m, T;
	vector<tuple<int, int, int>> edges;

	int res;

	Case() {}

	Case(int _n, int _T, const vector<tuple<int, int, int>> & _edges, int _res, bool connect = 0) :
		n(_n), m(size(_edges)), T(_T), edges(_edges), res(_res)
	{
		if (connect) {
			// all chanting: "BRING 'EM OUT, BRING 'EM OUT"
			n = 200;

			DSU dsu(n + 2);
			vector<vector<pair<int, long long>>> g(n + 2);
			for (const auto & [u, v, w] : edges) {
				dsu.join(u, v);
				g[u].emplace_back(v, w);
				g[v].emplace_back(u, w);
			}

			auto [sA, dA] = dijkstra(g, 0);
			auto [sB, dB] = dijkstra(g, n + 1);

			for (int i = 0; size(edges) < 10'000 && i <= n + 1; i++) {
				for (int j = i + 1; size(edges) < 10'000 && j <= n + 1; j++) {
					if (!dsu.same(i, j)) {
						long long w = T - min(dA[i], dB[i]) - min(dA[j], dB[j]) + 1;
						if (w >= 1 && w <= 1e9) {
							dsu.join(i, j);
							edges.emplace_back(i, j, w);
						}
					}
				}
			}

			// reset m
			m = size(edges);
		}

		// randomize the order of the edges so adj list construction is non-deterministic/order of visitation in SSSP (single-source-shortest-path) algorithms
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(begin(edges), end(edges), default_random_engine(seed));
	}
};

template<typename Os> Os & operator << (Os & os, const Case & c) {
	os << c.n << ' ' << c.m << ' ' << c.T << '\n';
	for (auto [u, v, w] : c.edges)
		os << u << ' ' << v << ' ' << w << '\n';
	return os;
}

vector<int> pr, comp;
const int N = 202;
bitset<N> isprime = bitset<N>().set();
int mind[N] = {};

int main() {
	isprime[0] = isprime[1] = 0;
	for (int i = 2; i < N; i++) {
		if (isprime[i]) {
			pr.push_back(i);
			mind[i] = i;
			for (int j = i + i; j < N; j += i) {
				if (isprime[j]) {
					isprime[j] = 0;
					mind[j] = i;
				}
			}
		}
	}

	for (int i = 0; i < N; i++)
		if (!isprime[i])
			comp.push_back(i);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	// handmade
	cout << "Enter handmade cases in the format\n\nt\nn m T\nu v w\n\nk or -1 if Love isn't always on time!\n" << endl;

	int t;
	cin >> t;
	vector<Case> cases(t);

	// handmade input
	for (auto & c : cases) {
		cin >> c.n >> c.m >> c.T;
		c.edges = decltype(c.edges)(c.m);
		for (auto & [u, v, w] : c.edges)
			cin >> u >> v >> w;
	}

	// handmade output
	for (auto & c : cases)
		cin >> c.res;

	// min case
	{
		vector<tuple<int, int, int>> edges = {{0, 1, 5e8}, {1, 2, 5e8}};
		cases.emplace_back(1, 1e9, edges, 2);
	}

	// min case
	{
		vector<tuple<int, int, int>> edges = {{0, 1, 5e8}, {1, 201, 5e8}};
		cases.emplace_back(200, 1e9, edges, 2);
		cases.emplace_back(200, 1e9, edges, 2, 1);
	}

	// min case
	{
		vector<tuple<int, int, int>> edges = {{0, 1, 1e9}};
		cases.emplace_back(200, 1e9, edges, -1);
		cases.emplace_back(200, 1e9, edges, -1, 1);
	}

	// max value of K
	{
		vector<tuple<int, int, int>> edges = {{0, 197, 1}, {197, 201, 1}};
		cases.emplace_back(200, 2, edges, 197);
		cases.emplace_back(200, 2, edges, 197, 1);
	}

	// line graph overflow
	{
		vector<int> verts(201);
		iota(begin(verts), end(verts), 0);
		shuffle(begin(verts) + 1, end(verts) - 1, default_random_engine(seed));

		vector<tuple<int, int, int>> edges(200);
		const int W = 1e8;
		for (int i = 0; i < 200; i++)
			edges[i] = tie(verts[i], verts[i + 1], W);
		cases.emplace_back(200, int(1e9), edges, -1);
		cases.emplace_back(200, int(1e9), edges, -1, 1);
	}

	// only middle values of K
	{
		vector<tuple<int, int, int>> edges;
		for (int i = 1; i <= 200; i++) {
			int w = 2 - (isprime[i] && i >= 101 && i <= 150);
			edges.emplace_back(0, i, w);
			edges.emplace_back(i, 201, 1);
		}

		cases.emplace_back(200, 2, edges, 101);
		cases.emplace_back(200, 2, edges, 101, 1);
	}

	// dense graph with large K value
	{
		vector<tuple<int, int, int>> edges;
		for (int i = 0; i < (int) size(pr); i++) {
			int w = 500 - i;
			edges.emplace_back(pr[i], 201, w);
		}

		for (int i = 1; i <= 100; i++) {
			for (int j = max(110, i + 1); j <= 200; j++) {
				edges.emplace_back(i, j, 499);
			}
		}

		for (int i = 0; i <= 200; i++) {
			if (!isprime[i]) {
				int w = i + 1;
				edges.emplace_back(0, i, w);
			}
		}

		cases.emplace_back(200, 970, edges, 131);
		cases.emplace_back(200, 970, edges, 131, 1);
	}

	// graph where # of paths grows exponentially as value of K is incremented
	{
		vector<tuple<int, int, int>> edges;
		vector<int> prev = {0};
		for (int d = 2; d <= 200; d++) {
			int n = size(prev);

			vector<int> curr;
			for (int i = 0; i < n; i++) {
				int u = prev[i];

				for (int v = d; v <= 200; v += d) {
					if (mind[v] == d) {
						edges.emplace_back(u, v, 1);
						curr.push_back(v);
					}
				}
			}

			if (!curr.empty()) {
				sort(begin(curr), end(curr));
				auto it = unique(begin(curr), end(curr));
				curr.resize(distance(begin(curr), it));
				prev = curr;
			}
		}

		edges.emplace_back(pr[size(pr) - 5], 201, 6);
		edges.emplace_back(pr[size(pr) - 4], 201, 5);
		edges.emplace_back(pr[size(pr) - 3], 201, 4);
		edges.emplace_back(pr[size(pr) - 2], 201, 3);
		edges.emplace_back(pr[size(pr) - 1], 201, 1);

		cases.emplace_back(200, 47, edges, 199);
		cases.emplace_back(200, 47, edges, 199, 1);
	}

	// sort by min prime factor (divisor), then create path through that graph with very well-connected components of min divisor
	{
		unordered_map<int, vector<int>> mists;
		for (int i = 1; i <= 200; i++)
			mists[mind[i]].push_back(i);

		vector<vector<int>> mands;
		for (auto & [_, ds] : mists) {
			shuffle(begin(ds), end(ds), default_random_engine(seed));
			mands.emplace_back();
			for (auto d : ds)
				mands.back().push_back(d);
		}

		shuffle(begin(mands), end(mands), default_random_engine(seed));

		vector<tuple<int, int, int>> edges;
		int tail = 0;
		for (auto & comp : mands) {
			edges.emplace_back(tail, comp[0], 1e5);
			if (size(comp) > 1) {
				edges.emplace_back(comp[0], comp[1], 1e5);
				tail = comp[1];
			} else {
				tail = comp[0];
			}
		}

		edges.emplace_back(tail, 201, 1e5);

		cases.emplace_back(200, size(edges) * 1e5, edges, 199);
		cases.emplace_back(200, size(edges) * 1e5, edges, 199, 1);
	}

	// SPFA break case
	{
		vector<int> verts(201);
		iota(begin(verts), end(verts), 1);
		shuffle(begin(verts), end(verts) - 1, default_random_engine(seed));

		vector<tuple<int, int, int>> edges;
		for (int i = 0; i < 200; i++)
			edges.emplace_back(verts[i], verts[i + 1], 1);

		int tot = 1e9;
		for (int i = size(verts) - 2; i >= 0; i--) {
			int w = tot - (size(verts) - (i + 1));
			edges.emplace_back(0, verts[i], w);
			tot--;
		}

		int T = tot + 1;
		cases.emplace_back(200, T, edges, 199);
	}

	// randomize the order of the cases (for caching purposes)
	shuffle(begin(cases), end(cases), default_random_engine(seed));

	// write out the cases
	ofstream in("line.in");
	in << size(cases) << '\n';
	for (const auto & c : cases)
		in << c;
	in.close();

	ofstream out("line.out");
	for (const auto & c : cases) {
		if (c.res == -1)
			out << "Love isn't always on time!\n";
		else
			out << c.res << '\n';
	}
	out.close();

	return 0;
}
