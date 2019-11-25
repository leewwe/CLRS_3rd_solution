#include <iostream>
#include <fstream>
// ��ģ�����֭���������Ҫʵ�ַ�����룬��Ҫ����.cpp�ļ�
#include "Btree.h"
#include "Btree.cpp"

using namespace std;

int main() {
	// �������ļ�����
	ifstream input("Btree.txt");
	char buf;
	vector<char> cv;
	while (input >> buf) {
		cv.push_back(buf);
	}

	Btree<char> bt(3);
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

	// ����ɾ������
	for (auto ri = cv.rbegin(); ri != cv.rend(); ++ri) {
		bt.erase(*ri);
	}
	bt.erase('A'); // ɾ�������ڵ�Ԫ��

	return 0;
}

