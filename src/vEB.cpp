#include "..\head\vEB.hh"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

/*****************************************************����ຯ��*****************************************************/
int vebNode::high(int x) {
	return x / static_cast<int>(sqrt(u));
}

int vebNode::low(int x) {
	return x % static_cast<int>(sqrt(u));
}

int vebNode::index(int x, int y) {
	return x * static_cast<int>(sqrt(u)) + y;
}
/*��������*/
/*
˵����
	�ͷ�summary�����е�cluster
*/
vebNode::~vebNode() {
	delete summary;
	for (auto& c : cluster) {
		delete c;
	}
}

/*****************************************************��Ԫ����*****************************************************/
int minimum(vebNode* V) {
	return V->min;
}

int maximum(vebNode* V) {
	return V->max;
}
/*****************************************************��Ա����*****************************************************/
/*���캯��*/
/*
������
	u��ȫ���С
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
/*��������*/
/*
������
	k��ȫ��u = 2^k
���أ�
	���ָ��
˵����
	˽�к��������ڴ�������ȫ���С�Ŀյ�van Emde Boas��
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

/*���Һ�̺���*/
/*
������
	x�������ҵ�Ԫ��
���أ�
	�����̴��ڣ����غ�̵�ֵ����������ڣ�����vebNode::NIL
˵����
	���з�װ�ӿڣ������ҵ�Ԫ��x���Բ�������
*/
int vEB::successor(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return vebNode::NIL;
	}
	return successor(root, x);
}
/*���Һ�̺���*/
/*
������
	V�������ҵĽ��
	x�������ҵ�Ԫ��
���أ�
	�����̴��ڣ����غ�̵�ֵ����������ڣ�����vebNode::NIL
˵����
	˽��ʵ�ֺ����������ҵ�Ԫ��x���Բ�������
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

/*����ǰ������*/
/*
������
	x�������ҵ�Ԫ��
���أ�
	���ǰ�����ڣ�����ǰ����ֵ����������ڣ�����vebNode::NIL
˵����
	���з�װ�ӿڣ������ҵ�Ԫ��x���Բ�������
*/
int vEB::predecessor(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return vebNode::NIL;
	}
	return predecessor(root, x);
}
/*����ǰ������*/
/*
������
	V�������ҵĽ��
	x�������ҵ�Ԫ��
���أ�
	���ǰ�����ڣ�����ǰ����ֵ����������ڣ�����vebNode::NIL
˵����
	˽��ʵ�ֺ����������ҵ�Ԫ��x���Բ�������
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

/*��֤�Ƿ���ں���*/
/*
������
	x������֤��Ԫ��
���أ�
	���ڷ���true�������ڷ���false
˵����
	���з�װ�ӿ�
*/
bool vEB::member(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return false;
	}
	return member(root, x);
}
/*��֤�Ƿ���ں���*/
/*
������
	V������֤�Ľ��
	x������֤��Ԫ��
���أ�
	���ڷ���true�������ڷ���false
˵����
	˽��ʵ�ֺ���
*/
bool vEB::member(vebNode* V, int x) {
	if (x > root->u || x < 0) {
		return false;
	}
	// ��ʹ���ж�Ԫ���Ƿ���ڵ�����isExist��ʱ�临�Ӷ�O(lglgu)
	if (x == V->min || x == V->max) {
		return true;
	}
	else if (V->u == 2) {
		return false;
	}
	else {
		return member(V->cluster[V->high(x)], V->low(x));
	}
	// ʹ���ж�Ԫ���Ƿ���ڵ�����isExist��ʱ�临�Ӷ�O(1)
	//return isExist[x];
}

/*���븨������*/
/*
������
	V��������Ľ��
	x���������ֵ
���أ�
	��
˵����
	˽�У����ڴ���յĽ��
*/
void vEB::insertEmpty(vebNode* V, int x) {
	V->min = x;
	V->max = x;
}
/*���뺯��*/
/*
������
	x��Ҫ�����Ԫ�� 
���أ�
	��
˵����
	���з�װ����
*/
void vEB::insert(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return;
	}
	// ��ǰ���Ԫ���Ƿ���vEB���У���������Ѿ������е�Ԫ�أ��������ṹ���ƻ�
	// ������O(lglgu)
	//if (!member(root, x)) {
	//	insert(root, x);
	//}
	// ������һ�ֽ���취���ٶ���ά��һ����¼Ԫ���Ƿ���ڵ����飬���ڳ���ʱ����Ԫ���Ƿ����
	if (!isExist[x]) {
		isExist[x] = true;
		insert(root, x);
	}
	else {
		cout << "the element " << x << " have been inserted in the vEB tree!!" << endl;
	}
}
/*���뺯��*/
/*
������
	V��������Ľ��
	x��Ҫ�����Ԫ��
���أ�
	��
˵����
	˽��ʵ�ֺ���
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

/*ɾ������*/
/*
������
	x����ɾ����Ԫ��
���أ�
	��
˵����
	���еķ�װ�ӿ�
*/
void vEB::erase(int x) {
	if (x >= root->u || x < 0) {
		cout << "the objected element is over universe!!" << endl;
		return;
	}
	// ��ǰ���Ԫ���Ƿ���vEB���У�����ɾ���������е�Ԫ�أ��������ṹ���ƻ�
	// ������O(lglgu)
	//if (member(root, x)) {
	//	erase(root, x);
	//}
	// ������һ�ֽ���취���ٶ���ά��һ����¼Ԫ���Ƿ���ڵ����飬���ڳ���ʱ����Ԫ���Ƿ����
	if (isExist[x]) {
		isExist[x] = false;
		erase(root, x);
	}
	else {
		cout << "the element " << x << " have been deleted from the vEB tree!!" << endl;
	}
}
/*ɾ������*/
/*
������
	V����ɾ��Ԫ�����ڵĽ��ָ��
	x����ɾ����Ԫ��
���أ�
	��
˵����
	˽�е�ʵ�ֺ���
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

