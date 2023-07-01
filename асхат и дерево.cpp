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

struct ver {
    ll val, l = -1, r = -1;
    ll p = -1;
};

ver val(ll num) {
    std::cout << "val " << num << std::endl;
    ver ans;
    std::cin >> ans.val >> ans.l >> ans.r;
    --ans.l, --ans.r;
    return ans;
}

void confirm(ll num) {
    std::cout << "confirm " << num << std::endl;
}

void change(ll num, ll l, ll r) {
    std::cout << "change " << num << ' ' << l << ' ' << r << std::endl;
}

std::vector<ver> gr;

ll find(ll num, ll x) {
    ver res = val(num + 1);
    gr[num].l = res.l, gr[num].r = res.r, gr[num].val = res.val;
    if (gr[num].l != -1) {
        gr[gr[num].l].p = num;
    }
    if (gr[num].r != -1) {
        gr[gr[num].r].p = num;
    }
    if (gr[num].val == x) return num;
    if (x < gr[num].val) {
        return find(gr[num].l, x);
    } else {
        return find(gr[num].r, x);
    }
}

ll chnum(ll num) {
    if (gr[num].p == -1) return -1;
    if (gr[gr[num].p].l == num) return 0;
    return 1;
}

v<ll> vers;

void attach(ll t, ll p, ll num) {
    if (t != -1) {
        gr[t].p = p;
    }
    if (p != -1) {
        if (num == 0) {
            gr[p].l = t;
        } else {
            gr[p].r = t;
        }
    }
}

void rotate(ll t) {
    ll num = chnum(t);
    ll p = gr[t].p;
    ll b;
    if (num == 0) {
        b = gr[t].r;
    } else {
        b = gr[t].l;
    }
    ll g = gr[p].p;
    if (g != -1) {
        gr[t].p = g;
        if (gr[p].p != -1) {
            if (chnum(p) == 0) {
                gr[g].l = t;
            } else {
                gr[g].r = t;
            }
        }
    } else {
        gr[t].p = -1;
    }
    attach(p, t, 1 - num);
    attach(b, p, num);
}

void splay(ll t) {
    ll tmp = t;
    while (gr[tmp].p != -1) {
        vers.push_back(tmp);
        tmp = gr[tmp].p;
    }
    vers.push_back(tmp);
    while (gr[t].p != -1) {
        if (gr[gr[t].p].p != -1) {
            if (chnum(t) == chnum(gr[t].p)) {
                rotate(gr[t].p);
            } else {
                rotate(t);
            }
        }
        rotate(t);
    }
    for (auto i : vers) {
        change(i + 1, gr[i].l + 1, gr[i].r + 1);
    }
    vers.clear();
}

void solve() {
    ll n, q;
    std::cin >> n >> q;
    for (int i = 0; i < q; ++i) {
        gr.clear();
        gr.resize(n);
        ll r, x;
        std::cin >> r >> x;
        --r;
        ll ans = find(r, x);
        splay(ans);
        confirm(ans + 1);
    }
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
    while (t--) {
        solve();
    }
}
