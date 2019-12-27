#pragma once
#include <vector>
#include <cmath>
using namespace std;
// ���͵Ĳ���ȫ����
template<typename T = char>
class fibHeap;

template<typename T = char>
struct Node;

// ��Ԫ����
template<typename T = char>
fibHeap<T> fibHeapUnion(fibHeap<T> H1, fibHeap<T> H2);

template<typename T = char>
void exchange(Node<T>* l, Node<T>* r);

// �����
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

// 쳲���������
template<typename T>
class fibHeap {
	template<typename T>
	friend fibHeap<T> fibHeapUnion(fibHeap<T> H1, fibHeap<T> H2);
	friend void exchange(Node<T>* l, Node<T>* r);
public:
	fibHeap(T l) : min(nullptr), n(0), limit(l){ } // ���캯����Ҫ������������T����Сֵ
	void insert(T k);
	void erase(T k);
	Node<T>* extractMin(void);
	inline Node<T>* getMin(void) { return min; }
	Node<T>* search(T k);
	void setChild(Node<T>* parent, Node<T>* child);
	void decreaseKey(Node<T>* x, T k);
	void setChild(T k); // �����õĺ������������ֱ�����н��ĺ���
	inline Node<T>* getNodeChild(Node<T>* root) { return root->cd; } //�����ã����ڻ�ȡ���ĺ���
private:
	fibHeap(Node<T>* m, T l) : min(m), limit(l){ } // ˽�й��캯������ֹ��������
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

/*�����㺯��*/
/*
������
	k�����ڹ�����Ĺؼ���
���أ�
	��
˵����
	����Ƿ�װ�����������û�ֱ�ӵ��ò�����
*/
template<typename T>
inline void fibHeap<T>::insert(T k) {
	auto x = new Node<T>(k);
	insert(x);
}

/*���뺯��*/
/*
������
	x��������Ľ��
���أ�
	��
˵����
	�����˽�к�����ʵ�ʵĲ��뺯����ʵ�֣������������ѵ�Ԫ����Ŀ
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

/*ɾ�����ķ�װ����*/
/*
������
	k����ɾ�����Ĺؼ���
���أ�
	��
˵����
	������������������ڣ�ֱ�ӷ��أ�������ڲŻ�ɾ��
*/
template<typename T>
inline void fibHeap<T>::erase(T k) {
	auto node = search(k);
	if (!node) {
		return;
	}
	erase(node);
}

/*ɾ������ʵ�ֺ���*/
/*
������
	x����ɾ���Ľ��
���أ�
	��
˵����
	���Ƚ��ؼ��ּ��ٵ�limit��Ȼ��ʹ��extract������
*/
template<typename T>
inline void fibHeap<T>::erase(Node<T>* x) {
	decreaseKey(x, limit);
	extractMin();
}


/*pop��������С쳲������ѵĶ�����*/
/*
������
	��
���أ�
	������Сkeyֵ�Ľ��
˵����
	�Ὣ��С���Ӷ���ɾ������������������С�ѵĴ�С
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

/*������װ����*/
/*
������
	k���������Ĺؼ���
���أ�
	����ؼ��ִ����ڶ��У����ذ����ؼ��ֵĽ�㣬���򷵻�nullptr
˵����
	���������һ�����������㷨O(n)
*/
template<typename T>
inline Node<T>* fibHeap<T>::search(T k) {
	return search(min, k);
}

/*����ʵ�ֺ���*/
/*
������
	root�����ܰ����ؼ���k�ĸ��ڵ�
	k���������Ĺؼ���
���أ�
	����ؼ��ִ����ڶ��У����ذ����ؼ��ֵĽ�㣬���򷵻�nullptr
˵����
	���������һ�����������㷨O(n)
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


/*�ؼ��ּ�ֵ����*/
/*
������
	x����Ŀ��ڵ�
	k����x->key���ٵ�k
���أ�
	��
˵����
	�����㷨�����ϵ�α����ʵ��
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

/*����min�ڵ�ĺ���*/
/*
������
	k�����ӽ��Ĺؼ���
���أ�
	��
˵����
	�������������debug�ģ��������û�ʹ�ö����ض��Ľ����뵽����
*/
template<typename T>
inline void fibHeap<T>::setChild(T k) {
	auto child = new Node<T>(k);
	setChild(min, child);
}

/*���ý��ĺ���*/
/*
������
	parent�����ڵ�
	child���ӽڵ�
���أ�
	��
˵����
	��child�������Ϊparent���ӽڵ�
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

/*�Ӹ��������Ƴ����*/
/*
������
	node�����Ƴ��ĸ��ڵ�
���أ�
	��
˵����
	ֻ�ǽ�node�ӽ���������Ƴ���������ı䱻�Ƴ�node����Ϣ����������Ǹ�˽�к���
*/
template<typename T>
inline void fibHeap<T>::removeRootListNode(Node<T>* node) {
	node->r->l = node->l;
	node->l->r = node->r;
}

/*�ϲ��������㺯��*/
/*
������
	��
���أ�
	��
˵����
	��������Ǹ�˽�к���������extractMin�����С�
	������������������ͬ�ȵĽ��ϲ�Ϊ������С�ѵ��������պϲ��ĸ��������ھ�����ͬ�ȵĽ�㣻ͬʱ��λ�µ�min���λ��
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

/*��node����ֱ�Ӻ�����������������*/
/*
������
	root��������ĸ������н��
���أ�
	��
˵����
	���Ǹ�˽�к���������extractMin�С�
	���ڽ�min����ֱ�Ӻ����������������У�Ҫע��ʹ����insert�����������������
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
	--n;// ����ʹ����insert����������insertʱ��������Ľ����Ŀ��1��
		// �������ǽ�root����ֱ�Ӻ����������������У���û��ʵ��������㣬������Ҫ���������ȷ�ļ�1ɾȥ
	end->p = nullptr;
	while (start != end) {
		auto s_tmp = start->r;
		insert(start);
		--n; // ���������ע��һ������
		start->p = nullptr;
		start = s_tmp;
	}
}

/*��y�������Ϊx�ĺ���*/
/*
������
	x�����ڵ�
	y�������ӵ��ӽڵ�
���أ�
	��
˵����
	˽�к���������extractMin�����У����ڽ�y����Ϊx�ĺ���
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

// ��Ԫ����ģ��
/*���Ϻ���*/
/*
������
	H1����1
	H2����2
���أ�
	�ϲ���Ķ�
˵����
	��Ԫ����
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

/*ר�õĽ�㽻������*/
/*
������
	l��r��Ҫ�������������
���أ�
	��
˵����
	����쳲������ѵĽ�㣬��ѭ�㷨����p294�Ļ���ֻ�ύ����������ָ��֮���Ԫ�أ��Ա�֤ѭ������Ĺ�ϵ��ȷ��
*/
template<typename T>
void exchange(Node<T>* l, Node<T>* r) {
	std::swap(l->key, r->key);
	std::swap(l->degree, r->degree);
	std::swap(l->mark, l->mark);
	std::swap(l->cd, r->cd);
	std::swap(l->p, r->p);
}
