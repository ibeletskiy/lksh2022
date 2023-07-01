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

    node() {
        max = 0;
        sl = 0;
    }
};

node* last;

void add(char c, node* root) {
    node* cur = new node();
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

void solve() {
    ll n;
    std::cin >> n;
    str a, b;
    std::cin >> a >> b;
    node* root = new node();
    last = root;
    for (int i = 0; i < n; ++i) {
        add(a[i], root);
    }
    ll len = 0;
    ll maxlen = 0, maxpos = 0;
    node* now = root;
    for (int i = 0; i < n; ++i) {
        while (now != root && !now->to.count(b[i])) {
            now = now->sl;
            len = now->max;
        }
        if (now->to.count(b[i])) { // потому что из root мог быть переход, но мы все равно закончили, т.к. root;
            ++len;
            now = now->to[b[i]];
        } else {
            len = 0;
        }
        if (len > maxlen) {
            maxlen = len;
            maxpos = i - len + 1;
        }
    }
    for (int i = 0; i < maxlen; ++i) {
        std::cout << b[maxpos + i];
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    std::freopen("freedom.in", "r", stdin);
    std::freopen("freedom.out", "w", stdout);
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
