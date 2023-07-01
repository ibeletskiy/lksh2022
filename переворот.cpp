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

struct Node {
	Node* ch[2];
	Node* p;
	ll val;
	ll c;
	ll min;
	bool rev;

	Node(ll x) {
		ch[0] = ch[1] = p = 0;
		val = x;
		c = 1;
		min = x;
		rev = false;
	}
};

void push(Node* t) {
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

ll getC(Node* t) {
	if (t == 0) return 0;
	return t->c;
}

ll getMin(Node* t) {
	if (t == 0) return Max;
	return t->min;
}

void recalc(Node* t) {
	if (t == 0) return;
	t->c = getC(t->ch[0]) + getC(t->ch[1]) + 1;
	t->min = std::min({ getMin(t->ch[0]), getMin(t->ch[1]), t->val });
}

bool isRoot(Node* t) {
	return t->p == 0;
}

ll chnum(Node* t) {
	return t == t->p->ch[1];
}

void attach(Node* t, Node* p, ll num) {
	if (t != 0) {
		t->p = p;
	}
	if (p != 0) {
		p->ch[num] = t;
	}
	recalc(t);
	recalc(p);
}

void rotate(Node* t) {
	ll num = chnum(t);
	Node* p = t->p;
	Node* b = t->ch[1 - num];
	Node* g = p->p;
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
}

void splay(Node* t) {
	if (t == 0) return;
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

Node* find(Node*& t, ll x) {
	if (t == 0) return 0;
	Node* now = t;
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

std::pair<Node*, Node*> split(Node* t, ll x) {
	if (t == 0) return { 0, 0 };
	Node* l = 0;
	Node* r = 0;
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

Node* merge(Node* l, Node* r) {
	if (l == 0) return r;
	if (r == 0) return l;
	push(l);
	while (l->ch[1] != 0) {
		l = l->ch[1];
		push(l);
	}
	splay(l);
	attach(r, l, 1);
	return l;
}

Node* insert(Node* t, ll x, ll val) {
	auto [l, r] = split(t, x);
	Node* now = new Node(val);
	return merge(merge(l, now), r);
}

void reverse(Node*& t, ll a, ll b) {
	Node* l, * m, * r;
	std::tie(l, r) = split(t, b - 1);
	std::tie(l, m) = split(l, a - 1);
	if (m != 0) {
		m->rev = true;
	}
	t = merge(merge(l, m), r);
}

ll min(Node*& t, ll a, ll b) {
	Node* l, * m, * r;
	std::tie(l, r) = split(t, b - 1);
	std::tie(l, m) = split(l, a - 1);
	ll ans = getMin(m);
	t = merge(merge(l, m), r);
	return ans;
}

void solve() {
	ll n, m;
	std::cin >> n >> m;
	Node* root = 0;
	for (int i = 0; i < n; ++i) {
		ll cnt;
		std::cin >> cnt;
		root = insert(root, i, cnt);
	}
	for (int i = 0; i < m; ++i) {
		ll t, l, r;
		std::cin >> t >> l >> r;
		++r;
		if (t == 1) {
			reverse(root, l, r);
		} else {
			std::cout << min(root, l, r) << '\n';
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	std::freopen("reverse.in", "r", stdin);
	std::freopen("reverse.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
