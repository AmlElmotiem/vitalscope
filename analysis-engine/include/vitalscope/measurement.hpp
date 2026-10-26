#pragma once

#include <cstdint>

namespace vitalscope {

// A single vital-signs reading for a patient at one point in time.
struct Measurement {
    std::int64_t timestamp_unix{};
    double heart_rate{};
    double spo2{};
    double systolic_bp{};
    double diastolic_bp{};
    double temperature{};
    double respiratory_rate{};
};

}  // namespace vitalscope
