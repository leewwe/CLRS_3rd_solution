#pragma once
#include <vector>
#include <iostream>
#include <stack>

using namespace std;

template<typename T> class Btree;

template<typename T = char>
Btree<T> join(Btree<T> t1, Btree<T> t2, T k);

template<typename T = char>
class Btree {
public:
	struct Node;
	friend Btree<T> join<T>(Btree<T> t1, Btree<T> t2, T k);
private:
	const int t;	// ��С��
	// �ڵ�ṹ��
	struct Node {
		Node() : n(0), leaf(true), h(0) {}
		//Node(int num) : n(num) { }
		Node(int height) : h(height) { }
		int n;				// �ؼ�����Ŀ
		vector<T> key;		// �ؼ������飬��Χ��[t-1,2*t-1]
		vector<Node*> c;	// ����ָ�룬��Ŀn+1
		bool leaf;			// ����Ƿ�ΪҶ�ӽڵ�
		int h;				// ��¼��ǰ�ڵ�ĸ߶�
	};
	Node* root; // ��
public:
	// �޲εĹ��캯����Ĭ��������С����tΪ2
	Btree() : t(2), root(NULL) {
		root = new Node();
#ifdef DEBUG
		cout << "DISK_WRITE(x)" << endl;
#endif // DEBUG
	}
	// ����һ�������Ĺ��캯����������С��tΪָ����ֵ
	Btree(const int minDegree) : t(minDegree){
		if (minDegree < 2) {
			t = 2;
		}
		root = new Node();
#ifdef DEBUG
		cout << "DISK_WRITE(x)" << endl;
#endif // DEBUG
	}
	Btree(const Btree<T>& rhs) : root(rhs.root), t(rhs.t) { }
	Btree(const Btree<T>::Node* node) : root(node) { }
	// ��̬���ϵĻ�������
	void erase(T k);				// ɾ��
	void insert(T k);				// ����
	pair<Node*, int>search(T k);	// ����
	pair<Node*, int> pred(T k);		// ǰ��
	pair<Node*, int> succ(T k);		// ���
	void print();					// ���ȫ��Ԫ��
	void print(Node*);				// ���ָ������Ԫ��
	int join(Btree tree, T k);		// ���Ӻ���
private:
	Node* findLeftNode_h(Node* root, int h);
	Node* findRightNode_h(Node* root, int h);
	void erase(Node* x, T k);
	void merge(Node* x, int i, Node* y, Node* z);
	void lShift(Node* x, int i, Node* l, Node* r);
	void rShift(Node* x, int i, Node* l, Node* r);
	T erase_pred(Node* x, T k);
	T erase_succ(Node* x, T k);
	pair<Node*, int>search(Node* x, T k);
	void splitChild(Node* x, int i);
	void insertNonFull(Node* x, T k);
	pair<Node*, int> pred(Node* x, T k);
	pair<Node*, int> succ(Node* x, T k);
};


