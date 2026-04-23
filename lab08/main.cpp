#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/vec_ops.h"

using namespace std;

const int N = 1024;

float a[N], b[N], c[N];

int main()
{
    srand(time(0));

    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }

    // CPU вычисления
    vec_mul(a, b, c, N);

    cout << "Multiplication:\n";
    for (int i = 0; i < 10; i++)
        cout << c[i] << " ";

    cout << "\n";

    vec_add(a, b, c, N);

    cout << "Addition:\n";
    for (int i = 0; i < 10; i++)
        cout << c[i] << " ";

    return 0;
}