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

void solve() {
	ll n, m;
	std::cin >> n >> m;
	std::vector<pll> list(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> list[i].first;
		list[i].second = i;
	}
	v<ll> check;
	for (int i = 0; i < m; ++i) {
		ll k, tmp = 0;
		std::cin >> k;
		for (int j = 0; j < k; ++j) {
			ll now;
			std::cin >> now;
			--now;
			tmp |= (1ll << now);
		}
		check.push_back(tmp);
	}
	std::sort(all(list));
	std::reverse(all(list));
	ll sum = 0;
	ll mask = 0;
	for (int i = 0; i < n; ++i) {
		ll tmp = mask | (1ll << list[i].second);
		bool now = true;
		for (int j = 0; j < m; ++j) {
			if ((check[j] & tmp) == check[j]) {
				now = false;
			}
		}
		if (now) {
			mask |= (1ll << list[i].second);
			sum += list[i].first;
		}
	}
	std::cout << sum;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	/*std::freopen("highways.in", "r", stdin);
	std::freopen("highways.out", "w", stdout);*/
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
