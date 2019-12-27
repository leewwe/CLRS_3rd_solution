#include <iostream>
#include "..\head\vEB.hh"
#include <cmath>

int vanEmdeBoas_main() {
	// 测试构建函数
	vEB V(16);
	// 测试插入函数
	for (int i = 0; i < 16; ++i) {
		V.insert(i);
	}
	// 测试插入已经再树中的元素
	V.insert(3);
	auto isExist = V.member(3);
	V.erase(3);
	V.erase(2);
	// 测试删除不在树中的元素
	V.erase(2);
	// 测试后继函数
	auto succ = V.successor(10);
	// 测试存在与否函数
	isExist = V.member(3);
	// 测试前驱
	auto pred = V.predecessor(4);
	pred = V.predecessor(10);
	return 0;
}


