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
#include <complex>
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

void dfs(v<v<pll>>& gr, ll i) {
	used[i] = 1;
	for (auto j : gr[i]) {
		if (!used[j.first]) {
			dfs(gr, j.first);
		}
	}
}

void solve() {
	ll n, m;
	std::cin >> n >> m;
	v<v<pll>> gr(n);
	for (int i = 0; i < m; ++i) {
		ll a, b, c;
		std::cin >> a >> b >> c;
		--a, --b;
		if (a > b) std::swap(a, b);
		if (a == b) continue;
		gr[a].push_back({ b, c });
		gr[b].push_back({ a, c });
	}
	ll ans = Max;
	used.assign(n, 0);
	dfs(gr, 0);
	for (int i = 0; i < n; ++i) {
		if (used[i] == 0) ans = 0;
	}
	used.assign(n, 0);
	while (n >= 2) {
		v<ll> sum(gr.size(), 0);
		v<ll> a(gr.size(), 0);
		std::priority_queue<pll> q;
		for (int i = 0; i < gr.size(); ++i) {
			if (!used[i]) {
				q.push({ sum[i], i });
			}
		}
		ll k = 0;
		ll s = -1;
		while (k < n - 1) {
			while (!q.empty() && sum[q.top().second] != q.top().first) {
				q.pop();
			}
			ll now = q.top().second;
			q.pop();
			for (auto j : gr[now]) {
				if (a[j.first] != 1) {
					sum[j.first] += j.second;
					q.push({ sum[j.first], j.first });
				}
			}
			++k;
			a[now] = 1;
			s = now;
		}
		ll t = -1;
		for (int i = 0; i < gr.size(); ++i) {
			if (a[i] == 0 && used[i] != 1) {
				t = i;
				break;
			}
		}
		ans = std::min(ans, sum[t]);
		used[t] = 1;
		gr[s].clear();
		for (int i = 0; i < gr.size(); ++i) {
			if (!used[i] && i != s && i != t) {
				for (auto& j : gr[i]) {
					if (j.first == t) j.first = s;
					if (j.first == s) {
						gr[s].push_back({i, j.second});
					}
				}
			}
		}
		--n;
	}
	std::cout << ans;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	std::freopen("globalcut.in", "r", stdin);
	std::freopen("globalcut.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
