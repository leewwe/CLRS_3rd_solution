#ifndef _INSERT_SORT_HH_
#define _INSERT_SORT_HH_

#include <vector>
using namespace std;

// 遗忘交换算法——插入排序
inline void compare_exchange(vector<int>& A, int i, int j);
void insert_sort_forget(vector<int>& ivec);
// template<typename T>
// void insertSort(vector<T>& A, int p, int r);
void insertSort(vector<int>& A, int p, int r);
#endif