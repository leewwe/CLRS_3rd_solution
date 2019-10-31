#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_
#include <vector>
using namespace std;

template<typename T>
void swap(T& a, T& b);

template<typename T>
void maxHeapify(vector<T>& A, int i);

template<typename T>
void maxHeapify(vector<T>& A, int i, int heapSize);

template<typename T>
void buildMaxHeap(vector<T>& A);

template<typename T>
void heapSort(vector<T>& A);

// 优先队列的返回堆顶的元素
template<typename T>
T heapMaximum(vector<int>& A);

template<typename T>
T heapExtractMax(vector<T>& A);

template<typename T>
void heapIncreseKey(vector<T>& A, int i, T key);

template<typename T>
void maxHeapInsert(vector<T>& A, T elem);

template<typename T>
void heapDelete(vector<T>& A, int i);

#endif