#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "memory_tests.h"

using namespace std;

int main()
{
    ofstream fout("results.csv");
    fout << "Size_KB,Sequential,Random,RandomIndex\n";

    mt19937 gen(42);

    vector<pair<size_t, size_t>> ranges = {
        {4 * 1024, 2 * 1024 * 1024},
        {512 * 1024, 32 * 1024 * 1024},
        {5 * 1024 * 1024, 150 * 1024 * 1024}
    };

    for (int range_id = 0; range_id < 3; range_id++)
    {
        size_t step = ranges[range_id].first;
        size_t max_size = ranges[range_id].second;

        for (size_t bytes = step; bytes <= max_size; bytes += step)
        {
            size_t n = bytes / sizeof(int);

            vector<int> a(n);
            for (size_t i = 0; i < n; i++)
                a[i] = i;

            vector<int> index_arr(n);
            uniform_int_distribution<int> dist(0, n - 1);

            for (size_t i = 0; i < n; i++)
                index_arr[i] = dist(gen);

            int repeats = max(5, (int)(1e7 / n));

            int runs = 5;

            double t_seq = 0, t_rand = 0, t_rand_idx = 0;

            for (int i = 0; i < runs; i++)
            {
                t_seq += test_sequential(a, repeats);
                t_rand += test_random(a, repeats);
                t_rand_idx += test_random_index(a, index_arr, repeats);
            }

            t_seq /= runs;
            t_rand /= runs;
            t_rand_idx /= runs;

            cout << "Size: " << bytes / 1024 << " KB done\n";

            fout << bytes / 1024 << ","
                 << t_seq << ","
                 << t_rand << ","
                 << t_rand_idx << "\n";
        }
    }

    fout.close();

    cout << "Done! Results saved to results.csv\n";
    return 0;
}