#include "matrix.h"
#include "statistics.h"
#include "timer.h"

#include <functional>
#include <iostream>
#include <windows.h>


void pinToSingleCore() {
    HANDLE process = GetCurrentProcess();

    const DWORD_PTR affinityMask = 1;
    SetProcessAffinityMask(process, affinityMask);
}

void printSeparator() {
    std::cout << "\n============================================================\n";
}

int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    pinToSingleCore();

    const int matrixSize = 200;
    const int runs = 10;

    std::cout << "LR1: Measurement of program execution time\n";
    std::cout << "Test: matrix multiplication " << matrixSize << "x" << matrixSize << "\n";
    std::cout << "The number of measurements for each timer: " << runs << "\n";

    const Matrix A = createRandomMatrix(matrixSize);
    const Matrix B = createRandomMatrix(matrixSize);

    // Тестовая задача
    auto task = [&A, &B]() {
        volatile double guard = 0.0;

        Matrix C = multiplyMatrices(A, B);

        guard = C[0][0];
        (void)guard;
    };

    const TimerType timers[] = {
        TimerType::TSC,
        TimerType::QueryPerformanceCounter,
        TimerType::GetTickCount64
    };

    for (TimerType timer : timers) {
        printSeparator();
        std::cout << "Timer: " << timerName(timer) << "\n";
        printSeparator();

        const std::vector<double> measurements = runMeasurements(timer, task, runs);

        printMeasurements(measurements);

        const Statistics stats = calculateStatistics(measurements);
        printStatistics(stats);
        printHistogram(measurements, 10);

        std::cout << "\n";
    }

    return 0;
}