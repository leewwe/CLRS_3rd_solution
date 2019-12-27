#include <iostream>
#include <utility>
#include "..\head\fibHeap.hh"





int fibHeap_main() {
	// 测试插入函数
	fibHeap<int> fh1(INT_MIN);
	fh1.insert(1);
	fh1.insert(2);
	fh1.insert(3);

	// 测试合并函数
	fibHeap<int> fh2(INT_MIN);
	fh2.insert(4);
	fh2.insert(5);
	fh2.insert(6);
	auto fh = fibHeapUnion(fh1, fh2);	


	for (int i = 7; i <= 15; ++i) {
		fh.insert(i);
	}
	// 测试抽取最小值函数（extractMin）
	auto min = fh.extractMin();
	min = fh.extractMin();
	
	// 测试查找函数
	auto res = fh.search(1);	// nullptr
	res = fh.search(15);		// 找到了
	res = fh.search(12);		// 找到了

	// 测试删除函数和关键字减值函数
	fh.erase(12);
	fh.erase(15);

	return 0;
}
