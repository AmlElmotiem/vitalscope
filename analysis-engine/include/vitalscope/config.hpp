#pragma once

namespace vitalscope::config {

// Clinical thresholds used by anomaly detection and scoring. Kept in one
// place so none of the detection logic hardcodes a magic number.
struct Thresholds {
    // Heart rate (bpm)
    double tachycardia_bpm = 100.0;
    double bradycardia_bpm = 60.0;

    // SpO2 (%)
    double spo2_warning = 94.0;
    double spo2_critical = 90.0;

    // Blood pressure (mmHg)
    double hypertension_systolic = 140.0;
    double hypotension_systolic = 90.0;

    // Temperature (°C)
    double fever_celsius = 38.0;
    double hypothermia_celsius = 35.0;
};

inline const Thresholds& Default() {
    static const Thresholds instance{};
    return instance;
}

}  // namespace vitalscope::config
