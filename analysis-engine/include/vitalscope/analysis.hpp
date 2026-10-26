#pragma once

#include <vector>

#include "vitalscope/anomaly.hpp"
#include "vitalscope/config.hpp"
#include "vitalscope/measurement.hpp"
#include "vitalscope/stats.hpp"

namespace vitalscope {

struct VitalStats {
    Stats heart_rate;
    Stats spo2;
    Stats systolic_bp;
    Stats diastolic_bp;
    Stats temperature;
    Stats respiratory_rate;
};

struct VitalTrends {
    Trend heart_rate{};
    Trend spo2{};
    Trend systolic_bp{};
    Trend diastolic_bp{};
    Trend temperature{};
    Trend respiratory_rate{};
};

struct AnalysisResult {
    VitalStats stats;
    VitalTrends trends;
    std::vector<Anomaly> anomalies;  // anomalies on the most recent measurement
    int early_warning_score{};       // based on the most recent measurement
};

// Analyzes a time-ordered list of measurements (oldest first). Throws
// std::invalid_argument if `measurements` is empty.
AnalysisResult AnalyzeMeasurements(const std::vector<Measurement>& measurements,
                                    const config::Thresholds& thresholds = config::Default());

}  // namespace vitalscope
