#include "..\head\matrixChainOrder.hh"

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