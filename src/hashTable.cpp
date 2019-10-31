#include <iostream>
#include <vector>
using namespace std;

// 全域散列
void hashTable(vector<int>& K, vector<int>& index){
    int a = 3;
    int b = 42;
    int p = 101;
    int m = K.size();
    for(auto k : K){
        int res = a * k + b;
        res %= p;
        res %= m;
        index.push_back(res);
    }
} 


int main_hashTable(){

    cout << "hello world" << endl;

    vector<int> K{10, 22, 37, 40, 52, 60, 70, 72, 75};
    vector<int> index;
    hashTable(K, index);
    
    return 0;
}