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
	ll sum;

	Node(ll x) {
		ch[0] = ch[1] = p = 0;
		val = x;
		c = 1;
		sum = x;
	}
};

ll getC(Node* t) {
	if (t == 0) return 0;
	return t->c;
}

ll getSum(Node* t) {
	if (t == 0) return 0;
	return t->sum;
}

void recalc(Node* t) {
	if (t == 0) return;
	t->c = getC(t->ch[0]) + getC(t->ch[1]) + 1;
	t->sum = getSum(t->ch[0]) + getSum(t->ch[1]) + t->val;
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
	recalc(l);
	recalc(r);
	return { l, r };
}

Node* merge(Node* l, Node* r) {
	if (l == 0) return r;
	if (r == 0) return l;
	while (l->ch[1] != 0) {
		l = l->ch[1];
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

ll sum(Node*& t0, Node*& t1, ll a, ll b) {
	ll a0, b0, a1, b1;
	if (a % 2 == 0) {
		a0 = a / 2;
		a1 = a / 2;
	} else {
		a0 = a / 2 + 1;
		a1 = a / 2;
	}
	if (b % 2 == 0) {
		b0 = (b - 1) / 2;
		b1 = (b - 1) / 2;
	} else {
		b0 = (b - 1) / 2;
		b1 = (b - 1) / 2 - 1;
	}
	Node* l0, * m0, * r0, * l1, * m1, * r1;
	std::pair<Node*, Node*> tmp;
	tmp = split(t0, b0 + 1);
	l0 = tmp.first, r0 = tmp.second;
	tmp = split(l0, a0);
	l0 = tmp.first, m0 = tmp.second;
	tmp = split(t1, b1 + 1);
	l1 = tmp.first, r1 = tmp.second;
	tmp = split(l1, a1);
	l1 = tmp.first, m1 = tmp.second;
	ll ans = getSum(m1) + getSum(m0);
	t0 = merge(merge(l0, m0), r0);
	t1 = merge(merge(l1, m1), r1);
	return ans;
}

void swap(Node*& t0, Node*& t1, ll a, ll b) {
	ll a0, b0, a1, b1;
	if (a % 2 == 0) {
		a0 = a / 2;
		a1 = a / 2;
	} else {
		a0 = a / 2 + 1;
		a1 = a / 2;
	}
	if (b % 2 == 0) {
		b0 = (b - 1) / 2;
		b1 = (b - 1) / 2;
	} else {
		b0 = (b - 1) / 2;
		b1 = (b - 1) / 2 - 1;
	}
	Node* l0, * m0, * r0, * l1, * m1, * r1;
	std::pair<Node*, Node*> tmp;
	tmp = split(t0, b0 + 1);
	l0 = tmp.first, r0 = tmp.second;
	tmp = split(l0, a0);
	l0 = tmp.first, m0 = tmp.second;
	tmp = split(t1, b1 + 1);
	l1 = tmp.first, r1 = tmp.second;
	tmp = split(l1, a1);
	l1 = tmp.first, m1 = tmp.second;
	t0 = merge(merge(l0, m1), r0);
	t1 = merge(merge(l1, m0), r1);
}

void solve(ll n1, ll m1) {
	ll n = n1, m = m1;
	Node* root0 = 0, * root1 = 0;
	for (int i = 0; i < n; ++i) {
		ll cnt;
		std::cin >> cnt;
		if (i % 2 == 0) {
			root0 = insert(root0, i / 2, cnt);
		} else {
			root1 = insert(root1, i / 2, cnt);
		}
	}
	for (int i = 0; i < m; ++i) {
		ll t, l, r;
		std::cin >> t >> l >> r;
		if (t == 1) {
			swap(root0, root1, l - 1, r);
		} else {
			std::cout << sum(root0, root1, l - 1, r) << '\n';
		}
	}
	
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	std::freopen("swapper.in", "r", stdin);
	std::freopen("swapper.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	ll num = 1;
	while (true) {
		ll n, m;
		std::cin >> n >> m;
		if (n == 0 && m == 0) break;
		std::cout << "Swapper " << num << ":" << '\n';
		solve(n, m);
		++num;
	}
}
