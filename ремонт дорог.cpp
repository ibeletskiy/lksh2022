#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <string>
#include <random>
#include <stack>
#include <queue>
#include <deque>
#include <chrono>
#include <map>
#include <cmath>
#include <bitset>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <numeric>
#include <cstring>

#define debug(a) std::cerr << #a << ": " << a << '\n';
#define all(a) (a).begin(), (a).end()
#define v std::vector

using ll = long long;
using ld = long double;
using str = std::string;
using pll = std::pair<ll, ll>;

const ll Max = 1e17, Min = -1e17;
const ld Pi = std::acos(-1);
const ld eps = 1e-18;
const ll Mod = 1e9 + 7;
const int Maxn = 1e5;
const int color_size = 201;

//std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count(); 

struct dsu {
	v<ll> p, size;

	void build(ll n) {
		p.resize(n);
		std::iota(all(p), 0);
		size.assign(n, 1);
	}

	ll find(ll a) {
		if (a == p[a]) return a;
		return p[a] = find(p[a]);
	}

	bool merge(ll a, ll b) {
		a = find(a);
		b = find(b);
		if (a == b) return false;
		if (size[a] < size[b]) {
			std::swap(a, b);
		}
		p[b] = a;
		size[a] += size[b];
		return true;
	}

	bool check(ll a, ll b) {
		a = find(a);
		b = find(b);
		if (a == b) return false;
		return true;
	}
};

struct edge {
	ll a, b;
	ll c;
};

ll bfs(v<v<ll>>& gr, v<ll>& s, v<ll>& t, v<ll>& p) {
	v<ll> dist(p.size(), Max);
	std::queue<ll> q;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i]) {
			dist[i] = 0;
			q.push(i);
		}
	}
	while (!q.empty()) {
		ll now = q.front();
		if (t[now]) return now;
		q.pop();
		for (auto j : gr[now]) {
			if (dist[j] == Max) {
				q.push(j);
				p[j] = now;
			}
			if (dist[j] > dist[now] + 1) {
				dist[j] = dist[now] + 1;
				p[j] = now;
			}
		}
	}
	return -1;
}

void solve() {
	ll n, m;
	std::cin >> n >> m;
	v<edge> e;
	for (int i = 0; i < m; ++i) {
		ll a, b, c;
		std::cin >> a >> b >> c;
		--a, --b;
		e.push_back({ a, b, c });
	}
	v<ll> colors(color_size, 0);
	dsu tree;
	v<ll> a(m, 0), xa(m, 1);
	for (int i = 0; i < n; ++i) {
		// make real n
		colors.assign(color_size, 0);
		v<ll> s(m), t(m);
		v<ll> left;
		tree.build(n);
		for (int j = 0; j < m; ++j) {
			if (a[j]) {
				left.push_back(j);
				tree.merge(e[j].a, e[j].b);
				++colors[e[j].c];
			}
		}
		// building s, t;
		for (int j = 0; j < m; ++j) {
			if (xa[j]) {
				if (colors[e[j].c] == 0) {
					t[j] = 1;
				}
				if (tree.check(e[j].a, e[j].b)) {
					s[j] = 1;
				}
			}
		}
		// building graph
		v<v<ll>> gr(m);
		// color part
		for (int j = 0; j < m; ++j) {
			if (xa[j]) {
				for (auto k : left) {
					if (colors[e[j].c] == 0 || (colors[e[j].c] == 1 && e[k].c == e[j].c)) {
						gr[j].push_back(k);
					}
				}
			}
		}
		// tree part
		for (auto k : left) {
			tree.build(n);
			for (auto j : left) {
				if (k != j) {
					tree.merge(e[j].a, e[j].b);
				}
			}
			for (int j = 0; j < m; ++j) {
				if (xa[j]) {
					if (tree.check(e[j].a, e[j].b)) {
						gr[k].push_back(j);
					}
				}
			}
		}
		// bfs moment
		v<ll> p(m, -1);
		ll beg = bfs(gr, s, t, p);
		if (beg == -1) break;
		// backtrack vertexes
		++a[beg];
		--xa[beg];
		for (int j = 0; p[beg] != -1; ++j) {
			beg = p[beg];
			if (j % 2 == 0) {
				--a[beg];
				++xa[beg];
			} else {
				++a[beg];
				--xa[beg];
			}
		}
	}
	v<ll> left;
	for (int j = 0; j < m; ++j) {
		if (a[j]) {
			left.push_back(j);
		}
	}
	std::cout << left.size() << '\n';
	for (auto i : left) {
		std::cout << i + 1 << ' ';
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	std::freopen("highways.in", "r", stdin);
	std::freopen("highways.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
