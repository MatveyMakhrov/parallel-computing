#pragma once

#include <vector>

using Matrix = std::vector<std::vector<double>>;

Matrix createRandomMatrix(int n);
Matrix multiplyMatrices(const Matrix& A, const Matrix& B);