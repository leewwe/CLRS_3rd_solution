#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <tuple>


// 部分c代码的头文件和警告去除
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

using namespace std;

// 按起点视角排序
bool startTimeSort(pair<int, int> a, pair<int, int> b) {
	return a.first < b.first ? true : false;
}

/* 练习题16.1-1的动态规划解法 */
/*
参数：
	a：时间表
返回
	c：记录区间S[i,j]的最大活动数
	act：记录回溯打印输出的路径
说明：
	传入的时间表需要在头尾分别加上a[0] = {0,0}和 a[n+1] = {∞,∞}两个虚假时间
*/
pair<vector<vector<int>>, vector<vector<int>>> activitySelector_dp(const vector<pair<int, int>>& a) {
	int n = a.size()- 2;
	vector<vector<int>> c(n + 2, vector<int>(n + 2, 0));
	vector<vector<int>> act(n + 2, vector<int>(n + 2, 0));
	for (int len = 2; len <= n + 1; ++len) {
		for (int i = 0; i <= n - len + 1; ++i) {
			int j = i + len;
			int k = j - 1;
			while (a[i].second < a[k].second) {
				if((a[i].second <= a[k].first) && a[k].second <= a[j].first && c[i][k] + c[k][j] + 1 > c[i][j])
				{
					c[i][j] = c[i][k] + c[k][j] + 1;
					act[i][j] = k;
				}
				--k;
			}
		}
	}

/*debug_info*/
	//for (auto a : c) {
	//	for (auto b : a) {
	//		cout << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : act) {
	//	for (auto b : a) {
	//		cout << b << ' ';
	//	}
	//	cout << endl;
	//}

	return { c, act };
}

/* 动态规划算法回溯输出一种可能的最大活动方案 */
/*
参数：
	c：记录区间S[i,j]的最大活动数的矩阵
	act：记录回溯打印输出的路径矩阵，选取的是S[i,j]中的哪个k
	a：时间表
	i,j：要打印的区间
返回：
	无
说明：
	i的参数只能填0，n+1，n是没有加如两个虚假活动时的值（即，如果填入了两个虚假活动，那么就填入n - 1）
*/
void printActivities(const vector<vector<int>>& c, const vector<vector<int>>& act, const vector<pair<int, int>>& a, const int i, const int j) {
	if (c[i][j] > 0) {
		int k = act[i][j];
		printActivities(c, act, a, i, k);
		printActivities(c, act, a, k, j);
		cout << k << ": {" << a[k].first << "," << a[k].second << "}" << endl;
	}
}

/************************************************************************************************************************/
/* 活动选择，尾递归写法 */
/*
参数：
	a：活动时间表，需要按照结束时间排好序，且添加好虚假活动a[-1] = {0,0}
	k：上次选择的活动
	ret：存储最终的结果
返回：
	无
说明：
	下标从0开始
*/
void greedyActivitySelector_rec(const vector<pair<int, int>>& a, const int& k, vector<pair<int, int>>& ret) {
	int m = k + 1;
	int n = a.size();
	auto comEnd = a[k].second;
	while (m < n && a[m].first < comEnd) {
		++m;
	}
	if (m < n) {
		ret.push_back(a[m]);
		greedyActivitySelector_rec(a, m, ret);
	}
	return;
}
/* 活动选择，迭代写法， */
/*
参数：
	a：活动时间表，需要按照结束时间排好序，无需要添加任何虚假活动，减少边界的测试逻辑
	k：上次选择的活动
	ret：存储最终的结果
返回：
	无
说明：
	下标从0开始，ret中存的是具体的活动
*/
void greedyActivitySelector(const vector<pair<int, int>>& a, vector<pair<int, int>>& ret) {
// 自己想的，根据上面的尾递归算法改的
	//int m = k + 1;
	//int n = a.size();
	//int comEnd = a[k].second;
	//while (m < n) {
	//	if (a[m].first < comEnd) {
	//		++m;
	//	}
	//	else {
	//		ret.push_back(a[m]);
	//		comEnd = a[m++].second;
	//	}
	//}

// 书上的算法，这个输入无需虚拟的活动{0,0}
	//ret.push_back(a[0]);
	//auto comEnd = a[0].second;
	//for (int m = 1; m < a.size(); ++m) {
	//	if (a[m].first >= comEnd) {
	//		ret.push_back(a[m]);
	//		comEnd = a[m].second;
	//	}
	//}
// 进一步简化逻辑
	decltype(a[0].second) comEnd = INT_MIN;
	for (auto c : a) {
		if (c.first >= comEnd) {
			ret.push_back(c);
			comEnd = c.second;
		}
	}
}
/* 活动选择，迭代写法， */
/*
参数：
	a：活动时间表，需要按照结束时间排好序，无需要添加任何虚假活动，减少边界的测试逻辑
	k：上次选择的活动
	ret：存储最终的结果
返回：
	无
说明：
	下标从0开始，ret中存的是活动的下标
*/
void greedyActivitySelector(const vector<pair<int, int>>& a, vector<int>& ret) {
	decltype(a[0].second) comEnd = INT_MIN;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i].first >= comEnd) {
			ret.push_back(i);
			comEnd = a[i].second;
		}
	}
}

/* 活动选择，迭代写法， */
/*
参数：
	a：活动时间表，需要按照结束时间排好序（按最晚开始时间），无需要添加任何虚假活动，减少边界的测试逻辑
	k：上次选择的活动
	ret：存储最终的结果
返回：
	无
说明：
	和上面的写法一样，只不过是选择最晚开始时间
*/
void greedyActivitySelector_reverse(const vector<pair<int, int>>& a, vector<pair<int, int>>& ret) {
	int startTime = a.back().first;
	for (auto iter = a.rbegin(); iter != a.rend(); ++iter) {
		if (iter->second <= startTime) {
			startTime = iter->first;
			ret.push_back(*iter);
		}
	}
}
/*读取数据*/
/*
参数：
	a：时间点序列
	fileName：读取文件的名字
	isContainFictitious：是否要在头尾添加虚拟点，默认不添加节点
		0->不添加虚拟节点，迭代程序
		1->只在头添加添加虚拟节点，递归
		2->在头尾都添加虚拟节点，DP
返回：
	无
*/
void getPoints(vector<pair<int, int>>& a, const string& fileName, const int& isContainFictitious = 0) {
	string begin;
	string end;
	ifstream fin(fileName);
	if (isContainFictitious) {
		a.push_back({ -1,-1 });
	}
	while (fin >> begin >> end) {
		int start_time = atoi(begin.c_str());
		int end_time = atoi(end.c_str());
		a.push_back({start_time, end_time});
	}
	if (isContainFictitious == 2) {
		a.push_back({ INT_MAX, INT_MAX });
	}
	fin.close();
}
/************************************************************************************************************************/
/* C语言版本, https://www.cnblogs.com/null00/archive/2012/05/15/2499863.html , 原版程序是方法二的代码 */
/* 使用的结构体，这实际上是个C++结构体 */
typedef struct point_t {
	point_t(int time_t, int is_start_t, int end_time_t)
	: time(time_t), is_start(is_start_t), end_time(end_time_t){}
	point_t() = default;
	int time;
	int is_start;
	int end_time;//若is_start为1，end_time写对应的时间；若is_start为0，end_time为-1
} point;

/*C语言的比较函数*/
//升序排列，若时间相同，则为终止时间的时间点排在前面
int compare_c(const void* a, const void* b) {
	if ((*(point*)a).time != (*(point*)b).time)
		return (*(point*)a).time > (*(point*)b).time;
	else
		return (*(point*)a).is_start < (*(point*)b).is_start;//这里得用小于
}
/* 处理函数 */
/*
参数：
	points：接收数据指针
	n：时间点的个数
返回：
	无
*/
void process_c(point* points, const int n) {
	//排序
	qsort(points, n, sizeof(point), compare_c);
	//最多n/2个教室
	vector<int> classrooms(n / 2);
	int count = 0;
	classrooms[count++] = points[0].end_time;
	printf("[%d, %d)在教室%d\n", points[0].time, points[0].end_time, count);
	int i;
	int j;
	for (i = 1; i < n; i++) {
		if (points[i].is_start == 1) {
			for (j = 0; j < count; j++) {
				if (classrooms[j] <= points[i].time) {
					classrooms[j] = points[i].end_time;
					printf("[%d, %d)在教室%d\n", points[i].time, points[i].end_time, j + 1);
					break;
				}
			}
			if (j == count) {
				classrooms[count++] = points[i].end_time;
				printf("[%d, %d)在教室%d\n", points[i].time, points[i].end_time, count);
			}
		}
	}
	printf("总共需要%d个教室.\n", count);
}
/* 读取数据函数 */
/*
参数：
	fileName：读取的文件名字符串
	n：用于存储返回的时间点个数
返回：
	points：数据数组指针
说明：
	这个函数内部使用了malloc，在不需要数据的时候需要注意释放和反复释放
*/
point* getPoints(const string& fileName, int* n) {
	ifstream fin(fileName);
	string start;
	
	fin >> start;
	if (start.empty()) {
		return NULL;
	}
	auto rows = atoi(start.c_str());
	point* points = (point*)malloc(2 * rows * sizeof(point));
	*n = rows;
	string end;
	while (fin >> start >> end) {
		rows--;
		int id = *n - rows - 1;
		int start_time = atoi(start.c_str());
		int end_time = atoi(end.c_str());
		point p1;
		p1.is_start = 1;
		p1.time = start_time;
		p1.end_time = end_time;
		points[2 * id] = p1;
		point p2;
		p2.is_start = 0;
		p2.time = end_time;
		p2.end_time = -1;
		points[2 * id + 1] = p2;
	}

	fin.close();
	return points;
}
/* 下面的程序是将上面的c语言程序修改为了C/C++混编，并做了略微的优化，节省了更多的内存 */
//每个时间点；是起始时间，还是终止时间；及其对应的结束时间
// 没有使用指针，结果就是更加安全了，无需考虑内存泄漏和释放无效指针
bool compare_c_cpp(const point& a, const point& b) {
	if (a.time != b.time) {
		return a.time < b.time;
	}
	else {
		return a.is_start > b.is_start;
	}
}
void process_c_cpp(vector<point>& points) {
	if (points.empty()) {
		return;
	}
	int n = points.size();
	sort(points.begin(), points.end(), compare_c_cpp);
	vector<int> classrooms; // (n / 2);
	int count = 1;
	classrooms.push_back(points[0].end_time);
	printf("[%d, %d)在教室%d\n", points[0].time, points[0].end_time, count);
	for (int i = 1; i < n; i++) {
		if (points[i].is_start == 1) {
			int j = 0;
			for (; j < count; j++) {
				if (classrooms[j] <= points[i].time) {
					classrooms[j] = points[i].end_time;
					printf("[%d, %d)在教室%d\n", points[i].time, points[i].end_time, j + 1);
					break;
				}
			}
			if (j == count) {
				classrooms.push_back(points[i].end_time);
				++count;
				printf("[%d, %d)在教室%d\n", points[i].time, points[i].end_time, count);
			}
		}
	}
	printf("总共需要%d个教室.\n", count);
}
void getPoints(vector<point>& points, const string& fileName) {
	string begin;
	string end;
	ifstream fin(fileName);

	while (fin >> begin >> end) {
		int start_time = atoi(begin.c_str());
		int end_time = atoi(end.c_str());


		point p1(start_time, 1, end_time);
		point p2(end_time, 0, -1);
		points.push_back(p1);
		points.push_back(p2);
	}
	fin.close();
}
/* 以下为借鉴思路，重新定义数据结构，可以输出对应的活动的下标 */
/* 第一版程序 */
/*
时间点结构体：
	isBegin ： 1 -> 代表是开头的时间， 0 -> 代表是结尾的时间
	id：对应的原始的活动编号的下标
	time：时间点
*/
struct Point {
	Point(int isBegin_t, int id_t, int time_t)
		: isBegin(isBegin_t), id(id_t), time(time_t)
	{}
	Point() = default;
	int isBegin;	
	int id;			
	int time;
};
// sort排序需要用到的谓词
// 处理了当时间点相同的时候时间终点应该在起点的情况
bool compare(const Point& a, const Point& b) {
	if (a.time != b.time) {
		return a.time < b.time;
	}
	else {
		return a.isBegin < b.isBegin;
	}
}
/*处理函数*/
/*
参数：
	points：传入的时间点序列，需要保持先起点后终点的顺序
返回：
	无
说明：
	基本思路是
*/
void process_cpp(const vector<Point> points) {
	if (points.empty()) {
		return;
	}
	auto points_buf = points;
	sort(points_buf.begin(), points_buf.end(), compare);
	int n = points.size();
	vector<int> classroom;
	classroom.push_back(points[points_buf[0].id * 2 - 1].time);
	cout << setw(3) << points_buf[0].id << " [" << setw(2) << points_buf[0].time << ","
		<< setw(2) << points[points_buf[0].id * 2 - 1].time << ")"
		<< " classroom " << setw(2) << classroom.size() << endl;
	for (int i = 1; i < n; ++i) {
		if (points_buf[i].isBegin) {
			int j = 0;
			while (j < classroom.size()) {
				if (classroom[j] <= points_buf[i].time) {
					classroom[j] = points[points_buf[i].id * 2 - 1].time;
					cout << setw(3) << points_buf[i].id << " [" << setw(2) << points_buf[i].time << ","
						<< setw(2) << points[points_buf[i].id * 2 - 1].time << ")"
						<< " classroom " << setw(2) << j + 1 << endl;
					break;
				}
				++j;
			}
			if (j == classroom.size()) {
				classroom.push_back(points[points_buf[i].id * 2 - 1].time);
				cout << setw(3) << points_buf[i].id << " [" << setw(2) << points_buf[i].time << ","
					<< setw(2) << points[points_buf[i].id * 2 - 1].time << ")"
					<< " classroom " << setw(2) << j + 1 << endl;
			}
		}
		int j = 0;
	}
	cout << "all need " << classroom.size() << " classrooms" << endl;
}

/* 从文件获取活动时间节点的函数 */
/*
参数：
	points：对应于按照时间节点先起点后终点的时间要求
	fileName：对应的数据文件
返回：
	无
*/
void getPoints(vector<Point>& points, const string& fileName) {
	string begin;
	string end;
	int i = 1;
	ifstream fin(fileName);

	while (fin >> begin >> end) {
		int beginTime = atoi(begin.c_str());
		int endTime = atoi(end.c_str());

		Point p1(1, i, beginTime);
		Point p2(0, i, endTime);
		points.push_back(p1);
		points.push_back(p2);
		++i;
	}
	fin.close();
}

/*进一步优化*/
// 两个比较函数
// compareTime是按照时间点排序
bool compareTime(const Point& a, const Point& b) {
	return a.time < b.time;
}
// compareID是按照id号排序
bool compareID(const pair<int, int>& a, const pair<int, int>& b) {
	return a.first < b.first;
}

// {[1, 4), [2, 5), [6, 7), [4, 8)}，以这个例子做推演，书中的原版算法需要将活动的起始时间和终止时间都填入到classroom中，
// 但实际上，只需要填入终止时间就可以了，因为同一个活动的终止时间永远不可能比开始时间早（画个图一目了然）
/*
参数：
	points：pair的first对应于起点，pair的second对应于终点
	activity_classroom_pair：返回的参数用来打印输出对应的活动编号和对应所应该在的教室
返回：
	所需要的教室的数目
说明：
	内部会对时间节点进行重新排序；输出的节点会按照id号排序号，以便输出的时候更美观
*/
int assignClassroom(const vector<pair<Point, Point>>& points, vector<pair<int, int>>& activity_classroom_pair) {
	vector<Point> points_seq;
	// 接收数据，只接收起点，因为对应的终点可以通过起点中包含的id计算出来
	for (auto c : points) {
		points_seq.push_back(c.first);
	}
	// 排序
	sort(points_seq.begin(), points_seq.end(), compareTime);
	vector<int> endTime;

	endTime.push_back(points[points_seq[0].id - 1].second.time);
	activity_classroom_pair.push_back({ points_seq[0].id, 1 });

	// 遍历整个开始时间序列
	for (auto iter = points_seq.begin() + 1; iter != points_seq.end(); ++iter) {
		auto begin = iter->time;
		int i = 0;
		// 遍历整个已有活动的教室
		for (; i < endTime.size(); ++i) {
			// 当开始时间序列大于或等于某个教室中的时间的时候，证明在该教室中的这个活动已经结束了，
			if (begin >= endTime[i]) {
				// 所以可以将当前活动的终止时间更新到这个教室中，即这里有新的活动，
				endTime[i] = points[iter->id - 1].second.time;
				activity_classroom_pair.push_back({ iter->id , i + 1 });
				break;
			}
		}
		// 如果所有已经有活动的教室在当前时间没有结束，我们需要另开一个教室去进行这个活动
		if (i == endTime.size()) {
			endTime.push_back(points[points_seq[i].id - 1].second.time);
			activity_classroom_pair.push_back({ iter->id , i + 1 });
		}
	}
	// 下面的代码是按照活动id号进行排序，为了打印好看
	sort(activity_classroom_pair.begin(), activity_classroom_pair.end(), compareID);
	return endTime.size();
}
/* 输出打印函数 */
/*
参数：
	points：对应的原始时间点
	activity_classroom_pair：返回的参数用来打印输出对应的活动编号和对应所应该在的教室
*/
void printActivityAssign(const vector<pair<Point, Point>>& points, 
	const vector<pair<int, int>>& activity_classroom_pair,
	const int& n 
	) {
	for (auto a : activity_classroom_pair) {
		cout << "No." << setw(2) << a.first
			<< " [" << setw(2) << points[a.first - 1].first.time << "," << setw(2 )<< points[a.first - 1].second.time << ")"
			<< " activity is in classroom No." << a.second << endl;
	}
	cout << "A total of " << n << " classrooms are required" << endl;
}
/* 从文件获取活动时间节点的函数 */
/*
参数：
	points：pair的first对应于起点，pair的second对应于终点
	fileName：对应的数据文件
返回：
	无
*/
void getPoints(vector<pair<Point, Point>>& points, const string& fileName) {
	string begin;
	string end;
	int i = 1;
	ifstream fin(fileName);

	while (fin >> begin >> end) {
		int beginTime = atoi(begin.c_str());
		int endTime = atoi(end.c_str());

		Point p1(1, i, beginTime);
		Point p2(0, i, endTime);
		points.push_back({ p1, p2 });
		++i;
	}
	fin.close();
}

/************************************************************************************************************************/
/* 练习题16.1-5带权值的活动选择，目标是最大权值 */
//没有太大改动，基本上就是在原来的基础上加了一维数据，原来的活动选择问题可以看作是权值全部都是1的特殊情况
/*
参数：
	a：三维的tuple数组
		1->代表起始时间
		2->代表终止时间
		3->代表权值
*/
pair<vector<vector<int>>, vector<vector<int>>> maxValueActivitySelector(const vector<tuple<int, int, int>>& a) {
	int n = a.size() - 2;
	vector<vector<int>> val(n + 2, vector<int>(n + 2, 0));
	vector<vector<int>> act(n + 2, vector<int>(n + 2, 0));
	for (int len = 2; len <= n + 1; ++len) {
		for (int i = 0; i <= n - len + 1; ++i) {
			int j = i + len;
			int k = j - 1;
			while (get<1>(a[i]) < get<1>(a[k])) {
				int tmp = val[i][k] + val[k][j] + get<2>(a[k]);
				if ((get<1>(a[i]) <= get<0>(a[k])) && get<1>(a[k]) <= get<0>(a[j]) && tmp > val[i][j])
				{
					val[i][j] = tmp;
					act[i][j] = k;
				}
				--k;
			}
		}
	}

	cout << "A maximum-value set of mutually \
compatible activities has value " << val[0].back() << endl;

	/*debug_info*/
	//for (auto a : val) {
	//	for (auto b : a) {
	//		cout << b << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (auto a : act) {
	//	for (auto b : a) {
	//		cout << b << ' ';
	//	}
	//	cout << endl;
	//}

	return { val, act };
}
/*打印函数*/
void printActivities(const vector<vector<int>>& val,
	const vector<vector<int>>& act, const vector<tuple<int, int, int>>& a, 
	const int i, const int j) {
	if (val[i][j] > 0) {
		int k = act[i][j];
		printActivities(val, act, a, i, k);
		printActivities(val, act, a, k, j);
		cout << setw(2) << k << ": [" << setw(2) <<get<0>(a[k]) << "," 
			<< setw(2) << get<1>(a[k]) << ")" 
			<< " val:" << setw(2) << get<2>(a[k]) <<endl;
	}
}

 /*读取数据函数*/
/*
参数：
	a：三维的tuple数组
		1->代表起始时间
		2->代表终止时间
		3->代表权值
	fileNam：读取文件的名字
返回：
	无
说明：
	需要添加虚拟点
*/
void getPoints(vector<tuple<int, int, int>>& a, const string& fileName) {
	string sBegin;
	string sEnd;
	string sVal;
	ifstream fin(fileName);
	a.push_back({ -1,-1,-1 });
	while (fin >> sBegin >> sEnd >> sVal) {
		int beginTime = atoi(sBegin.c_str());
		int endTime = atoi(sEnd.c_str());
		int val = atoi(sVal.c_str());
		a.push_back({ beginTime, endTime, val});
	}
	a.push_back({ INT_MAX, INT_MAX, -1 });
	fin.close();
}
/************************************************************************************************************************/
/*练习题16.2-2 0-1背包*/
/*
参数：
	val：价值序列
	weight：重量序列
	W：背包负重
返回：
	first：总价值矩阵，最终的c[n][W]代表最大的赃物价值
	second：辅助构造输出偷盗物编号的序列
*/
pair<vector<vector<int>>,vector<int>> DP_0_1_Knapsack(const vector<int>& val, const vector<int>& weight, const int W) {
	int n = val.size();
	vector<vector<int>> c(n + 1, vector<int>(W + 1, 0));
	vector<int> b(W + 1, -1);
	for (int i = 1; i <= n; ++i) {
		for (int w = 1; w <= W; ++w) {
			int tmp_w = weight[i - 1];
			if (tmp_w <= w) {
				if (val[i - 1] + c[i - 1][w - tmp_w] > c[i - 1][w]) {
					c[i][w] = val[i - 1] + c[i - 1][w - tmp_w];
					b[w] = i - 1;
				}
				else {
					c[i][w] = c[i - 1][w];
				}
			}
			else {
				c[i][w] = c[i - 1][w];
			}
		}
	}

	return { c,b };
}
/*
参数：
	val：价值序列
	weight：重量序列
	W：背包负重
返回：
	first：最大的赃物价值
	second：辅助构造输出偷盗物编号的序列
*/
pair<int, vector<int>> DP_0_1_Knapsack_spaceOptimal_1(const vector<int>& val, const vector<int>& weight, const int W) {
	int n = val.size();

	vector<int> curr_c(W + 1, 0);
	vector<int> pre_c(W + 1, 0);
	auto pCurr = &curr_c;
	auto pPre = &pre_c;
	vector<int> b(W + 1, -1);
	for (int i = 1; i <= n; ++i) {
		for (int w = 1; w <= W; ++w) {
			int tmp_w = weight[i - 1];
			if (tmp_w <= w) {
				if (val[i - 1] + (*pPre)[w - tmp_w] > (*pPre)[w]) {
					(*pCurr)[w] = val[i - 1] + (*pPre)[w - tmp_w];
					b[w] = i - 1;
				}
				else {
					(*pCurr)[w] = (*pCurr)[w];
				}
			}
			else {
				(*pCurr)[w] = (*pPre)[w];
			}
		}
		swap(pCurr, pPre);
	}

	return { pPre->back(), b};
}
/*
参数：
	weight：重量序列
	b；计算函数计算出的辅助输出序列
	W：背包最大承重
*/
void printStealSeq(const vector<int>& weight, const vector<int>& b, const int& W) {
	if (W < 0 || W > b.size() - 1 ||b[W] < 0) {
		return;
	}
	else {
		printStealSeq(weight, b, W - weight[b[W]]);
		cout << "The thief should steal the No." << b[W] << " goods" << endl;
	}
}

/************************************************************************************************************************/
/*练习题16.2-4穿越北达科塔州的最小补水次数*/
/*
参数：
	dist：距离序列
	M：教授一次补水可走的平均最大距离
返回：
	补水点的下标（从1开始）
*/
vector<int> minNumOfRefill(const vector<double>& dist, const double& M) {
	vector<int> refillPos;
	double resDist = M;

	int i = 0;
	while (i < dist.size()) {
		if (dist[i] > resDist) {
			refillPos.push_back(i);
			resDist = M;
			continue;
		}
		resDist -= dist[i];
		++i;
	}
	return refillPos;
}

/************************************************************************************************************************/
/*练习题16.2-5寻找最小的单位区间数目*/
/*
参数：
	points：点集序列
返回：
	map
		key：区间起点的值
		value：区间终点的值
*/
map<double, double> minNumOfInterval(const vector<double>& points) {
	if (points.empty()) {
		return map<double, double>();
	}
	auto sortedPoints{ points };
	sort(sortedPoints.begin(), sortedPoints.end());
	
	map<double, double> ret;
	double startBorder = sortedPoints[0];
	double endBorder= startBorder + 1;
	int i = 0;
	for (; i < sortedPoints.size(); ++i) {
		if (sortedPoints[i] > endBorder) {
			ret.emplace(startBorder, sortedPoints[i - 1]);
			startBorder = sortedPoints[i];
			endBorder = startBorder + 1;
		}
	}
	ret.emplace(startBorder, sortedPoints[i - 1]);
	return ret;
}

int main_greedyAlg(int argc, char** argv){
/*活动选择测试*/
	vector<pair<int, int>> a_table;
	//DP写法测试
	//getPoints(a_table, "activityTime.txt", 2);
	//auto res = activitySelector_dp(a_table);
	//printActivities(res.first, res.second, a_table, 0, a_table.size() - 1);
	
	//尾递归
	//getPoints(a_table, "activityTime.txt", 1);
	//vector<pair<int, int>> ret;
	//greedyActivitySelector_rec(a_table, 0, ret);
	
	//迭代测试
	//vector<pair<int, int>> ret;
	//getPoints(a_table, "activityTime.txt");
	//greedyActivitySelector(a_table, ret);

	//迭代测试，这个重载函数会返回活动下标 
	//vector<int> ret1;
	//getPoints(a_table, "activityTime.txt");
	//greedyActivitySelector(a_table, ret1);

	//测试以“最晚开始”的场景
	//getPoints(a_table, "activityTime.txt");
	//sort(a_table.begin(), a_table.end(), startTimeSort);
	//vector<pair<int, int>> ret;
	//greedyActivitySelector_reverse(a_table, ret);

/*练习题16.1-4区间图着色(interval-graph coloring)问题测试*/
	/*C语言程序测试，对于所有的函数都做了封装*/
	//int n = 0;
	//// 注意，这里getPoints内部调用了malloc，应该做有效性检测
	//point* points = getPoints("activityTime_c.txt", &n);
	//if (!points) {
	//	return -1;
	//}
	//process_c(points, 2 * n);
	//free(points);

	/*C/C++混编程序测试*/
	//vector<point> points;
	//getPoints(points, "activityTime.txt");
	//process_c_cpp(points);

	/*第一版程序的测试*/
	//vector<Point> points;
	//getPoints(points, "activityTime.txt");
	//process_cpp(points);
	
	/*优化版程序的测试*/
	//vector<pair<Point, Point>> points;
	//getPoints(points, "activityTime.txt");
	//vector<pair<int, int >> activity_classroom_pair;
	//auto n = assignClassroom(points, activity_classroom_pair);
	//printActivityAssign(points, activity_classroom_pair, n);

/*练习题16.1-5测试，最大和问题，这个必须用DP来做*/
	//vector<tuple<int, int, int>> a;
	//getPoints(a, "activityTime_weight.txt");
	//auto res =  maxValueActivitySelector(a);
	//printActivities(res.first, res.second, a, 0, a.size() - 1);

/*练习题16.2-2测试*/
	// vector<int> v{ 60,100,120 };
	// vector<int> w{ 10,20,30 };
	// DP_0_1_Knapsack(v, w, 50);
	// auto res = DP_0_1_Knapsack_spaceOptimal_1(v, w, 50);
	// printStealSeq(w, res.second, 50);
	
/*练习题16.2-4测试*/
	// vector<double> dist{ 3, 4, 2, 8, 2, 7, 2, 8, 4, 9 };
	// auto res = minNumOfRefill(dist, 10);

/*练习题16.2-5测试*/
	vector<double> points{ 1.9, 2.6, 3.4, 4.4, 8.3, 9.1, 1.4, 6.7, 9.4 };
	auto res = minNumOfInterval(points);
	return 0;
}


