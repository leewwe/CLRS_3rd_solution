#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <tuple>
#include <map>

// 部分c代码的头文件和警告去除
#include <stdio.h>
#include <stdlib.h>


// 按起点视角排序
bool startTimeSort(pair<int, int> a, pair<int, int> b);
pair<vector<vector<int>>, vector<vector<int>>> activitySelector_dp(const vector<pair<int, int>>& a);
void printActivities(const vector<vector<int>>& c, const vector<vector<int>>& act, const vector<pair<int, int>>& a, const int i, const int j);
void greedyActivitySelector_rec(const vector<pair<int, int>>& a, const int& k, vector<pair<int, int>>& ret);
void greedyActivitySelector(const vector<pair<int, int>>& a, vector<pair<int, int>>& ret);
void greedyActivitySelector(const vector<pair<int, int>>& a, vector<int>& ret);
void greedyActivitySelector_reverse(const vector<pair<int, int>>& a, vector<pair<int, int>>& ret);
void getPoints(vector<pair<int, int>>& a, const string& fileName, const int& isContainFictitious = 0);

