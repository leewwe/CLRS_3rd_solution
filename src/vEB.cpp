#include "..\head\vEB.hh"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

/*****************************************************结点类函数*****************************************************/
int vebNode::high(int x) {
	return x / static_cast<int>(sqrt(u));
}

int vebNode::low(int x) {
	return x % static_cast<int>(sqrt(u));
}

int vebNode::index(int x, int y) {
	return x * static_cast<int>(sqrt(u)) + y;
}
/*析构函数*/
/*
说明：
	释放summary和所有的cluster
*/
vebNode::~vebNode() {
	delete summary;
	for (auto& c : cluster) {
		delete c;
	}
}

/*****************************************************友元函数*****************************************************/
int minimum(vebNode* V) {
	return V->min;
}

int maximum(vebNode* V) {
	return V->max;
}
/*****************************************************成员函数*****************************************************/
/*构造函数*/
/*
参数：
	u：全域大小
*/
vEB::vEB(int u) : isExist(u, false) {
	if (u < 0) {
		u = 2;
		isExist.resize(2, false);
	}
	int k = static_cast<int>(ceil(log2(u)));
	root = makeVEB(k);
}

vEB::~vEB() {
	delete root;
}
/*创建函数*/
/*
参数：
	k：全域u = 2^k
返回：
	结点指针
说明：
	私有函数，用于创建给定全域大小的空的van Emde Boas树
*/
vebNode* vEB::makeVEB(int k) {
	if (k == 1) {
		return new vebNode();
	}
	else {
		vebNode* ret = new vebNode(k);
		ret->summary = makeVEB(static_cast<int>(ceil(k / 2.0)));
		for (auto& c : ret->cluster) {
			c = makeVEB(k / 2);
		}
		return ret;
	}
}

/*查找后继函数*/
/*
参数：
	x：带查找的元素
返回：
	如果后继存在，返回后继的值，如果不存在，返回vebNode::NIL
说明：
	公有封装接口，带查找的元素x可以不在树中
*/
int vEB::successor(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return vebNode::NIL;
	}
	return successor(root, x);
}
/*查找后继函数*/
/*
参数：
	V：带查找的结点
	x：带查找的元素
返回：
	如果后继存在，返回后继的值，如果不存在，返回vebNode::NIL
说明：
	私有实现函数，带查找的元素x可以不在树中
*/
int vEB::successor(vebNode* V, int x) {
	if (V->u == 2) {
		if (x == 0 && V->max == 1) {
			return 1;
		}
		else {
			return vebNode::NIL;
		}
	}
	else if (V->min != vebNode::NIL && x < V->min) {
		return V->min;
	}
	else {
		int max_low = maximum(V->cluster[V->high(x)]);
		if (max_low != vebNode::NIL && V->low(x) < max_low) {
			int offset = successor(V->cluster[V->high(x)], V->low(x));
			return V->index(V->high(x), offset);
		}
		else {
			int succ_cluster = successor(V->summary, V->high(x));
			if (succ_cluster == vebNode::NIL) {
				return vebNode::NIL;
			}
			else {
				int offset = minimum(V->cluster[succ_cluster]);
				return V->index(succ_cluster, offset);
			}
		}
	}
	return 0;
}

/*查找前驱函数*/
/*
参数：
	x：带查找的元素
返回：
	如果前驱存在，返回前驱的值，如果不存在，返回vebNode::NIL
说明：
	公有封装接口，带查找的元素x可以不在树中
*/
int vEB::predecessor(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return vebNode::NIL;
	}
	return predecessor(root, x);
}
/*查找前驱函数*/
/*
参数：
	V：带查找的结点
	x：带查找的元素
返回：
	如果前驱存在，返回前驱的值，如果不存在，返回vebNode::NIL
说明：
	私有实现函数，带查找的元素x可以不在树中
*/
int vEB::predecessor(vebNode* V, int x) {
	if (V->u == 2) {
		if (x == 1 && V->min == 0) {
			return 0;
		}
		else {
			return vebNode::NIL;
		}
	}
	else if (V->max != vebNode::NIL && x > V->max) {
		return V->max;
	}
	else {
		int min_low = minimum(V->cluster[V->high(x)]);
		if (min_low != vebNode::NIL && V->low(x) > min_low) {
			int offset = predecessor(V->cluster[V->high(x)], V->low(x));
			return V->index(V->high(x), V->low(x));
		}
		else {
			int pred_cluster = predecessor(V->summary, V->high(x));
			if (pred_cluster == vebNode::NIL) {
				if (V->min != vebNode::NIL && x > V->min) {
					return V->min;
				}
				else {
					return vebNode::NIL;
				}
			}
			else {
				int offset = maximum(V->cluster[pred_cluster]);
				return V->index(pred_cluster, offset);
			}
		}
	}
}

/*验证是否存在函数*/
/*
参数：
	x：待验证的元素
返回：
	存在返回true，不存在返回false
说明：
	公有封装接口
*/
bool vEB::member(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return false;
	}
	return member(root, x);
}
/*验证是否存在函数*/
/*
参数：
	V：待验证的结点
	x：待验证的元素
返回：
	存在返回true，不存在返回false
说明：
	私有实现函数
*/
bool vEB::member(vebNode* V, int x) {
	if (x > root->u || x < 0) {
		return false;
	}
	// 不使用判定元素是否存在的数组isExist，时间复杂度O(lglgu)
	if (x == V->min || x == V->max) {
		return true;
	}
	else if (V->u == 2) {
		return false;
	}
	else {
		return member(V->cluster[V->high(x)], V->low(x));
	}
	// 使用判定元素是否存在的数组isExist，时间复杂度O(1)
	//return isExist[x];
}

/*插入辅助函数*/
/*
参数：
	V：待插入的结点
	x：待插入的值
返回：
	无
说明：
	私有，用于处理空的结点
*/
void vEB::insertEmpty(vebNode* V, int x) {
	V->min = x;
	V->max = x;
}
/*插入函数*/
/*
参数：
	x：要插入的元素 
返回：
	无
说明：
	公有封装函数
*/
void vEB::insert(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return;
	}
	// 提前检查元素是否在vEB树中，避免插入已经在树中的元素，导致树结构被破坏
	// 代价是O(lglgu)
	//if (!member(root, x)) {
	//	insert(root, x);
	//}
	// 这是另一种解决办法，再顶层维护一个记录元素是否存在的数组，用于常数时间检查元素是否存在
	if (!isExist[x]) {
		isExist[x] = true;
		insert(root, x);
	}
	else {
		cout << "the element " << x << " have been inserted in the vEB tree!!" << endl;
	}
}
/*插入函数*/
/*
参数：
	V：待插入的结点
	x：要插入的元素
返回：
	无
说明：
	私有实现函数
*/
void vEB::insert(vebNode* V,int x) {
	if (V->min == vebNode::NIL) {
		insertEmpty(V, x);
	}
	else {
		if (x < V->min) {
			std::swap(x, V->min);
		}
		if (V->u > 2) {
			if (minimum(V->cluster[V->high(x)]) == vebNode::NIL) {
				insert(V->summary, V->high(x));
				insertEmpty(V->cluster[V->high(x)], V->low(x));
			}
			else {
				insert(V->cluster[V->high(x)], V->low(x));
			}
		}
		if (x > V->max) {
			V->max = x;
		}
	}
}

/*删除函数*/
/*
参数：
	x：待删除的元素
返回：
	无
说明：
	公有的封装接口
*/
void vEB::erase(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return;
	}
	// 提前检查元素是否在vEB树中，避免删除不在树中的元素，导致树结构被破坏
	// 代价是O(lglgu)
	//if (member(root, x)) {
	//	erase(root, x);
	//}
	// 这是另一种解决办法，再顶层维护一个记录元素是否存在的数组，用于常数时间检查元素是否存在
	if (isExist[x]) {
		isExist[x] = false;
		erase(root, x);
	}
	else {
		cout << "the element " << x << " have been deleted from the vEB tree!!" << endl;
	}
}
/*删除函数*/
/*
参数：
	V：待删除元素所在的结点指针
	x：待删除的元素
返回：
	无
说明：
	私有的实现函数
*/
void vEB::erase(vebNode* V, int x) {
	if (V->min == V->max) {
		V->min = vebNode::NIL;
		V->max = vebNode::NIL;
	}
	else if (V->u == 2) {
		if (x == 0) {
			V->min = 1;
		}
		else {
			V->min = 0;
		}
		V->max = V->min;
	}
	else {
		if (x == V->min) {
			int first_cluster = minimum(V->summary);
			x = V->index(first_cluster, minimum(V->cluster[first_cluster]));
			V->min = x;
		}
		erase(V->cluster[V->high(x)], V->low(x));
		if (minimum(V->cluster[V->high(x)]) == vebNode::NIL) {
			erase(V->summary, V->high(x));
			if (x == V->max) {
				int summary_max = maximum(V->summary);
				if (summary_max == vebNode::NIL) {
					V->max = V->min;
				}
				else {
					V->max = V->index(summary_max, maximum(V->cluster[summary_max]));
				}
			}
		}
		else if (x == V->max) {
			V->max = V->index(V->high(x), maximum(V->cluster[V->high(x)]));
		}
	}
}

