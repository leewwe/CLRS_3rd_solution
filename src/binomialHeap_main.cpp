#include <iostream>

using namespace std;

#include "binomialHeap.h"

int binomialHeap_main(int argc, char** argv) {

	// ���������յĶ����
	binomialHeap<char> bh3(-128);
	binomialHeap<char> bh4(-128);
	
	//bh3.insert('Z');
	// ���Գ�ȡ��Сֵ�����ڿն��³������
	bh3.extractMin();

	// ���ԿյĶ���Ѻϲ�
	bh3.merge(bh4);
	// ���Բ��뺯��
	for (int i = 0; i < 10; ++i) {
		bh3.insert('A' + i);
	}

	for (int i = 0; i < 10; ++i) {
		bh4.insert('P' + i);
	}
	// ���Գ�ȡ��С�ؼ��ֺ���
	auto retV3 = bh3.extractMin();
	auto retV4 = bh4.extractMin();
	// ������������
	auto searchRes = bh3.search('O');
	// ���Թؼ��ּ�ֵ����
	bh4.decreaseKey('W', 'A');
	// ������Ԫ�ϲ�����
	auto resH = unite(bh3, bh4);
	// ����ɾ������
	resH.erase('A');

	
	return 0;
}