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
	v<ll> check((1ll << n), 0);
	for (int i = 0; i < m; ++i) {
		ll tmp = 0;
		ll k;
		std::cin >> k;
		for (int j = 0; j < k; ++j) {
			ll now;
			std::cin >> now;
			--now;
			tmp |= (1ll << now);
		}
		check[tmp] = 1;
	}
	bool ans = true;
	if (!check[0]) ans = false;
	for (int i = 0; i < (1ll << n); ++i) {
		if (check[i]) {
			for (int j = i; j > 0; j = (j - 1) & i) {
				if (!check[j]) {
					ans = false;
				}
			}
		}
	}
	for (int i = 0; i < (1ll << n); ++i) {
		for (int j = 0; j < (1ll << n); ++j) {
			if (check[i] && check[j] && __builtin_popcount(i) < __builtin_popcount(j)) {
				bool tmp = false;
				for (int k = 0; k < n; ++k) {
					if (((1ll << k) & j) && !((1ll << k) & i)) {
						if (check[i | (1ll << k)]) {
							tmp = true;
						}
					}
				}
				if (!tmp) {
					ans = false;
				}
			}
		}
	}
	std::cout << (ans ? "YES" : "NO");
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
