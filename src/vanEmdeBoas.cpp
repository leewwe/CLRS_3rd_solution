#include <iostream>
#include "vEB.h"
#include <cmath>

int main() {
	vEB V(16);
	V.insert(2);
	V.insert(3);
	V.insert(4);
	V.insert(5);
	V.insert(7);
	V.insert(14);
	V.insert(15);
	auto isExist = V.member(3);
	V.erase(3);
	auto succ = V.successor(10);
	isExist = V.member(3);
	auto pred = V.predecessor(4);
	pred = V.predecessor(10);
	return 0;
}


