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

std::mt19937 rnd(36);

struct edge {
    ll b, c;

    bool operator> (edge other) {
        return c > other.c;
    }
};

struct node {
    node* l, * r;
    edge val;
    ll add;

    node(edge _val) {
        val = _val;
        l = r = 0;
        add = 0;
    }
};

edge val(node* t) {
    if (t == 0) return { -1, -1 };
    return t->val;
}

void push(node* t) {
    if (!t) return;
    if (t->add != 0) {
        if (t->l != 0) {
            t->l->add += t->add;
            t->l->val.c += t->add;
        }
        if (t->r) {
            t->r->add += t->add;
            t->r->val.c += t->add;
        }
        t->add = 0;
    }
}

node* mergeHeap(node* l, node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->val > r->val) std::swap(l, r);
    push(l);
    l->r = mergeHeap(l->r, r);
    if (rnd() % 2) {
        std::swap(l->l, l->r);
    }
    return l;
}

node* insert(node* t, edge val) {
    auto now = new node(val);
    return mergeHeap(t, now);
}

void pop(node*& t) {
    if (t == 0) return;
    push(t);
    auto now = t;
    t = mergeHeap(t->l, t->r);
    delete now;
}

v<v<pll>> gr, tr;

struct dsu {
    v<ll> p, size;
    v<node*> heap;

    void build(ll n) {
        p.clear();
        p.resize(n);
        size.clear();
        size.resize(n, 1);
        std::iota(all(p), 0);
        heap.clear();
        heap.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            for (auto j : tr[i]) {
                heap[i] = insert(heap[i], { j.first, j.second });
            }
        }
    }

    ll find(ll a) {
        if (a == p[a]) return a;
        return p[a] = find(p[a]);
    }

    node* act(ll a) {
        a = find(a);
        return heap[a];
    }

    ll merge(ll a, ll b) {
        a = find(a);
        b = find(b);
        if (a == b) return -1;
        if (size[a] < size[b]) std::swap(a, b);
        size[a] += size[b];
        p[b] = a;
        heap[a] = mergeHeap(heap[a], heap[b]);
        heap[b] = 0;
        return a;
    }
};

dsu ver;

ll ans = 0;

edge relax(ll i) {
    node* h = ver.act(i);
    edge now;
    while (true) {
        now = val(h);
        if (now.b != -1 && ver.find(now.b) == ver.find(i)) {
            pop(h);
        } else {
            break;
        }
    }
    ver.heap[ver.find(i)] = h;
    if (now.b == -1) return {-1, -1};
    return { ver.find(now.b), now.c };
}

v<ll> used;

void chinese(ll i) {
    v<ll> path = { i };
    while (true) {
        used[path.back()] = 1;
        if (path.back() == ver.find(0)) {
            while (path.size() != 1) {
                ll now = path.back();
                used[now] = 0;
                path.pop_back();
                path.back() = ver.merge(now, path.back());
            }
            used[path.back()] = 0;
            return;
        }
        edge next = relax(path.back());
        if (used[next.b]) {
            node* h = ver.act(path.back());
            h->add -= next.c;
            h->val.c -= next.c;
            ans += next.c;
            while (!path.empty()) {
                ll now = path.back();
                used[now] = 0;
                if (ver.find(now) == ver.find(next.b)) break;
                path.pop_back();
                path.back() = ver.merge(now, path.back());
            }
            used[path.back()] = 1;
            continue;
        }
        node* h = ver.act(path.back());
        h->add -= next.c;
        h->val.c -= next.c;
        ans += next.c;
        path.push_back(next.b);
    }
    used[path.back()] = 0;
}

void dfs(v<v<pll>>& gr, ll i) {
    used[i] = 1;
    for (auto j : gr[i]) {
        if (!used[j.first]) {
            dfs(gr, j.first);
        }
    }
}

void solve(ll n1, ll m1) {
    ans = 0;
    ll n, m;
    n = n1;
    m = m1;
    gr.clear();
    tr.clear();
    gr.resize(n), tr.resize(n);
    for (int i = 0; i < m; ++i) {
        ll a, b, c;
        std::cin >> a >> b >> c;
        --a, --b;
        gr[a].push_back({ b, c });
        tr[b].push_back({ a, c });
    }
    ver.build(n);
    used.assign(n, 0);
    dfs(gr, 0);
    bool check = true;
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            check = false;
        }
    }
    if (!check) {
        std::cout << "NO" << '\n';
        return;
    }
    used.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        if (ver.find(i) != ver.find(0)) {
            chinese(ver.find(i));
        }
    }
    std::cout << ans << '\n';
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
    ll n, m;
    while (std::cin >> n >> m) {
        solve(n, m);
    }
}
