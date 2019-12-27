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
	const int t;	// 最小度
	// 节点结构体
	struct Node {
		Node() : n(0), leaf(true), h(0) {}
		//Node(int num) : n(num) { }
		Node(int height) : h(height) { }
		int n;				// 关键字数目
		vector<T> key;		// 关键字数组，范围：[t-1,2*t-1]
		vector<Node*> c;	// 孩子指针，数目n+1
		bool leaf;			// 标记是否为叶子节点
		int h;				// 记录当前节点的高度
	};
	Node* root; // 根
public:
	// 无参的构造函数，默认设置最小度数t为2
	Btree() : t(2), root(NULL) {
		root = new Node();
#ifdef DEBUG
		cout << "DISK_WRITE(x)" << endl;
#endif // DEBUG
	}
	// 接受一个参数的构造函数，设置最小度t为指定的值
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
	// 动态集合的基本操作
	void erase(T k);				// 删除
	void insert(T k);				// 插入
	pair<Node*, int>search(T k);	// 搜索
	pair<Node*, int> pred(T k);		// 前驱
	pair<Node*, int> succ(T k);		// 后继
	void print();					// 输出全部元素
	void print(Node*);				// 输出指定根的元素
	int join(Btree tree, T k);		// 连接函数
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


