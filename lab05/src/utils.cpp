#include "../include/utils.h"
#include <random>

void generate_data(std::vector<int8_t>& a) {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(-128, 127);

    for (auto& x : a)
        x = static_cast<int8_t>(dist(gen));
}

int8_t find_min(const std::vector<int8_t>& a) {
    int8_t m = a[0];
    for (auto x : a)
        if (x < m) m = x;
    return m;
}

int8_t find_max(const std::vector<int8_t>& a) {
    int8_t m = a[0];
    for (auto x : a)
        if (x > m) m = x;
    return m;
}

bool compare(const std::vector<int8_t>& a, const std::vector<int8_t>& b) {
    for (size_t i = 0; i < a.size(); i++)
        if (a[i] != b[i]) return false;
    return true;
}