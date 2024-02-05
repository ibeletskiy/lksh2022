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
const ll Mod = 1e9 + 7;
const int Maxn = 1e5;
const int lg = 20;

//std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count(); 

ll sum(ll a, ll b) {
	a += b;
	if (a >= Mod) return a - Mod;
	return a;
}

ll sub(ll a, ll b) {
	a -= b;
	if (a < 0) return a + Mod;
	return a;
}

ll mul(ll a, ll b) {
	return a * b % Mod;
}

ll binpow(ll a, ll n) {
	ll ans = 1;
	while (n) {
		if (n & 1) {
			ans = mul(ans, a);
		}
		a = mul(a, a);
		n = n >> 1;
	}
	return ans;
}

ll k;

void fwht(v<ll>& a) {
	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < a.size(); ++j) {
			if ((j & (1ll << i)) == 0) {
				ll f = a[j];
				ll s = a[j + (1ll << i)];
				a[j] = sum(f, s);
				a[j + (1ll << i)] = sub(f, s);
			}
		}
	}
}

void fwht_inv(v<ll>& a) {
	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < a.size(); ++j) {
			if ((j & (1ll << i)) == 0) {
				ll f = a[j];
				ll s = a[j + (1ll << i)];
				a[j] = sum(f, s);
				a[j + (1ll << i)] = sub(f, s);
			}
		}
	}
	ll val = binpow(a.size(), Mod - 2);
	for (int i = 0; i < a.size(); ++i) {
		a[i] = mul(a[i], val);
	}
}

v<ll> xorConv(v<ll>& a, v<ll>& b) {
	fwht(a);
	fwht(b);
	v<ll> c(a.size());
	for (int i = 0; i < a.size(); ++i) {
		c[i] = mul(a[i], b[i]);
	}
	fwht_inv(c);
	return c;
}

void solve() {
	std::cin >> k;
	v<ll> a(1ll << k), b(1ll << k);
	for (int i = 0; i < (1ll << k); ++i) {
		std::cin >> a[i];
	}
	for (int i = 0; i < (1ll << k); ++i) {
		std::cin >> b[i];
	}
	v<ll> ans = xorConv(a, b);
	for (auto i : ans) {
		std::cout << i << ' ';
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
