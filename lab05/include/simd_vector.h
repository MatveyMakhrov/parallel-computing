#pragma once
#include <vector>
#include <cstdint>

void normalize_simd_avx2(
    const std::vector<int8_t>& a,
    std::vector<int8_t>& out,
    int8_t min_val,
    int8_t max_val,
    int V
);