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
	ll n;
	std::cin >> n;
	v<pll> list(n);
	ll sum = 0;
	for (int i = 0; i < n; ++i) {
		std::cin >> list[i].second >> list[i].first;
		sum += list[i].first;
	}
	std::sort(all(list));
	std::reverse(all(list));
	std::set<ll> s;
	for (int i = 0; i < n; ++i) {
		s.insert(i);
	}
	ll ans = 0;
	for (int i = 0; i < n; ++i) {
		if (list[i].second > n) {
			ans += list[i].first;
		} else {
			auto it = s.lower_bound(list[i].second);
			if (it == s.begin()) {
				continue;
			} else {
				ans += list[i].first;
				--it;
				s.erase(it);
			}
		}
	}
	std::cout << sum - ans;
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
