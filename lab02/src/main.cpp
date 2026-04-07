#include "matrix_ops.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <cmath>

template<typename Func>
double measureTime(Func func, int iterations = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < iterations; ++iter) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count() / iterations;
}

double computeGflops(double timeSeconds, size_t N) {
    double operations = 2.0 * N * N * N;  // N^3 умножений + N^3 сложений
    return operations / (timeSeconds * 1e9);
}

void saveToCSV(const std::string& filename, 
               const std::vector<std::string>& headers,
               const std::vector<std::vector<double>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i < headers.size() - 1) file << ",";
    }
    file << "\n";
    
    size_t rows = data.empty() ? 0 : data[0].size();
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < data.size(); ++col) {
            file << std::fixed << std::setprecision(6) << data[col][row];
            if (col < data.size() - 1) file << ",";
        }
        file << "\n";
    }
    
    file.close();
    std::cout << "Saved: " << filename << std::endl;
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "Matrix Multiplication Performance Analysis\n";
    std::cout << "===========================================\n\n";
    
    std::filesystem::create_directory("results");
    
    srand(42);
    
    std::vector<int> testSizes = {32, 64, 128, 256, 512, 1024, 2048};
    
    std::vector<double> classicGflops, transposedGflops, bufferedGflops, blockedGflops;
    std::vector<double> bufferedUnrolledGflops, blockedUnrolledGflops;
    
    std::cout << std::fixed << std::setprecision(4);
    
    for (size_t idx = 0; idx < testSizes.size(); ++idx) {
        int N = testSizes[idx];
        std::cout << "\n--- Testing N = " << N << " ---\n";
        
        Matrix A, B, C1, C2, C3, C4;
        Matrix C5, C6;
        C5.assign(N, std::vector<float>(N));
        C6.assign(N, std::vector<float>(N));
        
        // Инициализируем все матрицы
        initializeMatrices(A, B, C1, N);
        
        // Выделяем память для остальных матриц
        C2.assign(N, std::vector<float>(N));
        C3.assign(N, std::vector<float>(N));
        C4.assign(N, std::vector<float>(N));
        
        double timeClassic = 0.0, timeTransposed = 0.0, timeBuffered = 0.0, timeBlocked = 0.0;
        
        // Для маленьких матриц делаем больше итераций для точности
        int iterations = (N >= 1024) ? 1 : 3;
        if (N <= 64) iterations = 10; // Для очень маленьких матриц увеличиваем итерации

        std::cout << "Classic algorithm... ";
        timeClassic = measureTime([&]() { matrixMultiplyClassic(A, B, C1, N); }, iterations);
        double gflopsClassic = computeGflops(timeClassic, N);
        std::cout << timeClassic << " s, " << gflopsClassic << " GFLOP/s\n";
        classicGflops.push_back(gflopsClassic);

        std::cout << "Transposed algorithm... ";
        timeTransposed = measureTime([&]() { matrixMultiplyTransposed(A, B, C2, N); }, iterations);
        double gflopsTransposed = computeGflops(timeTransposed, N);
        std::cout << timeTransposed << " s, " << gflopsTransposed << " GFLOP/s\n";
        transposedGflops.push_back(gflopsTransposed);
        
        if (!compareMatrices(C1, C2, N)) {
            std::cout << "ERROR: Transposed result mismatch!\n";
        }

        std::cout << "Buffered algorithm... ";
        timeBuffered = measureTime([&]() { matrixMultiplyBuffered(A, B, C3, N); }, iterations);
        double gflopsBuffered = computeGflops(timeBuffered, N);
        std::cout << timeBuffered << " s, " << gflopsBuffered << " GFLOP/s\n";
        bufferedGflops.push_back(gflopsBuffered);
        
        if (!compareMatrices(C1, C3, N)) {
            std::cout << "ERROR: Buffered result mismatch!\n";
        }

        std::cout << "Buffered Unrolled... ";
        double timeBuffUnroll = measureTime([&]() {
            matrixMultiplyBufferedUnrolled(A, B, C5, N, 4);
        }, iterations);
        double gflopsBuffUnroll = computeGflops(timeBuffUnroll, N);
        std::cout << timeBuffUnroll << " s, " << gflopsBuffUnroll << " GFLOP/s\n";
        bufferedUnrolledGflops.push_back(gflopsBuffUnroll);

        if (!compareMatrices(C1, C5, N)) {
            std::cout << "ERROR: Buffered Unrolled mismatch!\n";
        }

        std::cout << "Blocked algorithm (block=64)... ";
        timeBlocked = measureTime([&]() { matrixMultiplyBlocked(A, B, C4, N, 64); }, iterations);
        double gflopsBlocked = computeGflops(timeBlocked, N);
        std::cout << timeBlocked << " s, " << gflopsBlocked << " GFLOP/s\n";
        blockedGflops.push_back(gflopsBlocked);
        
        if (!compareMatrices(C1, C4, N)) {
            std::cout << "ERROR: Blocked result mismatch!\n";
        }

        std::cout << "Blocked Unrolled... ";
        double timeBlockUnroll = measureTime([&]() {
            matrixMultiplyBlockedUnrolled(A, B, C6, N, 64, 4);
        }, iterations);
        double gflopsBlockUnroll = computeGflops(timeBlockUnroll, N);
        std::cout << timeBlockUnroll << " s, " << gflopsBlockUnroll << " GFLOP/s\n";
        blockedUnrolledGflops.push_back(gflopsBlockUnroll);

        if (!compareMatrices(C1, C6, N)) {
            std::cout << "ERROR: Blocked Unrolled mismatch!\n";
        }

        std::vector<double> steps = {0,1,2,3,4,5};
        std::vector<double> gflopsSteps = {
            gflopsClassic,
            gflopsTransposed,
            gflopsBuffered,
            gflopsBuffUnroll,
            gflopsBlocked,
            gflopsBlockUnroll
        };

        saveToCSV("results/step_N_" + std::to_string(N) + ".csv",
                {"Step","GFLOPS"},
                {steps, gflopsSteps});
        
        std::cout << "----------------------------------------\n";
    }

    // Сохраняем результаты сравнения алгоритмов
    std::vector<std::string> comparisonHeaders = {
        "N", "Classic", "Transposed", "Buffered", "Blocked",
        "BufferedUnrolled", "BlockedUnrolled"
    };
    std::vector<std::vector<double>> comparisonData;
    std::vector<double> sizesDouble(testSizes.begin(), testSizes.end());
    comparisonData.push_back(sizesDouble);
    comparisonData.push_back(classicGflops);
    comparisonData.push_back(transposedGflops);
    comparisonData.push_back(bufferedGflops);
    comparisonData.push_back(blockedGflops);
    comparisonData.push_back(bufferedUnrolledGflops);
    comparisonData.push_back(blockedUnrolledGflops);
    saveToCSV("results/algorithms_comparison.csv", comparisonHeaders, comparisonData);
    
    // Анализ размера блока
    std::cout << "\n=== Block Size Analysis (N = 1024) ===\n";
    const int analysisN = 1024;
    Matrix A, B, C;
    initializeMatrices(A, B, C, analysisN);
    
    std::vector<int> blockSizes = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    std::vector<double> blockGflops;
    
    for (int bs : blockSizes) {
        if (bs > analysisN) continue;
        
        // Создаем временную матрицу для каждого теста
        Matrix C_test;
        C_test.assign(analysisN, std::vector<float>(analysisN));
        
        std::cout << "Block size = " << bs << "... ";
        double time = measureTime([&]() { 
            matrixMultiplyBlocked(A, B, C_test, analysisN, bs); 
        }, 2);
        double gflops = computeGflops(time, analysisN);
        std::cout << time << " s, " << gflops << " GFLOP/s\n";
        blockGflops.push_back(gflops);
    }
    
    std::vector<std::string> blockHeaders = {"BlockSize", "GFLOPs"};
    std::vector<std::vector<double>> blockData;
    std::vector<double> blockSizesDouble(blockSizes.begin(), blockSizes.end());
    blockData.push_back(blockSizesDouble);
    blockData.push_back(blockGflops);
    saveToCSV("results/block_size_dependency.csv", blockHeaders, blockData);
    
    auto bestBlockIt = std::max_element(blockGflops.begin(), blockGflops.end());
    size_t bestBlockIdx = std::distance(blockGflops.begin(), bestBlockIt);
    int optimalBlockSize = blockSizes[bestBlockIdx];
    std::cout << "\nOptimal block size S* = " << optimalBlockSize 
              << " (" << *bestBlockIt << " GFLOP/s)\n";
    
    // Анализ развертки циклов для буферизованного метода
    std::cout << "\n=== Loop Unrolling Analysis (Buffered, N = 1024) ===\n";
    std::vector<int> unrollFactors = {1, 2, 4, 8, 16};
    std::vector<double> unrollBufferedGflops;
    
    for (int M : unrollFactors) {
        Matrix C_test;
        C_test.assign(analysisN, std::vector<float>(analysisN));
        
        std::cout << "Unroll factor M = " << M << "... ";
        double time = measureTime([&]() { 
            matrixMultiplyBufferedUnrolled(A, B, C_test, analysisN, M); 
        }, 2);
        double gflops = computeGflops(time, analysisN);
        std::cout << time << " s, " << gflops << " GFLOP/s\n";
        unrollBufferedGflops.push_back(gflops);
    }

    std::vector<std::string> unrollBufferedHeaders = {"UnrollFactor", "GFLOPs"};
    std::vector<std::vector<double>> unrollBufferedData;
    std::vector<double> unrollFactorsDouble(unrollFactors.begin(), unrollFactors.end());
    unrollBufferedData.push_back(unrollFactorsDouble);
    unrollBufferedData.push_back(unrollBufferedGflops);
    saveToCSV("results/unroll_buffered.csv", unrollBufferedHeaders, unrollBufferedData);
    
    auto bestUnrollBuffIt = std::max_element(unrollBufferedGflops.begin(), unrollBufferedGflops.end());
    int optimalBufferedM = unrollFactors[std::distance(unrollBufferedGflops.begin(), bestUnrollBuffIt)];
    std::cout << "\nOptimal unroll factor for buffered method M* = " << optimalBufferedM 
              << " (" << *bestUnrollBuffIt << " GFLOP/s)\n";

    // Анализ развертки циклов для блочного метода
    std::cout << "\n=== Loop Unrolling Analysis (Blocked, N = 1024, block=" 
              << optimalBlockSize << ") ===\n";
    std::vector<double> unrollBlockedGflops;
    
    for (int M : unrollFactors) {
        Matrix C_test;
        C_test.assign(analysisN, std::vector<float>(analysisN));
        
        std::cout << "Unroll factor M = " << M << "... ";
        double time = measureTime([&]() { 
            matrixMultiplyBlockedUnrolled(A, B, C_test, analysisN, optimalBlockSize, M); 
        }, 2);
        double gflops = computeGflops(time, analysisN);
        std::cout << time << " s, " << gflops << " GFLOP/s\n";
        unrollBlockedGflops.push_back(gflops);
    }

    std::vector<std::string> unrollBlockedHeaders = {"UnrollFactor", "GFLOPs"};
    std::vector<std::vector<double>> unrollBlockedData;
    unrollBlockedData.push_back(unrollFactorsDouble);
    unrollBlockedData.push_back(unrollBlockedGflops);
    saveToCSV("results/unroll_blocked.csv", unrollBlockedHeaders, unrollBlockedData);
    
    auto bestUnrollBlockIt = std::max_element(unrollBlockedGflops.begin(), unrollBlockedGflops.end());
    int optimalBlockedM = unrollFactors[std::distance(unrollBlockedGflops.begin(), bestUnrollBlockIt)];
    std::cout << "\nOptimal unroll factor for blocked method M* = " << optimalBlockedM 
              << " (" << *bestUnrollBlockIt << " GFLOP/s)\n";
    
    // Анализ масштабирования оптимизированных реализаций
    std::cout << "\n=== Scaling Analysis for Optimized Implementations ===\n";
    
    std::vector<double> optimizedBufferedGflops, optimizedBlockedGflops;
    
    for (size_t idx = 0; idx < testSizes.size(); ++idx) {
        int N = testSizes[idx];
        std::cout << "\n--- Testing N = " << N << " ---\n";
        
        Matrix A_opt, B_opt, C_opt1, C_opt2;
        initializeMatrices(A_opt, B_opt, C_opt1, N);
        C_opt2.assign(N, std::vector<float>(N));
        
        int iterations = (N >= 1024) ? 1 : 3;
        if (N <= 64) iterations = 10;

        std::cout << "Optimized buffered (M=" << optimalBufferedM << ")... ";
        double timeBuffOpt = measureTime([&]() { 
            matrixMultiplyBufferedUnrolled(A_opt, B_opt, C_opt1, N, optimalBufferedM); 
        }, iterations);
        double gflopsBuffOpt = computeGflops(timeBuffOpt, N);
        std::cout << timeBuffOpt << " s, " << gflopsBuffOpt << " GFLOP/s\n";
        optimizedBufferedGflops.push_back(gflopsBuffOpt);

        std::cout << "Optimized blocked (S=" << optimalBlockSize << ", M=" << optimalBlockedM << ")... ";
        double timeBlockOpt = measureTime([&]() { 
            matrixMultiplyBlockedUnrolled(A_opt, B_opt, C_opt2, N, optimalBlockSize, optimalBlockedM); 
        }, iterations);
        double gflopsBlockOpt = computeGflops(timeBlockOpt, N);
        std::cout << timeBlockOpt << " s, " << gflopsBlockOpt << " GFLOP/s\n";
        optimizedBlockedGflops.push_back(gflopsBlockOpt);
    }

    std::vector<std::string> optimizedHeaders = {"N", "Buffered_Opt", "Blocked_Opt"};
    std::vector<std::vector<double>> optimizedData;
    optimizedData.push_back(sizesDouble);
    optimizedData.push_back(optimizedBufferedGflops);
    optimizedData.push_back(optimizedBlockedGflops);
    saveToCSV("results/optimized_scaling.csv", optimizedHeaders, optimizedData);

    std::cout << "\n===========================================\n";
    std::cout << "FINAL RECOMMENDATIONS:\n";
    std::cout << "===========================================\n";
    std::cout << "Optimal block size S* = " << optimalBlockSize << "\n";
    std::cout << "Optimal unroll factor for buffered method M* = " << optimalBufferedM << "\n";
    std::cout << "Optimal unroll factor for blocked method M* = " << optimalBlockedM << "\n\n";
    
    std::cout << "All results saved to 'results/' directory as CSV files.\n";
    std::cout << "\nTo generate graphs, run:\n";
    std::cout << "  python plot_graphs.py\n";
    
    return 0;
}