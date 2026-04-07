#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <vector>
#include <cstddef>

using Matrix = std::vector<std::vector<float>>;

void matrixMultiplyClassic(const Matrix& A, const Matrix& B, Matrix& C, size_t N);
void matrixMultiplyTransposed(const Matrix& A, const Matrix& B, Matrix& C, size_t N);
void matrixMultiplyBuffered(const Matrix& A, const Matrix& B, Matrix& C, size_t N);
void matrixMultiplyBufferedUnrolled(const Matrix& A, const Matrix& B, Matrix& C, size_t N, size_t M);
void matrixMultiplyBlocked(const Matrix& A, const Matrix& B, Matrix& C, size_t N, size_t blockSize);
void matrixMultiplyBlockedUnrolled(const Matrix& A, const Matrix& B, Matrix& C, size_t N, size_t blockSize, size_t M);
bool compareMatrices(const Matrix& C1, const Matrix& C2, size_t N, float epsilon = 1e-5f);

void initializeMatrices(Matrix& A, Matrix& B, Matrix& C, size_t N);

#endif