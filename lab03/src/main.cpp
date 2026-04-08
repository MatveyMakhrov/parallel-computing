#include <iostream>
#include "../include/cpuinfo.h"

using namespace std;

int main() {
    cout << "Processor CPUID info\n\n";

    printEAX0();
    printEAX1();
    printCache();
    printEAX7();
    printBrand();

    return 0;
}