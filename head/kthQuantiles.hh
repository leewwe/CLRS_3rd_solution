#ifndef _K_TH_QUANTILES_HH_
#define _K_TH_QUANTILES_HH_
#include <vector>
using namespace std;

// void kthQuantiles(vector<int> A, int p, int r, int k, vector<int>& kLine);
vector<int> kthQuantiles(vector<int> A, int p, int r, int k);
pair<int, float> weightedMedian(vector<pair<int, float>> A, int p, int r, float w);


#endif