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
const ll Mod = 1e9 + 7;
const int Maxn = 1e5;
const int lg = 20;

//std::mt19937_64 rnd(std::chrono::steady_clock().time_since_epoch().count());

std::mt19937 rnd(42);

struct dsu {
    v<ll> p, size;
    v<ll> val;

    void add(ll a) {
        p.push_back(p.size());
        size.push_back(1);
        val.push_back(a);
    }

    ll getVal(ll a) {
        a = find(a);
        return val[a];
    }

    ll find(ll a) {
        if (a == p[a]) return a;
        return p[a] = find(p[a]);
    }

    bool merge(ll a, ll b, ll _val) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        p[b] = a;
        size[a] += size[b];
        val[a] = _val;
        return true;
    }
};

struct node {
    node* l, * r;
    pll val; // val id
    node* p;

    node(pll _val) {
        val = _val;
        l = r = p = 0;
    }
};

v<node*> heaps, ver;

dsu num;

node* merge(node* l, node* r) {
    if (!l) {
        if (r != 0) r->p = 0;
        return r;
    }
    if (!r) {
        l->p = 0;
        return l;
    }
    if (l->val > r->val) std::swap(l, r);
    l->p = 0;
    l->r = merge(l->r, r);
    if (l->r) l->r->p = l;
    if (rnd() % 2) {
        std::swap(l->l, l->r);
    }
    return l;
}

node* extractMin(node* t) {
    return merge(t->l, t->r);
}

void del(node* t) {
    ll ind = num.getVal(t->val.second);
    if (t->p == 0) {
        heaps[ind] = extractMin(t);
        return;
    }
    node* p = t->p;
    if (p->l == t) {
        p->l = 0;
    } else {
        p->r = 0;
    }
    t->p = 0;
    node* now = extractMin(t);
    heaps[ind] = merge(now, heaps[ind]);
}

void change(node* t, ll val) {
    ll ind = num.getVal(t->val.second);
    if (t->p == 0) {
        heaps[ind] = extractMin(t);
        t->val.first = val;
        t->p = 0, t->l = 0, t->r = 0;
        heaps[ind] = merge(heaps[ind], t);
        return;
    }
    node* p = t->p;
    if (p->l == t) {
        p->l = 0;
    } else {
        p->r = 0;
    }
    t->p = 0;
    node* now = extractMin(t);
    heaps[ind] = merge(now, heaps[ind]);
    t->val.first = val;
    t->p = 0, t->l = 0, t->r = 0;
    heaps[ind] = merge(heaps[ind], t);
}

void Merge(ll a, ll b) {
    if (heaps[a] == 0 || a == b) return;
    if (heaps[b] == 0) {
        ll tmp = num.find(heaps[a]->val.second);
        num.val[tmp] = b;
        heaps[b] = heaps[a];
        heaps[a] = 0;
        return;
    }
    num.merge(heaps[a]->val.second, heaps[b]->val.second, b);
    heaps[b] = merge(heaps[a], heaps[b]);
    heaps[a] = 0;
}

void solve() {
    // what the heck
    ll n, m;
    std::cin >> n >> m;
    heaps.resize(n, 0);
    ll cnt = 0;
    for (int i = 0; i < m; ++i) {
        ll t;
        std::cin >> t;
        if (t == 0) {
            ll val, a;
            std::cin >> a >> val;
            --a;
            ll id = cnt++;
            ver.push_back(new node({ val, id }));
            num.add(a);
            if (heaps[a] != 0) {
                num.merge(id, heaps[a]->val.second, a);
            }
            heaps[a] = merge(ver.back(), heaps[a]);
        } else if (t == 1) {
            ll a, b;
            std::cin >> a >> b;
            --a, --b;
            Merge(a, b);
        } else if (t == 2) {
            ll ind;
            std::cin >> ind;
            --ind;
            del(ver[ind]);
        } else if (t == 3) {
            ll ind, val;
            std::cin >> ind >> val;
            --ind;
            change(ver[ind], val);
        } else if (t == 4) {
            ll a;
            std::cin >> a;
            --a;
            std::cout << heaps[a]->val.first << '\n';
        } else if (t == 5) {
            ll a;
            std::cin >> a;
            --a;
            heaps[a] = extractMin(heaps[a]);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    std::freopen("binomial.in", "r", stdin);
    std::freopen("binomial.out", "w", stdout);
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
