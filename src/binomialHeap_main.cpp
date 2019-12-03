#include <iostream>

using namespace std;

#include "binomialHeap.h"

int binomialHeap_main(int argc, char** argv) {

	// 定义两个空的二项堆
	binomialHeap<char> bh3(-128);
	binomialHeap<char> bh4(-128);
	
	//bh3.insert('Z');
	// 测试抽取最小值函数在空堆下成立与否
	bh3.extractMin();

	// 测试空的二项堆合并
	bh3.merge(bh4);
	// 测试插入函数
	for (int i = 0; i < 10; ++i) {
		bh3.insert('A' + i);
	}

	for (int i = 0; i < 10; ++i) {
		bh4.insert('P' + i);
	}
	// 测试抽取最小关键字函数
	auto retV3 = bh3.extractMin();
	auto retV4 = bh4.extractMin();
	// 测试搜索函数
	auto searchRes = bh3.search('O');
	// 测试关键字键值函数
	bh4.decreaseKey('W', 'A');
	// 测试友元合并函数
	auto resH = unite(bh3, bh4);
	// 测试删除函数
	resH.erase('A');

	
	return 0;
}