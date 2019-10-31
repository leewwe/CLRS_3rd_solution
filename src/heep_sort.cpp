#include "../head/heap_sort.hh"
#include <vector>
using std::vector;

#define LEFT(i) (i) * (2) + (1)
#define RIGHT(i) LEFT(i) + (1)
#define PARENT(i) ((i) - (1)) / 2

template<typename T>
void Swap(T& a, T& b) {
	auto tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
void maxHeapify(vector<T>& A, int i) {
	auto heapSize = A.size();
	int l = LEFT(i);
	int r = RIGHT(i);
	while (true) {
		auto largest = i;
		if (l < heapSize && A[l] > A[i]) {
			largest = l;
		}
		if (r < heapSize && A[r] > A[largest]) {
			largest = r;
		}
		if (largest == i) {
			return;
		}
		Swap(A[i], A[largest]);
		i = largest;
		l = LEFT(i);
		r = RIGHT(i);
	}
}

template<typename T>
void maxHeapify(vector<T>& A, int i, int heapSize) {
	int l = LEFT(i);
	int r = RIGHT(i);
	while (true) {
		auto largest = i;
		if (l < heapSize && A[l] > A[i]) {
			largest = l;
		}
		if (r < heapSize && A[r] > A[largest]) {
			largest = r;
		}
		if (largest == i) {
			return;
		}
		Swap(A[i], A[largest]);
		i = largest;
		l = LEFT(i);
		r = RIGHT(i);
	}
}

template<typename T>
void buildMaxHeap(vector<T>& A) {
	int heapSize = A.size();
	for (auto i = heapSize / 2 - 1; i >= 0; --i) {
		maxHeapify(A, i);
	}
}

template<typename T>
void heapSort(vector<T>& A) {
	buildMaxHeap(A);
	for (auto i = A.size(); i > 1; --i) {
		Swap(A[i-1], A[0]);
		maxHeapify(A, 0, i - 1);
	}
}
// 优先队列的返回堆顶的元素
template<typename T>
T heapMaximum(vector<int>& A) {
	if (A.empty()) {
		return INT_MIN;
	}
	return A[0];
}

template<typename T>
T heapExtractMax(vector<T>& A) {
	if (A.empty()) {
		cerr << "heap underflow" << endl;
		return INT_MIN;
	}
	auto max = A[0];
	A[0] = A.back();
	A.pop_back();
	maxHeapify(A, 0);
	return max;
}

template<typename T>
void heapIncreseKey(vector<T>& A, int i, T key) {
	if (A.size() <= i) {
		cerr << "index over overflow" << endl;
		return;
	}
	if (key < A[i]) {
		cerr << "new key is smaller than current key" << endl;
		return;
	}
	//A[i] = key;
	//while (i > 0 && A[PARENT(i)] < A[i]) {
	//	//Swap(A[i], A[PARENT(i)]);//这部操作一般需要散步赋值才能实现
	//	i = PARENT(i);
	//}
	auto parent = PARENT(i);
	auto parent_key = A[parent];
	while (i > 0 && parent_key < key) {
		A[i] = parent_key;
		i = parent;
		parent = PARENT(i);
		parent_key = A[parent];
	}
	A[i] = key;
}

template<typename T>
void maxHeapInsert(vector<T>& A, T elem) {
	A.push_back(elem);
	//heapIncreseKey(A, A.size() - 1, elem);
	int i = A.size() - 1;
	while (i > 0 && A[PARENT(i)] < A[i]) {
		Swap(A[i], A[PARENT(i)]);
		i = PARENT(i);
	}
}

template<typename T>
void heapDelete(vector<T>& A, int i) {
	Swap(A[i], A.back());
	A.pop_back();
	maxHeapify(A, i);
}