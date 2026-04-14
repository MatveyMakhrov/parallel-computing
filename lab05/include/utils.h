#pragma once
#include <vector>
#include <cstdint>

void generate_data(std::vector<int8_t>& a);

int8_t find_min(const std::vector<int8_t>& a);
int8_t find_max(const std::vector<int8_t>& a);

bool compare(const std::vector<int8_t>& a, const std::vector<int8_t>& b);