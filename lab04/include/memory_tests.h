#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

#include <vector>

double test_sequential(const std::vector<int>& a, int repeats);

double test_random(const std::vector<int>& a, int repeats);

double test_random_index(const std::vector<int>& a, const std::vector<int>& index_arr, int repeats);

#endif