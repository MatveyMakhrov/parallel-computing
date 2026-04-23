#ifndef SOBEL_FILTER_H
#define SOBEL_FILTER_H

#include "image.h"
#include <chrono>

class SobelFilter {
public:
    // Высокоуровневая (скалярная) реализация
    static Image sobel_scalar(const Image& src);
    
    // SIMD-оптимизированная (AVX2) реализация
    static Image sobel_avx2(const Image& src);
    
    // Проверка корректности
    static bool validate(const Image& a, const Image& b, double epsilon = 1.0);
    
    // Измерение времени выполнения
    template<typename Func>
    static double measure_time(Func func, const Image& src, int iterations = 10) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            func(src);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count() / iterations;
    }
};

#endif // SOBEL_FILTER_H