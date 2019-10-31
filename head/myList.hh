#pragma once
#ifndef _MY_LIST_HH_
#define _MY_LIST_HH_
#include <iostream>
using namespace std;
//三个均为全局变量，其中PREV在各个函数过程中永远指向
//当前节点的前驱，FIRST则指向PREV的前驱，NEXT则指向当前节点的后继
unsigned long FIRST = 0, NEXT = 0, PREV = 0;

struct node {//节点类
	int data;
	unsigned long np;
	node(const int& d = int()) :data(d), np(0) {}
};

class list {//链表类
private:
	node* head;//头尾指针
	node* tail;
public:
	list() :head(nullptr), tail(nullptr) {}
	void insert(const int&);
	void create();
	void erase(const int&);
	node* locate(const int&);
	void seTraversal();//正向遍历
	void reTraversal();//反向遍历
	void edit(const int&, const int&);
	size_t size();
	bool empty() { return head == nullptr; }
};


#endif