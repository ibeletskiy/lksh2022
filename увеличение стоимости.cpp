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
#include <cassert>
#include <cstring>
#include <type_traits>
#include <array>

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
const ll Mod = 998244353;
const int Maxn = 1e5;
const int lg = 20;

//std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count(); 


v<ll> used;
ll tnow = 0;

v<ll> sdom, count_sdom;
v<pll> count_dom;

void makeTime(v<v<ll>>& gr, ll i, v<ll>& time) {
	used[i] = 1;
	time[i] = tnow++;
	for (auto j : gr[i]) {
		if (!used[j]) {
			makeTime(gr, j, time);
		}
	}
}

void makePrev(v<v<ll>>& gr, ll i, v<ll>& p) {
	used[i] = 1;
	for (auto j : gr[i]) {
		if (!used[j]) {
			makePrev(gr, j, p);
			p[j] = i;
		}
	}
}

ll unite(v<ll>& p, ll a, ll u) {
	if (p[a] <= u) return a;
	ll now = unite(p, p[a], u);
	count_sdom[a] = std::min(count_sdom[a], count_sdom[p[a]]);
	p[a] = now;
	return now;
}

struct Bin {
	v<v<ll>> up;
	v<v<pll>> min;
	v<ll> dist;

	void dfs(v<v<ll>>& gr, ll i, ll p, ll d) {
		dist[i] = d;
		for (auto j : gr[i]) {
			if (j != p) {
				up[0][j] = i;
				min[0][j] = { sdom[j], j };
				dfs(gr, j, i, d + 1);
			}
		}
	}

	Bin(v<v<ll>>& gr) {
		up.resize(lg, v<ll>(gr.size(), 0));
		min.resize(lg, v<pll>(gr.size(), { Max, -1 }));
		dist.resize(gr.size(), 0);
		dfs(gr, 0, -1, 0);
		for (int i = 1; i < lg; ++i) {
			for (int j = 0; j < gr.size(); ++j) {
				up[i][j] = up[i - 1][up[i - 1][j]];
				min[i][j] = std::min(min[i - 1][up[i - 1][j]], min[i - 1][j]);
			}
		}
	}

	pll get(ll a, ll b) {
		if (dist[a] < dist[b]) {
			std::swap(a, b);
		}
		ll dif = dist[a] - dist[b];
		pll ans = { Max, -1 };
		for (int i = lg - 1; i >= 0; --i) {
			if ((1ll << i) <= dif) {
				dif -= (1ll << i);
				ans = std::min(ans, min[i][a]);
				a = up[i][a];
			}
		}
		return ans;
	}
};

v<ll> findDom(v<v<ll>>& grt, ll c) {
	ll n = grt.size();
	ll n1 = n;
	tnow = 0;
	used.assign(n, 0);
	v<ll> dom(n, Min);
	dom.assign(n, Min);
	sdom.assign(n, Max);
	count_sdom.assign(n, Max);
	v<ll> time(n, -1), rev(n, -1);
	makeTime(grt, c, time);
	v<v<ll>> gr(tnow), tr(tnow);
	for (int i = 0; i < n; ++i) {
		if (time[i] == -1) {
			dom[i] = -1;
			continue;
		}
		rev[time[i]] = i;
		for (auto j : grt[i]) {
			gr[time[i]].push_back(time[j]);
			tr[time[j]].push_back(time[i]);
		}
	}
	dom[0] = 0;
	sdom[0] = 0;
	count_sdom[0] = 0;
	n = tnow;
	used.assign(n, 0);
	v<ll> p(n, 0), copy(n, 0);
	makePrev(gr, 0, p);
	copy = p;
	// semidominators find
	for (int i = n - 1; i >= 0; --i) {
		for (auto j : tr[i]) {
			if (j < i) {
				sdom[i] = std::min(sdom[i], j);
			} else {
				unite(p, j, i);
				sdom[i] = std::min(sdom[i], count_sdom[j]);
			}
		}
		count_sdom[i] = sdom[i];
	}
	//dominators find
	p = copy;
	v<v<ll>> grtmp(n);
	for (int i = 0; i < n; ++i) {
		grtmp[p[i]].push_back(i);
	}
	Bin ups(grtmp);
	for (int i = 0; i < n; ++i) {
		pll val = ups.get(i, sdom[i]);
		if (val.first >= sdom[i]) {
			dom[i] = sdom[i];
		} else {
			dom[i] = dom[val.second];
		}
	}
	v<ll> ans(n1);
	for (int i = 0; i < n1; ++i) {
		if (time[i] == -1) {
			ans[i] = -1;
		} else {
			ans[i] = rev[dom[time[i]]];
		}
	}
	return ans;
}

v<ll> dij(v<v<pll>>& gr, ll beg) {
	ll n = gr.size();
	v<ll> dist(n, Max);
	dist[beg] = 0;
	std::set<pll> s;
	for (int i = 0; i < n; ++i) {
		s.insert({ dist[i], i });
	}
	while (!s.empty()) {
		ll now = s.begin()->second;
		s.erase(s.begin());
		for (auto j : gr[now]) {
			if (dist[j.first] > dist[now] + j.second) {
				s.erase({ dist[j.first], j.first });
				dist[j.first] = dist[now] + j.second;
				s.insert({ dist[j.first], j.first });
			}
		}
	}
	return dist;
}

void dfs(v<v<ll>>& gr, ll i, v<ll>& dp) {
	dp[i] = 1;
	for (auto j : gr[i]) {
		dfs(gr, j, dp);
		dp[i] += dp[j];
	}
}

struct Edge {
	ll first, second;
	ll num;
};

void solve() {
	ll n, m;
	std::cin >> n >> m;
	v<v<Edge>> grt(n);
	v<v<pll>> grt2(n);
	for (int i = 0; i < m; ++i) {
		ll a, b, c;
		std::cin >> a >> b >> c;
		--a, --b;
		grt[a].push_back({ b, c, i });
		grt[b].push_back({ a, c, i });
		grt2[a].push_back({ b, c });
		grt2[b].push_back({ a, c });
	}
	v<ll> dist = dij(grt2, 0);
	v<v<pll>> gr2(n);
	v<v<ll>> gr(n);
	for (int i = 0; i < n; ++i) {
		for (auto j : grt[i]) {
			if (dist[i] + j.second == dist[j.first]) {
				gr2[i].push_back({ j.first, j.num });
				gr[i].push_back(j.first);
			}
		}
	}
	v<ll> dom = findDom(gr, 0);
	v<v<ll>> domgr(n);
	for (int i = 1; i < n; ++i) {
		if (dom[i] != -1) {
			domgr[dom[i]].push_back(i);
		}
	}
	v<ll> dp(n);
	dfs(domgr, 0, dp);
	v<ll> ans(m, 0);
	for (int i = 0; i < n; ++i) {
		for (auto j : gr2[i]) {
			if (dom[j.first] == i) {
				ans[j.second] = dp[j.first];
			}
		}
	}
	for (auto i : ans) {
		std::cout << i << '\n';
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	//std::freopen("reverse.in", "r", stdin);
	//std::freopen("reverse.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
