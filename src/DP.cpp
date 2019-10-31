#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>
#include <fstream>


/****************************************************************************************************************/
using namespace std;

int max(int a, int b) {
	return a >= b ? a : b;
}

int memoCutRodAux(const vector<int>& p, const int& n, vector<int>& r) {
	if (r[n] >= 0) {
		return r[n];
	}
	int q = 0;
	if (n == 0) {
		q = 0;
	}
	else {
		q = INT_MIN;
		for (int i = 1; i < n + 1; ++i) {
			q = max(q, p[i] + memoCutRodAux(p, n - i, r));
		}
	}
	r[n] = q;
	return q;
}


int memoCutRod(const vector<int>& p, const int& n) {
	vector<int> r(n + 1, INT_MIN);
	return memoCutRodAux(p, n, r);
}

int bottomUpCutRod(const vector<int>& p, const int& n) {
	vector<int> r(n + 1, INT_MIN);
	r[0] = 0;
	for (int j = 1; j < n + 1; ++j) {
		int q = INT_MIN;
		for (int i = 1; i <= j; ++i) {
			q = max(q, p[i] + r[j - i]);
		}
		r[j] = q;
	}
	return r[n];
}

void externedBottomUpCutRod(vector<int> p, const int& n, vector<int>& r, vector<int>& s) {
	r[0] = 0;
	s[0] = 0;
	for (int j = 1; j < n + 1; ++j) {
		int q = INT_MIN;
		for (int i = 1; i <= j; ++i) {
			int tmp = p[i] + r[j - i];
			if (q < tmp) {
				q = tmp;
				s[j] = i;
			}
		}
		r[j] = q;
	}
}


int externedMemoCutRod(const vector<int>& p, const int& n, vector<int>& r, vector<int>& s) {
	if (r[n] >= 0) {
		return r[n];
	}
	int q = 0;
	if (n == 0) {
		q = 0;
	}
	else {
		q = INT_MIN;
		for (int i = 1; i < n + 1; ++i) {
			auto tmp = p[i] + externedMemoCutRod(p, n - i, r, s);
			if (q < tmp) {
				q = tmp;
				s[n] = i;
			}
		}
	}
	r[n] = q;
	return r[n];
}

void printCutRodSolution(vector<int> p, int n, const vector<int>& s) {
	while (n > 0) {
		cout << s[n] << ' ';
		n = n - s[n];
	}
}
/****************************************************************************************************************/

int memoDpFibonacciCalcAux(const int n, vector<int>& r) {
	int val = 0;
	if (n == 1) {
		val = 1;
	}
	else {
		val = memoDpFibonacciCalcAux(n - 1, r) + r[n - 2];
	}
	r[n] = val;
	return val;
}

int memoDpFibonacciCalc(const int n) {
	vector<int> r(n + 1, 1);
	return memoDpFibonacciCalcAux(n, r);
}

int bottomUpFibonacciCalc(const int n) {
	vector<int> r(n + 1, 1);
	r[0] = 1;
	r[1] = 1;
	int val = 0;
	for (int j = 2; j < n + 1; ++j) {
		val = r[j - 2] + r[j - 1];
		r[j] = val;
	}
	return val;
}
/****************************************************************************************************************/
void matrixChainOrder(const vector<int>& p, vector<vector<int>>& m, vector<vector<int>>& s) {
	int n = p.size() - 1;
	for (int i = 0; i < n; ++i) {
		m[i][i] = 0;
	}
	for (int l = 2; l <= n; ++l) {
		for (int i = 0; i < n - l + 1; ++i) {
			int j = i + l - 1;
			m[i][j] = INT_MAX;
			for (int k = i; k <= j - 1; ++k) {
				int q = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
				if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j - 1] = k;
				}
			}
		}
	}
}

void printOptimalParens(vector<vector<int>>& s, int i, int j) {
	if (i == j) {
		cout << "A" <<i;
	}
	else {
		cout << "(";
		printOptimalParens(s, i, s[i][j - 1]);
		printOptimalParens(s, s[i][j - 1] + 1, j);//在访问下标的时候要减1，其他的时候作为范围输入的时候，j不能减1
		cout << ")";
	}
}

/****************************************************************************************************************/

void lscLength(const vector<int>& X, const vector<int>& Y, vector<vector<int>>& c, vector<vector<int>>& b) {
	int m = X.size();
	int n = Y.size();

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (X[i] == Y[i]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = -1;
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i][j] = 1;
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i][j] = 0;
			}
		}
	}
}


template<typename T>
pair<vector<vector<int>>, vector<vector<int>>> lscLength(const vector<T>& X, const vector<T>& Y) {
	int m = X.size();
	int n = Y.size();

	vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));
	vector<vector<int>> b(m + 1, vector<int>(n + 1, 2));

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (X[i - 1] == Y[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 0;
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i][j] = -1;
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i][j] = 1;
			}
		}
	}
	return { c, b };
}




template<typename T>
void printLSC(const vector<vector<int>>& b, const vector<T>& X, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	if (b[i][j] == 0) {
		printLSC(b, X, i - 1, j - 1);
		cout << X[i - 1];
	}
	else if (b[i][j] == -1) {
		printLSC(b, X, i - 1, j);
	}
	else if(b[i][j] == 1){
		printLSC(b, X, i, j - 1);
	}
}

template<typename T>
vector<vector<int>> lscLength_optimal(const vector<T>& X, const vector<T>& Y) {
	int m = X.size();
	int n = Y.size();

	vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (X[i - 1] == Y[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
			}
			else {
				c[i][j] = c[i][j - 1];
			}
		}
	}
	return c;
}


template<typename T>
void printLSC_optimal(const vector<vector<int>>& c, const vector<T>& X, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	
	if (c[i][j] - c[i - 1][j - 1] == 1 && c[i - 1][j] == c[i][j - 1] && c[i - 1][j - 1] == c[i][j - 1]) {
		printLSC_optimal(c, X, i - 1, j - 1);
		cout << X[i - 1];
	}
	else if (c[i - 1][j] >= c[i][j - 1]) {
		printLSC_optimal(c, X, i - 1, j);
	}
	else {
		printLSC_optimal(c, X, i, j - 1);
	}
}


template<typename T>
void printLSC_optimal(const vector<vector<int>>& c, const vector<T>& X, const vector<T>& Y, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	
	if (X[i - 1] == Y[j - 1]) {
		printLSC_optimal(c, X, i - 1, j - 1);
		cout << X[i - 1];
	}
	else if (c[i - 1][j] >= c[i][j - 1]) {
		printLSC_optimal(c, X, i - 1, j);
	}
	else {
		printLSC_optimal(c, X, i, j - 1);
	}
}

// !!带备忘的最长公共子序列函数，有问题，不能填满整个c表 !!
template<typename T>
int memorizedLSCLength_optimal_aux(const vector<T>& X, const vector<T>& Y, vector<vector<int>>& c, int i, int j) {
	//if (i == 0 || j == 0) {
	//	c[i][j] = 0;
	//}
	//else if (c[i][j] > -1) {
	//	return c[i][j];
	//}
	//else if (X[i - 1] == Y[j - 1]) {
	//	c[i][j] = memorizedLSCLength_optimal_aux(X, Y, c, i - 1, j - 1) + 1;
	//}
	//else {
	//	//c[i][j] = max(memorizedLSCLength_optimal_aux(X, Y, c, i - 1, j)
	//	//	, memorizedLSCLength_optimal_aux(X, Y, c, i, j - 1));
	//	c[i][j] = max(memorizedLSCLength_optimal_aux(X, Y, c, i, j - 1)
	//		, memorizedLSCLength_optimal_aux(X, Y, c, i - 1, j));
	//}
	//return c[i][j];


	if (c[i][j] > -1) {
		return c[i][j];
	}
	if (i == 0 || j == 0) {
		c[i][j] = 0;
		return c[i][j];
	}
	if (X[i - 1] == Y[j - 1]) {
		return c[i][j] = memorizedLSCLength_optimal_aux(X, Y, c, i - 1, j - 1) + 1;
	}
	return c[i][j] = max(memorizedLSCLength_optimal_aux(X, Y, c, i, j - 1)
		, memorizedLSCLength_optimal_aux(X, Y, c, i - 1, j));
}


template<typename T>
vector<vector<int>> memorizedLSCLength_optimal(const vector<T>& X, const vector<T>& Y) {
	int m = X.size();
	int n = Y.size();

	vector<vector<int>> c(m + 1, vector<int>(n + 1, -1));
	memorizedLSCLength_optimal_aux(X, Y, c, m, n);

	return c;
}
// !! !!



// 返回最长公共子序列的长度，对空间进行优化，只使用两行
template<typename T>
int lscLength_space_optimal_first(const vector<T>& X, const vector<T>& Y) {
	// 选取较短的一行作为c的度量
	int m = 0;
	int n = 0;
	string* x_p = &X;
	string* y_p = &Y;
	if (X.size() >= Y.size()) {
		m = X.size();
		n = Y.size();
	}
	else {
		m = Y.size();
		n = X.size();
		// 要保证每次，内层循环使用的较短的那个，定义一个指针是一种写法，
		// 下面的函数传参也是一种解决办法，理论上使用指针的方法更省内存一点，因为不涉及函数栈的调用
		swap(x_p, y_p);
	}

	auto preRow_p = new vector<int>(n + 1, 0);
	auto nowRow_p = new vector<int>(n + 1, 0);

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if ((*x_p)[i - 1] == (*y_p)[j - 1]) {
				(*nowRow_p)[j] = (*preRow_p)[j - 1] + 1;
			}
			else if ((*preRow_p)[j] >= (*nowRow_p)[j - 1]) {
				(*nowRow_p)[j] = (*preRow_p)[j];
			}
			else {
				(*nowRow_p)[j] = (*nowRow_p)[j - 1];
			}
		}
		swap(preRow_p, nowRow_p);
	}
	// 注意返回的是pre，而不是now，是因为最后做了一次swap
	// 也可以将swap放在内层循环的入口，但是不好理解
	return (*preRow_p)[n];
}


// 返回最长公共子序列的长度，对空间进行优化，只使用min(m,n) + 1的内存，而且理论上tmp是存在于cpu寄存器中的
template<typename T>
void lscLength_space_optimal_aux(const vector<T>& text1, const vector<T>& text2, vector<int>& c, const int& m, const int& n) {
	for (int i = 1; i <= m; ++i) {
		int tmp = c[0];
		for (int j = 1; j <= n; ++j) {
			if (text1[i - 1] == text2[j - 1]) {
				++tmp;
				std::swap(c[j], tmp);
			}
			else {
				tmp = c[j];
				if (c[j] < c[j - 1]) {
					c[j] = c[j - 1];
				}
			}
		}
	}
}

template<typename T>
int lscLength_space_optimal(const vector<T>& X, const vector<T>& Y) {
	// 选取较短的一行作为c的度量
	int m = 0;
	int n = 0;
	vector<int> c;
	if (X.size() >= Y.size()) {
		m = X.size();
		n = Y.size();
		c.resize(n + 1, 0);
		lscLength_space_optimal_aux(X, Y, c, m, n);
	}
	else {
		m = Y.size();
		n = X.size();
		c.resize(n + 1, 0);
		lscLength_space_optimal_aux(Y, X, c, m, n);
	}
	return c[n];
}


/****************************************************************************************************************/

// 函数功能：用二分法寻找元素x在有序数组S中的位置
// 参数x：要插入有序数组S的元素
// 参数S：有序数组S
// 参数ls：有序数组S的当前的元素个数
// 返回值：x在有序数组S中的位置
int find_pos(int x, vector<int> S, int ls) {
	int left = 0;
	int right = ls;
	int rBound = right;
	while (left < right) {
		int mid = (left + right) / 2;
		if (S[mid] >= x) {
			right = mid;
		}
		else {
			left = mid + 1;
		}
	}
	if(right >= rBound){		// 检查是否越界
		return right;
	}
	else {
		if (S[right] >= x) {
			return right;
		}
		else {
			return right + 1;
		}
	}
}


// 参数x：输入序列
// 返回值：LMIS的长度，以及每个元素的e[i]
pair<int, vector<int>> LMIS(vector<int> X) {
	auto n = X.size();
	vector<int> S(n, 0);
	vector<int> e(n, 0);
	S[0] = X[0];
	e[0] = 1;

	int ls = 1;
	for (int i = 1; i < n; ++i) {
		int pos = find_pos(X[i], S, ls);	// 返回的是S中的位置
		S[pos] = X[i];
		e[i] = ++pos;						// 存储长度，并将pos转换为长度
		if (pos > ls) {
			ls = pos;
		}
	}
	return { ls,e };
}
// 函数功能：打印出最长单调递增子序列
// 参数x：输入序列
// 参数e：保存以每个元素X]结尾的最长单调递增子序列的长度e[i]
// 参数：最长单调递增子序列的长度太
void printLMIS(const vector<int>& X, const vector<int>& e, int k) {
	vector<int> Y(k, 0);	//数组Y保存LMIS中每个元素在原数组中的index
	int i = X.size() - 1;
	int j = k;
	while (j > 0) {
		if (e[i] == j) {
			Y[j - 1] = i;
			--j;
		}
		--i;
	}
	for (auto c : Y) {
		cout << X[c] << ',';
	}
	cout << endl;
}



/****************************************************************************************************************/
/* 最优二叉搜索树 */
/* 参数：
p -> 关键字的概率序列，1~n
q -> 伪关键字的概率序列，0~n
n -> 关键字节点的数目
*/
/* 返回值：
e -> e[i][j]：以节点i到节点j期望代价最优值
root -> root[i][j]：以节点i到节点j期望代价最优值的根节点的下标
*/
pair<vector<vector<double>>, vector<vector<int>>> optimalBST(const vector<double>& p, const vector<double>& q, const int n, const int rootFlag = 0) {
// 原始的伪代码改的，数组下标是对应的
	//vector<vector<double>> e(n + 2, vector<double>(n + 1, 1000.0));//初始化为无穷
	//vector<vector<int>> root(n + 2, vector<int>(n + 1, 0));
	//vector<vector<double>> w(n + 2, vector<double>(n + 1, 1000.0));//初始化为无穷
	//for (auto i = 1; i <= n + 1; ++i) {
	//	e[i][i - 1] = q[i - 1];
	//	w[i][i - 1] = q[i - 1];
	//}
	//for (int len = 1; len <= n; ++len) {
	//	for (int i = 1; i <= n - len + 1; ++i) {
	//		int j = i + len - 1;
	//		w[i][j] = w[i][j - 1] + p[j - 1] + q[j];
	//		for (int r = i; r <= j; ++r) {
	//			auto tmp = e[i][r - 1] + e[r + 1][j] + w[i][j];
	//			if (tmp < e[i][j]) {
	//				e[i][j] = tmp;
	//				root[i][j] = r;
	//			}
	//		}
	//	}
	//}

// 节省内存的代码，e不用多一行，root也不用多一行
	vector<vector<double>> e(n + 1, vector<double>(n + 1, 1000.0));//初始化为无穷
	vector<vector<double>> w(n + 1, vector<double>(n + 1, 1000.0));//初始化为无穷
	for (auto i = 0; i < n + 1; ++i) {
		e[i][i] = q[i];
		w[i][i] = q[i];
	}
	vector<vector<int>> root;
	if (!rootFlag) {
		root.resize(n, vector<int>(n, 0));				// 当输出的root辅助矩阵是从root[0...n - 1, 0...n - 1]时使用，省内存
	}
	else {
		root.resize(n + 1, vector<int>(n + 1, 0));	// 当输出的root辅助矩阵是从root[1...n, 1...n]时使用
	}

	//for (int len = 1; len <= n; ++len) {
	//	for (int i = 0; i < n - len + 1; ++i) {
	//		int j = i + len;
	//		w[i][j] = w[i][j - 1] + p[j - 1] + q[j];
	//		for (int r = i; r < j; ++r) {
	//			auto tmp = e[i][r] + e[r + 1][j] + w[i][j];
	//			if (tmp < e[i][j]) {
	//				e[i][j] = tmp;
	//				if (!rootFlag) {
	//					root[i][j - 1] = r;			// 当输出的root辅助矩阵是从root[0...n - 1, 0...n - 1]时使用，省内存
	//				}
	//				else {
	//					root[i + 1][j] = r + 1;	// 当输出的root辅助矩阵是从root[1...n, 1...n]时使用
	//				}
	//				
	//				
	//			}
	//		}
	//	}
	//}

// 进一步优化，使O(n3) -> O(n2)
	for (int len = 1; len <= n; ++len) {
		for (int i = 0; i < n - len + 1; ++i) {
			int j = i + len;
			w[i][j] = w[i][j - 1] + p[j - 1] + q[j];
			if (i < j - 1) {
				for (int r = root[i][j - 2]; r <= root[i + 1][j - 1]; ++r) {
					auto tmp = e[i][r] + e[r + 1][j] + w[i][j];
					if (tmp < e[i][j]) {
						e[i][j] = tmp;
						if (!rootFlag) {
							root[i][j - 1] = r;			// 当输出的root辅助矩阵是从root[0...n - 1, 0...n - 1]时使用，省内存
						}
						else {
							root[i + 1][j] = r + 1;	// 当输出的root辅助矩阵是从root[1...n, 1...n]时使用
						}
					}
				}
			}
			else {	// 处理对角线的情况
				e[i][j] = e[i][j - 1] + e[i + 1][j] + w[i][j];
				root[i][j - 1] = i;
			}
		}
	}

/* 测试输出w数组 */
	//for (auto c : w) {
	//	for (auto b : c) {
	//		cout << setw(6) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	return { e, root };
}



/* 输出最优搜索二叉树结构的辅助函数 */
/* 参数：
	i：起点
	j：终点
	r：根
*/
void constructOptimalBST_aux(const vector<vector<int>>& root, int i, int j, int r) {
	//if (j < i - 1 || i >= static_cast<int>(root.size()) || j >= static_cast<int>(root.size())) {
	if (j < i - 1) {
		return;
	}
	// 调整计算节点值和输出伪关键字的顺序
	// 先输出伪关键字，能保证数组下标越过数组下标之前提前返回
	if (j == i - 1) {
		if (j < r) {
			cout << 'd' << j << "是k" << r << "的左孩子" << endl;
		}
		else {
			cout << 'd' << j << "是k" << r << "的右孩子" << endl;
		}
		return;	// 遇到伪关键字要返回，不能再继续向下走
	}

	int rootNode = root[i][j];//子树根节点
	if (rootNode < r) {
		cout << "k" << rootNode << "是" << "k" << r << "的左孩子" << endl;
	}
	else {
		cout << "k" << rootNode << "是" << "k" << r << "的右孩子" << endl;
	}

	constructOptimalBST_aux(root, i, rootNode - 1, rootNode);
	constructOptimalBST_aux(root, rootNode + 1, j, rootNode);
}


/* 输出最优二叉搜索树的结构 */
/* 参数：
	root -> 构造使用的辅助数组
	i：起点
	j：终点
	rootFlag：当输入的root矩阵是省内存模式时，rootFlag = 0（默认）；当root是一般模式（书中的模式）时，rootFlag = 1
*/
void constructOptimalBST(const vector<vector<int>>& root, int i, int j, int rootFlag = 0) {
	if (i > root.size() || j > root.size()) {
		return;
	}

	int startor = INT_MIN;
	if (!rootFlag) {
		startor = root[0][root[0].size() - 1];
		--j;
	}
	else {
		startor = root[1][root[1].size() - 1];
	}

	int rootNode = root[i][j];//子树根节点
	if (rootNode == startor) {
			// 输出整个树的根
		cout << 'k' << rootNode << "是根" <<endl;
		constructOptimalBST_aux(root, i, rootNode - 1, rootNode);
		constructOptimalBST_aux(root, rootNode + 1, j, rootNode);
	}
}



/****************************************************************************************************************/
/* 最长回文序列 */
/* 
参数：
	X -> 输入序列
返回：
	p -> X[i][j]最长回文序列的长度
	b -> X[i][j]对应的最长回文序列的方向
*/
template<typename T>
pair<vector<vector<int>>, vector<vector<int>>> longestPalindrome(const T& X) {
	int n = X.size();
	vector<vector<int>> p(n + 1, vector<int>(n + 1, 1));
	vector<vector<int>> b(n + 1, vector<int>(n + 1, 0));

	for (int i = 1; i <= n - 1; ++i) {
		int j = i + 1;
		if (X[i - 1] == X[j - 1]) {
			p[i][j] = 2;
		}
		else {
			b[i][j] = 1;
		}
	}

	for (int i = n - 2; i >= 1; --i) {
		for (int j = i + 2; j >= n; --j) {
			if (X[i] == X[j]) {
				p[i][j] = p[i + 1][j - 1] + 2;
				b[i][j] = 0;
			}
			else if (p[i + 1][j] >= p[i][j - 1]) {
				p[i][j] = p[i + 1][j];
				b[i][j] = 1;
			}
			else {
				p[i][j] = p[i][j - 1];
				b[i][j] = -1;
			}
		}
	}
	return{ p,b };
}
// 上个模板的string特例化版本
template<>
pair<vector<vector<int>>, vector<vector<int>>> longestPalindrome(const string& X) {
	int n = X.size();
	vector<vector<int>> p(n + 1,vector<int>(n + 1, -1));
	vector<vector<int>> b(n + 1, vector<int>(n + 1, -2));


	// 初始化基础情况情况
	for (int i = 1; i <= n - 1; ++i) {
		p[i][i] = 1;
		int j = i + 1;
		if (X[i - 1] == X[j - 1]) {
			p[i][j] = 2;
			b[i][j] = 0;
		}
		else {
			p[i][j] = 1;
			b[i][j] = 1;
		}
	}
	p[n][n] = 1;

	for (int i = n - 2; i >= 1; --i) {
		for (int j = i + 2; j <= n; ++j) {
			if (X[i - 1] == X[j - 1]) {
				p[i][j] = p[i + 1][j - 1] + 2;
				b[i][j] = 0;
			}
			else if (p[i + 1][j] >= p[i][j - 1]) {
				p[i][j] = p[i + 1][j];
				b[i][j] = 1;
			}
			else {
				p[i][j] = p[i][j - 1];
				b[i][j] = -1;
			}
		}
	}
	return{ p,b };
}

// 数组下标的优化，没有什么实质上的改变
pair<vector<vector<int>>, vector<vector<int>>> longestPalindrome_optimal(const string& X) {
	int n = X.size();
	vector<vector<int>> p(n, vector<int>(n, -1));
	vector<vector<int>> b(n, vector<int>(n, -2));

	// 初始化数组
	for (int i = 0; i < n - 1; ++i) {
		p[i][i] = 1;
		int j = i + 1;
		if (X[i] == X[j]) {
			p[i][j] = 2;
			b[i][j] = 0;
		}
		else {
			p[i][j] = 1;
			b[i][j] = 1;
		}
	}
	p[n - 1][n - 1] = 1;


	// 计算
	for (int i = n - 3; i >= 0; --i) {
		for (int j = i + 2; j < n; ++j) {
			if (X[i] == X[j]) {
				p[i][j] = p[i + 1][j - 1] + 2;
				b[i][j] = 0;
			}
			else if (p[i + 1][j] >= p[i][j - 1]) {
				p[i][j] = p[i + 1][j];
				b[i][j] = 1;
			}
			else {
				p[i][j] = p[i][j - 1];
				b[i][j] = -1;
			}
		}
	}
	return{ p,b };
}

template<typename T>
void genLPS(const T& X, const int i, const int j, const vector<vector<int>>& b, T& S) {
	if (i > j) {
	}
	else if (i == j) {
		S.push_back(X[i - 1]);
	}
	else if (b[i][j] == 0) {
		genLPS(X, i + 1, j - 1, b, S);
		S.insert(S.cbegin(), X[i - 1]);
		S.push_back(X[i - 1]);
	}
	else if (b[i][j] == 1) {
		genLPS(X, i + 1, j, b, S);
	}
	else {
		genLPS(X, i, j - 1, b, S);
	}
}
template<>
void genLPS(const string& X, const int i, const int j, const vector<vector<int>>& b, string& S) {
	if (i > j) {
	}
	else if (i == j) {
		S += X[i - 1];
	}
	else if (b[i][j] == 0) {
		genLPS(X, i + 1, j - 1, b, S);
		S = X[i - 1] + S + X[i - 1];
	}
	else if (b[i][j] == 1) {
		genLPS(X, i + 1, j, b, S);
	}
	else {
		genLPS(X, i, j - 1, b, S);
	}
}

void genLPS_optimal(const string& X, const int i, const int j, const vector<vector<int>>& b, string& S) {
	if (i > j) {
	}
	else if (i == j) {
		S += X[i];
	}
	else if (b[i][j] == 0) {
		genLPS_optimal(X, i + 1, j - 1, b, S);
		S = X[i] + S + X[i];
	}
	else if (b[i][j] == 1) {
		genLPS_optimal(X, i + 1, j, b, S);
	}
	else {
		genLPS_optimal(X, i, j - 1, b, S);
	}
}

/* 以下几个函数只能返回最终的长度，不能返回用于指导输出的辅助矩阵 */
// leetcode中文论坛的解法，使用了矩阵，没有基础情况的初始
    // int longestPalindrome(const string& s) {
    //     int n = s.size();
    //     vector<vector<int>> p(n, vector<int>(n, 0));
    //     // 计算
    //     for (int i = n - 1; i >= 0; --i) {
    //         p[i][i] = 1;
    //         for (int j = i + 1; j < n; ++j) {
    //             if (s[i] == s[j]) {
    //                 p[i][j] = p[i + 1][j - 1] + 2;
    //             }
    //             else if (p[i + 1][j] >= p[i][j - 1]) {
    //                 p[i][j] = p[i + 1][j];
    //             }
    //             else {
    //                 p[i][j] = p[i][j - 1];
    //             }
    //         }
    //     }

    //     return p[0][n - 1];
    // }

// 优化过的解法，减少空间使用，只是用两行的写法
    // int longestPalindrome_space_optimal_twoLines(const string& s) {
    //     int n = s.size();
    //     // 使用两个指针
    //     auto preRow_p = new vector<int>(n, 0);
    //     auto nowRow_p = new vector<int>(n, 0);

    //     for (int i = n - 1; i >= 0; --i) {
    //         (*nowRow_p)[i] = 1;
    //         for (int j = i + 1; j < n; ++j) {
    //             if (s[i] == s[j]) {
    //                 (*nowRow_p)[j] = (*preRow_p)[j - 1] + 2;
    //             }
    //             else if ((*preRow_p)[j] >= (*nowRow_p)[j - 1]) {
    //                 (*nowRow_p)[j] = (*preRow_p)[j];
    //             }
    //             else {
    //                 (*nowRow_p)[j] = (*nowRow_p)[j - 1];
    //             }
    //         }
    //         // 交换指针而不是交换整个vector
    //         swap(preRow_p, nowRow_p);
    //     }
    //     // 返回的时候注意，返回的是pre而不是now，因为最后做了一次交换
    //     return (*preRow_p)[n - 1];
    // }

// 只是用一行的写法，最终优化版本
    int longestPalindrome_space_optimal_final(const string& s) {
        int n = s.size();
        vector<int> p(n, 0);

        // 计算
        int tmp = 0;
        for (int i = n - 1; i >= 0; --i) {
            p[i] = 1;
            int p_tmp = 0;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) {
                    int tmp = p_tmp;
                    p_tmp = p[j];
                    p[j] = tmp + 2;
                }
                else if (p[j] < p[j - 1]) {
                    p_tmp = p[j];
                    p[j] = p[j - 1];
                }
                else {
                    p_tmp = p[j];
                }
            }
        }
        return p[n - 1];
    }

/****************************************************************************************************************/

//函数中使用的相关类型模板
template<typename T>
using coordi = pair<T, T>;
template<typename T>
using martrix = vector<vector<T>>;

/* 计算|xy| */
/*
参数：
	x：x的坐标
	y：y的坐标
返回：
	距离
*/
template<typename I, typename O>
O length(const coordi<I>& x, const coordi<I>& y) {
	O ret;
	ret = pow(x.first - y.first, 2) + pow(x.second - y.second, 2);
	ret = sqrt(ret);
	return ret;
}

/* 计算并构造双调欧几里得旅行商问题 */
/*
参数
	p：按x坐标排好序的坐标序列
返回
	first：P：距离矩阵
	second：C：坐标矩阵
*/
template<typename P, typename C>
pair<martrix<P>, martrix<C>> EuclideanTSP(const vector<coordi<P>>& p) {
// 原版程序：输出下标是[1 , n]对应的
	//int n = p.size();
	//martrix<P> b(n + 1,vector<double>(n + 1, 0));
	//martrix<C> r(n, vector<C>(n + 1, 0));
	// /* 对p按x坐标排序 */
	//b[1][1] = 0;
	//b[1][2] = length<P, P>(p[0], p[1]);
	//b[2][2] = b[1][2];
	//for (int j = 3; j <= n; ++j) {			// 从3开始计算是因为，至少需要从p1开始的两点来计算，而前两点可以直接计算
	//	for (int i = 1; i < j - 1; ++i) {	// ①计算i < j - 1
	//		b[i][j] = b[i][j - 1] + length<P, P>(p[j - 2], p[j - 1]);
	//		r[i][j] = j - 1;
	//	}
	//	b[j - 1][j] = 10000.0;
	//	for (int k = 1; k < j - 1; ++k)	{	// ②计算i = j - 1
	//		auto q = b[k][j - 1] + length<P, P>(p[k - 1], p[j - 1]);
	//		if (q < b[j - 1][j]) {
	//			b[j - 1][j] = q;
	//			r[j - 1][j] = k;
	//		}
	//	}
	//	b[j][j] = b[j - 1][j] + length<P, P>(p[j - 2], p[j - 1]);
	//}
	//
	//return{ b,r };

// 省内存版：输出下标为[0, n - 1]
	//int n = p.size();
	//martrix<P> b(n, vector<double>(n, 0));
	//martrix<C> r(n - 1, vector<C>(n, 0));
	//// 计算初始的几个坐标
	//b[0][0] = 0;
	//b[0][1] = length<P, P>(p[0], p[1]);
	//b[1][1] = b[0][1];
	//for (int j = 2; j < n; ++j) {			// 从3开始计算是因为，至少需要从p1开始的两点来计算，而前两点可以直接计算
	//	for (int i = 0; i < j - 1; ++i) {	// ①计算i < j - 1
	//		b[i][j] = b[i][j - 1] + length<P, P>(p[j - 1], p[j]);
	//		r[i][j] = j - 1;
	//	}
	//	b[j - 1][j] = 10000.0;
	//	for (int k = 0; k < j - 1; ++k)	{	// ②计算i = j - 1
	//		auto q = b[k][j - 1] + length<P, P>(p[k], p[j]);
	//		if (q < b[j - 1][j]) {
	//			b[j - 1][j] = q;
	//			r[j - 1][j] = k;
	//		}
	//	}
	//	b[j][j] = b[j - 1][j] + length<P, P>(p[j - 1], p[j]);
	//}
	//return{ b,r };

// 进一步减少r对于内存使用的版本
	int n = p.size();
	martrix<P> b(n, vector<double>(n, 0));
	martrix<C> r(n - 1, vector<C>(n - 1, 0));
	/* 对p按x坐标排序 */
	/* 计算初始的几个坐标 */
	b[0][0] = 0;
	b[0][1] = length<P, P>(p[0], p[1]);
	b[1][1] = b[0][1];
	for (int j = 2; j < n; ++j) {			// 从3开始计算是因为，至少需要从p1开始的两点来计算，而前两点可以直接计算
		for (int i = 0; i < j - 1; ++i) {	// ①计算i < j - 1
			b[i][j] = b[i][j - 1] + length<P, P>(p[j - 1], p[j]);
			r[i][j - 1] = j - 1;
		}
		b[j - 1][j] = 10000.0;
		for (int k = 0; k < j - 1; ++k) {	// ②计算i = j - 1
			auto q = b[k][j - 1] + length<P, P>(p[k], p[j]);
			if (q < b[j - 1][j]) {
				b[j - 1][j] = q;
				r[j - 1][j - 1] = k;
			}
		}
		b[j][j] = b[j - 1][j] + length<P, P>(p[j - 1], p[j]);
	}

	return{ b,r };
}

/* 构造双调欧几里得旅行商连线图的辅助函数 */
/*
参数
	p：按x坐标排好序的坐标序列
返回
	无
输出
	按x坐标排好序的坐标序列中的点的连线方式
*/
template<typename C>
void printPath(const martrix<C>& r, const int i, const int j) {
// 对应于下标[1，n]
	//if (i < j) {
	//	auto k = r[i][j];
	//	if (k != i) {
	//		cout << 'p' << k << endl;
	//	}
	//	if (k > 1) {
	//		printPath(r, i, k);
	//	}
	//}
	//else {
	//	auto k = r[j][i];
	//	if (k > 1) {
	//		printPath(r, k, j);
	//		cout << 'p' << k << endl;
	//	}
	//}

// 对应于下标[0，n-1]
	//if (i < j) {
	//	auto k = r[i][j];
	//	if (k != i) {
	//		cout << 'p' << k << endl;
	//	}
	//	if (k > 0) {
	//		printPath(r, i, k);
	//	}
	//}
	//else {
	//	auto k = r[j][i];
	//	if (k > 0) {
	//		printPath(r, k, j);
	//		cout << 'p' << k << endl;
	//	}
	//}

// 进一步减少r对内存的使用的版本
	if (i < j) {
		auto k = r[i][j - 1];
		if (k != i) {
			cout << 'p' << k << endl;
		}
		if (k > 0) {
			printPath(r, i, k);
		}
	}
	else {
		auto k = r[j][i - 1];
		if (k > 0) {
			printPath(r, k, j);
			cout << 'p' << k << endl;
		}
	}
}

/* 构造双调欧几里得旅行商连线图的封装函数 */
/*
参数
	p：按x坐标排好序的坐标序列
返回
	无
输出
	按x坐标排好序的坐标序列中的点的连线方式
*/
template<typename C>
void printTour(const martrix<C>& r, const int n) {
// 对应于下标[1，n]
	//cout << 'p' << n << endl;
	//cout << 'p' << n - 1 << endl;
	//auto k = r[n - 1][n];
	//printPath(r, k, n - 1);
	//cout << 'p' << k << endl;

// 对应于下标[0, n - 1]
	//cout << 'p' << n - 1 << endl;
	//cout << 'p' << n - 2 << endl;
	//auto k = r[n - 2][n - 1];
	//printPath(r, k, n - 2);
	//cout << 'p' << k << endl;

// 进一步减少r对于内存的使用的版本
	cout << 'p' << n - 1 << endl;
	cout << 'p' << n - 2 << endl;
	auto k = r[n - 2][n - 2];
	printPath(r, k, n - 2);
	cout << 'p' << k << endl;
}


/****************************************************************************************************************/

#include <fstream>

/* 获取单词表的函数 */
bool GetWord(const string& inFileName, int lineCapacity,
	vector<string>& wordTable, vector<int>& wordLenTable)
{
	ifstream inFile(inFileName);
	string word;

	while (inFile >> word)
	{
		if (static_cast<int>(word.size()) > lineCapacity)
		{
			inFile.close();
			return false;
		}

		wordTable.push_back(word);
		wordLenTable.push_back(word.size());
	}

	inFile.close();
	return true;
}

/* problem 15-4 整齐打印 */
/* 版本1：答案中的基本解法
参数：
	l：单词长度表
	M：每行允许的长度
返回：
	c：最终的消耗序列
	p：辅助矩阵，存储每行的起始位置，在打印的时候需要从后向前递推
*/
pair<vector<int>, vector<int>> printNeatly(vector<int> l, const int M) {
	int n = l.size();
	vector<vector<int>> extra(n + 1, vector<int>(n + 1, 0));
	vector<vector<int>> lc(n + 1, vector<int>(n + 1, 0));
	vector<int> c(n + 1, 0);
	vector<int> p(n + 1, 0);

	for (int i = 1; i <= n; ++i) {
		extra[i][i] = M - l[i - 1];
		for (int j = i + 1; j <= n; ++j) {
			extra[i][j] = extra[i][j - 1] - l[j - 1] - 1;
		}
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = i; j <= n; ++j) {
			if (extra[i][j] < 0) {
				lc[i][j] = INT_MAX;
			}
			else if (j == n && extra[i][j] >= 0) {
				lc[i][j] = 0;
			}
			else {
				lc[i][j] = pow(extra[i][j], 3);
			}
		}
	}

	c[0] = 0;
	for (int j = 1; j <= n; ++j) {
		c[j] = INT_MAX;
		for (int i = 1; i <= j; ++i) {
			if (lc[i][j] == INT_MAX) {
				continue;
			}
			int tmp = c[i - 1] + lc[i][j];
			
			/*if (tmp < 0) {
				continue;
			}*/
			if (tmp < c[j]) {
				c[j] = tmp;
				p[j] = i;
			}
		}
	}


	//ofstream file("out.txt");
	//for (auto a : lc) {
	//	for (auto b : a) {
	//		file << setw(20) << b << ' ';
	//	}
	//	file << endl;
	//}
	//file.close();

	return { c,p };
}

/* 空间复杂度优化第1版,参考https://blog.csdn.net/yangtzhou的程序
主要降低了lc的存储，不需要存储那么多的INT_MAX，这样同时使最后一个循环的访问次数降低，降低了时间复杂度
空间复杂度是不变的，因为我们还存储这extra，这个的空间复杂度为O(n^2),只是将lc的使用空间降到了O(nM)
参数：
	l：单词长度表
	M：每行允许的长度
返回：
	c：最终的消耗序列
	p：辅助矩阵，存储每行的起始位置，在打印的时候需要从后向前递推
*/

pair<vector<int>, vector<int>> printNeatlyOptimal_first(vector<int> l, const int M) {
	int n = l.size();
	vector<vector<int>> extra(n + 1, vector<int>(n + 1, 0));
	vector<vector<int>> lc(n + 1, vector<int>());
	vector<int> c(n + 1, 0);
	vector<int> p(n + 1, 0);

	for (int i = 1; i <= n; ++i) {
		extra[i][i] = M - l[i - 1];
		for (int j = i + 1; j <= n; ++j) {
			extra[i][j] = extra[i][j - 1] - l[j - 1] - 1;
		}
	}

	// j -> 第j个单词作为结尾
	for (int j = 1; j <= n; ++j) {
		// 这里要倒着输出，即这一行最多存入多少个单词，且该单词是以第j个单词为结尾的
		// i越小，表示存入的单词越多，但是最多存入j个单词
		for (int i = j; i >= 1; --i) {
			if (extra[i][j] < 0) {
				break;
			}
			else if (j == n) {
				lc[j].push_back(0);
			}
			else {
				lc[j].push_back(pow(extra[i][j], 3));
			}
		}
	}

	// 设置一个初始的值，方便计算，减少控制逻辑
	c[0] = 0;
	for (int j = 1; j <= n; ++j) {
		//p[j] = j;
		int i = j;
		c[j] = INT_MAX;
		for (auto elem : lc[j]) {
			// 这里不要使用i--，因为后边的if中还要使用i
			int curCost = elem + c[i - 1];
			if (curCost < c[j])
			{
				c[j] = curCost;
				// 这里也不要使用i--，因为if可能不执行
				p[j] = i;
			}
			--i;
		}
	}

	return { c,p };
}

/* 空间复杂度优化第2版,参考https://blog.csdn.net/yangtzhou的程序
在first的基础上进一步降低存储，将extra删去了，这就是https://blog.csdn.net/yangtzhou中的程序
这样同时使最后一个循环的访问次数降低，降低了时间复杂度
空间复杂度降到了O(nM)
参数：
	l：单词长度表
	M：每行允许的长度
返回：
	c：最终的消耗序列
	p：辅助矩阵，存储每行的起始位置，在打印的时候需要从后向前递推
*/
pair<vector<int>, vector<int>> printNeatlyOptimal_second(vector<int> l, const int M) {
	int n = l.size();
	//vector<vector<int>> extra(n + 1, vector<int>(n + 1, 0));
	vector<vector<int>> lc(n + 1, vector<int>());
	vector<int> c(n + 1, 0);
	vector<int> p(n + 1, 0);

	// j -> 第j个单词作为结尾
	for (int j = 1; j <= n; ++j) {
		int lineExtra = M;
		// 这里要倒着输出，即这一行最多存入多少个单词，且该单词是以第j个单词为结尾的
		// i越小，表示存入的单词越多，但是最多存入j个单词
		for (int i = j; i >= 1; --i) {
			lineExtra -= l[i - 1];

			// 这里是减去单词与单词之间的空格
			if (i != j) {
				lineExtra -= 1;
			}

			if (lineExtra < 0) {
				break;
			}
			else if (j == n) {
				lc[j].push_back(0);
			}
			else {
				lc[j].push_back(pow(lineExtra, 3));
			}
		}
	}

	// 设置一个初始的值，方便计算，减少控制逻辑
	c[0] = 0;
	for (int j = 1; j <= n; ++j) {
		//p[j] = j;
		int i = j;
		c[j] = INT_MAX;
		for (auto elem : lc[j]) {
			// 这里不要使用i--，因为后边的if中还要使用i
			int curCost = elem + c[i - 1];
			if (curCost < c[j])
			{
				c[j] = curCost;
				// 这里也不要使用i--，因为if可能不执行
				p[j] = i;
			}
			--i;
		}
	}

	return { c,p };
}

/* 空间复杂度优化第3版，答案的建议
这次对于extra和lc我们都是现场计算
进一步降低空间复杂度，降到了O(n)
参数：
	l：单词长度表
	M：每行允许的长度
返回：
	c：最终的消耗序列
	p：辅助矩阵，存储每行的起始位置，在打印的时候需要从后向前递推
*/
pair<vector<int>, vector<int>> printNeatlyOptimal_third(vector<int> l, const int M) {
	int n = l.size();
	vector<int> c(n + 1, 0);
	vector<int> p(n + 1, 0);

	// 设置一个初始的值，方便计算，减少控制逻辑
	c[0] = 0;
	for (int j = 1; j <= n; ++j) {
		//int i = j;
		c[j] = INT_MAX;
		// 初始的extra值为M
		int extra = M;
		for (int i = j; i >= 1; --i) {
			
			extra = extra - l[i - 1];
			if (i != j) {
				--extra;
			}
			// 这里再extra < 0后break同样可以减少内层循环的迭代次数
			if (extra < 0) {
				break;
			}
			int lc = INT_MAX;
			// 由于不存在了extra < 0的情况了，所以直接就不存在∞的情况了
			if (j == n) {
				lc = 0;
			}
			else {
				lc = pow(extra, 3);
			}

			// 这里不要使用i--，因为后边的if中还要使用i
			int curCost = lc + c[i - 1];

			if (curCost < c[j])
			{
				c[j] = curCost;
				// 这里也不要使用i--，因为if可能不执行
				p[j] = i;
			}
			//--i;
		}
	}

	return { c,p };
}




/* 打印出输出的格式 */
/*
参数：
	p：上一步计算出的用来指导输出的数组
	j：终点坐标
返回：
	行数k
*/
/* 说明：
	输出的格式为：k i j
	表示当前行中输出原来单词序列中的第i->j个单词
*/

int printLines(const vector<int>& p, int j) {
	int i = p[j];
	int k = 1;
	if (i == 1) {
		k = 1;
	}
	else {
		k = printLines(p, i - 1) + 1;
	}
	
	cout << setw(2) << k << ' '
		<< setw(2) << i << ' '
		<< setw(2) << j << ' '
		<< endl;

	return k;
}

/* 打印输出 */
/*
参数：
	wordTab：单词序列
	wordLenTab：单词长度序列
	p：上一步计算出的用来指导输出的数组
	j：最后行的结尾坐标，用来计算最后一行的起点i
	M：每行的长度
	filler：每行末尾的填充字符，默认为空格
返回：无
*/
/* 说明：
	输出排好序的结果，并将每行末尾的空格输出为填充字符，填充字符为空格	
*/
void printLines(const vector<string>& wordTab, const vector<int>& wordLenTab, 
	const vector<int>& p, const int j, 
	const int& M, const int& filler = ' ') {
	

	int i = p[j];
	if (i != 1) {
		printLines(wordTab, wordLenTab, p, i - 1, M, filler);
	}

	int len = 0;
	for (int k = i; k < j; ++k) {
		cout << wordTab[k - 1] << ' ';
		len += wordLenTab[k - 1] + 1;
	}
	cout << wordTab[j - 1];
	len += wordLenTab[j - 1];
	for (int k = len; k < M; ++k) {
		cout << static_cast<char>(filler);
	}
	cout << endl;
}

/****************************************************************************************************************/

enum editFlag{
	NONE = -1,
	COPY = 0,
	REPLACE,
	DELETE,
	INSERT,
	TWIDDLE,
	KILL
};



/* 计算编辑距离，problem 15-5 */
/*
参数：
	x：原始的字符串
	y：目标字符串
	e：每步操作的代价数组
返回：
	c[i][j]：从x[1…i]到y[1…j]的代价
	op[i][j]：从x[1…i]到y[1…j]计算出c[i][j]的最后一步操作
说明：
	计算出编辑距离的矩阵，同时返回用于重构输出编辑操作的序列的辅助矩阵op
*/
pair<vector<vector<double>>, vector<vector<int>>>
editDistance(const string& x, const string& y, const vector<double>& e) {
	int m = x.size();
	int n = y.size();

	vector<vector<double>> c(m + 1, vector<double>(n + 1, 0));
	vector<vector<int>> op(m + 1, vector<int>(n + 1, 0));

	for (int i = 0; i <= m; ++i) {
		c[i][0] = i * e[editFlag::DELETE];
		op[i][0] = editFlag::DELETE;
	}
	for (int j = 0; j <= n; ++j) {
		c[0][j] = j * e[INSERT];
		op[0][j] = INSERT;
	}


	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			c[i][j] = 1000.0;
			// copy
			double cost = c[i - 1][j - 1] + e[COPY];
			if (x[i - 1] == y[j - 1]) {
				c[i][j] = cost;
				op[i][j] = COPY;
			}

			// replace
			cost = c[i - 1][j - 1] + e[REPLACE];
			if (x[i - 1] != y[j - 1] && cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = REPLACE;
			}

			//// twiddle
			//if (i >= 2 && j >= 2
			//	&& x[i - 1] == y[j - 2] && x[i - 2] == y[j - 1]
			//	&& (cost = c[i - 2][j - 2] + e[TWIDDLE]) < c[i][j]) {
			//	c[i][j] = cost;
			//	op[i][j] = TWIDDLE;
			//}

			// delete
			cost = c[i - 1][j] + e[DELETE];
			if (cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = DELETE;
			}

			// insert
			cost = c[i][j - 1] + e[INSERT];
			if (cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = INSERT, y[j];
			}
		}
	}

	// kill：需要记录i值
	//int end_i = 0;
	//for (int i = 0; i <= m - 1; ++i) {
	//	double cost = c[i][n] + e[KILL];
	//	if (cost < c[m][n]) {
	//		c[m][n] = cost;
	//		op[m][n] = KILL;
	//		end_i = i;
	//	}
	//}
	
	return { c, op };
}


/* opSequence的打印函数 */
/*
参数：
	opID：操作ID号
返回：
	无
说明：辅助函数
*/
void opSequence_print(const int& opID) {
	switch (opID) {
		case COPY:
			cout << "COPY" << endl;
			break;
		case DELETE:
			cout << "DELETE" << endl;
			break;
		case INSERT:
			cout << "INSERT" << endl;
			break;
		case KILL:
			cout << "KILL" << endl;
			break;
		case REPLACE:
			cout << "REPLACE" << endl;
			break;
		case TWIDDLE:
			cout << "TWIDDLE" << endl;
			break;
		default:
			break;
	}
}
/* 输出操作序列函数 */
/*
参数：
	op：上一步计算出的操作矩阵
返回：
	无
说明：根据操作矩阵指导输出操作序列
*/
void opSequence(const vector<vector<int>>& op, int i, int j) {
	if (i == 0 && j == 0) {
		return;
	}
	auto i_buf = i;
	auto j_buf = j;
	auto opID = op[i][j];
	if (opID == COPY || opID == REPLACE) {
		--i;
		--j;
	}
	else if (opID == TWIDDLE) {
		i -= 2;
		j -= 2;
	}
	else if (opID == DELETE) {
		--i;
	}
	else if (opID == INSERT) {
		--j;
	}
	else {
		i = op[i][j];
	}
	opSequence(op, i, j);
	opSequence_print(opID);
}



/* 以下三个程序式leetcode上的程序和优化，在没有书中的程序那么复杂，只涉及了四种操作
插入 删除 替换 还有复制，
复制在题中没有明确说明，但是是存在的，就是不作任何操作，所以可以理解为跳过（skip），这也是书中的题和leetcode中的题的微妙区别
另外值得学习的是，leetcode论坛中有个解法求最小值的方法逻辑较为简单，而且当涉及到很多的分支的时候，需要考虑比较次数，可以采用CLRS第9章第1节中探讨的最大值和最小值的求法问题
 */
/* CLRS原版程序，需要使用矩阵，O(mn)
int minDistance(const string& word1, const string& word2) {
    int m = word1.size();
    int n = word2.size();

    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) {
        c[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        c[0][j] = j;
    }


    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            c[i][j] = INT_MAX;

            // copy
            int cost = c[i - 1][j - 1];
            if (word1[i - 1] == word2[j - 1]) {
                c[i][j] = cost;
            }

            // replace
            cost = c[i - 1][j - 1] + 1;
            if (word1[i - 1] != word2[j - 1] && cost < c[i][j]) {
                c[i][j] = cost;
            }

            // delete
            cost = c[i - 1][j] + 1;
            if (cost < c[i][j]) {
                c[i][j] = cost;
            }

            // insert
            cost = c[i][j - 1] + 1;
            if (cost < c[i][j]) {
                c[i][j] = cost;
            }
        }
    }
    return c[m][n];
} */

// 逻辑简单版，原理和原版的程序一样，都需要使用矩阵，O(mn)
/* int minDistance(const string& word1, const string& word2) {
    int m = word1.size();
    int n = word2.size();

    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) {
        c[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        c[0][j] = j;
    }


    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            c[i][j] = INT_MAX;

            // copy/skip
            if (word1[i - 1] == word2[j - 1]) {
                c[i][j] = c[i - 1][j - 1];
                continue;
            }
            // 其余三种情况 替换 删除 插入
            else {
                c[i][j] = min(c[i - 1][j - 1] + 1, min(c[i - 1][j] + 1, c[i][j - 1] + 1));
            }
        }
    }
    return c[m][n];
} */

// 优化空间复杂度，只是用一行就可以O(min(m,n)))
int minDistance(const string& word1, const string& word2) {
	auto p1 = &word1;
	auto p2 = &word2;

	if (word1.size() < word2.size()) {
		swap(p1, p2);
	}

	vector<int> c(p2->size() + 1, 0);

	for (int j = 0; j <= p2->size(); ++j) {
		c[j] = j;
	}

	for (int i = 1; i <= p1->size(); ++i) {
		c[0] = i;
		int tmp = i - 1;
		for (int j = 1; j <= p2->size(); ++j) {			
			// copy/skip
			int c_j = c[j];
			if ((*p1)[i - 1] == (*p2)[j - 1]) {
				c[j] = tmp;
			}
			// 其余三种情况 替换 删除 插入
			else {
				c[j] = min(tmp + 1, min(c[j] + 1, c[j - 1] + 1));
			}
			tmp = c_j;
		}
	}
	return c.back();
}

/* DNA对齐函数 */
// 版本1，minDistance的修改版，将对应的操作直接写死在函数内部
int DNAAlign(const string& word1, const string& word2) {
	auto p1 = &word1;
	auto p2 = &word2;

	if (word1.size() < word2.size()) {
		swap(p1, p2);
	}

	vector<int> c(p2->size() + 1, 0);

	for (int j = 0; j <= p2->size(); ++j) {
		c[j] = j * 2;
	}

	for (int i = 1; i <= p1->size(); ++i) {
		c[0] = i * 2;
		int tmp = (i - 1) * 2;
		for (int j = 1; j <= p2->size(); ++j) {
			// copy/skip
			int c_j = c[j];
			if ((*p1)[i - 1] == (*p2)[j - 1]) {
				c[j] = tmp - 1;
			}
			// 其余三种情况 替换 删除 插入
			else {
				c[j] = min(tmp + 1, min(c[j] + 2, c[j - 1] + 1));
			}
			tmp = c_j;
		}
	}
	return c.back();

	//int m = word1.size();
	//int n = word2.size();

	//vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

	//for (int i = 0; i <= m; ++i) {
	//	c[i][0] = i * 2;
	//}
	//for (int j = 0; j <= n; ++j) {
	//	c[0][j] = j * 2;
	//}


	//for (int i = 1; i <= m; ++i) {
	//	for (int j = 1; j <= n; ++j) {
	//		c[i][j] = INT_MAX;

	//		// copy
	//		int cost = c[i - 1][j - 1] - 1;
	//		if (word1[i - 1] == word2[j - 1]) {
	//			c[i][j] = cost;
	//		}

	//		// replace
	//		cost = c[i - 1][j - 1] + 1;
	//		if (word1[i - 1] != word2[j - 1] && cost < c[i][j]) {
	//			c[i][j] = cost;
	//		}

	//		// delete
	//		cost = c[i - 1][j] + 2;
	//		if (cost < c[i][j]) {
	//			c[i][j] = cost;
	//		}

	//		// insert
	//		cost = c[i][j - 1] + 2;
	//		if (cost < c[i][j]) {
	//			c[i][j] = cost;
	//		}
	//	}
	//}
	//return c[m][n];
}


// editDistance的修改版，将twiddle和kill删除
// 注意返回的是负值
pair<vector<vector<double>>, vector<vector<int>>>
DNAAlign(const string& x, const string& y, const vector<double>& e) {
	int m = x.size();
	int n = y.size();

	vector<vector<double>> c(m + 1, vector<double>(n + 1, 0));
	vector<vector<int>> op(m + 1, vector<int>(n + 1, 0));

	for (int i = 0; i <= m; ++i) {
		c[i][0] = i * e[editFlag::DELETE];
		op[i][0] = editFlag::DELETE;
	}
	for (int j = 0; j <= n; ++j) {
		c[0][j] = j * e[INSERT];
		op[0][j] = INSERT;
	}

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			c[i][j] = 1000.0;
			// copy
			double cost = c[i - 1][j - 1] + e[COPY];
			if (x[i - 1] == y[j - 1]) {
				c[i][j] = cost;
				op[i][j] = COPY;
			}

			// replace
			cost = c[i - 1][j - 1] + e[REPLACE];
			if (x[i - 1] != y[j - 1] && cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = REPLACE;
			}

			// delete
			cost = c[i - 1][j] + e[DELETE];
			if (cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = DELETE;
			}

			// insert
			cost = c[i][j - 1] + e[INSERT];
			if (cost < c[i][j]) {
				c[i][j] = cost;
				op[i][j] = INSERT, y[j];
			}
		}
	}

	ofstream out("c_out.txt");
	for (auto a : c) {
		for (auto b : a) {
			out << setw(2) << b << ' ';
		}
		out << endl;
	}
	out.close();

	return { c, op };
}

// 上一步的空间优化，只返回最后对齐后的结果，不会返回整个矩阵，同时会返回用于指导输出的op矩阵
pair<int, vector<vector<int>>>
DNAAlign_optimal(const string& x, const string& y, const vector<double>& e) {
	int m = x.size();
	int n = y.size();

	vector<int> c(n + 1, 0);
	vector<vector<int>> op(m + 1, vector<int>(n + 1, 0));

	for (int i = 0; i <= m; ++i) {
		op[i][0] = editFlag::DELETE;
	}
	for (int j = 0; j <= n; ++j) {
		c[j] = j * e[INSERT];
		op[0][j] = INSERT;
	}

	for (int i = 1; i <= m; ++i) {
		c[0] = i * e[DELETE];
		double tmp = (i - 1) * e[DELETE];
		for (int j = 1; j <= n; ++j) {
			// copy
			double cost = tmp + e[COPY];
			double c_j = c[j];
			c[j] = 1e5;
			if (x[i - 1] == y[j - 1]) {
				c[j] = cost;
				op[i][j] = COPY;	
			}
			// replace
			cost = tmp + e[REPLACE];
			if (x[i - 1] != y[j - 1] && cost < c[j]) {
				c[j] = cost;
				op[i][j] = REPLACE;
			}
			// delete
			//cost = c[j] + e[DELETE];
			cost = c_j + e[DELETE];
			if (cost < c[j]) {
				c[j] = cost;
				op[i][j] = DELETE;
			}
			// insert
			cost = c[j - 1] + e[INSERT];
			if (cost < c[j]) {
				c[j] = cost;
				op[i][j] = INSERT;
			}
			tmp = c_j;
		}
	}

	return { -c.back(), op };
}

/*打印DNA对齐后的两个DNA字符串*/
// 辅助打印函数
void DNAAlignPrint_aux(const vector<int>& opSeq, const string& x, const string& y) {
	int i = 0;
	int j = 0;
	for (auto opID : opSeq) {
		switch (opID) {
		case COPY:
		case REPLACE:
			cout << x[i++];
			break;
		case DELETE:
			cout << x[i++];
			break;
		case INSERT:
			cout << ' ';
			break;

		default:
			break;
		}
	}
	cout << endl;
	for (auto opID : opSeq) {
		switch (opID) {
		case COPY:
		case REPLACE:
			cout << y[j++];
			break;
		case DELETE:
			cout << ' ';
			break;
		case INSERT:
			cout << y[j++];
			break;
		default:
			break;
		}
	}
}
// 产生操作序列函数
void genDNAAlignOpSeq(const vector<vector<int>>& op, int i, int j, vector<int>& opSeq) {
	if (i == 0 && j == 0) {
		return;
	}
	auto i_buf = i;
	auto j_buf = j;
	auto opID = op[i][j];
	if (opID == COPY || opID == REPLACE) {
		--i;
		--j;
	}
	else if (opID == DELETE) {
		--i;
	}
	else if (opID == INSERT) {
		--j;
	}
	else {
		i = op[i][j];
	}
	genDNAAlignOpSeq(op, i, j, opSeq);
	opSeq.push_back(opID);
}
// 封装函数
void DNAAlignPrint(const vector<vector<int>>& op, const string& x, const string& y) {
	vector<int> opSeq;
	int m = x.size();
	int n = y.size();

	genDNAAlignOpSeq(op, m, n, opSeq);
	for (auto c : opSeq) {
		cout << c << endl;
	}
	DNAAlignPrint_aux(opSeq, x, y);
}

/****************************************************************************************************************/
/* 接缝剪裁的图像压缩 */
/* 计算函数 */
/*
参数：
	d：图像矩阵
返回：
	disr：用于存储以d[i][j]为结尾的破坏都
	next：存储了第i行的操作，由于第一行没有操作，所以只存了第2~m行（实际范围为0~m-2）
	pos：1->m行的操作的结尾

*/
enum direction {
	UL = -1,
	D = 0,
	UR = 1
};
tuple<vector<vector<double>>, vector<vector<int>>, int> compressImage(const vector<vector<double>>& d) {
	if (d.empty() || d[0].empty()) {
		return { vector<vector<double>>(), vector<vector<int>>(), -1 };
	}
	int m = d.size();
	if (m == 1) {
		double tmp = d[0][0];
		int pos = 0;;
		for (int j = 1; j < d[0].size(); ++j) {
			if (d[0][j] < tmp) {
				tmp = d[0][j];
				pos = j;
			}
		}
		return { d, vector<vector<int>>(), pos };
	}
	int n = d[0].size();
	vector<vector<double>> disr(m, vector<double>(n, 1e5));
	vector<vector<int>> next(m - 1, vector<int>(n, 0));

	for (int j = 0; j < m; ++j) {
		disr[0][j] = d[0][j];
	}
	for (int i = 1; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (j == 0) {
				if (disr[i - 1][j] > disr[i - 1][j + 1]) {
					disr[i][j] = disr[i - 1][j + 1];
					next[i - 1][j] = UR;
				}
				else {
					disr[i][j] = disr[i - 1][j];
					next[i - 1][j] = D;
				}
			}
			else if (j == n - 1) {
				if (disr[i - 1][j] > disr[i - 1][j - 1]) {
					disr[i][j] = disr[i - 1][j - 1];
					next[i - 1][j] = UL;
				}
				else {
					disr[i][j] = disr[i - 1][j];
					next[i - 1][j] = D;
				}
			}
			else {
				if (disr[i - 1][j - 1] > disr[i - 1][j]) {
					disr[i][j] = disr[i - 1][j];
					next[i - 1][j] = D;
				}
				else {
					disr[i][j] = disr[i - 1][j - 1];
					next[i - 1][j] = UL;
				}
				if (disr[i][j] > disr[i - 1][j + 1]) {
					disr[i][j] = disr[i - 1][j + 1];
					next[i - 1][j] = UR;
				}
			}
			disr[i][j] += d[i][j];
		}
	}

	auto lastDisr = disr[m - 1][0];
	int lastPos = 0;
	for (int j = 1; j < n; ++j) {
		if (disr[m - 1][j] < lastDisr) {
			lastDisr = disr[m - 1][j];
			lastPos = j;
		}
	}

	return { disr, next, lastPos };

}

/* 空间优化的接缝剪裁的图像压缩函数 */
/*
参数：
	d：图像破坏度矩阵
返回：
	lastDisr：最终的最小破坏度
	next：存储了第i行的操作，由于第一行没有操作，所以只存了第2~m行（实际范围为0~m-2）
	pos：1->m行的操作的结尾
*/
tuple<double, vector<vector<int>>, int> compressImage_spaceOptimal(const vector<vector<double>>& d) {
	if (d.empty() || d[0].empty()) {
		return { -1, vector<vector<int>>(), -1 };
	}
	int m = d.size();
	if (m == 1) {
		double tmp = d[0][0];
		int pos = 0;;
		for (int j = 1; j < d[0].size(); ++j) {
			if (d[0][j] < tmp) {
				tmp = d[0][j];
				pos = j;
			}
		}
		return { tmp, vector<vector<int>>(), pos };
	}
	int n = d[0].size();
	vector<double> disr(n, 1e5);
	vector<vector<int>> next(m - 1, vector<int>(n, 0));

	for (int j = 0; j < m; ++j) {
		disr[j] = d[0][j];
	}
	for (int i = 1; i < m; ++i) {
		double left = 0;
		double middle = disr[0];
		double right = disr[1];
		for (int j = 0; j < n; ++j) {
			left = middle;
			middle = disr[j];
			if (j == 0) {
				next[i - 1][j] = D;
				if (middle > right) {
					disr[j] = right;
					next[i - 1][j] = UR;
				}
			}
			else if (j == n - 1) {
				next[i - 1][j] = D;
				if (middle > left) {
					disr[j] = left;
					next[i - 1][j] = UL;
				}
			}
			else {
				right = disr[j + 1];
				next[i - 1][j] = D;
				if (left < middle) {
					disr[j] = left;
					next[i - 1][j] = UL;
				}
				if (middle > right) {
					disr[j] = right;
					next[i - 1][j] = UR;
				}
			}
			disr[j] += d[i][j];
		}
	}

	auto lastDisr = disr[0];
	int lastPos = 0;
	for (int j = 1; j < n; ++j) {
		if (disr[j] < lastDisr) {
			lastDisr = disr[j];
			lastPos = j;
		}
	}

	return { lastDisr, next, lastPos };

}

/* 打印函数 */
/*
参数：
	d：输入的破坏都矩阵
	next：上一步计算出的记录操作辅助输出矩阵
	startPos：最后一行的操作
返回：
	无
说明：
	通过倒着推回去，中间存储一个栈（vector）来记录位置，最后正向输出
*/
void printSeamChain(const vector<vector<double>>& d, const vector<vector<int>>& next, const int& startPos) {
	
	if (startPos < 0 || next.empty()) {
		return;
	}
	if (next.empty()) {
		cout << d[0][startPos];
		return;
	}
	vector<int> chain(next.size() + 1, 0);

	chain[chain.size() - 1] = startPos;
	int pos = startPos;
	for (int i = chain.size() - 1; i > 0; --i) {
		chain[i] = pos;
		switch (next[i - 1][pos]) {
		case UL:
			--pos;
			break;
		case UR:
			++pos;
			break;
		case D:
		default:
			break;
		}
	}
	chain[0] = pos;
	for (int i = 0; i < d[0].size(); ++i) {
		cout << d[i][chain[i]] << endl;
	}
}

/****************************************************************************************************************/

/* 切割字符串，类似于矩阵乘法次数计算 */
/*
参数：
	n：目标字符串的大小
	L：切割位置序列，需要提前排序
返回：
	cost：操作数矩阵
	breaks：操作序列
*/
// 算法导论原版程序，没有进行优化，下标从1开始，同时浪费很多内存
pair<vector<vector<int>>, vector<vector<int>>> breakString(int n, vector<int> L) {
	if (L.empty() || L.back() >= n) {
		return { vector<vector<int>>(), vector<vector<int>>() };
	}
	
	L.insert(L.begin(), 0);
	L.push_back(n);
	int m = L.size();
	vector<vector<int>> cost(m + 1, vector<int>(m + 1, 0));
	vector<vector<int>> breaks(m + 1, vector<int>(m + 1, 0));

	for (int len = 3; len <= m; ++len) {
		for (int i = 1; i <= m - len + 1; ++i) {
			int j = i + len - 1;
			cost[i][j] = INT_MAX;
			for (int k = i + 1; k <= j - 1; ++k) {
				int tmp = cost[i][k] + cost[k][j];
				if (tmp < cost[i][j]) {
					cost[i][j] = tmp;
					breaks[i][j] = k;
				}
			}
			cost[i][j] = cost[i][j] + L[j - 1] - L[i - 1];
		}
	}

	cout << "The minimum cost of breaking the string is " << cost[1][m] << endl;
	return { cost, breaks };
}
void printBreaks(vector<int> L, const vector<vector<int>>& breaks, int i, int j) {
	if (L.empty() || breaks.empty() || i > j) {
		return;
	}
	if (j - i >= 2) {
		int k = breaks[i][j];
		cout << L[k - 2] << endl;
		printBreaks(L, breaks, i, k);
		printBreaks(L, breaks, k, j);
	}
}

// 优化第一版，两个矩阵下标从0开始
pair<vector<vector<int>>, vector<vector<int>>> breakString_spaceOptimal_first(int n, vector<int> L) {
	if (L.empty() || L.back() >= n) {
		return { vector<vector<int>>(), vector<vector<int>>() };
	}

	L.insert(L.begin(), 0);
	L.push_back(n);
	int m = L.size();
	vector<vector<int>> cost(m, vector<int>(m, 0));
	vector<vector<int>> breaks(m, vector<int>(m, 0));

	for (int len = 3; len <= m; ++len) {
		for (int i = 0; i < m - len + 1; ++i) {
			int j = i + len - 1;
			cost[i][j] = INT_MAX;
			for (int k = i + 1; k <= j - 1; ++k) {
				int tmp = cost[i][k] + cost[k][j];
				if (tmp < cost[i][j]) {
					cost[i][j] = tmp;
					breaks[i][j] = k;
				}
			}
			cost[i][j] = cost[i][j] + L[j] - L[i];
		}
	}

	cout << "The minimum cost of breaking the string is " << cost[0][m - 1] << endl;
	return { cost, breaks };
}
void printBreaks_first(vector<int> L, const vector<vector<int>>& breaks, int i, int j) {
	if (L.empty() || breaks.empty() || i > j) {
		return;
	}
	if (j - i >= 2) {
		int k = breaks[i][j];
		cout << L[k - 1] << endl;
		printBreaks_first(L, breaks, i, k);
		printBreaks_first(L, breaks, k, j);
	}
}

// 优化第二版，两个矩阵下标从0开始，并删去了不可能出现的i = j情况（即切割点重叠的情况）
pair<vector<vector<int>>, vector<vector<int>>> breakString_spaceOptimal_second(int n, vector<int> L) {
	if (L.empty() || L.back() >= n) {
		return { vector<vector<int>>(), vector<vector<int>>() };
	}

	L.insert(L.begin(), 0);
	L.push_back(n);
	int m = L.size();
	vector<vector<int>> cost(m - 1, vector<int>(m - 1, 0));
	vector<vector<int>> breaks(m - 1, vector<int>(m - 1, 0));

	for (int len = 3; len <= m; ++len) {
		for (int i = 0; i < m - len + 1; ++i) {
			int j = i + len - 1;
			cost[i][j - 1] = INT_MAX;
			for (int k = i + 1; k <= j - 1; ++k) {
				int tmp = cost[i][k - 1] + cost[k][j - 1];
				if (tmp < cost[i][j - 1]) {
					cost[i][j - 1] = tmp;
					breaks[i][j - 1] = k;
				}
			}
			cost[i][j - 1] += L[j] - L[i];
		}
	}

	cout << "The minimum cost of breaking the string is " << cost[0][m - 2] << endl;
	return { cost, breaks };
}
void printBreaks_second(vector<int> L, const vector<vector<int>>& breaks, int i, int j) {
	if (L.empty() || breaks.empty() || i > j) {
		return;
	}
	if (j - i >= 1) {
		int k = breaks[i][j];
		cout << L[k - 1] << endl;
		printBreaks_second(L, breaks, i, k - 1);
		printBreaks_second(L, breaks, k, j);
	}
}

// 优化第三版，
pair<vector<vector<int>>, vector<vector<int>>> breakString_spaceOptimal_third(int n, vector<int> L) {
	if (L.empty() || L.back() >= n) {
		return { vector<vector<int>>(), vector<vector<int>>() };
	}

	L.insert(L.begin(), 0);
	L.push_back(n);
	int m = L.size();
	vector<vector<int>> cost(m - 2, vector<int>(m - 2, 0));
	vector<vector<int>> breaks(m - 2, vector<int>(m - 2, 0));

	for (int i = 0; i < L.size() - 2; ++i) {
		cost[i][i] = L[i + 2] - L[i];
		breaks[i][i] = i + 1;
	}

	for (int len = 4; len <= m; ++len) {
		for (int i = 0; i < m - len + 1; ++i) {
			int j = i + len - 1;
			cost[i][j - 2] = INT_MAX;
			
			for (int k = i + 1; k <= j - 1; ++k) {
				int tmp = 0;
				if (k == i + 1) {
					tmp = cost[k][j - 2];
				}
				else if (k == j - 1) {
					tmp = cost[i][k - 2];
				}
				else {
					tmp = cost[i][k - 2] + cost[k][j - 2];
				}

				if (tmp < cost[i][j - 2]) {
					cost[i][j - 2] = tmp;
					breaks[i][j - 2] = k;
				}
			}
			cost[i][j - 2] += L[j] - L[i];
		}
	}

	cout << "The minimum cost of breaking the string is " << cost[0][m - 3] << endl;
	return { cost, breaks };
}
void printBreaks_third(vector<int> L, const vector<vector<int>>& breaks, int i, int j) {
	if (L.empty() || breaks.empty() || i > j) {
		return;
	}
	if (j >= i) {
		int k = breaks[i][j];
		cout << L[k - 1] << endl;
		printBreaks_third(L, breaks, i, k - 2);
		printBreaks_third(L, breaks, k, j);
	}
}

/****************************************************************************************************************/
/* 投资规划，参考https://blog.csdn.net/yangtzhou/article/details/84405454 */
/*
参数：
	r：每年的投资汇报率
	startMoney：初始资金
	f1：不转换投资的管理费
	f2：转换投资的管理费
返回：
	e：e[i][j]第j年投资i理财产品获得的最大收入
	p：p[i][j]记录第j年投资第i个理财产品获得最大收入时，上一年投资的哪个理财产品的index
*/
pair<vector<vector<double>>, vector<vector<int>>>
investStrategy(const vector<vector<double>>& r, const double& startMoney, const double& f1, const double& f2) {

	if (r.empty() || r[0].empty()) {
		return { vector<vector<double>>(), vector<vector<int>>() };
	}
	// n种投资
	int n = r.size();
	// m年
	int m = r[0].size();
	vector<vector<double>> e(n, vector<double>(m, 0));
	vector<vector<int>> p(n, vector<int>(m, -1));

	double lastYearMax = -1;
	int lastYearPos = -1;
	for (int i = 0; i < n; ++i) {
		int tmp = r[i][0] * startMoney;
		e[i][0] = tmp;
		if (tmp > lastYearMax) {
			lastYearMax = tmp;
			lastYearPos = i;
		}
	}

	for (int j = 1; j < m; ++j) {
		double currYearMax = -1;
		int currYearPos = -1;
		for (int i = 0; i < n; ++i) {
			// 计算第j年，第i种投资，不做交换的回报
			double e1 = (e[i][j - 1] - f1) * r[i][j];
			// 计算第j年，转换到第i种投资的回报
			double e2 = (lastYearMax - f2) * r[i][j];

			if (e1 > e2) {
				e[i][j] = e1;
				p[i][j] = i;
				if (e1 > currYearMax) {
					currYearMax = e1;
					currYearPos = i;
				}
			}
			else {
				e[i][j] = e2;
				p[i][j] = lastYearPos;
				if (e2 > currYearMax) {
					currYearMax = e2;
					currYearPos = i;
				}
			}
		}
		lastYearMax = currYearMax;
		lastYearPos = currYearPos;
	}
	//vector<int> pos(lastYearPos);
	//for (int j = m - 2; j >= 0; --j) {
	//	pos
	//}
	return { e,p };
}
/* 打印函数，输出最后的打印方案根据p矩阵 */
/*
参数：
	p：上一步计算出的指导输出的p矩阵
	j：第j年
	S：第j年选择的是哪个理财产品
返回：
	无
*/
void printInvestStrategy(const vector<vector<int>>& p, const int& j, const int& S) {
	if (j >= 0) {
		int tmp = p[S][j];
		printInvestStrategy(p, j - 1, tmp);
		cout << "the " << j << " years invest: " << S << endl;
	}
}

//优化第一版
/*
参数：
	r：每年的投资汇报率
	startMoney：初始资金
	f1：不转换投资的管理费
	f2：转换投资的管理费
返回：
	max_e：最大收入
	pos：记录获得最大收入时，投资序列
*/
pair<double, vector<int>>
investStrategy_spaceOptimal(const vector<vector<double>>& r, const double& startMoney, const double& f1, const double& f2) {

	if (r.empty() || r[0].empty()) {
		return { -1, vector<int>() };
	}
	// n种投资
	int n = r.size();
	// m年
	int m = r[0].size();
	vector<double> e(n, 0);
	vector<vector<int>> p(n, vector<int>(m, -1));

	double lastYearMax = -1;
	int lastYearPos = -1;
	const auto& max_e = lastYearMax;
	for (int i = 0; i < n; ++i) {
		int tmp = r[i][0] * startMoney;
		e[i] = tmp;
		if (tmp > lastYearMax) {
			lastYearMax = tmp;
			lastYearPos = i;
		}
	}

	for (int j = 1; j < m; ++j) {
		double currYearMax = -1;
		int currYearPos = -1;
		for (int i = 0; i < n; ++i) {
			// 计算第j年，第i种投资，不做交换的回报
			double e1 = (e[i] - f1) * r[i][j];
			// 计算第j年，转换到第i种投资的回报
			double e2 = (lastYearMax - f2) * r[i][j];

			if (e1 > e2) {
				e[i] = e1;
				p[i][j] = i;
				if (e1 > currYearMax) {
					currYearMax = e1;
					currYearPos = i;
				}
			}
			else {
				e[i] = e2;
				p[i][j] = lastYearPos;
				if (e2 > currYearMax) {
					currYearMax = e2;
					currYearPos = i;
				}
			}
		}
		lastYearMax = currYearMax;
		lastYearPos = currYearPos;
	}
	vector<int> pos(m, -1);
	pos[m - 1] = lastYearPos;
	for (int j = m - 2; j >= 0; --j) {
		pos[j] = p[pos[j + 1]][j + 1];
	}
	return { max_e, pos };
}
/* 打印函数，打印投资方案 */
/*
参数：
	seq：投资序列
返回：
	无
*/
void printInvestStrategy(const vector<int>& seq) {
	for (int j = 0; j < seq.size(); ++j) {
		cout << "the " << j << "years invest: " << seq[j] << endl;
	}
}

/****************************************************************************************************************/

int main01() {

// 切钢条测试
	//vector<int> p{ 0,1,5,8,8,10,17,17,20,24,30 };

	//for (int i = 0; i < 13; ++i) {
	//	cout << memoCutRod(p, i) << endl;
	//}
	//for (int i = 0; i < 11; ++i) {
	//	cout << bottomUpCutRod(p, i) << endl;
	//}

	//vector<int> r(10 + 1, 0);
	//vector<int> s = r;
	//externedBottomUpCutRod(p, 10, r, s);
	//
	//printCutRodSolution(p, 10, s);

	//vector<int> r(10 + 1, INT_MIN);
	//vector<int> s(10 + 1, 0);

	//externedMemoCutRod(p, 10, r, s);

	//for (int i = 0; i < r.size(); ++i) {
	//	cout << r[i] << " " << s[i] << endl;
	//}

	//int res = memoDpFibonacciCalc(5);
	//int res = bottomUpFibonacciCalc(5);


// 矩阵链乘法次数测试
	//vector<int> p{ 30, 35, 15, 5, 10, 20, 25 };
	//vector<int> p{ 5, 10, 3, 12, 5, 50, 6 };
	//vector<int> p{ 1000,100,20,10,1000 };
	//
	//vector<vector<int>> m(p.size() - 1, vector<int>(p.size() - 1, 0));
	//vector<vector<int>> s(p.size() - 2, vector<int>(p.size() - 2, 0));

	//matrixChainOrder(p, m, s);

	//for (auto c : m) {
	//	for (auto b : c) {
	//		cout << setw(6) << b << " ";
	//	}
	//	cout << endl;
	//}

	//cout << endl;

	//for (auto c : s) {
	//	for (auto b : c) {
	//		cout << setw(6) << b << " ";
	//	}
	//	cout << endl;
	//}

	//printOptimalParens(s, 0, s.size());

// 最长公共子序列测试
	//vector<char> X{ 'A', 'B','C', 'B','D','A','B' };
	//vector<char> Y{ 'B','D','C','A','B','A' };

	//auto res = lscLength(X, Y);
	//auto c = res.first;
	//auto b = res.second;

	//for (auto m : c) {
	//	for (auto n : m) {
	//		cout << setw(3) <<n << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	//for (auto m : b) {
	//	for (auto n : m) {
	//		cout << setw(3) <<n << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//printLSC(b, X, X.size(), Y.size());
	//cout << endl;

	//vector<int> A{ 1,0,0,1,0,1,0,1 };
	//vector<int> B{ 0,1,0,1,1,0,1,1,0 };

	//auto c = lscLength_optimal(X, Y);
	//printLSC_optimal(c, X, X.size(), Y.size());

	//auto c = lscLength_optimal(A, B);
	//printLSC_optimal(c, A, A.size(), B.size());
	//auto c1 = lscLength_optimal(X, Y);
	//printLSC_optimal(c1, X, Y, X.size(), Y.size());

	//string a{ "ABC" };
	//string b{ "BCD" };
	//cout << a << endl << b << endl;
	//cout << a + b << endl;

	//auto c = memorizedLSCLength_optimal(X, Y);
	//for (auto m : c) {
	//	for (auto n : m) {
	//		cout << setw(3) <<n << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	//printLSC_optimal(c, X, Y, X.size(), Y.size());

	//auto num = lscLength_space_optimal(X, Y);


	//auto preRow_p = new vector<int>(10, 0);
	//auto nowRow_p = new vector<int>(10, 1);

	//for (auto c : *preRow_p) {
	//	cout << c << ' ';
	//}
	//cout << endl;
	//for (auto c : *nowRow_p) {
	//	cout << c << ' ';
	//}
	//cout << endl;

	//swap(preRow_p, nowRow_p);

	//for (auto c : *preRow_p) {
	//	cout << c << ' ';
	//}
	//cout << endl;
	//for (auto c : *nowRow_p) {
	//	cout << c << ' ';
	//}
	//cout << endl;

	//auto num = lscLength_space_optimal_first(X, Y);

// 最长递增子序列测试
	//vector<int> Z{ 9,1,2,4,6,3,5,0 };
	//auto res = LMIS(Z);
	//printLMIS(Z, res.second, 4);

// 最优搜索二叉树
	//vector<double> p{ 0.15,0.1,0.05,0.1,0.2 };
	//vector<double> q{ 0.05,0.1,0.05,0.05,0.05,0.1 };

	//vector<double> p{ 0.04,0.06,0.08,0.02,0.1,0.12, 0.14 };
	//vector<double> q{ 0.06,0.06,0.06,0.06,0.05,0.05,0.05,0.05 };

	//auto res = optimalBST(p, q, p.size(), 1);
	//auto res = optimalBST(p, q, p.size());
	//for (auto c : res.first) {
	//	for (auto b : c) {
	//		cout << setw(6) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto c : res.second) {
	//	for (auto b : c) {
	//		cout << setw(6) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	////vector<double> p1{ 0.04,0.06,0.08,0.02,0.1,0.12,0.14 };
	////vector<double> q1{ 0.06,0.06,0.06,0.06,0.05,0.05,0.05,0.05 };
	////auto res1 = optimalBST(p1, q1, p1.size());
	////auto e = res1.first;
	////auto col = e[0].size();
	////cout << e[0][p1.size()] << endl;
	////
	////for (auto c : res1.first) {
	////	for (auto b : c) {
	////		cout << setw(6) << b << ' ';
	////	}
	////	cout << endl;
	////}
	////cout << endl;
	////for (auto c : res1.second) {
	////	for (auto b : c) {
	////		cout << setw(6) << b << ' ';
	////	}
	////	cout << endl;
	////}
	////cout << endl;


	////constructOptimalBST(res.second, 1, p.size(), 1);
	//constructOptimalBST(res.second, 0, p.size());

// 最长回文子序列测试
	//string X{ "character" };
	//auto res = longestPalindrome(X);
	//for (int i = 1; i <= X.size();++i) {
	//	for (int j = 1; j <= X.size(); ++j) {
	//		cout << setw(6) << res.first[i][j] << ' ';
	//	}
	//	cout << endl;
	//}
	//
	//cout << endl;
	//for (int i = 1; i <= X.size(); ++i) {
	//	for (int j = 1; j <= X.size(); ++j) {
	//		cout << setw(6) << res.second[i][j] << ' ';
	//	}
	//	cout << endl;
	//}
	//
	//string S;
	//genLPS(X, 1, X.size(), res.second, S);

	//auto res = longestPalindrome_optimal(X);
	//for (auto a : res.first) {
	//	for (auto b : a) {
	//		cout << setw(6) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//
	//cout << endl;
	//for (auto a : res.second) {
	//	for (auto b : a) {
	//		cout << setw(6) << b << ' ';
	//	}
	//}
	//	cout << endl;
	//
	//string S;
	//genLPS_optimal(X, 0, X.size() - 1, res.second, S);

// 双调欧几里得旅行商测试
	//vector<coordi<double>> p{ {0, 7}, { 1,1 }, { 2,4 }, { 5,5 }, { 6,2 }, { 7,6 }, { 8,3 }};

	//auto res = EuclideanTSP<double, int>(p);

	//for (auto a : res.first) {
	//	for (auto b : a) {
	//		cout << setw(6) << setprecision(3) <<  b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : res.second) {
	//	for (auto b : a) {
	//		cout << setw(6) << setprecision(3) <<  b << ' ';
	//	}
	//	cout << endl;
	//}

	//printTour(res.second, p.size());


// 整齐打印测试
	//vector<string> wordTable;
	//vector<int> wordLenTable;

	//if (!GetWord("input.txt", 64, wordTable, wordLenTable))
	//return -1;

	////auto res = printNeatly(wordLenTable, 64);
	////auto res = printNeatlyOptimal_first(wordLenTable, 64);
	////auto res = printNeatlyOptimal_second(wordLenTable, 64);
	//auto res = printNeatlyOptimal_third(wordLenTable, 64);

	//////printLines(res.second, wordLenTable.size());

	//printLines(wordTable, wordLenTable, res.second, wordTable.size(), 64, '*');

	//for (auto a : res.first) {
	//	cout << a << ' ';
	//}
	//cout << endl;

	//for (auto a : res.second) {
	//	cout << setw(2) << a << ' ';
	//}

// 编辑距离测试
	//string x{ "algorithm" };
	//string y{ "altruistic" };
	////vector<double> edit_cost{ 1, 1.5, 0.9, 0.9, 1.5, 5 };
	//vector<double> edit_cost{ 1, -1, -2, -2, 10000.0, 10000.0 };

	//auto res = editDistance(x, y, edit_cost);

	//opSequence(res.second, x.size(), y.size());

	//string x{ "horse" };
	//string y{ "ros" };
	////vector<double> edit_cost{ 1, 1.5, 0.9, 0.9, 1.5, 5 };
	//vector<double> edit_cost{ 1, -1, -2, -2, 10000.0, 10000.0 };

	//auto res = minDistance(y, x);

	//opSequence(res.second, x.size(), y.size());

// DNA对齐
	//string d1{ "GATCGGCAT" };
	//string d2{ "CAATGTGAATC" };
	//vector<double> e{ -1,1,2,2 };



	//auto res1 = DNAAlign(d1, d2, e);
	//auto res2 = DNAAlign_optimal(d1, d2, e);
	////auto res2 = DNAAlign(d1, d2);

	//opSequence(res1.second, d1.size(), d2.size());
	//cout << endl;
	//opSequence(res2.second, d1.size(), d2.size());

	//vector<int> ret;
	//DNAAlignPrint(res1.second, d1, d2);
	//DNAAlignPrint(res2.second, d1, d2);
	////DNAAlignPrint(res1.second, d1.size(), d2.size(), ret);
	////cout << "*****************************" << endl;
	////DNAAlign_print(ret, d1, d2);

// 接缝裁剪的图像压缩的测试
	//vector<vector<double>> d(10, vector<double>(10));
	//ifstream fin("d_out.txt");
	//string str_buf;
	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		fin >> str_buf;
	//		d[i][j] = atof(str_buf.c_str());
	//	}
	//}

	////for (auto a : d) {
	////	for (auto b : a) {
	////		cout << setw(3) << int(b) << ' ';
	////	}
	////	cout << endl;
	////}
	//fin.close();

	//auto res = compressImage(d);

	//for (auto a : get<0>(res)) {
	//	for (auto b : a) {
	//		cout << setw(3) << b << ' ';
	//	}
	//	cout << endl;
	//}

	//cout << endl;

	//for (auto a : get<1>(res)) {
	//	for (auto b : a) {
	//		cout << setw(3) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	//cout << setw(3) << get<2>(res) << endl;


	//printSeamChain(d, get<1>(res), get<2>(res));

// 拆分字符串测试
	/* 原版 */
	//vector<int> L{ 2,8,10 };
	vector<int> L{ 20, 17, 14, 11, 25 };
	//sort(L.begin(), L.end());
	//auto res = breakString(30, L);
	//for (auto a : res.first) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : res.second) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//sort(L.begin(), L.end());
	//printBreaks(L, res.second, 1, L.size() + 2);

	/* 版本1 */
	//sort(L.begin(), L.end());
	//auto res1 = breakString_spaceOptimal_first(30, L);
	//for (auto a : res1.first) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : res1.second) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//printBreaks_first(L, res1.second, 0, L.size() + 1);

	/* 版本2 */
	//sort(L.begin(), L.end());
	//auto res2 = breakString_spaceOptimal_second(30, L);
	//for (auto a : res2.first) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : res2.second) {
	//	for (auto b : a) {
	//		cout << setw(2) << b << ' ';
	//	}
	//	cout << endl;
	//}
	//printBreaks_second(L, res2.second, 0, L.size());

	/* 版本3 */
	// sort(L.begin(), L.end());
	// auto res2 = breakString_spaceOptimal_third(30, L);
	// for (auto a : res2.first) {
	// 	for (auto b : a) {
	// 		cout << setw(2) << b << ' ';
	// 	}
	// 	cout << endl;
	// }
	// cout << endl;
	// for (auto a : res2.second) {
	// 	for (auto b : a) {
	// 		cout << setw(2) << b << ' ';
	// 	}
	// 	cout << endl;
	// }
	// printBreaks_third(L, res2.second, 0, L.size() - 1);

// 最佳投资策略测试
	//vector<vector<double>> r(8, vector<double>(10));
	//ifstream fin("r_out.txt");
	//string str_buf;
	//for (int i = 0; i < r.size(); ++i) {
	//	for (int j = 0; j < r[0].size(); ++j) {
	//		fin >> str_buf;
	//		r[i][j] = atof(str_buf.c_str());
	//	}
	//}
	////for (auto a : r) {
	////	for (auto b : a) {
	////		cout << setw(4) << b << ' ';
	////	}
	////	cout << endl;
	////}
	////cout << endl;
	//fin.close();

	////auto res = investStrategy(r, 10000, 200, 400);
	////for (auto a : res.first) {
	////	for (auto b : a) {
	////		cout << setw(10) << b << ' ';
	////	}
	////	cout << endl;
	////}
	////cout << endl;
	////printInvestStrategy(res.second, 9, 6);
	//auto res1 = investStrategy_spaceOptimal(r, 10000, 200, 400);
	//for (auto a : res1.second) {
	//	cout << setw(10) << a << ' ';
	//}
	//cout << endl;
	//printInvestStrategy(res1.second);


	return 0;

}
