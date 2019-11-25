#include <iostream>
#include <fstream>
// 类模板的蜜汁操作，如果要实现分离编译，需要包含.cpp文件
#include "Btree.h"
#include "Btree.cpp"

using namespace std;

int main() {
	// 读测试文件数据
	ifstream input("Btree.txt");
	char buf;
	vector<char> cv;
	while (input >> buf) {
		cv.push_back(buf);
	}

	Btree<char> bt(3);
	// 测试插入函数
	for (auto c : cv) {
		bt.insert(c);
	}
	// 测试前驱
	auto pre = bt.pred('C');
	// 测试后继
	auto suc = bt.succ('I');

	// 测试输出函数
	bt.print();

	// 测试删除函数
	for (auto ri = cv.rbegin(); ri != cv.rend(); ++ri) {
		bt.erase(*ri);
	}
	bt.erase('A'); // 删除不存在的元素

	return 0;
}

