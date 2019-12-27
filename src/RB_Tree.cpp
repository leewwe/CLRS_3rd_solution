#include "..\head\RB_Tree.hh"

template<typename T>
void RB_Tree<T>::leftRotate(TreeNode* x) {
	auto y = x->right;
	x->right = y->left;
	if (y->left != NIL) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == NIL) {
		root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}
template<typename T>
void RB_Tree<T>::rightRotate(TreeNode* y) {
	auto x = y->left;
	y->left = x->right;
	if (x->right != NIL) {
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == NIL) {
		root = x;
	}
	else if (y == y->parent->right) {
		y->parent->right = x;
	}
	else {
		y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}


template<typename T>
void RB_Tree<T>::RB_search(pair<TreeNode*, bool>& ret, const T& val, TreeNode* node) {
	if (node == NIL) {
		return;
	}
	if (val < node->val) {
		RB_search(ret, val, node->left);
	}
	else if (val > node->val) {
		RB_search(ret, val, node->right);
	}
	else {
		ret.first = node;
		ret.second = true;
	}
}

template<typename T>
pair<typename RB_Tree<T>::TreeNode*, bool> RB_Tree<T>::RB_search(const T& val) {
	pair<TreeNode*, bool> ret({ NIL, false });
	RB_search(ret, val, root);
	return ret;
}

template<typename T>
void RB_Tree<T>::RB_insert(T val) {
	TreeNode* y = NIL;
	auto x = root;
	while (x != NIL) {
		y = x;
		if (val < x->val) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	auto z = new TreeNode;
	z->parent = y;
	z->val = val;
	if (y == NIL) {
		root = z;
	}
	else if (z->val < y->val) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = NIL;
	z->right = NIL;
	z->color = RED;
	RB_insertFixUp(z);
}

template<typename T>
void RB_Tree<T>::RB_insertFixUp(TreeNode* z) {
	while (z->parent->color == RED) {
		if (z->parent->parent == NIL) {
			break;
		}
		if (z->parent == z->parent->parent->left) {	
			auto y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					z = z->parent;
					leftRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRotate(z->parent->parent);
			}
		}
		else {
			auto y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					rightRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				leftRotate(z->parent->parent);
			}
		}
	}
	root->color = BLACK;
}

template<typename T>
void RB_Tree<T>::RB_transplant(TreeNode* u, TreeNode* v) {
	if (u->parent == NIL) {
		root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	v->parent = u->parent;	// 这里在u不是root，v是NIL的时候，会修改NIL->parent，需要在对应的位置进行修正
}

template<typename T>
bool RB_Tree<T>::RB_delete(const T& val) {
	auto res = RB_search(val);
	if (!res.second) {
		return false;
	}
	else {
		RB_delete(res.first);
		return true;
	}
}

template<typename T>
void RB_Tree<T>::RB_delete(TreeNode* z) {
	auto y = z;
	auto yOriginalColor = y->color;
	TreeNode* x;
	if (z->left == NIL) {
		x = z->right;
		RB_transplant(z, x);
	}
	else if (z->right == NIL) {
		x = z->left;
		RB_transplant(z, x);
	}
	else {
		y = RB_min(z->right);
		yOriginalColor = y->color;
		x = y->right;
		if (y->parent == z) {
			x->parent = y;	// 当x为NIL时，这里会修改NIL->parent，需要在最后进行修正
		}
		else {
			RB_transplant(y, x);	
			y->right = z->right;
			y->right->parent = y;
		}
		RB_transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		if (yOriginalColor == BLACK) {
			RB_deleteFixUp(x);
		}
	}
	NIL->parent = NIL;	// 恢复在删除过程中被修改的NIL->parent
	delete z;
}

template<typename T>
void RB_Tree<T>::RB_deleteFixUp(TreeNode* x) {
	while (x != root && x->color == BLACK) {
		if (x == x->parent->left) {
			auto w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				leftRotate(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->color = RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;	
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(w);
				x = root;
			}
		}
		else {
			auto w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rightRotate(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

template<typename T>
typename RB_Tree<T>::TreeNode* RB_Tree<T>::RB_min(TreeNode* node) {
	while (node->left != NIL) {
		node = node->left;
	}
	return node;
}

template<typename T>
typename RB_Tree<T>::TreeNode* RB_Tree<T>::RB_max(TreeNode* node) {
	while (node != NIL) {
		node = node->right;
	}
	return node;
}
