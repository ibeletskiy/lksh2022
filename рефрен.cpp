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

struct node {
    ll max;
    node* sl;
    std::map<char, node*> to;
    ll count;
    ll tout;
    std::pair<node*, char> prev;

    node() {
        max = 0;
        sl = 0;
        count = 0;
        tout = -1;
        prev = { 0, '0' };
    }
};

ll timer = 0;

v<node*> list;

node* last;

void add(char c, node* root) {
    node* cur = new node();
    list.push_back(cur);
    cur->count = 1;
    cur->max = last->max + 1;
    node* p = last;
    while (p != 0 && p->to.count(c) == 0) {
        p->to[c] = cur;
        p = p->sl;
    }
    if (p == 0) {
        cur->sl = root;
        last = cur;
        return;
    }
    node* q = p->to[c];
    if (p->max + 1 == q->max) {
        cur->sl = q;
    } else {
        node* nw = new node();
        list.push_back(nw);
        nw->max = p->max + 1;
        nw->sl = q->sl;
        nw->to = q->to;
        q->sl = nw;
        cur->sl = nw;
        while (p != 0 && p->to[c] == q) {
            p->to[c] = nw;
            p = p->sl;
        }
    }
    last = cur;
}

void top(node* t) {
    for (auto j : t->to) {
        if (j.second->tout == -1) {
            top(j.second);
        }
    }
    t->tout = timer++;
}

void solve() {
    ll n, m;
    std::cin >> n >> m;
    str s = "";
    for (int i = 0; i < n; ++i) {
        ll now;
        std::cin >> now;
        --now;
        s += '0' + now;
    }
    node* root = new node();
    list.push_back(root);
    last = root;
    for (int i = 0; i < n; ++i) {
        add(s[i], root);
    }
    std::sort(all(list), [](node* a, node* b) {
        return a->max > b->max;
        });
    for (auto i : list) {
        if (i->sl != 0) {
            i->sl->count += i->count;
        }
    }
    ll ans = 0;
    ll len = 0;
    node* beg = 0;
    for (auto i : list) {
        if (i->max * i->count > ans) {
            ans = i->max * i->count;
            len = i->max;
            beg = i;
        }
    }
    std::sort(all(list), [](node* a, node* b) {
        return a->tout > b->tout;
        });
    v<ll> res;
    for (auto i : list) {
        for (auto [c, j] : i->to) {
            if (i->max + 1 == j->max) {
                j->prev = { i, c };
            }
        }
    }
    while (beg->prev.first != 0) {
        res.push_back((beg->prev.second - '0') + 1);
        beg = beg->prev.first;
    }
    std::reverse(all(res));
    std::cout << ans << '\n' << len << '\n';
    for (auto i : res) {
        std::cout << i << ' ';
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    std::freopen("refrain.in", "r", stdin);
    std::freopen("refrain.out", "w", stdout);
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
