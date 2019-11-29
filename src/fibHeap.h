#pragma once
#include <vector>
#include <cmath>
using namespace std;
// 类型的不完全声明
template<typename T = char>
class fibHeap;

template<typename T = char>
struct Node;

// 友元函数
template<typename T = char>
fibHeap<T> fibHeapUnion(fibHeap<T> H1, fibHeap<T> H2);

template<typename T = char>
void exchange(Node<T>* l, Node<T>* r);

// 结点类
template<typename T>
struct Node {
	T key;
	Node* p;
	Node* l;
	Node* r;
	Node* cd;
	int degree;
	bool mark;
	Node() = default;
	Node(T k) : key(k), p(nullptr), l(this), r(this), cd(nullptr), degree(0), mark(false) { }
};

// 斐波那契堆类
template<typename T>
class fibHeap {
	template<typename T>
	friend fibHeap<T> fibHeapUnion(fibHeap<T> H1, fibHeap<T> H2);
	friend void exchange(Node<T>* l, Node<T>* r);
public:
	fibHeap(T l) : min(nullptr), n(0), limit(l){ } // 构造函数需要传入数据类型T的最小值
	void insert(T k);
	void erase(T k);
	Node<T>* extractMin(void);
	inline Node<T>* getMin(void) { return min; }
	Node<T>* search(T k);
	void setChild(Node<T>* parent, Node<T>* child);
	void decreaseKey(Node<T>* x, T k);
	void setChild(T k); // 调试用的函数，用于外层直接树中结点的孩子
	inline Node<T>* getNodeChild(Node<T>* root) { return root->cd; } //调试用，用于获取结点的孩子
private:
	fibHeap(Node<T>* m, T l) : min(m), limit(l){ } // 私有构造函数，阻止被外界访问
	Node<T>* search(Node<T>* root, T k);
	void insert(Node<T>* x);
	void erase(Node<T>* x);
	void removeRootListNode(Node<T>* node);
	void consolidate(void);
	void spreadChild(Node<T>* root);
	void link(Node<T>* x, Node<T>* y);
	void cut(Node<T>* parent, Node<T>* child);
	void cascadingCut(Node<T>* y);
	Node<T>* min;
	int n;
	const T limit;
};

/*插入结点函数*/
/*
参数：
	k：用于构造结点的关键字
返回：
	无
说明：
	这个是封装函数，用于用户直接调用插入结点
*/
template<typename T>
inline void fibHeap<T>::insert(T k) {
	auto x = new Node<T>(k);
	insert(x);
}

/*插入函数*/
/*
参数：
	x：待插入的结点
返回：
	无
说明：
	这个是私有函数，实际的插入函数的实现，会增加整个堆的元素数目
*/
template<typename T>
inline void fibHeap<T>::insert(Node<T>* x) {
	if (min == nullptr) {
		min = x;
	}
	else {
		x->l = min;
		x->r = min->r;
		min->r->l = x;
		min->r = x;
		if (x->key < min->key) {
			min = x;
		}
	}
	++n;
}

/*删除结点的封装函数*/
/*
参数：
	k：待删除结点的关键字
返回：
	无
说明：
	首先搜索，如果不存在，直接返回，如果存在才会删除
*/
template<typename T>
inline void fibHeap<T>::erase(T k) {
	auto node = search(k);
	if (!node) {
		return;
	}
	erase(node);
}

/*删除结点的实现函数*/
/*
参数：
	x：待删除的结点
返回：
	无
说明：
	首先将关键字减少到limit，然后使用extract将其抽出
*/
template<typename T>
inline void fibHeap<T>::erase(Node<T>* x) {
	decreaseKey(x, limit);
	extractMin();
}


/*pop并返回最小斐波那契堆的顶点结点*/
/*
参数：
	无
返回：
	具有最小key值的结点
说明；
	会将最小结点从堆中删除，但不会析构，减小堆的大小
*/
template<typename T>
inline Node<T>* fibHeap<T>::extractMin(void) {
	if (!min) {
		return min;
	}
	auto z = min;
	spreadChild(min);
	removeRootListNode(z);
	if (z == z->r) {
		min = nullptr;
	}
	else {
		min = z->r;
		consolidate();
	}
	--n;
	z->l = z;
	z->r = z;
	return z;
}

/*搜索封装函数*/
/*
参数：
	k：待搜索的关键字
返回：
	如果关键字存在于堆中，返回包含关键字的结点，否则返回nullptr
说明：
	这个搜索是一个线性搜索算法O(n)
*/
template<typename T>
inline Node<T>* fibHeap<T>::search(T k) {
	return search(min, k);
}

/*搜索实现函数*/
/*
参数：
	root：可能包含关键字k的父节点
	k：待搜索的关键字
返回：
	如果关键字存在于堆中，返回包含关键字的结点，否则返回nullptr
说明：
	这个搜索是一个线性搜索算法O(n)
*/
template<typename T>
inline Node<T>* fibHeap<T>::search(Node<T>* root, T k) {
	if (root == nullptr) {
		cout << "the heap is empty!" << endl;
		return nullptr;
	}
	auto end = root;
	auto start = root->r;
	if (end->key == k) {
		return end;
	}
	if (end->cd) {
		auto res = search(end->cd, k);
		if (res == min) {
			return nullptr;
		}
		if (res && res->key == k) {
			return res;
		}
	}
	Node<T>* res(nullptr);
	while (start != end) {
		if (start->key == k) {
			return start;
		}
		else if (start->cd) {
			res = search(start->cd, k);
			if (res == min) {
				return nullptr;
			}
			if (res && res->key == k) {
				return res;
			}
		}
		start = start->r;
	}
	if (res == min) {
		return nullptr;
	}
	return res;
}


/*关键字键值函数*/
/*
参数：
	x：待目标节点
	k：从x->key减少到k
返回：
	无
说明：
	按照算法导论上的伪代码实现
*/
template<typename T>
inline void fibHeap<T>::decreaseKey(Node<T>* x, T k) {
	if (k > x->key) {
		cout << "new key is bigger than current key" << endl;
		return;
	}
	x->key = k;
	auto y = x->p;
	if (y && x->key < y->key) {
		cut(y, x);
		cascadingCut(y);
	}
	if (x->key < min->key) {
		min = x;
	}
	return;
}

/*设置min节点的孩子*/
/*
参数：
	k：孩子结点的关键字
返回：
	无
说明：
	这个函数是用作debug的，用于在用户使用对象将特定的结点插入到堆中
*/
template<typename T>
inline void fibHeap<T>::setChild(T k) {
	auto child = new Node<T>(k);
	setChild(min, child);
}

/*设置结点的孩子*/
/*
参数：
	parent：父节点
	child：子节点
返回：
	无
说明：
	将child结点设置为parent的子节点
*/
template<typename T>
inline void fibHeap<T>::setChild(Node<T>* parent, Node<T>* child) {
	if (!parent->cd) {
		parent->cd = child;
		child->p = parent;
		++(parent->degree);
		child->l = child;
		child->r = child;
		return;
	}
	child->p = parent;
	child->r = parent->cd->r;
	child->l = parent->cd;
	parent->cd->r->l = child;
	parent->cd->r = child;
	++(parent->degree);
}

/*从根链表中移除结点*/
/*
参数：
	node：待移除的根节点
返回：
	无
说明：
	只是将node从结点链表中移除，并不会改变被移除node的信息，这个函数是个私有函数
*/
template<typename T>
inline void fibHeap<T>::removeRootListNode(Node<T>* node) {
	node->r->l = node->l;
	node->l->r = node->r;
}

/*合并根链表结点函数*/
/*
参数：
	无
返回：
	无
说明：
	这个函数是个私有函数，用在extractMin函数中。
	遍历根链表，将具有相同度的结点合并为符合最小堆的树，最终合并的根链表不存在具有相同度的结点；同时定位新的min结点位置
*/
template<typename T>
inline void fibHeap<T>::consolidate(void) {
	int D_n = int(log2(n));
	vector<Node<T>*> vRootListNode_p(int(D_n + 1), nullptr);
	auto start = min->r;
	auto end = min;
	vRootListNode_p[end->degree] = end;
	while (start != end) {
		auto x = start;
		auto d = x->degree;
		while (vRootListNode_p[d] != nullptr) {
			auto y = vRootListNode_p[d];
			if (x->key > y->key) {
				exchange(x, y);
			}
			if (y == end) {
				end = end->r;
				min = end;
			}
			link(x, y);
			vRootListNode_p[d] = nullptr;
			++d;
		}
		vRootListNode_p[d] = x;
		start = start->r;
	}
	for (int i = 0; i <= D_n; ++i) {
		if (vRootListNode_p[i] && vRootListNode_p[i]->key < min->key) {
			min = vRootListNode_p[i];
		}
	}
}

/*将node结点的直接孩子提升到根链表中*/
/*
参数：
	root：待分离的根链表中结点
返回：
	无
说明：
	这是个私有函数，用在extractMin中。
	用于将min结点的直接孩子提升到跟链表中，要注意使用了insert函数后的修正操作。
*/
template<typename T>
inline void fibHeap<T>::spreadChild(Node<T>* root) {
	if (root->cd == nullptr) {
		return;
	}
	auto start = root->cd->r;
	auto end = root->cd;
	root->cd = nullptr;
	insert(end);
	--n;// 由于使用了insert函数，会在insert时对整个类的结点数目加1，
		// 而这里是将root结点的直接孩子提升到根链表中，并没有实际新增结点，所以需要将这个不正确的加1删去
	end->p = nullptr;
	while (start != end) {
		auto s_tmp = start->r;
		insert(start);
		--n; // 这里和上面注释一个含义
		start->p = nullptr;
		start = s_tmp;
	}
}

/*将y结点链接为x的孩子*/
/*
参数：
	x：父节点
	y：待链接的子节点
返回：
	无
说明：
	私有函数，用于extractMin函数中，用于将y链接为x的孩子
*/
template<typename T>
inline void fibHeap<T>::link(Node<T>* x, Node<T>* y) {
	removeRootListNode(y);
	setChild(x, y);
}

template<typename T>
inline void fibHeap<T>::cut(Node<T>* parent, Node<T>* child) {
	removeRootListNode(child);
	--(parent->degree);
	if (parent->degree == 0) {
		parent->cd = nullptr;
	}
	child->p = nullptr;
	insert(child);
	--n;
	child->mark = false;
}

template<typename T>
inline void fibHeap<T>::cascadingCut(Node<T>* y) {
	auto z = y->p;
	if (z) {
		if (y->mark == false) {
			y->mark = true;
		}
		else {
			cut(z, y);
			cascadingCut(z);
		}
	}
}

// 友元函数模板
/*联合函数*/
/*
参数：
	H1：堆1
	H2：堆2
返回：
	合并后的堆
说明：
	友元函数
*/
template<typename T>
inline fibHeap<T> fibHeapUnion(fibHeap<T> H1, fibHeap<T> H2) {
	fibHeap<T> H(H1.min, H1.limit);
	H1.min->r->l = H2.min->l;
	H2.min->l->r = H1.min->r;
	H1.min->r = H2.min;
	H2.min->l = H1.min;

	if (H1.min == nullptr || (H2.min != nullptr && H2.min->key < H1.min->key)) {
		H.min = H2.min;
	}
	H.n = H1.n + H2.n;
	return H;

}

/*专用的结点交换函数*/
/*
参数：
	l、r：要交换的两个结点
返回：
	无
说明：
	对于斐波那契堆的结点，遵循算法导论p294的话，只会交换除了左右指针之外的元素，以保证循环链表的关系正确性
*/
template<typename T>
void exchange(Node<T>* l, Node<T>* r) {
	std::swap(l->key, r->key);
	std::swap(l->degree, r->degree);
	std::swap(l->mark, l->mark);
	std::swap(l->cd, r->cd);
	std::swap(l->p, r->p);
}
