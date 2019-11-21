#pragma once
#include <utility>
using namespace std;

template<typename T>
class RB_Tree {
private:
	const enum { RED, BLACK };
	struct TreeNode {
		TreeNode* left;
		TreeNode* right;
		TreeNode* parent;
		int color;
		T val;
	};
	TreeNode* root;
	TreeNode nil;
	TreeNode* NIL;
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

	pair<TreeNode*, bool> RB_search(const T& val);
	void RB_insert(T val);
	bool RB_delete(const T& val);

	TreeNode* RB_min(TreeNode* node);
	TreeNode* RB_max(TreeNode* node);
private:
	// ??????
	void RB_search(pair<TreeNode*, bool>& ret, const T& val, TreeNode* node);
	// ???????
	void leftRotate(TreeNode* node);
	void rightRotate(TreeNode* node);
	// ??????
	void RB_insertFixUp(TreeNode* node);
	// ????
	void RB_transplant(TreeNode* u, TreeNode* v);
	// ??????
	void RB_delete(TreeNode* z);
	// ??????????
	void RB_deleteFixUp(TreeNode* x);
};

