#include "../include/baseline.h"

void normalize_baseline(
    const std::vector<int8_t>& a,
    std::vector<int8_t>& out,
    int8_t min_val,
    int8_t max_val,
    int V
) {
    int range = max_val - min_val;

    for (size_t i = 0; i < a.size(); i++) {
        int val = static_cast<int>(a[i]) - min_val;
        val = val * V / range;
        out[i] = static_cast<int8_t>(val);
    }
}