#include "..\head\kthQuantiles.hh"
#include "..\head\quickSort.hh"
// 正常情况的处理
void kthQuantiles(vector<int> A, int p, int r, int k, vector<int>& kLine, int& d){
    // 只有一个划分
    if(k == 1 ){
        return;
    }

    int mid = k / 2; 
    // 有两个划分
    if(k == 2){
        kLine.push_back(select(A, p, r, mid*d));
        return;
    }
    
    kLine.push_back(select(A, p, r, mid*d));
    kthQuantiles(A, p, mid*d + p - 1, mid, kLine, d);
    kthQuantiles(A, mid*d + p, r, k - mid, kLine, d);
}

// 封装函数
vector<int> kthQuantiles(vector<int> A, int p, int r, int k){
    vector<int> ret;
    int arrSize = (r - p + 1);
    // A是空的 或者 不正确的划分：k==0、不能整除、k > 数组大小
    if(A.empty() || k == 0 || arrSize % k || k > arrSize){
        return ret;
    }
    // 只有一个划分
    if(k == 1){
        ret.push_back(select(A, p, r, arrSize));
        return ret;
    }

    // 有arrSize个划分
    if(k == arrSize){
        return A;
    }
    
    int d = arrSize / k;
    kthQuantiles(A, p, r, k ,ret, d);
    
    return ret;
}

// 接受主元的划分函数
int partition(vector<pair<int, float>>& A, int p, int r, pair<int, float> pivot){
	int i = p - 1;
	int pivotPos = 0;
	for(int j = p; j <= r; ++j){
		if(A[j].first <= pivot.first){
			if(pivot.first == A[j].first){
				pivotPos = i+1;
			}
			swap(A[++i], A[j]);
		}
	}
	swap(A[i], A[pivotPos]);
	return i; 
}

void insertSort(vector<pair<int, float>>& A, int p, int r){
	for (auto j = p + 1; j <= r ;++j) {
		auto key = A[j];
		auto i = j - 1;
		for (; i >= p && A[i].first > key.first; --i) {
			A[i + 1] = A[i];
		}
		A[i + 1] = key;
	}
}

pair<int, float> select(vector<pair<int, float>>& A, int p, int r, int i) {
	if(p >= r){
		return A[p];
	}
	/* !!以下带偏移量的，不要忘了 “+p” */
	int numGroup = (r - p + 1) / numOfEachGrp;
	// int residue = (r - p + 1) % numOfEachGrp;
	for(int cnt = 0; cnt < numGroup; ++cnt){
		insertSort(A, p + cnt * numOfEachGrp, (cnt + 1) * numOfEachGrp - 1);
	}
	insertSort(A, numGroup * numOfEachGrp + p, r);
	
	vector<pair<int, float>> mid; 
	for(int cnt = 0; cnt < numGroup; ++cnt){
		mid.push_back(A[p + cnt * numOfEachGrp + numOfEachGrp / 2]);
	}
	if(numGroup * numOfEachGrp + p <= r){
		mid.push_back(A[(numGroup * numOfEachGrp + p + r) / 2]);
	}
	// 中位数的中位数
	auto midNum = select(mid, 0, mid.size() - 1, (mid.size() - 1)/ 2 + 1);
	
	auto q = partition(A, p, r, midNum);

	int k = q - p + 1;
	if(i == k){
		return A[q];
	}
	else if(i < k){
		return select(A, p, q - 1, i);
	}
	else{
		return select(A, q + 1, r, i - k);
	}
}

pair<int, float>& median(vector<pair<int, float>>& A, int p, int r){
	return select(A, p, r, (r - p) / 2 + 1);
}


pair<int, float> weightedMedian(vector<pair<int, float>> A, int p, int r, float w){
    if(p > r){
        return {INT_MIN, 0.0};
    }
    else if(p == r){
        return A[0];
    }
    else if((r - p) == 1){
        if(A[p].second >= A[r].second){
            return A[p];
        }
        else{
            return A[r];
        }
    }
    else{
        auto mid = median(A, p, r);
        int q = partition(A, p, r, mid);
        float W_L = 0;
        for(int i = p; i < q; ++i){
            W_L += A[i].second;
        }
        float W_G = 0;
        for(int i = q + 1; i <= r; ++i){
            W_G += A[i].second;
        }
        if(W_L < 0.5 && W_G < 0.5){
            return mid;
        }
        else if(W_L > 0.5){
            mid.second += W_G;
            return weightedMedian(A, p, q, w);
        }
        else{
            mid.second += W_L;
            return weightedMedian(A, q, r, w);
        }
    }

}

