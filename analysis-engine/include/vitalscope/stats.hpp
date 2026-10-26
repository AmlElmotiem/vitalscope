#pragma once

#include <vector>

namespace vitalscope {

struct Stats {
    double average{};
    double minimum{};
    double maximum{};
    double std_dev{};
};

enum class Trend { Rising, Falling, Stable };

// Computes average, min, max and sample standard deviation of `values`.
// Returns a default-initialized Stats (all zeros) if `values` is empty.
Stats ComputeStats(const std::vector<double>& values);

// Fits a simple linear regression (value over index) and classifies the
// slope as Rising, Falling or Stable using `slope_threshold` as the
// minimum absolute slope required to count as a real trend.
Trend ComputeTrend(const std::vector<double>& values, double slope_threshold = 0.01);

}  // namespace vitalscope
