#pragma once
#include <utility>
#include <iostream>
// ����ȫ��ģ������
template<typename T = char>
class binomialHeap;
template<typename T = char>
struct Node;
// ��Ԫģ�庯������
template<typename T>
binomialHeap<T> unite(binomialHeap<T> H1, binomialHeap<T> H2);
template<typename T>
binomialHeap<T> merge(binomialHeap<T> H1, binomialHeap<T> H2);
template<typename T>
void link(Node<T>* par, Node<T>* ch);
template<typename T>
void exchangeNode(Node<T>* l, Node<T>* r);
// ģ���㶨��
template<typename T>
struct Node {
	T key;			// �ؼ���
	Node* p;		// ���ڵ�
	Node* s;		// �ֵܽ��
	Node* child;	// ���ӽ��
	int degree;		// ��
	bool isRoot;	// �Ƿ��Ǹ�
	Node(T k) : key(k), p(nullptr), s(nullptr), child(nullptr), isRoot(true), degree(0) { }
};

// ������ඨ��
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

/********************************��Ա����***************************************************/
/*���뺯��*/
/*
������
	k���ؼ���ֵ
���أ�
	��
˵����
	��ɶ�Ԫ�صĲ��룬�½�һ����ʱ�Ķѣ�Ȼ��ϲ�this�Ѻ���ʱ��
*/
template<typename T>
inline void binomialHeap<T>::insert(T k) {
	binomialHeap<T> H(k, minValue);
	unite(H);
}

/*�ϲ�����*/
/*
������
	H��Ҫ�ϲ��Ķ�
���أ�
	��
˵����
	���Ǹ���Ա�������ǽ�����Ķ�H�ϲ���this���У������㷨�����е�α����һ�£�����΢�޸�
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

/*�ϲ������������*/
/*
������
	H�����ϲ��Ķ�
���أ�
	��
˵����
	���Ǹ�˽�г�Ա���������ڽ�H�ĸ������ն�����ϲ���this�ĸ�������
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

/*��ȡ��С�ؼ��ֺ���*/
/*
������
	��
���أ�
	��С�ؼ���
˵����
	���Ǹ���װ��������ɳ�ȡ�ؼ��ֺ��ͷ��ڴ�����ã�����ǿնѣ��᷵����Сֵ�����׳�������Ϣ
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

/*��ȡ��С�ؼ��ֺ���*/
/*
������
	��
���أ�
	������С�ؼ��ֵĽ��
˵����
	���Ǹ�˽��ʵ�ֺ���������ԭ�������α������һ���ģ�����΢���޸�
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

/*��������*/
/*
������
	k��Ŀ��ؼ���
���أ�
	Ŀ��ؼ��ֵĽ��
˵����
	���Ǹ���װ����
*/
template<typename T>
inline Node<T>* binomialHeap<T>::search(T k) {
	return search(head, k);
}

/*��������*/
/*
������
	k��Ŀ��ؼ���
	root��Ҫ�����ĸ��ڵ�
���أ�
	Ŀ��ؼ��ֵĽ��
˵����
	���Ǹ�ʵ�ֺ������Ǹ����������㷨���������ڶ��ڿɺϲ��Ѷ��ԣ�����������Ч�ʶ�����
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


/*�ؼ��ּ�ֵ����*/
/*
������
	tar��Ŀ��ؼ���ֵ
	k����ֵ��Ĺؼ���ֵ
���أ�
	��ֵ�Ƿ�ɹ�
˵����
	��װ���������tar < k���򷵻�false���׳�������Ϣ���������û�а���tar�Ľ�㣬����false���׳���Ӧ�Ĵ�����Ϣ
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

/*�ؼ��ּ�ֵ����*/
/*
������
	node�������ؼ��ֵĽ��
	k����ֵ��Ĺؼ���ֵ
���أ�
	��ֵ�Ƿ�ɹ�
˵����
	˽��ʵ�ֺ����������е�α������һ�µ�
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

/*ɾ������*/
/*
������
	k����ɾ���Ĺؼ���ֵ
���أ�
	ɾ���Ƿ�ɹ�
˵����
	���û�йؼ���k������false�����׳�������Ϣ�������е�α������һ����
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
/************************************��Ԫ����***************************************************/

/*���Ӻ���*/
/*
������
	par�����ڵ�
	ch�����ӽ��
���أ�
	��
˵����
	��ch����Ϊpar�ĵ�һ������
*/
template<typename T>
void link(Node<T>* par, Node<T>* ch) {
	ch->p = par;
	ch->s = par->child;
	par->child = ch;
	ch->isRoot = false;
	++(par->degree);
}

/*������㺯��*/
/*
������
	l��r�����������������
���أ�
	��
˵����
	ֻ�����ؼ���ֵ
*/
template<typename T>
inline void exchangeNode(Node<T>* l, Node<T>* r) {
	std::swap(l->key, r->key);
}

/*�ϲ���Ԫ����*/
/*
������
	H1��H2��Ҫ�ϲ���������
���أ�
	�ϲ��Ķ�
˵����
	�����е�α����һ��
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

/*�ϲ��ѵĸ�������Ԫ*/
/*
������
	H1��H2��Ҫ�ϲ��������������
���أ�
	�ϲ���Ķ�
˵����
	��Ԫ���������ո����Ķ�����ϲ�
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
