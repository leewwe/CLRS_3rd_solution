// rbTree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "RB_Tree.h"
using namespace std;


int rbTree_main()
{
	RB_Tree rbt;

	for (int i = 6; i >= 0; --i) {
		rbt.RB_insert(i);
	}

	auto ret = rbt.RB_search(7);
	rbt.RB_delete(5);
	return 0;
}


