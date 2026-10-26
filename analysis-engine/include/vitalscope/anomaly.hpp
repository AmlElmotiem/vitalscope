#pragma once

#include <string>
#include <vector>

#include "vitalscope/config.hpp"
#include "vitalscope/measurement.hpp"

namespace vitalscope {

enum class Severity { Info, Warning, Critical };

struct Anomaly {
    std::string code;     // short machine-readable identifier, e.g. "tachycardia"
    std::string message;  // human-readable description
    Severity severity{};
};

// Detects anomalies in a single measurement against `thresholds`.
std::vector<Anomaly> DetectAnomalies(const Measurement& measurement,
                                      const config::Thresholds& thresholds = config::Default());

}  // namespace vitalscope
