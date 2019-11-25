#include "Btree.h"

/*插入函数*/
/*
参数：
	k：要插入的元素
返回：
	无
说明：
	接口封装函数
	对应于书中的伪代码，在遇到2*t-1时，会提升中间元素，分裂节点
*/
template<typename T>
void Btree<T>::insert(T k) {
	auto r = root;
	if (r->n == 2 * t - 1) {
		auto s = new Node;
		root = s;
		s->leaf = false;
		s->c.push_back(r);
		splitChild(s, 0);
		insertNonFull(s, k);
	}
	else {
		insertNonFull(r, k);
	}
}

/*删除函数*/
/*
参数：
	k：要删除的元素
返回：
	无
说明：
	接口封装函数
	如果树中没有该元素，会输出一条错误信息后返回
*/
template<typename T>
void Btree<T>::erase(T k) {
	erase(root, k);
}

/*搜索函数*/
/*
参数：
	k：搜索的目标元素
返回：
	first：指向节点的指针
	second：对应下标
说明：
	接口封装函数
	如果，没有该元素，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::search(T k) {
	return search(root, k);
}

/*前驱*/
/*
参数：
	k：目标元素
返回：
	first：指向节点的指针
	second：对应下标
说明：
	接口封装函数
	如果，没有该元素的前驱，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::pred(T k) {
	return pred(root, k);
}

/*后继*/
/*
参数：
	k：目标元素
返回：
	first：指向节点的指针
	second：对应下标
说明：
	接口封装函数
	如果，没有该元素的后继，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::succ(T k) {
	return succ(root, k);
}

/*输出函数*/
/*
参数：
	无
返回：
	无
说明：
	按照类似二叉树中序遍历的方式来输出全部元素
*/

template<typename T>
void Btree<T>::print() {
	print(root);
}
/*输出函数*/
/*
参数：
	x：树的根
返回：
	无
说明：
	输出所有以x为根的树的元素，同样是类似二叉的中序遍历
*/
template<typename T>
void Btree<T>::print(Node* x) {
	if (x->c.empty()) {
		for (auto a : x->key) {
			cout << a << ' ';
		}
	}
	else {
		int i = 0;
		for (int i = 0; i < x->c.size(); ++i) {
#ifdef DEBUG
			cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
			print(x->c[i]);
			if (i < x->n) {
				cout << x->key[i] << ' ';
			}
		}
	}
}

/*删除函数的实现*/
/*
参数：
	x：待搜索的节点
	k：目标元素
返回：
	无
说明：
	这是真正执行操作的函数，私有函数
*/
template<typename T>
void Btree<T>::erase(Node* x, T k) {
	if (x->leaf) {
		for (int i = 0; i < x->n; ++i) {
			if (x->key[i] == k) {
				x->key.erase(x->key.begin() + i);
				--(x->n);
				return;
			}
		}
		cout << "there is no \'" << k <<"\' element in the tree!" << endl;
		return;
	}
	int i = 0;
	while (i < x->n && x->key[i] < k) {
		++i;
	}
	if (i < x->n && x->key[i] == k) {
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
		if (x->c[i]->n >= t) {
			auto k_tmp = erase_pred(x->c[i], k);
			x->key[i] = k_tmp;
			erase(x->c[i], k_tmp);
			return;
		}
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i + 1 << "]);" << endl;
#endif // DEBUG
		if (x->c[i+1]->n >= t) {
			auto k_tmp = erase_succ(x->c[i + 1], k);
			x->key[i] = k_tmp;
			erase(x->c[i + 1], k_tmp);
			return;
		}
		merge(x, i, x->c[i], x->c[i + 1]);
		erase(x->c[i], k);
		if (x->n == 0) {
			root = x->c[i];
#ifdef DEBUG
			cout << "DISK_WRITE(x);" << endl;
			cout << "DISK_WRITE(x->[c" << i << "]);" << endl;
#endif // DEBUG
			delete x;
		}
		return;
	}
#ifdef DEBUG
	cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
	if (x->c[i]->n >= t) {
		erase(x->c[i], k);
		return;
	}
	Node* y = NULL;
	Node* z = NULL;
	if (i < x->n) {
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i + 1 << "]);" << endl;
#endif // DEBUG
		z = x->c[i + 1];
		if (z->n >= t) {
			lShift(x, i, x->c[i], z);
			erase(x->c[i], k);
			return;
		}
	}
	if (i > 0) {
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i - 1 << "]);" << endl;
#endif // DEBUG
		y = x->c[i - 1];
		if (y->n >= t) {
			rShift(x, i - 1, y, x->c[i]);
			erase(x->c[i], k);
			return;
		}
	}
	if (i == 0) {
		merge(x, i, x->c[i], z);
	}
	else if (i == x->n) {
		merge(x, i - 1, y, x->c[i]);
		--i;
	}
	else {
		merge(x, i, x->c[i], z);
	}
	erase(x->c[i], k);
	if (x->n == 0) {
		root = x->c[i];
#ifdef DEBUG
		cout << "DISK_WRITE(x);" << endl;
		cout << "DISK_WRITE(x->[c" << i << "]);" << endl;
#endif // DEBUG
		delete x;
	}
	return;
}

/*合并操作*/
/*
参数：
	x：父节点
	i：父节点中对应的儿子的下标
	y：合并到的节点
	z：y的右兄弟节点
返回：
	无
说明：
	私有函数
	这个函数是在当x->c[i]的两个儿子元素个数都小于t时调用的合并，调用后按照[y x[i] z]的顺序合并，会delete z释放堆
*/
template<typename T>
void Btree<T>::merge(Node* x, int i, Node* y, Node* z) {
	y->key.push_back(x->key[i]);
	for (auto k_ele : z->key) {
		y->key.push_back(k_ele);
	}
	for (auto c_ele : z->c) {
		y->c.push_back(c_ele);
	}
	x->key.erase(x->key.begin() + i);
	x->c.erase(x->c.begin() + i + 1);
	y->n += z->n + 1;
	--(x->n);
#ifdef DEBUG
	cout << "DISK_WRITE(x);" << endl;
	cout << "DISK_WRITE(y);" << endl;
	cout << "DISK_WRITE(z);" << endl;
#endif // DEBUG
	delete z;
}

/*左移操作*/
/*
参数：
	x：父节点
	i：父节点中对应的儿子的下标
	l：x->c[i]的左兄弟
	r：x->c[i]的右兄弟
返回：
	无
说明：
	私有函数
	x->key[i]  =>  l->key[l->n]
	r->key[0]  =>  x->key[i]
	r->c[0]  =>  l->c[l->n+1]
*/
template<typename T>
void Btree<T>::lShift(Node* x, int i, Node* l, Node* r)	{
	l->key.push_back(x->key[i]);
	x->key[i] = r->key[0];
	if (!l->leaf) {
		l->c.push_back(r->c[0]);
		r->c.erase(r->c.begin());
	}
	++(l->n);
	r->key.erase(r->key.begin());
	--(r->n);
#ifdef DEBUG
	cout << "DISK_WRITE(x);" << endl;
	cout << "DISK_WRITE(x->[c" << i << "]);" << endl;
	cout << "DISK_WRITE(x->[c" << i + 1 << "]);" << endl;
#endif // DEBUG
}

/*右移操作*/
/*
参数：
	x：父节点
	i：父节点中对应的儿子的下标
	l：x->c[i]的左兄弟
	r：x->c[i]的右兄弟
返回：
	无
说明：
	私有函数
	x->key[i]  插入到  r->key[0]
	l->key[l->n-1]  =>  x->key[i]
	l->c[r->n]  插入到  r->c[0]
*/
template<typename T>
void Btree<T>::rShift(Node* x, int i, Node* l, Node* r) {
	r->key.insert(r->key.begin(), x->key[i]);
	if (!r->leaf) {
		r->c.insert(r->c.begin(), l->c[l->n]);
		l->c.erase(l->c.end() - 1);
	}
	++(r->n);
	x->key[i] = l->key.back();
	l->key.erase(l->key.end() - 1);
	--(l->n);
#ifdef DEBUG
	cout << "DISK_WRITE(x);" << endl;
	cout << "DISK_WRITE(x->[c" << i - 1 << "]);" << endl;
	cout << "DISK_WRITE(x->[c" << i << "]);" << endl;
#endif // DEBUG

}

/*删除中的前驱函数*/
/*
参数：
	x：前驱所在的树根节点
	k：目标元素
返回：
	k的前驱
说明：
	私有函数，处理逻辑很简单
*/
template<typename T>
T Btree<T>::erase_pred(Node* x, T k) {
	if (x->leaf) {
		return x->key[x->n - 1];
	}
	else {
#ifdef DEBUG
		cout << "DISK_READ(x->c[" << x->n << "])" << endl;
#endif // DEBUG
		return erase_pred(x->c[x->n], k);
	}
	return T();
}

/*删除中的后继函数*/
/*
参数：
	x：后所在的树根节点
	k：目标元素
返回：
	k的后继
说明：
	私有函数，处理逻辑很简单
*/
template<typename T>
T Btree<T>::erase_succ(Node* x, T k) {
	if (x->leaf) {
		return x->key[0];
	}
	else {
#ifdef DEBUG
		cout << "DISK_READ(x->c[0])" << endl;
#endif // DEBUG
		return erase_succ(x->c[0], k);
	}
	return T();
}

/*搜索函数实现函数*/
/*
参数：
	x：树的根节点
	k：目标元素
返回：
	first：指向节点的指针
	second：对应下标
说明：
	私有函数
	如果，没有该元素，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::search(Node* x, T k) {
	int i = 0;
	while (i < x->n && k > x->key[i]) {
		++i;
	}
	if (i < x->n && k == x->key[i]) {
		return { x, i };
	}
	else if (x->leaf) {
		return { NULL, -1 };
	}
	else {
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
		search(x->c[i], k);
	}
	return { NULL, -1 };
}

/*分裂节点函数*/
/*
参数：
	x：待分裂的节点的父节点
	i：待分裂的节点的下标
返回：
	无
说明：
	这个函数会将节点从中间点的位置分裂开，中间的元素提升至x，并将后半部分分裂成一个新的节点，在x中用一个指针连接
*/
template<typename T>
void Btree<T>::splitChild(Node* x, int i) {
	auto z = new Node();
#ifdef DEBUG
	cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
	auto y = x->c[i];
	z->leaf = y->leaf;
	z->n = t - 1;
	z->key.resize(t - 1);
	for (int j = 0; j < t - 1; ++j) {
		z->key[j] = (y->key[j + t]);
	}
	if (!y->leaf) {
		z->c.resize(t);
		for (int j = 0; j < t; ++j) {
			z->c[j] = (y->c[j + t]);
		}
		y->c.resize(t);
	}

	++(x->n);
	x->key.resize(x->n);
	x->c.resize(x->n + 1);

	for (int j = x->n - 1; j > i; --j) {
		x->key[j] = x->key[j - 1];
	}
	x->key[i] = y->key[t - 1];
	y->n = t - 1;
	y->key.resize(t - 1);
	for (int j = x->n; j > i + 1; --j) {
		x->c[j] = x->c[j - 1];
	}
	x->c[i + 1] = z;
#ifdef DEBUG
	cout << "DISK_WRITE(x)" << endl;
	cout << "DISK_WRITE(x->c["<< i <<"])" << endl;
	cout << "DISK_WRITE(x->c["<< i + 1 <<"])" << endl;
#endif // DEBUG
}

/*插入不满节点*/
/*
参数：
	x：待插入的树的根节点
	k：待插入的元素
返回：
	无
说明：
	用于插入不满的节点，如果节点满了会分裂
*/
template<typename T>
void Btree<T>::insertNonFull(Node* x, T k) {
	int i = x->n - 1;
	if (x->leaf) {
		x->key.resize(++(x->n));
		while (i >= 0 && k < x->key[i]) {
			x->key[i + 1] = x->key[i];
			--i;
		}
		x->key[i + 1] = k;
#ifdef DEBUG
		cout << "DISK_WRITE(x);" << endl;
#endif // DEBUG

	}
	else {
		while (i >= 0 && k < x->key[i]) {
			--i;
		}
		++i;
#ifdef DEBUG
		cout << "DISK_READ(x->[c" << i << "]);" << endl;
#endif // DEBUG
		if (x->c[i]->n == 2 * t - 1) {
			splitChild(x, i);
			if (k > x->key[i]) {
				++i;
			}
		}
		insertNonFull(x->c[i], k);
	}
}

/*前驱的实现函数*/
/*
参数：
	x：前驱所在树的根节点
	k：待求前驱的元素
返回：
	first：前驱所在的节点
	second：所在节点的下标
说明：
	私有函数
	如果，没有该元素的前驱，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::pred(Node* x, T k) {
	int i = 0;
	while (i < x->n && k > x->key[i]) {
		++i;
	}
	if (x->leaf) {
		if (i == 0) {
			return { NULL, -1 };
		}
		else {
			return { x, i - 1 };
		}
	}
	else {
		auto tmp_res = pred(x->c[i], k);
		if (!tmp_res.first) {
			if (i == 0) {
				return tmp_res;
			}
			else {
				return { x, i - 1 };
			}
		}
		else {
			return tmp_res;
		}
	}
}

/*后继的实现函数*/
/*
参数：
	x：后继所在树的根节点
	k：待求前驱的元素
返回：
	first：后继所在的节点
	second：所在节点的下标
说明：
	私有函数
	如果，没有该元素的后继，则会返回{NULL, -1}，即一个非法的位置
*/
template<typename T>
pair<typename Btree<T>::Node*, int> Btree<T>::succ(Node* x, T k) {
	int i = x->n - 1;
	while (i > -1 && x->key[i] > k) {
		--i;
	}
	if (x->leaf) {
		if (i == x->n - 1) {
			return { NULL, -1 };
		}
		else {
			return { x, i + 1 };
		}
	}
	else {
		auto res = succ(x->c[i + 1], k);
		if (!res.first) {
			if (i == x->n - 1) {
				return res;
			}
			else {
				return { x, i + 1 };
			}
		}
		else {
			return res;
		}
	}
}



