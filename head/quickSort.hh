#ifndef _QUICK_SORT_HH_
#define _QUICK_SORT_HH_
#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <utility>
#include <limits.h>
#include <map>

using namespace std;

const int numOfEachGrp = 5;

// 不接受主元的划分函数
int partition(vector<int>& A, int p, int r);
// 接受主元的划分函数
int partition(vector<int>& A, int p, int r, int pivot);
// 最基本的快速排序
void quickSort(vector<int>& A, int p, int r);
// 尾递归快速排序，只使用一次递归
void tailRecursiveQuickSort(vector<int>& A, int p, int r);
// 改进的尾递归快速排序，选取每次划分中较短的子序列来进行排序，其余的部分交给循环去处理
void modifinedTailRecursiveQuickSort(vector<int>& A, int p, int r);
// 选择问题的解答，选出第i小的元素，最坏情况O(n)
int select(vector<int>& A, int p, int r, int i);
// 选出中位数，O(n)
int median(vector<int> A, int p, int r);
// 最佳性能快速排序，比随机化的快速排序还要好，每次先选取中位数以达到最佳划分，拥有最坏情况的O(nlgn)
void bestQuickSort(vector<int>& A, int p, int r);
// 找到最接近中位数的 k 个数
vector<int> findTheNearestKnums(vector<int> A, int p, int r, int k);
// 找到两个数组等大小范围中的中位数
int twoArrayMedian(vector<int> X, int x_p, int x_r, vector<int> Y, int y_p, int y_r);
// 改进的select函数，对小的i有优化
int modifiedSelect(vector<int>& A, int p, int r, int i);
#endif