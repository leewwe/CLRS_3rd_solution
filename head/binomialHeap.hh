#pragma once
#include <utility>
#include <iostream>
// 不完全类模板声明
template<typename T = char>
class binomialHeap;
template<typename T = char>
struct Node;
// 友元模板函数声明
template<typename T>
binomialHeap<T> unite(binomialHeap<T> H1, binomialHeap<T> H2);
template<typename T>
binomialHeap<T> merge(binomialHeap<T> H1, binomialHeap<T> H2);
template<typename T>
void link(Node<T>* par, Node<T>* ch);
template<typename T>
void exchangeNode(Node<T>* l, Node<T>* r);
// 模板结点定义
template<typename T>
struct Node {
	T key;			// 关键字
	Node* p;		// 父节点
	Node* s;		// 兄弟结点
	Node* child;	// 孩子结点
	int degree;		// 度
	bool isRoot;	// 是否是根
	Node(T k) : key(k), p(nullptr), s(nullptr), child(nullptr), isRoot(true), degree(0) { }
};

// 二项堆类定义
template<typename T>
class binomialHeap {
	template<typename T>
	friend binomialHeap<T> unite(binomialHeap<T> H1, binomialHeap<T> H2);
	template<typename T>
	friend binomialHeap<T> merge(binomialHeap<T> H1, binomialHeap<T> H2);
	template<typename T>
	friend void link(Node<T>* par, Node<T>* ch);
public:
	binomialHeap(T k, const T m) : head(nullptr), min(nullptr), minValue(m) { 
		head = new Node<T>(k);
		min = head;
	}
	binomialHeap(Node<T>* node, const T m) : head(node), min(node), minValue(m) { }
	binomialHeap(const T m) : head(nullptr), min(nullptr), minValue(m) { }
	inline Node<T>* getMin() { return min; }
	
	inline void setNode(T k) {
		auto node = new Node<T>(k);
		auto curr = head;
		while (curr->s) {
			curr = curr->s;
		}
		curr->s = node;
	}
	void insert(T k);
	void unite(binomialHeap<T> H);
	void merge(binomialHeap<T> H);
	T extractMin();
	Node<T>* search(T k);
	bool decreaseKey(T tar, T k);
	bool erase(T k);
private:
	Node<T>* search(Node<T>* root, const T& k);
	bool decreaseKey(Node<T>* node, T k);
	Node<T>* extractMin_retNode();
private:
	Node<T>* head;
	Node<T>* min;
	const T minValue;
};

/********************************成员函数***************************************************/
/*插入函数*/
/*
参数：
	k：关键字值
返回：
	无
说明：
	完成堆元素的插入，新建一个临时的堆，然后合并this堆和临时堆
*/
template<typename T>
inline void binomialHeap<T>::insert(T k) {
	binomialHeap<T> H(k, minValue);
	unite(H);
}

/*合并函数*/
/*
参数：
	H：要合并的堆
返回：
	无
说明：
	这是个成员函数，是将传入的堆H合并到this堆中，基本算法和书中的伪代码一致，有略微修改
*/
template<typename T>
inline void binomialHeap<T>::unite(binomialHeap<T> H) {
	merge(H);
	if (head == nullptr) {
		return;
	}
	Node<T>* prev = nullptr;
	Node<T>* curr = head;
	Node<T>* next = head->s;

	while (next) {
		if ((curr->degree != next->degree) || (next->s && next->s->degree == curr->degree)) {
			prev = curr;
			curr = next;
		}
		else if (curr->key <= next->key) {
			curr->s = next->s;
			link(curr, next);
		}
		else {
			if (!prev) {
				head = next;
			}
			else {
				prev->s = next;
			}
			link(next, curr);
			curr = next;
		}
		next = curr->s;
	}
}

/*合并根单链表操作*/
/*
参数：
	H：待合并的堆
返回：
	无
说明：
	这是个私有成员函数，用于将H的根链表按照度升序合并到this的根链表中
*/
template<typename T>
inline void binomialHeap<T>::merge(binomialHeap<T> H) {
	if (!H.head) {
		return;
	}
	if (min && H.min) {
		min = min->key > H.min->key ? H.min : min;
	}
	else if(H.min){
		min = H.min;
		head = H.head;
		return;
	}
	Node<T>* p = head;
	Node<T>* h_p = H.head;
	Node<T>* prev_p = nullptr;
	while (p && h_p) {
		if(p->degree >= h_p->degree) {
			auto h_p_s = h_p->s;
			h_p->s = p;
			if (p == head) {
				head = h_p;
			}
			if (prev_p) {
				prev_p->s = h_p;
			}
			h_p = h_p_s;
		}
		prev_p = p;
		p = p->s;
	}

	if (!p) {
		prev_p->s = h_p;
	}
}

/*抽取最小关键字函数*/
/*
参数：
	无
返回：
	最小关键字
说明：
	这是个封装函数，完成抽取关键字和释放内存的作用，如果是空堆，会返回最小值，并抛出错误信息
*/
template<typename T>
inline T binomialHeap<T>::extractMin() {
	if (head == nullptr) {
		std::cout << "the heap is empty!" << std::endl;
		return minValue;
	}
	auto retMin = extractMin_retNode();
	auto retKey = retMin->key;
	if (retMin == head) {
		head = nullptr;
		min = nullptr;
	}
	delete retMin;
	return retKey;
}

/*抽取最小关键字函数*/
/*
参数：
	无
返回：
	包含最小关键字的结点
说明：
	这是个私有实现函数，基本原理和书中伪代码是一样的，有略微的修改
*/
template<typename T>
inline Node<T>* binomialHeap<T>::extractMin_retNode() {
	auto ret_min = min;
	Node<T>* child_p = min->child;
	min->child = nullptr;
	if (!child_p) {
		return ret_min;
	}
	binomialHeap<T> H(minValue);
	auto child_p_s = child_p->s;
	child_p->p = nullptr;
	child_p->s = nullptr;
	child_p->isRoot = true;
	H.head = child_p;
	H.min = child_p;
	child_p = child_p_s;
	Node<T>* h_p = H.head;
	while (child_p) {
		child_p_s = child_p->s;
		child_p->p = nullptr;
		child_p->s = h_p;
		child_p->isRoot = true;
		H.head = child_p;
		h_p = H.head;
		H.min = child_p->key < H.min->key ? child_p : H.min;
		child_p = child_p_s;
	}
	h_p = head;
	Node<T>* prev_h_p = nullptr;
	while (h_p != min) {
		prev_h_p = h_p;
		h_p = h_p->s;
	}
	prev_h_p->s = h_p->s;
	h_p->s = nullptr;
	min = head;
	unite(H);

	return ret_min;
}

/*搜索函数*/
/*
参数：
	k：目标关键字
返回：
	目标关键字的结点
说明：
	这是个封装函数
*/
template<typename T>
inline Node<T>* binomialHeap<T>::search(T k) {
	return search(head, k);
}

/*搜索函数*/
/*
参数：
	k：目标关键字
	root：要搜索的根节点
返回：
	目标关键字的结点
说明：
	这是个实现函数，是个线性搜索算法，这是由于对于可合并堆而言，对于搜索的效率都不高
*/
template<typename T>
inline Node<T>* binomialHeap<T>::search(Node<T>* root, const T& k) {
	if (!root) {
		return nullptr;
	}
	auto h_p = root;
	Node<T>* child_res = nullptr;
	while (h_p) {
		if (h_p->key == k) {
			return h_p;
		}
		else {
			child_res = search(h_p->child, k);
		}
		if (!child_res) {
			h_p = h_p->s;
		}
		else {
			return child_res;
		}
	}
	return nullptr;
}


/*关键字减值函数*/
/*
参数：
	tar：目标关键字值
	k：减值后的关键字值
返回：
	减值是否成功
说明：
	封装函数，如果tar < k，则返回false，抛出错误信息；如果堆中没有包含tar的结点，返回false，抛出对应的错误信息
*/
template<typename T>
inline bool binomialHeap<T>::decreaseKey(T tar, T k) {
	if (k > tar) {
		std::cout << "new key is greater than current key!" << std::endl;
		return false;
	}
	auto node = search(head, tar);
	if (!node) {
		std::cout << "the key " << k << " is not exist in the heap!" << std::endl;
		return false;
	}
	return decreaseKey(node, k);
}

/*关键字减值函数*/
/*
参数：
	node：包含关键字的结点
	k：减值后的关键字值
返回：
	减值是否成功
说明：
	私有实现函数，和书中的伪代码是一致的
*/
template<typename T>
inline bool binomialHeap<T>::decreaseKey(Node<T>* node, T k) {
	node->key = k;
	auto par = node->p;
	auto ch = node;
	while (par && ch->key < par->key) {
		exchangeNode(par, ch);
		ch = par;
		par = ch->p;
	}
	if (!par && ch->key < min->key) {
		min = ch;
	}
	return true;
}

/*删除函数*/
/*
参数：
	k：待删除的关键字值
返回：
	删除是否成功
说明：
	如果没有关键字k，返回false，并抛出错误信息；和书中的伪代码是一样的
*/
template<typename T>
inline bool binomialHeap<T>::erase(T k) {
	auto node = search(k);
	if (!node) {
		std::cout << "the element "<< k <<" is not exist in the heap!" << std::endl;
		return false;
	}
	decreaseKey(node, minValue);
	extractMin();
	return true;
}
/************************************友元函数***************************************************/

/*链接函数*/
/*
参数：
	par：父节点
	ch：孩子结点
返回：
	无
说明：
	将ch链接为par的第一个孩子
*/
template<typename T>
void link(Node<T>* par, Node<T>* ch) {
	ch->p = par;
	ch->s = par->child;
	par->child = ch;
	ch->isRoot = false;
	++(par->degree);
}

/*交换结点函数*/
/*
参数：
	l、r：待交换的两个结点
返回：
	无
说明：
	只交换关键字值
*/
template<typename T>
inline void exchangeNode(Node<T>* l, Node<T>* r) {
	std::swap(l->key, r->key);
}

/*合并友元函数*/
/*
参数：
	H1、H2：要合并的两个堆
返回：
	合并的堆
说明：
	和书中的伪代码一样
*/
template<typename T>
binomialHeap<T> unite(binomialHeap<T> H1, binomialHeap<T> H2) {
	binomialHeap<T> H(merge(H1, H2));
	if (H.head == nullptr) {
		return H;
	}
	Node<T>* prev = nullptr;
	Node<T>* curr = H.head;
	Node<T>* next = H.head->s;

	while (next) {
		if ((curr->degree != next->degree) || (next->s && next->s->degree == curr->degree)) {
			prev = curr;
			curr = next;
		}
		else if (curr->key <= next->key) {
			curr->s = next->s;
			link(curr, next);
		}
		else {
			if (!prev) {
				H.head = next;
			}
			else {
				prev->s = next;
			}
			link(next, curr);
			curr = next;
		}
		next = curr->s;
	}

	return H;
}

/*合并堆的根链表友元*/
/*
参数：
	H1、H2：要合并根链表的两个堆
返回：
	合并后的堆
说明：
	友元函数，按照根结点的度升序合并
*/
template<typename T>
binomialHeap<T> merge(binomialHeap<T> H1, binomialHeap<T> H2) {
	auto minValue = H1.minValue < H1.minValue ? H1.minValue : H2.minValue;
	binomialHeap<T> H(minValue);
	if (!H1.head && !H2.head) {
		H.min = nullptr;
		H.head = nullptr;
		return H;
	}
	else if (!H1.head && H2.head) {
		H.min = H2.min;
		H.head = H2.head;
		return H;
	}
	else if (H1.head && !H2.head) {
		H.min = H1.min;
		H.head = H1.head;
		return H;
	}
	H.min = H1.min->key > H2.min->key ? H2.min : H1.min;
	Node<T>* h1_p = H1.head;
	Node<T>* h2_p = H2.head;
	if (h1_p->degree > h2_p->degree) {
		H.head = h2_p;
		h2_p = h2_p->s;
	}
	else {
		H.head = h1_p;
		h1_p = h1_p->s;
	}
	Node<T>* H_curr = H.head;
	H_curr->s = nullptr;
	while (h1_p && h2_p) {
		if (h1_p->degree > h2_p->degree) {
			H_curr->s = h2_p;
			h2_p = h2_p->s;
		}
		else {
			H_curr->s = h1_p;
			h1_p = h1_p->s;
		}
		H_curr = H_curr->s;
		H_curr->s = nullptr;
	}

	if (!h1_p && !h2_p) {}
	else if (!h1_p) {
		H_curr->s = h2_p;
	}
	else {
		H_curr->s = h1_p;
	}
	return H;
}
