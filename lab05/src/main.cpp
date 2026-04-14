#include <iostream>
#include <vector>
#include <chrono>

#include "utils.h"
#include "baseline.h"
#include "simd_vector.h"

using Clock = std::chrono::high_resolution_clock;

void normalize_baseline_unroll2(const std::vector<int8_t>& a, std::vector<int8_t>& out,
                               int8_t min_val, int8_t max_val, int V) {
    int range = max_val - min_val;
    int n = a.size();

    for (int i = 0; i < n; i += 2) {
        for (int k = 0; k < 2 && i + k < n; k++) {
            int val = static_cast<int>(a[i + k]) - min_val;
            val = val * V / range;
            out[i + k] = static_cast<int8_t>(val);
        }
    }
}

void normalize_baseline_unroll4(const std::vector<int8_t>& a, std::vector<int8_t>& out,
                               int8_t min_val, int8_t max_val, int V) {
    int range = max_val - min_val;
    int n = a.size();

    for (int i = 0; i < n; i += 4) {
        for (int k = 0; k < 4 && i + k < n; k++) {
            int val = static_cast<int>(a[i + k]) - min_val;
            val = val * V / range;
            out[i + k] = static_cast<int8_t>(val);
        }
    }
}

void normalize_baseline_unroll8(const std::vector<int8_t>& a, std::vector<int8_t>& out,
                               int8_t min_val, int8_t max_val, int V) {
    int range = max_val - min_val;
    int n = a.size();

    for (int i = 0; i < n; i += 8) {
        for (int k = 0; k < 8 && i + k < n; k++) {
            int val = static_cast<int>(a[i + k]) - min_val;
            val = val * V / range;
            out[i + k] = static_cast<int8_t>(val);
        }
    }
}

int main() {
    const int N = 1'000'000;
    const int V = 255;

    std::vector<int8_t> a(N);
    std::vector<int8_t> out(N);

    generate_data(a);

    int8_t min_val = find_min(a);
    int8_t max_val = find_max(a);

    std::cout << "N = " << N << "\n\n";

    {
        auto start = Clock::now();
        normalize_baseline(a, out, min_val, max_val, V);
        auto end = Clock::now();

        std::cout << "Baseline: "
                  << std::chrono::duration<double>(end - start).count()
                  << " sec\n";
    }

    {
        auto start = Clock::now();
        normalize_baseline_unroll2(a, out, min_val, max_val, V);
        auto end = Clock::now();

        std::cout << "Baseline unroll x2: "
                  << std::chrono::duration<double>(end - start).count()
                  << " sec\n";
    }

    {
        auto start = Clock::now();
        normalize_baseline_unroll4(a, out, min_val, max_val, V);
        auto end = Clock::now();

        std::cout << "Baseline unroll x4: "
                  << std::chrono::duration<double>(end - start).count()
                  << " sec\n";
    }

    {
        auto start = Clock::now();
        normalize_baseline_unroll8(a, out, min_val, max_val, V);
        auto end = Clock::now();

        std::cout << "Baseline unroll x8: "
                  << std::chrono::duration<double>(end - start).count()
                  << " sec\n";
    }

    {
        auto start = Clock::now();
        normalize_simd_avx2(a, out, min_val, max_val, V);
        auto end = Clock::now();

        std::cout << "SIMD AVX2: "
                  << std::chrono::duration<double>(end - start).count()
                  << " sec\n";
    }

    return 0;
}