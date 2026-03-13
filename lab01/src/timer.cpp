#include "timer.h"
#include <windows.h>
#include <vector>
#include <functional>
#include <intrin.h>

std::string timerName(TimerType type) {
    switch (type) {
        case TimerType::TSC:
            return "Time Stamp Counter (RDTSC)";
        case TimerType::QueryPerformanceCounter:
            return "QueryPerformanceCounter";
        case TimerType::GetTickCount64:
            return "GetTickCount64";
        default:
            return "UnknownTimer";
    }
}

double measureWithTSC(const std::function<void()>& task) {
    unsigned __int64 tscStart = __rdtsc();
    Sleep(1000);
    unsigned __int64 tscEnd = __rdtsc();

    double cpuFreqHz = static_cast<double>(tscEnd - tscStart);

    tscStart = __rdtsc();
    task();
    tscEnd = __rdtsc();

    double elapsedMs = (tscEnd - tscStart) * 1000.0 / cpuFreqHz;
    return elapsedMs;
}

// Замер с использованием QueryPerformanceCounter
double measureWithQPC(const std::function<void()>& task) {
    LARGE_INTEGER frequency{}, start{}, end{};
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    task();
    QueryPerformanceCounter(&end);
    const double elapsedMs =
        (static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;
    return elapsedMs;
}

// Замер с использованием GetTickCount64
double measureWithGetTickCount64(const std::function<void()>& task) {
    const ULONGLONG start = GetTickCount64();
    task();
    const ULONGLONG end = GetTickCount64();
    return static_cast<double>(end - start);
}

// Запуск нескольких измерений
std::vector<double> runMeasurements(
    TimerType type,
    const std::function<void()>& task,
    int runs
) {
    std::vector<double> results;
    results.reserve(runs);

    for (int i = 0; i < runs; ++i) {
        double t = 0.0;

        switch (type) {
            case TimerType::TSC:
                t = measureWithTSC(task);
                break;
            case TimerType::QueryPerformanceCounter:
                t = measureWithQPC(task);
                break;
            case TimerType::GetTickCount64:
                t = measureWithGetTickCount64(task);
                break;
        }

        results.push_back(t);
    }

    return results;
}