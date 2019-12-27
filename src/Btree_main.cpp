#include <iostream>
#include <fstream>
#include <algorithm>
// 类模板的蜜汁操作，如果要实现分离编译，需要包含.cpp文件
#include "..\head\Btree.hh"
#include "Btree.cpp"

using namespace std;

int btree_main() {
	// 读测试文件数据
	ifstream input("Btree.txt");
	char buf;
	vector<char> cv;
	while (input >> buf) {
		cv.push_back(buf);
	}

	Btree<char> bt;
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
	cout << endl;
	// 测试删除函数
	for (auto ri = cv.rbegin(); ri != cv.rend(); ++ri) {
		bt.erase(*ri);
	}
	bt.erase('A'); // 删除不存在的元素
	
	// 测试连接
	sort(cv.begin(), cv.end());
	const int insertIndex = 10;
	for (auto i = 0; i != insertIndex; ++i) {
		bt.insert(cv[i]);
	}
	//bt.print();
	cout << endl;
	Btree<char> bt1;
	for (auto i = insertIndex + 1; i != cv.size(); ++i) {
		bt1.insert(cv[i]);
	}
	//bt1.print();
	
	//auto res = bt.join(bt1, cv[insertIndex]);	// 成员函数版
	auto rettree = join<char>(bt, bt1, cv[insertIndex]);	// 友元函数版
	return 0;
}

