#include "..\head\Strassen.hh"

matrix matrix_plus(matrix& A, int A_r, int A_c, matrix& B, int B_r, int B_c, int n, int op) {
	matrix ret;
	for (auto r = 0; r < n; ++r) {
		ret.push_back(row(A[A_r + r].begin() + A_c, A[A_r + r].begin() + A_c + n));
		for (int c = 0; c < n; ++c) {
			ret[r][c] += op * B[B_c + r][B_r + c];
		}
	}
	return ret;
}

matrix matrix_plus(matrix& A, matrix& B, int n ,int op) {
	matrix ret;
	for (auto r = 0; r < n; ++r) {
		ret.push_back(A[r]);
		for (int c = 0; c < n; ++c) {
			ret[r][c] += op * B[r][c];
		}
	}
	return ret;
}

matrix partition_matrix(matrix& A, int r, int c, int n) {
	matrix ret;
	for (auto i = r; i <r + n; ++i) {
		ret.push_back(row(A[i].begin() + c, A[i].begin() + c + n));
	}
	return ret;
}

matrix martrix_combine(matrix& C11, matrix& C12, matrix& C21, matrix& C22) {
	matrix ret = C11;
	for (auto i = 0; i < C12.size(); ++i) {
		ret[i].insert(ret[i].end(), C12[i].begin(), C12[i].end());
	}
	auto old_size = ret.size();
	ret.insert(ret.end(), C21.begin(), C21.end());
	for (auto i = old_size; i < ret.size(); ++i) {
		ret[i].insert(ret[i].end(), C22[i - old_size].begin(), C22[i - old_size].end());
	}
	return ret;
}

matrix strassen_matrix_mul(matrix A, matrix B) {
	int n = A.size();
	auto C = matrix(n, row(n, 0));
	if (n == 1) {
		C[0][0] = A[0][0] * B[0][0];
	}
	else {
		int mid = n / 2;
		// 1 -> 分割矩阵
		auto A11 = partition_matrix(A, 0, 0, n / 2);
		auto A12 = partition_matrix(A, 0, mid, n / 2);
		auto A21 = partition_matrix(A, mid, 0, n / 2);
		auto A22 = partition_matrix(A, mid, mid, n / 2);
		auto B11 = partition_matrix(B, 0, 0, n / 2);
		auto B12 = partition_matrix(B, 0, mid, n / 2);
		auto B21 = partition_matrix(B, mid, 0, n / 2);
		auto B22 = partition_matrix(B, mid, mid, n / 2);
		// 2 -> 计算10个S
		auto S1 = matrix_plus(B12, B22, n / 2, -1);
		auto S2 = matrix_plus(A11, A12, n / 2, 1);
		auto S3 = matrix_plus(A21, A22, n / 2, 1);
		auto S4 = matrix_plus(B21, B11, n / 2, -1);
		auto S5 = matrix_plus(A11, A22, n / 2, 1);
		auto S6 = matrix_plus(B11, B22, n / 2, 1);
		auto S7 = matrix_plus(A12, A22, n / 2, -1);
		auto S8 = matrix_plus(B21, B22, n / 2, 1);
		auto S9 = matrix_plus(A11, A21, n / 2, -1);
		auto S10 = matrix_plus(B11, B12, n / 2, 1);
		// 3 -> 计算7个P
		auto P1 = strassen_matrix_mul(A11, S1);
		auto P2 = strassen_matrix_mul(S2, B22);
		auto P3 = strassen_matrix_mul(S3, B11);
		auto P4 = strassen_matrix_mul(A22, S4);
		auto P5 = strassen_matrix_mul(S5, S6);
		auto P6 = strassen_matrix_mul(S7, S8);
		auto P7 = strassen_matrix_mul(S9, S10);
		// 4 -> 计算C11， C12, C21, C22
		auto C11 = matrix_plus(P5, P4, n / 2, 1);
		C11 = matrix_plus(C11, P2, n / 2, -1);
		C11 = matrix_plus(C11, P6, n / 2, 1);
		auto C12 = matrix_plus(P1, P2, n / 2, 1);
		auto C21 = matrix_plus(P3, P4, n / 2, 1);
		auto C22 = matrix_plus(P5, P1, n / 2, 1);
		C22 = matrix_plus(C22, P3, n / 2, -1);
		C22 = matrix_plus(C22, P7, n / 2, -1);
		// 5 -> 合并C11， C12, C21, C22
		C = martrix_combine(C11, C12, C21, C22);
	}
	return C;
}