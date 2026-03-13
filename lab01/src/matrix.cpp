#include "matrix.h"

#include <random>

Matrix createRandomMatrix(int n) {
    Matrix m(n, std::vector<double>(n));

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 10.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m[i][j] = dist(rng);
        }
    }

    return m;
}

Matrix multiplyMatrices(const Matrix& A, const Matrix& B) {
    const int n = static_cast<int>(A.size());
    Matrix C(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            const double aik = A[i][k];
            for (int j = 0; j < n; ++j) {
                C[i][j] += aik * B[k][j];
            }
        }
    }

    return C;
}