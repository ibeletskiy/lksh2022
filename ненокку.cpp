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
    ll dp;

    node() {
        max = 0;
        sl = 0;
        dp = 0;
    }
};

node* last;

ll ans = 0;

void add(char c, node* root) {
    node* cur = new node();
    cur->max = last->max + 1;
    node* p = last;
    while (p != 0 && p->to.count(c) == 0) {
        p->to[c] = cur;
        cur->dp += p->dp;
        ans += p->dp;
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
            q->dp -= p->dp;
            p->to[c] = nw;
            nw->dp += p->dp;
            p = p->sl;
        }
    }
    last = cur;
}

bool check(str s, node* root) {
    for (int i = 0; i < s.size(); root = root->to[s[i]], ++i) {
        if (root->to.count(s[i]) == 0) {
            return false;
        }
    }
    return true;
}

void solve() {
    char t;
    str text;
    node* root = new node();
    last = root;
    while (std::cin >> t >> text) {
        for (int i = 0; i < text.size(); ++i) {
            text[i] = std::tolower(text[i]);
        }
        if (t == '?') {
            std::cout << (check(text, root) ? "YES" : "NO") << '\n';
        } else {
            for (int i = 0; i < text.size(); ++i) {
                add(text[i], root);
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    /*std::freopen("chinese.in", "r", stdin);
    std::freopen("chinese.out", "w", stdout);*/
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
