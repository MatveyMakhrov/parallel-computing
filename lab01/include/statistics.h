#pragma once

#include <vector>

struct Statistics {
    double min = 0.0;
    double mean = 0.0;
    double variance = 0.0;
    double stddev = 0.0;
    double ciLow = 0.0;
    double ciHigh = 0.0;
};

Statistics calculateStatistics(const std::vector<double>& data);

void printMeasurements(const std::vector<double>& data);
void printStatistics(const Statistics& stats);
void printHistogram(const std::vector<double>& data, int bins = 10);