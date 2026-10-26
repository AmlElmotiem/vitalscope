#include "vitalscope/anomaly.hpp"

#include <gtest/gtest.h>

using vitalscope::DetectAnomalies;
using vitalscope::Measurement;
using vitalscope::Severity;

namespace {

Measurement NormalMeasurement() {
    return Measurement{
        .timestamp_unix = 0,
        .heart_rate = 75,
        .spo2 = 98,
        .systolic_bp = 120,
        .diastolic_bp = 80,
        .temperature = 37.0,
        .respiratory_rate = 16,
    };
}

}  // namespace

TEST(DetectAnomaliesTest, NormalMeasurementHasNoAnomalies) {
    EXPECT_TRUE(DetectAnomalies(NormalMeasurement()).empty());
}

TEST(DetectAnomaliesTest, DetectsTachycardia) {
    auto measurement = NormalMeasurement();
    measurement.heart_rate = 120;
    const auto anomalies = DetectAnomalies(measurement);
    ASSERT_EQ(anomalies.size(), 1u);
    EXPECT_EQ(anomalies[0].code, "tachycardia");
    EXPECT_EQ(anomalies[0].severity, Severity::Warning);
}

TEST(DetectAnomaliesTest, DetectsCriticalHypoxemia) {
    auto measurement = NormalMeasurement();
    measurement.spo2 = 85;
    const auto anomalies = DetectAnomalies(measurement);
    ASSERT_EQ(anomalies.size(), 1u);
    EXPECT_EQ(anomalies[0].code, "hypoxemia");
    EXPECT_EQ(anomalies[0].severity, Severity::Critical);
}

TEST(DetectAnomaliesTest, DetectsWarningHypoxemia) {
    auto measurement = NormalMeasurement();
    measurement.spo2 = 93;
    const auto anomalies = DetectAnomalies(measurement);
    ASSERT_EQ(anomalies.size(), 1u);
    EXPECT_EQ(anomalies[0].severity, Severity::Warning);
}

TEST(DetectAnomaliesTest, DetectsFever) {
    auto measurement = NormalMeasurement();
    measurement.temperature = 39.0;
    const auto anomalies = DetectAnomalies(measurement);
    ASSERT_EQ(anomalies.size(), 1u);
    EXPECT_EQ(anomalies[0].code, "fever");
}

TEST(DetectAnomaliesTest, MultipleAnomaliesAreAllReported) {
    auto measurement = NormalMeasurement();
    measurement.heart_rate = 120;
    measurement.temperature = 39.0;
    EXPECT_EQ(DetectAnomalies(measurement).size(), 2u);
}
