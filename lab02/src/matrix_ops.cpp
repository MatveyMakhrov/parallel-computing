#include "matrix_ops.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iostream>

void matrixMultiplyClassic(const Matrix& A, const Matrix& B, Matrix& C, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (size_t k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void matrixMultiplyTransposed(const Matrix& A, const Matrix& B, Matrix& C, size_t N) {
    Matrix BT(N, std::vector<float>(N));
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            BT[j][i] = B[i][j];
        }
    }

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (size_t k = 0; k < N; ++k) {
                sum += A[i][k] * BT[j][k];
            }
            C[i][j] = sum;
        }
    }
}

void matrixMultiplyBuffered(const Matrix& A, const Matrix& B, Matrix& C, size_t N) {
    std::vector<float> columnBuffer(N);
    
    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < N; ++i) {
            columnBuffer[i] = B[i][j];
        }
        
        for (size_t i = 0; i < N; ++i) {
            float sum = 0.0f;
            for (size_t k = 0; k < N; ++k) {
                sum += A[i][k] * columnBuffer[k];
            }
            C[i][j] = sum;
        }
    }
}

void matrixMultiplyBufferedUnrolled(const Matrix& A, const Matrix& B, Matrix& C,
                                    size_t N, size_t M) {
    std::vector<float> columnBuffer(N);
    
    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < N; ++i) {
            columnBuffer[i] = B[i][j];
        }
        
        for (size_t i = 0; i < N; ++i) {
            float sum1 = 0.0f, sum2 = 0.0f, sum3 = 0.0f, sum4 = 0.0f;
            size_t k = 0;
            
            for (; k + 3 < N; k += 4) {
                sum1 += A[i][k]     * columnBuffer[k];
                sum2 += A[i][k + 1] * columnBuffer[k + 1];
                sum3 += A[i][k + 2] * columnBuffer[k + 2];
                sum4 += A[i][k + 3] * columnBuffer[k + 3];
            }
            float sum = sum1 + sum2 + sum3 + sum4;
            
            for (; k < N; ++k) {
                sum += A[i][k] * columnBuffer[k];
            }
            
            C[i][j] = sum;
        }
    }
}

void matrixMultiplyBlocked(const Matrix& A, const Matrix& B, Matrix& C,
                          size_t N, size_t blockSize) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            C[i][j] = 0.0f;
        }
    }
    
    for (size_t ii = 0; ii < N; ii += blockSize) {
        size_t iMax = std::min(ii + blockSize, N);
        for (size_t jj = 0; jj < N; jj += blockSize) {
            size_t jMax = std::min(jj + blockSize, N);
            for (size_t kk = 0; kk < N; kk += blockSize) {
                size_t kMax = std::min(kk + blockSize, N);
                
                for (size_t i = ii; i < iMax; ++i) {
                    for (size_t j = jj; j < jMax; ++j) {
                        float sum = 0.0f;
                        for (size_t k = kk; k < kMax; ++k) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] += sum;
                    }
                }
            }
        }
    }
}

void matrixMultiplyBlockedUnrolled(const Matrix& A, const Matrix& B, Matrix& C,
                                  size_t N, size_t blockSize, size_t M) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            C[i][j] = 0.0f;
        }
    }
    
    for (size_t ii = 0; ii < N; ii += blockSize) {
        size_t iMax = std::min(ii + blockSize, N);
        for (size_t jj = 0; jj < N; jj += blockSize) {
            size_t jMax = std::min(jj + blockSize, N);
            for (size_t kk = 0; kk < N; kk += blockSize) {
                size_t kMax = std::min(kk + blockSize, N);
                
                for (size_t i = ii; i < iMax; ++i) {
                    for (size_t j = jj; j < jMax; ++j) {
                        float s1 = 0.0f, s2 = 0.0f, s3 = 0.0f, s4 = 0.0f;
                        size_t k = kk;
                        
                        for (; k + 3 < kMax; k += 4) {
                            s1 += A[i][k]     * B[k][j];
                            s2 += A[i][k + 1] * B[k + 1][j];
                            s3 += A[i][k + 2] * B[k + 2][j];
                            s4 += A[i][k + 3] * B[k + 3][j];
                        }
                        float sum = s1 + s2 + s3 + s4;
                        
                        for (; k < kMax; ++k) {
                            sum += A[i][k] * B[k][j];
                        }
                        
                        C[i][j] += sum;
                    }
                }
            }
        }
    }
}

bool compareMatrices(const Matrix& C1, const Matrix& C2, size_t N, float epsilon) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (std::abs(C1[i][j] - C2[i][j]) > epsilon) {
                std::cout << "Mismatch at (" << i << "," << j << "): "
                          << C1[i][j] << " vs " << C2[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

void initializeMatrices(Matrix& A, Matrix& B, Matrix& C, size_t N) {
    A.assign(N, std::vector<float>(N));
    B.assign(N, std::vector<float>(N));
    C.assign(N, std::vector<float>(N));
    
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            A[i][j] = static_cast<float>(rand()) / RAND_MAX;
            B[i][j] = static_cast<float>(rand()) / RAND_MAX;
            C[i][j] = 0.0f;
        }
    }
}