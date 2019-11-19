#pragma once
#include <utility>
using namespace std;
struct TreeNode {
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;
	int color;
	int val;
};
class RB_Tree
{
public:
	RB_Tree() : NIL(&nil) {
		NIL->color = BLACK;
		NIL->parent = NIL;
		NIL->left = NIL;
		NIL->right = NIL;
		NIL->val = INT_MIN;
		root = NIL;
		root->parent = NIL;
	}
	~RB_Tree() = default;

	pair<TreeNode*, bool> RB_search(const int& val);
	void RB_insert(int val);
	bool RB_delete(const int& val);

	TreeNode* RB_min(TreeNode* node);
	TreeNode* RB_max(TreeNode* node);
private:
	// �������Ҳ���
	void RB_search(pair<TreeNode*, bool>& ret, const int& val, TreeNode* node);
	// ��������������
	void leftRotate(TreeNode* node);
	void rightRotate(TreeNode* node);
	// �����޸�����
	void RB_insertFixUp(TreeNode* node);
	// ��ֲ����
	void RB_transplant(TreeNode* u, TreeNode* v);
	// ����ɾ������
	void RB_delete(TreeNode* z);
	// ɾ���޸�������Բ���
	void RB_deleteFixUp(TreeNode* x);
	
private:
	const enum { RED, BLACK };
	TreeNode* root;
	TreeNode nil;
	TreeNode* NIL;
};

