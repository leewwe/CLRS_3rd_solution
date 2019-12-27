#include <iostream>
#include <vector>
#include "..\head\RB_Tree.hh"
#include "RB_Tree.cpp" // 为解决模板分离编译
using namespace std;


int main()
{
	RB_Tree<int> rbt;

	for (int i = 6; i >= 0; --i) {
		rbt.RB_insert(i);
	}

	auto ret = rbt.RB_search(7);
	rbt.RB_delete(5);
	return 0;
}


