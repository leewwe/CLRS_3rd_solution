#include <iostream>
#include <fstream>
#include <algorithm>
// ��ģ�����֭���������Ҫʵ�ַ�����룬��Ҫ����.cpp�ļ�
#include "..\head\Btree.hh"
#include "Btree.cpp"

using namespace std;

int btree_main() {
	// �������ļ�����
	ifstream input("Btree.txt");
	char buf;
	vector<char> cv;
	while (input >> buf) {
		cv.push_back(buf);
	}

	Btree<char> bt;
	// ���Բ��뺯��
	for (auto c : cv) {
		bt.insert(c);
	}
	// ����ǰ��
	auto pre = bt.pred('C');
	// ���Ժ��
	auto suc = bt.succ('I');

	// �����������
	bt.print();
	cout << endl;
	// ����ɾ������
	for (auto ri = cv.rbegin(); ri != cv.rend(); ++ri) {
		bt.erase(*ri);
	}
	bt.erase('A'); // ɾ�������ڵ�Ԫ��
	
	// ��������
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
	
	//auto res = bt.join(bt1, cv[insertIndex]);	// ��Ա������
	auto rettree = join<char>(bt, bt1, cv[insertIndex]);	// ��Ԫ������
	return 0;
}

