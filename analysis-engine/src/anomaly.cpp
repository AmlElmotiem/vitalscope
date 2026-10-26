#include "vitalscope/anomaly.hpp"

namespace vitalscope {

std::vector<Anomaly> DetectAnomalies(const Measurement& measurement,
                                      const config::Thresholds& thresholds) {
    std::vector<Anomaly> anomalies;

    if (measurement.heart_rate > thresholds.tachycardia_bpm) {
        anomalies.push_back({"tachycardia", "Heart rate above normal range", Severity::Warning});
    }
    if (measurement.heart_rate < thresholds.bradycardia_bpm) {
        anomalies.push_back({"bradycardia", "Heart rate below normal range", Severity::Warning});
    }

    if (measurement.spo2 < thresholds.spo2_critical) {
        anomalies.push_back(
            {"hypoxemia", "Oxygen saturation critically low", Severity::Critical});
    } else if (measurement.spo2 < thresholds.spo2_warning) {
        anomalies.push_back(
            {"hypoxemia", "Oxygen saturation below normal range", Severity::Warning});
    }

    if (measurement.systolic_bp > thresholds.hypertension_systolic) {
        anomalies.push_back(
            {"hypertension", "Systolic blood pressure above normal range", Severity::Warning});
    }
    if (measurement.systolic_bp < thresholds.hypotension_systolic) {
        anomalies.push_back(
            {"hypotension", "Systolic blood pressure below normal range", Severity::Warning});
    }

    if (measurement.temperature > thresholds.fever_celsius) {
        anomalies.push_back({"fever", "Body temperature above normal range", Severity::Warning});
    }
    if (measurement.temperature < thresholds.hypothermia_celsius) {
        anomalies.push_back(
            {"hypothermia", "Body temperature below normal range", Severity::Warning});
    }

    return anomalies;
}

}  // namespace vitalscope
