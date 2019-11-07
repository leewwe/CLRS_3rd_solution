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
/************************************************************************************************************************/
/*霍夫曼编码，贪心*/
// 霍夫曼编码的二叉树节点结构
/*
left：左孩子指针
right：有孩子指针
parent：父亲指针
val：first：对应的ASCII字符
	second：对应的频次
*/
struct huffmanNode {
	huffmanNode* left;
	huffmanNode* right;
	huffmanNode* parent;
	pair<char, int> val;
	huffmanNode() : left(NULL), right(NULL), parent(NULL), val({ -1,-1 }) {}
	huffmanNode(pair<char, int> v)
		: left(NULL), right(NULL), parent(NULL), val(v) {}
};

// 排序谓词
struct compareFre {
	constexpr bool operator()(const huffmanNode& lhs, const huffmanNode& rhs) {
		return lhs.val.second > rhs.val.second;
	}
};

/*
参数：
	C：字符频次表
返回：
	用来编码的树根指针
*/
huffmanNode* huffmanEncode(const map<char, int>& C) {
	if (C.empty()) {
		return NULL;
	}
	// 使用优先队列，按照频次进行排序
	priority_queue<huffmanNode, vector<huffmanNode>, compareFre> Q(C.begin(), C.end());
	int n = C.size();

	for (int i = 0; i < n - 1; ++i) {
		auto x = new huffmanNode(Q.top());
		Q.pop();
		auto y = new huffmanNode(Q.top());
		Q.pop();

		auto node = new huffmanNode;
		x->parent = node;
		y->parent = node;
		node->left = x;
		node->right = y;
		node->val.first = -1;
		node->val.second = x->val.second + y->val.second;

		Q.emplace(*node);
	}
	auto root = new huffmanNode(Q.top());
	return root;
}

/*输出函数的辅助函数*/
/*
参数：
	root：用来构造编码表的根指针
	res：用于返回的编码表
	str：中间传递的用的string
返回：
	无
*/
void outHuffmanEncode_aux(const huffmanNode* root, map<char, string>& res, string& str) {
	if (!root) {
		return;
	}
	// 判断根节点
	if (root->parent == NULL) {
	}
	// 判断非叶子节点
	else if (root->val.first == -1) {
		if (root->parent->left == root) {
			str += "0";
		}
		else {
			str += "1";
		}
	}
	// 叶子节点
	else {
		if (root->parent->left == root) {
			str += "0";
			res[root->val.first] += str;
		}
		else {
			str += "1";
			res[root->val.first] += str;
		}
		str.erase(str.end() - 1);
	}
	// 递归调用
	outHuffmanEncode_aux(root->left, res, str);
	outHuffmanEncode_aux(root->right, res, str);
	// 非叶子节点回溯时删除刚刚添加的节点
	if (root->left && root->right && root->parent) {
		str.erase(str.end() - 1);
	}
}
/*输出函数*/
/*
参数：
	root：用来构造编码表的根指针
	res：用于返回的编码表
返回：
	无
*/
void outHuffmanEncode(const huffmanNode* root, map<char, string>& res) {
	string str;
	outHuffmanEncode_aux(root, res, str);
}
/*统计文章的字符频次*/
/*
参数：
	filename：读取文件的名字
返回：
	字符频次表
*/
map<char, int> getChOcurrFreq(string fileName) {
	map<char, int> chFreqMap;
	char c = -1;
	ifstream fin(fileName);
	while (fin >> c) {
		chFreqMap[c]++;
	}

	return chFreqMap;
}
/************************************************************************************************************************/
/*拟阵求解任务调度问题*/
/*求解单位时间任务调度问题的数据结构*/
struct taskItem {
	taskItem(int id, int d, int w) : ID(id), ddl(d), weight(w) {}
	int ID;
	int ddl;
	int weight;
}; 
/*按weight排序的谓词*/
bool compareTaksWeight(const taskItem& a, const taskItem& b) {
	return a.weight > b.weight;
}
/*按deadline排序的谓词*/
bool compareTaskDDL(const taskItem& a, const taskItem& b) {
	return a.ddl < b.ddl;
}
/*判定是否满足拟阵条件（是否是独立的提前任务）的函数*/
/*
参数：
	earlyTaskTab：当前的提前任务列表
	wait2InsertTask：待检测的任务
返回：
	true：可以加入到提前列表
	false：不可以加入提前列表
说明：
	可以改进的
*/
bool isAddEarlyTask(vector<taskItem> earlyTaskTab, const taskItem& wait2InsertTask) {
	earlyTaskTab.push_back(wait2InsertTask);
	sort(earlyTaskTab.begin(), earlyTaskTab.end(), compareTaskDDL);
	for (int i = 0; i < earlyTaskTab.size(); ++i) {
		if (earlyTaskTab[i].ddl < i + 1) {
			return false;
		}
	}
	return true;
}

/*调度函数*/
/*
参数：
	taskTab：任务列表（函数会对其进行修改）
返回：
	first：提前的任务
	second：延后的任务
*/
pair<vector<int>, vector<int>> matroid4UnitTaskSchedule(vector<taskItem>& taskTab) {
	sort(taskTab.begin(), taskTab.end(), compareTaksWeight);
	
	vector<int> lateIDseq;
	vector<taskItem> earlyTaskTab{ taskTab[0], taskTab[1] };
	for (int i = 2; i < taskTab.size(); ++i) {
		if (isAddEarlyTask(earlyTaskTab, taskTab[i])) {
			earlyTaskTab.push_back(taskTab[i]);
		}
		else {
			lateIDseq.push_back(taskTab[i].ID);
		}
	}

	sort(earlyTaskTab.begin(), earlyTaskTab.end(), compareTaskDDL);
	vector<int> earlyIDseq;
	for (auto a : earlyTaskTab) {
		earlyIDseq.push_back(a.ID);
	}
	return { earlyIDseq,lateIDseq };
}
/*读取任务列表*/
/*
参数：
	taskTab：用于返回的任务列表
	fileName：要读取的文件
返回：
	无
*/
void getTaskTab(vector<taskItem>& taskTab, string fileName) {
	ifstream fin(fileName);
	int id = -1;
	int ddl = -1;
	int weight = -1;
	while (fin >> id >> ddl >> weight) {
		taskTab.push_back(taskItem(id, ddl, weight));
	}
}

/************************************************************************************************************************/
/*思考题16-4中对于检查独立性的改进，原来每次需要O(n)，改进之后需要o(n)*/
/*调度函数，优化独立想检查*/
/*
参数：
	taskTab：任务列表（函数会对其进行修改）
返回：
	first：提前的任务列表
	second：延后任务的开始位置（也就是提前任务之后）
*/
pair<vector<taskItem>, int> matroid4UnitTaskSchedule_optimal(vector<taskItem>& taskTab) {
	sort(taskTab.begin(), taskTab.end(), compareTaksWeight);

	vector<taskItem> taskSeq(taskTab.size());
	// 记录最后存入延后任务的位置（也就是提前任务之后）
	int lateStartPos = taskTab.size();
	for (int i = 0; i < taskTab.size(); ++i) {
		// 检查是否在该任务的ddl之前有空槽（检查拟阵是否可以扩展）
		int k = taskTab[i].ddl - 1;
		while(k >= 0) {
			if (taskSeq[k].ID < 0) {
				taskSeq[k] = taskTab[i];
				break;
			}
			--k;
		}
		// 如果检查完所有的ddl之前的任务没有空槽（证明拟阵无法完成扩展）
		if (k < 0) {
			for (int j = taskSeq.size() - 1; j >= taskTab[i].ddl; --j) {
				if (taskSeq[j].ID < 0) {
					taskSeq[j] = taskTab[i];
					--lateStartPos;
					break;
				}
			}
		}
	}
	// 按照ID号对所有的提前任务进行排序
	sort(taskSeq.begin(), taskSeq.begin() + lateStartPos, compareTaskDDL);

	return { taskSeq, lateStartPos };
}

/************************************************************************************************************************/
/*思考题16-1贪心解法*/
/*
参数：
	coins：按面值从小到大排序好的面值序列
	n：需要兑换的钱数
返回：
	每种面值的需要的个数
*/
vector<int> coinsChange_greedy(const vector<int>& coins, int n) {
	vector<int> ret(coins.size());
	for (int i = coins.size() - 1; i >= 0 ; --i) {
		ret[i] = n / coins[i];
		n %= coins[i];
	}
	return ret;
}
/*思考题16-1DP解法*/
/*
参数：
	coins：按面值从小到大排序好的面值序列
	n：需要兑换的钱数
返回：
	first：j美分需要的硬币个数
	second：j美分最后需要的硬币面值
*/
pair<vector<int>, vector<int>> coinsChange_DP(const vector<int>& coins, int n) {
	vector<int> c(n + 1, 0);
	vector<int> denom(n + 1, 0);
	for (int j = 1; j <= n; ++j) {
		c[j] = INT_MAX;
		for (int i = 0; i < coins.size(); ++i) {
			// 这里写的很妙，首先验证了j - coins[i] >= 0，利用了&&的短接特性，
			if (j >= coins[i] && 1 + c[j - coins[i]] < c[j]) {
				c[j] = 1 + c[j - coins[i]];
				denom[j] = coins[i];
			}
		}
	}
	return { c, denom };
}
/*思考题16-1贪心算法的打印函数*/
/*
参数：
	nums：每种面值需要的个数
	coins：按面值从小到大排序好的面值序列
返回：
	无
*/
void print_greedy(const vector<int>& nums, const vector<int>& coins){
	int sumCoins = 0;
	cout << "找零方案为：" << endl;
	for (int i = 0; i < coins.size();++i) {
		if (nums[i]) {
			cout << coins[i] << "硬币需要" << nums[i] << "个" << endl;
			sumCoins += nums[i];
		}
	}
	cout << "总共需要" << sumCoins << "枚硬币" << endl;
}
/*思考题16-1DP算法给出找零方案的函数*/
/*
参数：
	j：需要兑换的钱数
	nums：每种面值需要的个数
	changeMethod：用于返回的找零方案
返回：
	无
*/
void giveChange_DP(int j, const pair<vector<int>,vector<int>>& c_denom, map<int,int>& changeMethod) {
	if (j > 0) {
		++changeMethod[c_denom.second[j]];
		giveChange_DP(j - c_denom.second[j], c_denom, changeMethod);
	}
}
/*思考题16-1DP算法找零方案的打印函数函数*/
/*
参数：
	j：需要兑换的钱数
	nums：每种面值需要的个数
	coins：按面值从小到大排序好的面值序列
返回：
	无
*/
void printChange_DP(int j, const pair<vector<int>, vector<int>>& c_denom, const vector<int>& coins) {
	// 计算出相应的找零方案
	map<int, int> changeMethod;
	for (auto a : coins) {
		changeMethod.emplace(a, 0);
	}
	giveChange_DP(j, c_denom, changeMethod);
	// 存储临时总的硬币个数
	int sumCoins = 0;
	cout << "找零方案为：" << endl;
	for (auto a : changeMethod) {
		if (a.second) {
			cout << "面值为" << a.first << "需要" << a.second << "枚" <<endl;
			sumCoins += a.second;
		}
	}
	cout << "总共需要" << sumCoins << "枚硬币" << endl;
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
	// vector<double> points{ 1.9, 2.6, 3.4, 4.4, 8.3, 9.1, 1.4, 6.7, 9.4 };
	// auto res = minNumOfInterval(points);
/*霍夫曼编码测试*/
	// // 读取数据，并构造相应的字符频次表
	// auto C = getChOcurrFreq("alpha.txt");

	// // 编码，返回一棵树的根指针
	// auto res = huffmanEncode(C);
	
	// // 编码表
	// map<char, string>ret;
	// outHuffmanEncode(res, ret);

	// for (auto a : ret) {
	// 	cout << a.first << "|" << a.second << endl;
	// }
/*用拟阵求解任务调度问题*/	
	//vector<taskItem> taskTab;
	//// 读取任务表，代编号的
	//getTaskTab(taskTab, "taskTab.txt");

	////// 测试练习题16.5-1
	////for (auto& a : taskTab) {
	////	a.weight = 80 - a.weight;
	////}
	////auto res = matroid4UnitTaskSchedule(taskTab);

	//// 思考题16-4，对于独立性检查的改进测试
	//auto res = matroid4UnitTaskSchedule_optimal(taskTab);

/*思考题16-1硬币找零测试*/
	// vector<int> coins{ 1,25,10, 5 };
	// // 贪心算法测试
	// sort(coins.begin(), coins.end());
	// auto res = coinsChange_greedy(coins, 30);
	// print_greedy(res, coins);
	// // DP算法测试
	// auto res1 = coinsChange_DP(coins, 30);
	// printChange_DP(30, res1, coins);
// 
	return 0;
}


