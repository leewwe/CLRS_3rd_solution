
#include "..\head\insertSort.hh"
// 遗忘交换算法——插入排序
inline void compare_exchange(vector<int>& A, int i, int j) {
	if (A[i] > A[j]) {
		auto tmp = A[i];
		A[i] = A[j];
		A[j] = tmp;
	}
}
void insert_sort_forget(vector<int>& ivec) {
	for (int j = 1; j < ivec.size(); ++j) {
		for (int i = j - 1; i > -1; --i) {
			compare_exchange(ivec, i, i + 1);
		}
	}
}

void insertSort(vector<int>& A, int p, int r){
	for (auto j = p + 1; j <= r ;++j) {
		auto key = A[j];
		auto i = j - 1;
		for (; i >= p && A[i] > key; --i) {
			A[i + 1] = A[i];
		}
		A[i + 1] = key;
	}
}

