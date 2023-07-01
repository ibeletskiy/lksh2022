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

void dfs(v<v<ll>>& gr, ll i, v<v<ll>>& gr_r, v<v<ll>>& tr) {
	used[i] = 1;
	for (auto j : gr[i]) {
		if (!used[j]) {
			dfs(gr, j, gr_r, tr);
		}
		gr_r[i].push_back(j);
		tr[j].push_back(i);
	}
}

void makeSort(v<v<ll>>& gr, ll i, v<ll>& sort) {
	used[i] = 1;
	for (auto j : gr[i]) {
		if (!used[j]) {
			makeSort(gr, j, sort);
		}
	}
	sort.push_back(i);
}

ll timer = 0;

void dfs(v<v<ll>>& gr, ll i, v<ll>& tin, v<ll>& tout) {
	used[i] = 1;
	tin[i] = timer++;
	for (auto j : gr[i]) {
		if (!used[j]) {
			dfs(gr, j, tin, tout);
		}
	}
	tout[i] = timer++;
}

void solve(ll n1, ll m1) {
	ll n, m;
	n = n1, m = m1;
	v<v<ll>> gr(n);
	v<v<pll>> gr_ans(n);
	for (int i = 0; i < m; ++i) {
		ll a, b;
		std::cin >> a >> b;
		--a, --b;
		gr[a].push_back(b);
		gr_ans[a].push_back({ b, i });
	}
	v<ll> dom = findDom(gr, 0);
	v<v<ll>> dgr(n);
	for (int i = 0; i < n; ++i) {
		if (dom[i] != -1) {
			dgr[dom[i]].push_back(i);
		}
	}
	v<ll> tin(n, -1), tout(n, -1);
	used.assign(n, 0);
	v<ll> ans;
	dfs(dgr, 0, tin, tout);
	for (int i = 0; i < n; ++i) {
		if (dom[i] == -1) {
			continue;
		}
		for (auto j : gr_ans[i]) {
			if (!(tin[j.first] < tin[i] && tout[j.first] > tout[i])) {
				ans.push_back(j.second + 1);
			}
		}
	}
	std::sort(all(ans));
	std::cout << ans.size() << '\n';
	for (auto i : ans) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
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
	ll n, m;
	while (std::cin >> n >> m) {
		solve(n, m);
	}
}
