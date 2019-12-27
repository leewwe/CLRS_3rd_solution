#include <iostream>
#include <vector>
using namespace std;
#include ".\head\ve.hh"
#include "head\insertSort.hh"
#include "head\quickSort.hh"
#include "head\kthQuantiles.hh"
/*******************************************************************************************************************/

struct singleListNode {
    singleListNode(int k) : key(k), np(NULL)/* , prev(0), next(0) */{
    }
    singleListNode* np;
    int key;
};

class doubleList{
private:
    singleListNode* head;
    singleListNode* tail;
public:
    doubleList() : head(nullptr), tail(nullptr){

    }
    void insert(singleListNode* node);
    inline singleListNode* getHead(){ return head; }
    inline singleListNode* getTail() { return tail; }
    inline void setHead(singleListNode* node) { head = node; }
    inline void setTail(singleListNode* node) { tail = node; }
    
};

void doubleList::insert(singleListNode* node){
	if (head) {
		head->np = reinterpret_cast<singleListNode*>(reinterpret_cast<uint64_t>(head->np) ^ reinterpret_cast<uint64_t>(node));
		node->np = reinterpret_cast<singleListNode*>(0 ^ reinterpret_cast<uint64_t>(head));
		head = node;
	}
	else {
		head = node;
	}
}






int main(){
//
    // cout << "test" << endl;
    // vector<int> A{3, 5, 9, 2, 1, 4, 7, 8, 6, 0, 10, 12, 11, 13, 14, 15, 17, 18, 16, 19};
    // insertSort(A, 5, 9);
    // int a = select(A, 0, 14, 13);
    // int mid = median(A, 0, 14);
    // quickSort(A, 0, 9);
    // int pivotPos = partition(A, 0, 12, 5);
    // bestQuickSort(A, 0, 12);

    // vector<int> A{3, 5, 9, 2, 1, 4, 7, 8, 6, 0, 10, 12, 11, 13, 14, 15, 17, 18, 16, 19};
    // vector<int> kLine = kthQuantiles(A, 0, 19, 4);
    
    // vector<int> A{3, 5, 9, 2, 1, 4, 7, 8, 6, 10, 12, 11, 13, 14, 15, 17, 18, 16, 19};
    // int res = median(A, 0, 15);
    // auto res = findTheNearestKnums(A, 0, 14, 4);
    // vector<int> X{1, 3, 5, 7, 9};
    // vector<int> Y{2, 4, 6, 8, 10};
    
    // int median = twoArrayMedian(X, 0, 4, Y, 0, 4);

    // vector<pair<int, float>> A{
    //     {5,0.0333}, {6,0.0417}, {7,0.05}, {8,0.0583},
    //     {1,0.0}, {2,0.0083}, {3,0.0167}, {4,0.025},
    //     {13,0.1}, {14,0.1083}, {15,0.1167}, {16,0.125},
    //     {9,0.0667}, {10,0.075}, {11,0.0833}, {12,0.0917}
    // };
    // auto mid = median(A, 0, 6);
    // auto res = weightedMedian(A, 0, 15, 0.5);

    // print(A);
    // vector<int> A{3, 5, 9, 2, 1, 4, 7, 8, 6, 10, 12, 11};
    // auto res = modifiedSelect(A, 0, 10, 4);
//
    auto elem0 = new singleListNode(0);
    auto elem1 = new singleListNode(1);
    auto elem2 = new singleListNode(2);
    doubleList dL;
    dL.insert(elem0);
    
    return 0;
}