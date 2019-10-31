#include "../head/countSort.hh"

vector<uint32_t> countingSort(vector<uint32_t> A) {
	vector<int> value2Index(A.size(), 0);
	
	decltype(A) out(A.size(), 0);

	for (int i = 0; i < A.size(); ++i) {
		auto value = A[i];
		++value2Index[value];
	}

	for (int i = 1; i < value2Index.size(); ++i) {
		value2Index[i] = value2Index[i] + value2Index[i - 1];
	}

	for (int j = A.size() - 1; j >= 0; --j) {
		auto pos = --value2Index[A[j]];
		out[pos] = A[j];
	}
	return out;
}

void countingSort(vector<uint32_t>& A, uint32_t maxValue) {
	vector<int> value2Index(maxValue + 1, 0);

	for (int i = 0; i < A.size(); ++i) {
		auto value = A[i];
		++value2Index[value];
	}

	int j = A.size() - 1;
	for (int i = value2Index.size() - 1; i > -1; --i) {
		auto& cnt = value2Index[i];
		while ((cnt--) > 0) {
			A[j--] = i;
		}
	}
}

