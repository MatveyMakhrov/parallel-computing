#include "statistics.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>

Statistics calculateStatistics(const std::vector<double>& data) {
    Statistics stats{};

    if (data.empty()) {
        return stats;
    }

    stats.min = *std::min_element(data.begin(), data.end());

    const double sum = std::accumulate(data.begin(), data.end(), 0.0);
    stats.mean = sum / static_cast<double>(data.size());

    double sqSum = 0.0;
    for (double x : data) {
        const double d = x - stats.mean;
        sqSum += d * d;
    }

    // Выборочная дисперсия
    if (data.size() > 1) {
        stats.variance = sqSum / static_cast<double>(data.size() - 1);
    } else {
        stats.variance = 0.0;
    }

    stats.stddev = std::sqrt(stats.variance);

    // 95% доверительный интервал для среднего (приближённо через 1.96)
    const double z = 1.96;
    const double margin = z * stats.stddev / std::sqrt(static_cast<double>(data.size()));
    stats.ciLow = stats.mean - margin;
    stats.ciHigh = stats.mean + margin;

    return stats;
}

void printMeasurements(const std::vector<double>& data) {
    std::cout << "Measurements:\n";
    for (std::size_t i = 0; i < data.size(); ++i) {
        std::cout << std::setw(2) << (i + 1) << " : "
                  << std::fixed << std::setprecision(4)
                  << data[i] << " ms\n";
    }
}

void printStatistics(const Statistics& stats) {
    std::cout << "\nStatistics:\n";
    std::cout << "Minimum: " << std::fixed << std::setprecision(4) << stats.min << " ms\n";
    std::cout << "Average: " << std::fixed << std::setprecision(4) << stats.mean << " ms\n";
    std::cout << "Variance: " << std::fixed << std::setprecision(6) << stats.variance << "\n";
    std::cout << "SKO: " << std::fixed << std::setprecision(6) << stats.stddev << " ms\n";
    std::cout << "95% interval: ["
              << std::fixed << std::setprecision(4)
              << stats.ciLow << "; " << stats.ciHigh << "] ms\n";
}

void printHistogram(const std::vector<double>& data, int bins) {
    if (data.empty() || bins <= 0) {
        return;
    }

    const auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
    const double minVal = *minIt;
    const double maxVal = *maxIt;

    std::cout << "\nThe histogram:\n";

    if (std::abs(maxVal - minVal) < 1e-12) {
        std::cout << "[" << minVal << " - " << maxVal << "] ";
        for (std::size_t i = 0; i < data.size(); ++i) {
            std::cout << "#";
        }
        std::cout << "\n";
        return;
    }

    const double binWidth = (maxVal - minVal) / static_cast<double>(bins);
    std::vector<int> counts(bins, 0);

    for (double x : data) {
        int idx = static_cast<int>((x - minVal) / binWidth);
        if (idx == bins) {
            idx = bins - 1;
        }
        counts[idx]++;
    }

    for (int i = 0; i < bins; ++i) {
        const double left = minVal + i * binWidth;
        const double right = left + binWidth;

        std::cout << "["
                  << std::fixed << std::setprecision(4)
                  << left << " - " << right << "] ";

        for (int j = 0; j < counts[i]; ++j) {
            std::cout << "#";
        }

        std::cout << "\n";
    }
}