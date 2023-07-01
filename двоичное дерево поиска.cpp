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

	Node(ll x) {
		ch[0] = ch[1] = p = 0;
		val = x;
	}
};

bool isRoot(Node* t) {
	return t->p == 0 || (t->p->ch[0] != t && t->p->ch[1] != t);
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

Node* nextEq(Node*& t, ll x) {
	if (t == 0) return 0;
	Node* min = 0;
	auto now = t;
	while (true) {
		if (now->val >= x) {
			if (min == 0 || min->val > now->val) {
				min = now;
			}
		}
		if (now->val <= x) {
			if (now->ch[1] == 0) break;
			now = now->ch[1];
		} else {
			if (now->ch[0] == 0) {
				break;
			}
			now = now->ch[0];
		}
	}
	splay(now);
	t = now;
	splay(min);
	if (min != 0) {
		t = min;
	}
	return min;
}

Node* prevEq(Node*& t, ll x) {
	if (t == 0) return 0;
	Node* max = 0;
	auto now = t;
	while (true) {
		if (now->val <= x) {
			if (max == 0 || max->val < now->val) {
				max = now;
			}
		}
		if (now->val < x) {
			if (now->ch[1] == 0) break;
			now = now->ch[1];
		} else {
			if (now->ch[0] == 0) break;
			now = now->ch[0];
		}
	}
	splay(now);
	t = now;
	splay(max);
	if (max != 0) {
		t = max;
	}
	return max;
}

Node* next(Node*& t, ll x) {
	if (t == 0) return 0;
	Node* min = 0;
	auto now = t;
	while (true) {
		if (now->val > x) {
			if (min == 0 || min->val > now->val) {
				min = now;
			}
		}
		if (now->val <= x) {
			if (now->ch[1] == 0) break;
			now = now->ch[1];
		} else {
			if (now->ch[0] == 0) {
				break;
			}
			now = now->ch[0];
		}
	}
	splay(now);
	t = now;
	splay(min);
	if (min != 0) {
		t = min;
	}
	return min;
}

Node* prev(Node*& t, ll x) {
	if (t == 0) return 0;
	Node* max = 0;
	auto now = t;
	while (true) {
		if (now->val < x) {
			if (max == 0 || max->val < now->val) {
				max = now;
			}
		}
		if (now->val < x) {
			if (now->ch[1] == 0) break;
			now = now->ch[1];
		} else {
			if (now->ch[0] == 0) break;
			now = now->ch[0];
		}
	}
	splay(now);
	t = now;
	splay(max);
	if (max != 0) {
		t = max;
	}
	return max;
}

bool exist(Node*& t, ll x) {
	if (t == 0) return 0;
	prevEq(t, x);
	return t->val == x;
}

std::pair<Node*, Node*> split(Node* t, ll x) {
	if (t == 0) return { 0, 0 };
	auto tmp = nextEq(t, x);
	Node* l = 0;
	Node* r = t;
	if (t != tmp) {
		return { r, l };
	}
	if (r != 0) {
		l = r->ch[0];
		r->ch[0] = 0;
	}
	if (l != 0) {
		r->p = 0;
	}
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

Node* insert(Node* t, ll x) {
	auto [l, r] = split(t, x);
	Node* now = new Node(x);
	if (r != 0 && r->val == x) return merge(l, r);
	return merge(merge(l, now), r);
}

Node* erase(Node* t, ll x) {
	if (t == 0) return 0;
	prevEq(t, x);
	if (t->val == x) {
		Node* l = t->ch[0];
		Node* r = t->ch[1];
		if (l != 0) l->p = 0;
		if (r != 0) r->p = 0;
		delete t;
		t = merge(l, r);
	}
	return t;
}

void solve() {
	/*ll n;
	std::cin >> n;*/
	str s;
	ll x;
	Node* root = 0;
	while (std::cin >> s >> x) {
		//std::cin >> s >> x;
		if (s == "insert") {
			root = insert(root, x);
		} else if (s == "delete") {
			root = erase(root, x);
		} else if (s == "exists") {
			if (exist(root, x)) {
				std::cout << "true" << '\n';
			} else {
				std::cout << "false" << '\n';
			}
		} else if (s == "next") {
			Node* now = next(root, x);
			if (now == 0) {
				std::cout << "none" << '\n';
			} else {
				std::cout << now->val << '\n';
			}
		} else if (s == "prev") {
			Node* now = prev(root, x);
			if (now == 0) {
				std::cout << "none" << '\n';
			} else {
				std::cout << now->val << '\n';
			}
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cout << std::fixed << std::setprecision(10);
	std::freopen("bst.in", "r", stdin);
	std::freopen("bst.out", "w", stdout);
	ll t = 1;
	//std::cin >> t;
	while (t--) {
		solve();
	}
}
