#ifndef _DP_HH_H
#define _DP_HH_H

#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>


using namespace std;

int max(int a, int b);
int memoCutRodAux(const vector<int>& p, const int& n, vector<int>& r);
int memoCutRod(const vector<int>& p, const int& n);
int bottomUpCutRod(const vector<int>& p, const int& n);
void externedBottomUpCutRod(vector<int> p, const int& n, vector<int>& r, vector<int>& s);
int externedMemoCutRod(const vector<int>& p, const int& n, vector<int>& r, vector<int>& s);
void printCutRodSolution(vector<int> p, int n, const vector<int>& s);
int memoDpFibonacciCalcAux(const int n, vector<int>& r);
int memoDpFibonacciCalc(const int n);
int bottomUpFibonacciCalc(const int n);
void matrixChainOrder(const vector<int>& p, vector<vector<int>>& m, vector<vector<int>>& s);
void printOptimalParens(vector<vector<int>>& s, int i, int j);
template<typename T>
void lscLength(const vector<T>& X, const vector<T>& Y, vector<vector<int>>& c, vector<vector<int>>& b);
template<typename T>
pair<vector<vector<int>>, vector<vector<int>>> lscLength(const vector<T>& X, const vector<T>& Y);
template<typename T>
void printLSC(const vector<vector<int>>& b, const vector<T>& X, int i, int j);
template<typename T>
vector<vector<int>> lscLength_optimal(const vector<T>& X, const vector<T>& Y);
template<typename T>
void printLSC_optimal(const vector<vector<int>>& c, const vector<T>& X, int i, int j);
template<typename T>
void printLSC_optimal(const vector<vector<int>>& c, const vector<T>& X, const vector<T>& Y, int i, int j);
template<typename T>
int memorizedLSCLength_optimal_aux(const vector<T>& X, const vector<T>& Y, vector<vector<int>>& c, int i, int j);
template<typename T>
vector<vector<int>> memorizedLSCLength_optimal(const vector<T>& X, const vector<T>& Y);
template<typename T>
int lscLength_space_optimal_first(const vector<T>& X, const vector<T>& Y);
template<typename T>
int lscLength_space_optimal(const vector<T>& X, const vector<T>& Y);
int find_pos(int x, vector<int> S, int ls);
pair<int, vector<int>> LMIS(vector<int> X);
void printLMIS(const vector<int>& X, const vector<int>& e, int k);


#endif