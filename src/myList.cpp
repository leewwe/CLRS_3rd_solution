#include "..\head\myList.hh"

void list::insert(const int& t) {//采用尾插法
	NEXT = 0;
	node* p = new node(t);
	PREV = reinterpret_cast<unsigned long>(tail);
	if (head == nullptr)
	{//如果是第一个节点
		head = p;
		tail = p;
		p->np = PREV ^ NEXT;
	}
	else
	{
		tail = p;
		p->np = PREV ^ NEXT;
		node* q = reinterpret_cast<node*>(PREV);
		q->np = q->np ^ reinterpret_cast<unsigned long>(p);
	}
}

void list::create() {
	size_t N;
	int t;
	cout << "Enter the number of nodes: ";
	cin >> N;
	cout << "Enter " << N << " element(s)" << endl;
	for (int i = 0; i != N; ++i)
	{
		cin >> t;
		insert(t);
	}
}

node* list::locate(const int& t) {
	PREV = 0;
	node* p = head, * r = head;
	while (p != nullptr && p->data != t)
	{
		p = reinterpret_cast<node*>(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
	}
	return p;//返回的p要么是要查找的节点，要么为空表示没有此节点
}

void list::erase(const int& t) {
	node* p = locate(t), * r;
	if (p == nullptr)
	{//没有此节点
		cout << t << " isn't exist!" << endl;
		return;
	}
	r = p;//记下被删节点
	NEXT = PREV ^ p->np;//PREV已在查找中设置为当前节点的前驱
	unsigned long ulp = reinterpret_cast<unsigned long>(p);
	if (p == head && p == tail) head = tail = nullptr;//若链表只有一个节点
	else if (p == head)
	{//若为头结点
		p = reinterpret_cast<node*>(NEXT);
		p->np = PREV ^ ulp ^ p->np;
		head = p;
	}
	else if (p == tail)
	{//若为尾节点
		p = reinterpret_cast<node*>(PREV);
		p->np = p->np ^ ulp ^ NEXT;
		tail = p;
	}
	else
	{//一般情况
		p = reinterpret_cast<node*>(PREV);
		p->np = p->np ^ ulp ^ NEXT;
		p = reinterpret_cast<node*>(NEXT);
		p->np = PREV ^ ulp ^ p->np;
	}
	delete r;
}

void list::edit(const int& old, const int& New) {
	node* p = locate(old);
	if (p == nullptr)
		cout << " isn't exist!" << endl;
	else p->data = New;
}

size_t list::size() {
	size_t count = 0;
	PREV = 0;
	node* p = head, * r = head;
	while (p != nullptr)
	{
		++count;
		p = reinterpret_cast<node*>(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
	}
	return count;
}

void list::seTraversal() {
	PREV = 0;
	node* p = head, * r = head;
	while (p != nullptr)
	{
		cout << p->data;
		p = reinterpret_cast<node*>(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
		if (p != nullptr) cout << ' ';
		else cout << endl;
	}
}

void list::reTraversal() {
	NEXT = 0;
	node* p = tail, * r = tail;
	while (p != nullptr)
	{
		cout << p->data;
		p = reinterpret_cast<node*>(p->np ^ NEXT);
		NEXT = reinterpret_cast<unsigned long>(r);
		r = p;
		if (p != nullptr) cout << ' ';
		else cout << endl;
	}
}