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
const int lg = 20;

//std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count(); 

v<ll> used;
ll tnow = 0;

v<ll> sdom, dom, count_sdom;
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
		min.resize(lg, v<pll>(gr.size(), {Max, -1}));
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

void solve(ll n1, ll m1) {
	ll n, m;
	n = n1, m = m1;
	tnow = 0;
	v<v<ll>> grt(n);
	used.assign(n, 0);
	for (int i = 0; i < m; ++i) {
		ll a, b;
		std::cin >> a >> b;
		--a, --b;
		grt[a].push_back(b);
	}
	dom.assign(n, Min);
	sdom.assign(n, Max);
	count_sdom.assign(n, Max);
	count_dom.assign(n, { Min, -1 });
	v<ll> time(n, -1), rev(n, -1);
	makeTime(grt, 0, time);
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
	count_dom[0] = { 0, 0 };
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
	v<v<ll>> q(n);
	for (int i = 0; i < n; ++i) {
		q[sdom[i]].push_back(i);
		count_dom[i] = { sdom[i], i };
	}
	Bin ups(grtmp);
	v<pll> vals(n, {Max, -1});
	for (int i = n - 1; i >= 0; --i) {
		for (auto j : q[i]) {
			vals[j] = ups.get(i, j);
		}
	}
	for (int i = 0; i < n; ++i) {
		if (vals[i].first >= sdom[i]) {
			dom[i] = sdom[i];
		} else {
			dom[i] = dom[vals[i].second];
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
	for (auto j : ans) {
		if (j == -1) {
			std::cout << j << ' ';
		} else {
			std::cout << j + 1 << ' ';
		}
	}
	std::cout << '\n';
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	/*std::freopen("twost.in", "r", stdin);
	std::freopen("twost.out", "w", stdout);*/
	ll t = 1;
	//std::cin >> t;
	ll n, m;
	while (std::cin >> n >> m) {
		solve(n, m);
	}
}
