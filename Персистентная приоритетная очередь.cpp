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
const ll Mod = 239017;
const int Maxn = 1e5;
const int lg = 20;
const ll Mod2 = 1e9 + 1;

//std::mt19937_64 rnd(std::chrono::steady_clock().time_since_epoch().count());

ll sum(ll a, ll b) {
    return (a + b) % Mod;
}

ll sub(ll a, ll b) {
    return ((a - b) + Mod) % Mod;
}

ll mul(ll a, ll b) {
    return a * b % Mod;
}

std::mt19937 rnd(42);

struct node {
    node* l, * r;
    ll val;

    node(ll _val) {
        val = _val;
        l = r = 0;
    }
};

node* merge(node* l, node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->val > r->val) std::swap(l, r);
    node* now = new node(l->val);
    now->l = l->l, now->r = l->r;
    now->r = merge(now->r, r);
    if (rnd() % 2) {
        std::swap(now->l, now->r);
    }
    return now;
}

node* insert(node* t, ll x) {
    return merge(t, new node(x));
}

node* extractMin(node* t) {
    return merge(t->l, t->r);
}

void solve() {
    ll n;
    std::cin >> n;
    v<node*> ver(1);
    ver[0] = 0;
    ll s = 0;
    for (int i = 0; i < n; ++i) {
        ll t;
        std::cin >> t;
        if (t == 1) {
            ll a1, a2;
            std::cin >> a1 >> a2;
            ll a, x;
            a = (a1 + s) % (i + 1);
            x = (a2 + 17 * s) % Mod2;
            ver.push_back(insert(ver[a], x));
            if (ver.back() != 0) {
                ll ans = ver.back()->val;
                s = sum(s, ans);
                std::cout << ans << '\n';
            } else {
                std::cout << "empty" << '\n';
            }
        } else if (t == 2) {
            ll a1, a2;
            std::cin >> a1 >> a2;
            ll a, b;    
            a = (a1 + s) % (i + 1);
            b = (a2 + 13 * s) % (i + 1);
            ver.push_back(merge(ver[a], ver[b]));
            if (ver.back() != 0) {
                ll ans = ver.back()->val;
                s = sum(s, ans);
                std::cout << ans << '\n';
            } else {
                std::cout << "empty" << '\n';
            }
        } else {
            ll a1;
            std::cin >> a1;
            ll a;
            a = (a1 + s) % (i + 1);
            if (ver[a] == 0) {
                std::cout << "empty" << '\n';
                ver.push_back(0);
            } else {
                ll ans = ver[a]->val;
                std::cout << ans << '\n';
                s = sum(s, ans);
                ver.push_back(extractMin(ver[a]));
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    //std::freopen("binomial.in", "r", stdin);
    //std::freopen("binomial.out", "w", stdout);
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
