#ifndef _STRASSEN_HH_
#define _STRASSEN_HH_
#include <vector>
#include <iostream>
#include <utility>
#include <tuple>
using namespace std;

using matrix = vector<vector<int>>;
using elem = vector<vector<int>>;
using row = vector<int>;

matrix matrix_plus(matrix& A, int A_r, int A_c, matrix& B, int B_r, int B_c, int n, int op);
matrix matrix_plus(matrix& A, matrix& B, int n ,int op);
matrix partition_matrix(matrix& A, int r, int c, int n);
matrix martrix_combine(matrix& C11, matrix& C12, matrix& C21, matrix& C22);
matrix strassen_matrix_mul(matrix A, matrix B);



#endif