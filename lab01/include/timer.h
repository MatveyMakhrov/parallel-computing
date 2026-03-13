#pragma once

#include <functional>
#include <string>
#include <vector>

enum class TimerType {
    TSC,
    QueryPerformanceCounter,
    GetTickCount64
};

std::string timerName(TimerType type);

double measureWithTSC(const std::function<void()>& task);
double measureWithQPC(const std::function<void()>& task);
double measureWithGetTickCount64(const std::function<void()>& task);

std::vector<double> runMeasurements(
    TimerType type,
    const std::function<void()>& task,
    int runs
);