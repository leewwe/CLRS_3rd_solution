#include "..\head\quickSort.hh"
#include "..\head\insertSort.hh"

// 不接受主元的划分函数
int partition(vector<int>& A, int p, int r) {
	auto& x = A[r];
	int i = p - 1;
	int eqCnt = 0;
	for (auto j = p; j < r; ++j) {
		if (A[j] <= x) {
			exchange(A[++i], A[j]);
		}
		// 统计相等数目，当完全相等的时候返回范围的中点，而不是开头，
		// 或者在exchange那行<=号的时候，不返回结尾
		if (A[j] == x) {
			++eqCnt;
		}
	}
	exchange(A[++i], x);
	return i + eqCnt / 2;
}
// 接受主元的划分函数
int partition(vector<int>& A, int p, int r, int pivot){
	int i = p - 1;
	int pivotPos = 0;
	for(int j = p; j <= r; ++j){
		if(A[j] <= pivot){
			if(pivot == A[j]){
				pivotPos = i+1;
			}
			swap(A[++i], A[j]);
		}
	}
	swap(A[i], A[pivotPos]);
	return i; 
}
// 快速排序原始代码
void quickSort(vector<int>& A, int p, int r) {
	if (p >= r) {
		return;
	}
	auto q = partition(A, p, r);
	quickSort(A, p, q - 1);
	quickSort(A, q + 1, r);
}
// 尾递归快速排序
void tailRecursiveQuickSort(vector<int>& A, int p, int r) {
	while (p < r) {
		int q = partition(A, p, r);
		tailRecursiveQuickSort(A, p, q - 1);
		p = q + 1;
	}
}

// 选取每次划分中较短的子序列来进行排序，其余的部分交给循环去处理
void modifinedTailRecursiveQuickSort(vector<int>& A, int p, int r) {
	while (p < r) {
		int q = partition(A, p, r);
		if (q <= (p + r) / 2) {
			modifinedTailRecursiveQuickSort(A, p, q - 1);
			p = q + 1;
		}
		else {
			modifinedTailRecursiveQuickSort(A, q + 1, r);
			r = q - 1;
		}
	}
}

// 选择函数
int select(vector<int>& A, int p, int r, int i) {
	if(p >= r){
		return A[p];
	}
	/* !!以下带偏移量的，不要忘了 “+p” */
	int numGroup = (r - p + 1) / numOfEachGrp;
	// int residue = (r - p + 1) % numOfEachGrp;
	for(int cnt = 0; cnt < numGroup; ++cnt){
		insertSort(A, p + cnt * numOfEachGrp, (cnt + 1) * numOfEachGrp - 1);
	}
	insertSort(A, numGroup * numOfEachGrp + p, r);
	
	vector<int> mid; 
	for(int cnt = 0; cnt < numGroup; ++cnt){
		mid.push_back(A[p + cnt * numOfEachGrp + numOfEachGrp / 2]);
	}
	if(numGroup * numOfEachGrp + p <= r){
		mid.push_back(A[(numGroup * numOfEachGrp + p + r) / 2]);
	}
	// 中位数的中位数
	int midNum = select(mid, 0, mid.size() - 1, (mid.size() - 1) / 2 + 1);
	
	int q = partition(A, p, r, midNum);

	int k = q - p + 1;
	if(i == k){
		return A[q];
	}
	else if(i < k){
		return select(A, p, q - 1, i);
	}
	else{
		return select(A, q + 1, r, i - k);
	}
}


// 查找中位数，不影响原始数据
int median(vector<int> A, int p, int r){
	return select(A, p, r, (r - p) / 2 + 1);
}

// 最佳快速排序
void bestQuickSort(vector<int>& A, int p, int r){
	if (p >= r) {
		return;
	}
	int pivot = median(A, p, r);
	// 使用接受主元的partition（划分函数）
	int q = partition(A, p, r, pivot);
	bestQuickSort(A, p, q - 1);
	bestQuickSort(A, p + 1, r);
}

// 找到中位数附近的k个数
vector<int> findTheNearestKnums(vector<int> A, int p, int r, int k){
	int mid = median(A, p, r);
	vector<int> absNums;
	for(int i = p; i <= r; ++i){
		absNums.push_back(abs(A[i] - mid));
	}
	int bounder = select(absNums, 0, absNums.size() - 1, k);
	vector<int> ret;
	for(int i = p; i <= r; ++i){
		int absDiff = abs(A[i] - mid);
		if(absDiff && absDiff <= bounder){
			ret.push_back(A[i]);
		}
	}
	return ret;
}


// 找到两个等大数组中的中位数
// 中间调用函数，在前一个数组中找共同的中位数，看是否能找到
pair<int, bool> findMedian(vector<int> A, int& a_p, int& a_r, int low, int high, vector<int> B, int& b_p, int& n) {
	if (a_p > a_r) {
		return { 0, false };
	}
	else {
		int mid = (low + high) / 2;
		if (mid == a_r && A[a_r] <= B[b_p]) {
			return { A[a_r], true };
		}
		else if (mid < a_r && (A[mid] >= B[n - (mid - a_p) - 1] && A[mid] <= B[n - (mid - a_p)])) {
			return { A[mid], true };
		}
		else if (A[mid] > B[n - (mid - a_p)]) {
			return findMedian(A, a_p, a_r, low , mid - 1, B, b_p, n);
		}
		else {
			return findMedian(A, a_p, a_r, mid + 1, high, B, b_p, n);
		}
	}
}
// 封装函数
int twoArrayMedian(vector<int> X, int x_p, int x_r, vector<int> Y, int y_p, int y_r) {
	int x_n = x_r - x_p;
	int y_n = y_r - y_p;
	if (x_n != y_n) {
		return INT_MIN;
	}
	auto median = findMedian(X, x_p, x_r, x_p, x_r, Y, y_p, y_n);
	if (!median.second) {
		median = findMedian(Y, y_p, y_r, y_p, y_r, X, x_p, x_n);
	}
	return median.first;
}

// 改进的select函数，对于小的i有优化
int modifiedSelect(vector<int>& A, int p, int r, int k){
	int arraySize = r - p + 1;
	if(arraySize == 0){
		return INT_MIN;
	} 
	else if(arraySize == 1){
		return A[p];
	}
	else if(k >= arraySize / 2){
		return select(A, p, r, k);
	}
	vector<int> smaller;
	map<int, int> larger;
	int halfArrSize = arraySize / 2;
	// 新建这个map是为了存储大的一半，并完成算法中要求的在移动A[p + m + i] 的同时移动A[p + i]的目的
	// 不是真正的移动，是通过访问最后的smaller中的前i小的元素，获取，前larger部分的对应部分
	for(int j = p; j < p + halfArrSize; ++j){
		if(A[j] < A[j + halfArrSize]){
			smaller.push_back(A[j]);
			larger.emplace(A[j], A[j + halfArrSize]);
		}
		else{
			smaller.push_back(A[j + halfArrSize]);
			larger.emplace(A[j + halfArrSize], A[j]);
		}
	}
	int i_th = 0;
	if(arraySize % 2){
		smaller.push_back(A[r]);
		larger.emplace(A[r], A[r]);
		i_th = modifiedSelect(smaller, 0, smaller.size() - 1, k);
	}
	else{
		i_th = modifiedSelect(smaller, 0, smaller.size() - 1, k);
	}

	smaller.resize(k);
	for(int j = 0; j < k; ++j){
		smaller.push_back(larger[smaller[j]]);
	}

	return select(smaller, 0, smaller.size() - 1, k);
}


