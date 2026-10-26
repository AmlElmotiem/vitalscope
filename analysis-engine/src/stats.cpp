#include "vitalscope/stats.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numeric>

namespace vitalscope {

Stats ComputeStats(const std::vector<double>& values) {
    if (values.empty()) {
        return Stats{};
    }

    const double sum = std::accumulate(values.begin(), values.end(), 0.0);
    const double average = sum / static_cast<double>(values.size());

    double minimum = values.front();
    double maximum = values.front();
    double squared_diff_sum = 0.0;
    for (const double value : values) {
        minimum = std::min(minimum, value);
        maximum = std::max(maximum, value);
        const double diff = value - average;
        squared_diff_sum += diff * diff;
    }

    // Sample standard deviation (n - 1); a single value has no spread.
    const double variance =
        values.size() > 1 ? squared_diff_sum / static_cast<double>(values.size() - 1) : 0.0;

    return Stats{average, minimum, maximum, std::sqrt(variance)};
}

Trend ComputeTrend(const std::vector<double>& values, double slope_threshold) {
    const std::size_t n = values.size();
    if (n < 2) {
        return Trend::Stable;
    }

    // Simple linear regression: value = slope * index + intercept.
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double x = static_cast<double>(i);
        const double y = values[i];
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
    }

    const double denominator = static_cast<double>(n) * sum_xx - sum_x * sum_x;
    if (denominator == 0.0) {
        return Trend::Stable;
    }

    const double slope = (static_cast<double>(n) * sum_xy - sum_x * sum_y) / denominator;

    if (slope > slope_threshold) return Trend::Rising;
    if (slope < -slope_threshold) return Trend::Falling;
    return Trend::Stable;
}

}  // namespace vitalscope
