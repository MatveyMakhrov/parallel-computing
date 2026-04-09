#include "../include/memory_tests.h"
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

volatile long long global_sum = 0;

double test_sequential(const vector<int>& a, int repeats)
{
    long long sum = 0;

    auto start = high_resolution_clock::now();

    for (int r = 0; r < repeats; r++)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            sum += a[i];
        }
    }

    auto end = high_resolution_clock::now();
    global_sum = sum;

    double total_ns = duration<double, nano>(end - start).count();
    return total_ns / (a.size() * repeats);
}

double test_random(const vector<int>& a, int repeats)
{
    long long sum = 0;
    mt19937 gen(42);
    uniform_int_distribution<int> dist(0, a.size() - 1);

    auto start = high_resolution_clock::now();

    for (int r = 0; r < repeats; r++)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            int idx = dist(gen);
            sum += a[idx];
        }
    }

    auto end = high_resolution_clock::now();
    global_sum = sum;

    double total_ns = duration<double, nano>(end - start).count();
    return total_ns / (a.size() * repeats);
}

double test_random_index(const vector<int>& a,
                         const vector<int>& index_arr,
                         int repeats)
{
    long long sum = 0;

    auto start = high_resolution_clock::now();

    for (int r = 0; r < repeats; r++)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            sum += a[index_arr[i]];
        }
    }

    auto end = high_resolution_clock::now();
    global_sum = sum;

    double total_ns = duration<double, nano>(end - start).count();
    return total_ns / (a.size() * repeats);
}