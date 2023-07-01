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
	node* ch[2];
	node* p;
	ll val;
	ll c;
	bool rev;

	node(ll x) {
		ch[0] = ch[1] = p = 0;
		val = x;
		c = 1;
		rev = false;
	}
};

void push(node* t) {
	if (t == 0) return;
	if (t->ch[0] != 0) {
		t->ch[0]->rev ^= t->rev;
	}
	if (t->ch[1] != 0) {
		t->ch[1]->rev ^= t->rev;
	}
	if (t->rev) {
		std::swap(t->ch[0], t->ch[1]);
	}
	t->rev = 0;
}

ll getC(node* t) {
	if (t == 0) return 0;
	return t->c;
}

void recalc(node* t) {
	if (t == 0) return;
	t->c = getC(t->ch[0]) + getC(t->ch[1]) + 1;
}

bool isRoot(node* t) {
	return t->p == 0 || (t->p->ch[0] != t && t->p->ch[1] != t);
}

ll chnum(node* t) {
	return t == t->p->ch[1];
}

void attach(node* t, node* p, ll num) {
	if (t != 0) {
		t->p = p;
	}
	if (p != 0) {
		p->ch[num] = t;
	}
	recalc(t);
	recalc(p);
}

void rotate(node* t) {
	ll num = chnum(t);
	node* p = t->p;
	node* b = t->ch[1 - num];
	node* g = p->p;
	if (g != 0) {
		t->p = g;
		if (!isRoot(p)) {
			g->ch[chnum(p)] = t;
			recalc(g);
		}
	} else {
		t->p = 0;
	}
	attach(p, t, 1 - num);
	attach(b, p, num);
	recalc(t);
}

void splay(node* t) {
	if (t == 0) return;
	node* tmp = t;
	v<node*> to_push;
	while (!isRoot(tmp)) {
		tmp = tmp->p;
		to_push.push_back(tmp);
	}
	while (!to_push.empty()) {
		push(to_push.back());
		to_push.pop_back();
	}
	push(t);
	while (!isRoot(t)) {
		if (!isRoot(t->p)) {
			if (chnum(t) == chnum(t->p)) {
				rotate(t->p);
			} else {
				rotate(t);
			}
		}
		rotate(t);
	}
}

node* find(node*& t, ll x) {
	if (t == 0) return 0;
	node* now = t;
	while (now != 0) {
		push(now);
		if (getC(now->ch[0]) + 1 == x) {
			break;
		}
		if (getC(now->ch[0]) < x) {
			if (now->ch[1] == 0) break;
			x -= getC(now->ch[0]) + 1;
			now = now->ch[1];
		} else {
			now = now->ch[0];
		}
	}
	splay(now);
	if (now != 0) {
		t = now;
	}
	return now;
}

std::pair<node*, node*> split(node* t, ll x) {
	if (t == 0) return { 0, 0 };
	node* l = 0;
	node* r = 0;
	if (x == 0) {
		r = t;
	} else {
		find(t, x);
		l = t;
		if (l != 0) {
			r = l->ch[1];
			l->ch[1] = 0;
		}
		if (r != 0) {
			r->p = 0;
		}
	}
	push(l);
	push(r);
	recalc(l);
	recalc(r);
	return { l, r };
}

void expose(node* t) {
	if (t == 0) return;
	splay(t);
	t->ch[1] = 0;
	recalc(t);
	while (t->p != 0) {
		splay(t->p);
		attach(t, t->p, 1);
		recalc(t->p);
		splay(t);
	}
}

void makeRoot(node* t) {
	if (t == 0) return;
	expose(t);
	t->rev ^= 1;
}

void link(node* a, node* b) {
	if (a == 0 || b == 0) return;
	makeRoot(a);
	makeRoot(b);
	b->p = a;
}

void cut(node* a, node* b) {
	if (a == 0 || b == 0) return;
	makeRoot(b);
	makeRoot(a);
	push(a);
	a->ch[1] = 0;
	b->p = 0;
}

ll query(node* a, node* b) {
	makeRoot(a);
	makeRoot(b);
	if (isRoot(a) && a != b) return 0;
	return getC(b);
}

void solve() {
	ll n, m;
	std::cin >> n >> m;
	v<node*> ver(n);
	for (int i = 0; i < n; ++i) {
		ver[i] = new node(i);
	}
	for (int i = 0; i < m; ++i) {
		str t;
		ll a, b;
		std::cin >> t >> a >> b;
		--a, --b;
		if (t == "get") {
			std::cout << query(ver[a], ver[b]) - 1 << '\n';
		} else if (t == "link") {
			link(ver[a], ver[b]);
		} else if (t == "cut") {
			cut(ver[a], ver[b]);
		}
		
	}
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    std::freopen("linkcut.in", "r", stdin);
    std::freopen("linkcut.out", "w", stdout);
    ll t = 1;
    //std::cin >> t;
    while (t--) {
        solve();
    }
}
