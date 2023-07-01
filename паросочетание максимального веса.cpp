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

//std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count(); 

v<ll> par, used;

bool find(v<v<ll>>& gr, ll i) {
	used[i] = 1;
	for (auto j : gr[i]) {
		if (par[j] == -1 || (!used[par[j]] && find(gr, par[j]))) {
			par[i] = j;
			par[j] = i;
			return true;
		}
	}
	return false;
}

void solve() {
	ll n;
	std::cin >> n;
	v<v<ll>> gr(2 * n);
	v<pll> list(n);
	par.resize(2 * n, -1);
	for (int i = 0; i < n; ++i) {
		std::cin >> list[i].first;
		list[i].second = i;
	}
	for (int i = 0; i < n; ++i) {
		ll k;
		std::cin >> k;
		for (int j = 0; j < k; ++j) {
			ll c;
			std::cin >> c;
			gr[i].push_back(n + c - 1);
			gr[n + c - 1].push_back(i);
		}
	}
	std::sort(all(list), [](pll a, pll b) {
		return a.first * a.first > b.first * b.first;
		});
	for (int i = 0; i < n; ++i) {
		used.assign(2 * n, 0);
		if (par[list[i].second] == -1) {
			find(gr, list[i].second);
		}
	}
	for (int i = 0; i < n; ++i) {
		if (par[i] != -1) {
			std::cout << par[i] + 1 - n << ' ';
		} else {
			std::cout << 0 << ' ';
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	/*std::freopen("sweets.in", "r", stdin);
	std::freopen("sweets.out", "w", stdout);*/
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
