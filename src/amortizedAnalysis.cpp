#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iomanip>
using namespace std;

/********************************************************************************************************/
/*思考题17-1*/
/*位逆序操作*/
/*对于二进制数组的逆序操作，k=32*/
uint32_t reverseBits(const uint32_t& n) {
	uint32_t ret = 0;
	for (int i = 0; i < 16; ++i) {
		uint32_t pre = (0x80000000 >> i)& n;
		uint32_t suf = (1 << i) & n;
		ret |= pre >> (32 - 2 * i - 1);
		ret |= (suf << (32 - 2 * i - 1));
	}
	return ret;
}
/*
参数：
	a：待逆序的数组
返回：
	逆序之后的数组
*/
vector<uint32_t> bit_reverse(vector<uint32_t> a) {
	vector<bool> m(a.size(), true );
	for (uint32_t i = 0; i < m.size(); ++i) {
		if (m[i]) {
			swap(a[i], a[reverseBits(i)]);
		}
	}
	return a;
}

/*位逆序计数器，只能右移一位*/
/*
参数：
	n：待逆序增加的数字
返回：
	逆序增加之后的结果
说明：
	这个是参考答案中的结果，每次只右移一位
伪代码：
	 ______________________________________
	|BIT-REVERSED-INCREMENT(a)			   |
	|	let m be a 1 followed by k-1 zeroes|
	|	while m & a != 0				   |
	|		a = a ^ m					   |
	|		m >> 1						   |
	|	return m | a					   |
	|______________________________________|
*/
//uint32_t bitReversedIncrement(uint32_t n) {
//	uint32_t ret = 0x80000000;
//	while (ret & n) {
//		n ^= ret;
//		ret >>= 1;	// 只右移一位
//	}
//	return ret | n;
//}

/*位逆序计数器，仿照原版的increment的代码写的*/
/*
参数：
	n：待逆序增加的数字
返回：
	逆序增加之后的结果
说明：
	这个是参考答案中的结果，每次可以右移多位
伪代码：
	 ___________________________________
	|BIT-REVERSED-INCREMENT(A)			|
	|	i = 31							|
	|	while i >= 0 and A[i] == 1		|
	|		A[i] = 0					|
	|		i = i - 1					|
	|	if i >= 0						|
	|		A[i] = 1					|
	|___________________________________|
*/
uint32_t bitReversedIncrement(uint32_t n) {
	uint32_t i = 0;
	while (i < 32 && (n & (0x80000000 >> i))) {
		n &= ~(0x80000000 >> i);	// 右移多位
		++i;
	}
	if (i < 32) {
		n |= (0x80000000 >> i);
	}
	return n;
}


/*二进制计数器，只能左移一位*/
/*
参数：
	n：待增加的数字
返回：
	增加之后的结果
说明：
	这个是参考答案中的结果，每次只左移一位
伪代码：
	 ______________________________________
	|INCREMENT(a)						   |
	|	let m be 1						   |
	|	while m & a != 0				   |
	|		a = a ^ m					   |
	|		m << 1						   |
	|	return m | a					   |
	|______________________________________|
*/
//uint32_t increment(uint32_t n) {
//	uint32_t ret = 1;
//	while (ret & n) {
//		n ^= ret;
//		ret <<= 1;
//	}
//	return ret | n;
//}
/*二进制计数器，原版的increment*/
/*
参数：
	n：待增加的数字
返回：
	增加之后的结果
说明：
	每次可以左移多位
伪代码：
	 ___________________________________
	|INCREMENT(A)						|
	|	i = 0							|
	|	while i < A.length and A[i] == 1|
	|		A[i] = 0					|
	|		i = i + 1					|
	|	if i < A.length					|
	|		A[i] = 1					|
	|___________________________________|
*/
uint32_t increment(uint32_t n) {
	int i = 0;
	while (i < 32 && n & (0x00000001 << i)) {
		n &= ~(0x00000001 << i);	// 左移多位
		++i;
	}
	if (i < 32) {
		n |= (0x00000001 << i);
	}
	return n;
}

/********************************************************************************************************/
/*思考题17-2动态二分查找*/
/*支持动态二分查找的类*/
class dynamicBinarySearchClass{
public:
	// 构造函数
	dynamicBinarySearchClass() : eleArray(0), n(0) {	}
	// 插入函数
	void insert(int ele);

	inline void set_n(int i) {
		n |= ~(0x00000001 << i);
	}
	pair<int, int> search(int ele);
	
	bool erase(int row, int col);
private:
	// 私有操作
	int binary_search(const vector<int>& eleSeq, const int& target);
private:
	//数据
	vector<vector<int>> eleArray;	// 维护的多个数组
	uint32_t n;						// 存储元素的个数
};

/*支持动态二分查找的类，插入操作*/
/*
参数：
	ele：插入的元素
返回：
	无
说明：
	按照答案介绍的方法狗构造
*/
void dynamicBinarySearchClass::insert(int ele) {
	if (eleArray.empty()) {
		eleArray.push_back(vector<int>{ele});
		eleArray[0].reserve(1);
		++n;
		return;
	}
	++n; // 先增加总insert之后的元素个数
	uint32_t i = 1;	// 记录第k位最多存在的元素个数，满足i=k^2
	int k = 0; // 记录当前的位数
	while (i < n) {
		if (eleArray[k].empty()) { // 每一位或者为empty，或者为full（2^k），如果出现了不满的情况，
			if (k == 0) {// 只有在最低位（k=0）的时候才需要添加元素，也就是需要插入的元素，
				eleArray[k].push_back(ele);
				return;
			}
		}
		else { // 否则，必然是full的，那么此时有需要执行merge了，
			if (k == 0) {// 判断是否为最低位
				eleArray[k].push_back(ele);
			}
			if (eleArray[k].size() == i) {// 判断是否已经达到的有效的二进制数的要求，如果达到了要求，那么就可以直接退出了
				return;
			}
			else if (k == eleArray.size() - 1) {// 判断是否应该扩张vector<vector>数组
				eleArray.emplace_back(eleArray[k]);
				sort(eleArray[k + 1].begin(), eleArray[k + 1].end());
				eleArray[k + 1].reserve(i * 2); // 再每次申请了内存之后，对于大小调整空间大小，以节省内存
				eleArray[k].clear();
			}
			else {
				eleArray[k + 1].insert(eleArray[k + 1].end(), eleArray[k].begin(), eleArray[k].end());
				sort(eleArray[k + 1].begin(), eleArray[k + 1].end());
				eleArray[k].clear();
			}
		}

		++k;
		i *= 2;
	}
	return;
}
/*支持动态二分查找的类，搜索操作*/
/*
参数：
	ele：目标元素
返回：
	first：所在的位数k
	second：所在位数的第几个数，从0~2^(k-1)
说明：
	如果不存在这样的元素，返回{-1, -1}
*/
pair<int, int> dynamicBinarySearchClass::search(int ele) {
	for (int i = 0; i < eleArray.size(); ++i) {
		if (eleArray[i].empty()) {
			continue;
		}
		int index = -1;
		if ((index = binary_search(eleArray[i], ele)) == -1) {
			continue;
		}
		else {
			return { i,index };
		}
	}
	return { -1,-1 };
}
/*对每一位数据序列的二分查找*/
/*
参数：
	eleSeq：目标可能所在的序列
	target：搜索的目标元素
返回：
	所在序列的下标
说明：
	如果在该序列中不存在该目标元素，返回-1
*/
int dynamicBinarySearchClass::binary_search(const vector<int>& eleSeq, const int& target) {
	int l = 0;
	int h = eleSeq.size() - 1;
	int m = (l + h) / 2;
	while (l < h) {
		if (eleSeq[m] < target) {
			l = m + 1;
		}
		else if (eleSeq[m] > target) {
			h = m;
		}
		m = (l + h) / 2;
	}

	if (eleSeq[m] == target) {
		return m;
	}
	return -1;
}

/*支持动态二分查找的类，删除操作*/
/*
参数：
	row：位数
	col：该位数中的元素位置
返回：
	删除是否成功的bool
说明：
	该函数需要先执行search操作找到元素位置后，在执行次操作，否则会报两个错误
*/
bool dynamicBinarySearchClass::erase(int row, int col) {
	if (row < 0 || col < 0) {
		std::cout << "Illegal position!" << endl;
		return false;
	}
	if (eleArray[row].empty() || eleArray[row].size() <= col) {
		std::cout << "This position have no elements, unauthorized access!" << endl;
		return false;
	}
	int k = 0;
	//1. Find the smallest j for which the array A_j with 2^j elements is full.Let y be	the last element of A_j .
	while (!(n & (0x00000001 << k))) {
		++k;
	}
	--n;// 因为要删除，所以减小n（元素个数）的值
	//2. Let x be in the array A_i.If necessary, find which array this is by using the	search procedure.
	//这步是由形参传入的待删除的目标元素的位置
	
	//3. Remove x from A_i and put y into A_i.Then move y to its correct place in A_i .
	eleArray[row][col] = eleArray[k].back();
	sort(eleArray[row].begin(), eleArray[row].end());


	//4. Divide A_j(which now has 2^j - 1 elements left) : The first element goes into
	//	array A_0, the next 2 elements go into array A1, the next 4 elements go into
	//	array A_2, and so forth.Mark array Aj as empty. The new arrays are created
	//	already sorted.
	int cnt = 1;
	int i = 0;
	while (i < k) {
		vector<int> buf(eleArray[k].begin() + cnt / 2, eleArray[k].begin() + cnt / 2 + cnt);
		//eleArray[k].erase(eleArray[k].begin(), eleArray[k].begin() + cnt);
		eleArray[i++] = buf;
		cnt *= 2;
	}
	// 这里做统一清空A_j的操作，包括在第2步中的将y放到A_i的正确位置和第4步中的将A_j设置为empty
	eleArray[k].clear();
	return true;
}

/********************************************************************************************************/

int amortizedAnalysis_main(int argc, char** argv) {
// 思考题17-1位逆序的二进制计数器测试
	//uint32_t res = 0;
	//for (int i = 0; i < 10; ++i) {
	//// 测试逆序递增
	//	//cout << dec << (res >> 28) << endl;	// 这里的右移只是为了让输出好看一些，去掉了不必要的0
	//	//res = bitReversedIncrement(res);
	//// 测试递增
	//	cout << dec << res << endl;
	//	res = increment(res);
	//}
// 思考题17-2动态二分查找的测试
	dynamicBinarySearchClass dBS;
	// 测试插入
	for (uint32_t i = 31; i >= 1; --i) {
		dBS.insert(i);
	}
	// 查找测试
	auto searchRes = dBS.search(1023);
	// 测试删除，这里注意，一定要配合search函数来使用，否则会出现非法访问
	for(int i = 0; i < 15; ++i)
		dBS.erase(3, 5);

	return 0;
}