#pragma once
#include <vector>

using std::vector;

struct vebNode {
	static const int NIL = -1;
	int u;
	vebNode* summary;//摘要指针
	vector<vebNode*> cluster;//簇指针数组
	int min;
	int max;

	vebNode(int k) : summary(nullptr), min(NIL), max(NIL), u(1<<k), cluster(k, nullptr){}
	vebNode() : summary(nullptr), min(NIL), max(NIL), u(2), cluster(){}
	~vebNode();
	inline int high(int x);
	inline int low(int x);
	inline int index(int x, int y);
};

int minimum(vebNode* V);
int maximum(vebNode* V);

class vEB {
	friend int minimum(vebNode* V);
	friend int maximum(vebNode* V);
public:
	vEB(int u);
	~vEB();
	bool member(int x);
	int successor(int x);
	int predecessor(int x);
	void insert(int x);
	void erase(int x);
private:
	bool member(vebNode* V, int x);
	int successor(vebNode* V, int x);
	int predecessor(vebNode* V, int x);
	void insert(vebNode* V, int x);
	void erase(vebNode* V, int x);
	inline void insertEmpty(vebNode* V, int x);
	vebNode* makeVEB(int k);
private:
	vebNode* root;
	// 判断元素是否存在的辅助数组，缺点是会占用大量的内存空间，体现了空间换速度的思想
	vector<bool> isExist;
};

