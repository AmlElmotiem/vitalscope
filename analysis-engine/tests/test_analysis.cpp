#include "vitalscope/analysis.hpp"

#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

using vitalscope::AnalyzeMeasurements;
using vitalscope::Measurement;
using vitalscope::Trend;

TEST(AnalyzeMeasurementsTest, EmptyInputThrows) {
    EXPECT_THROW(AnalyzeMeasurements({}), std::invalid_argument);
}

TEST(AnalyzeMeasurementsTest, ComputesStatsAndTrendAcrossMeasurements) {
    const std::vector<Measurement> measurements{
        Measurement{.heart_rate = 70, .spo2 = 98, .systolic_bp = 120, .temperature = 37.0,
                    .respiratory_rate = 16},
        Measurement{.heart_rate = 80, .spo2 = 97, .systolic_bp = 122, .temperature = 37.1,
                    .respiratory_rate = 16},
        Measurement{.heart_rate = 90, .spo2 = 96, .systolic_bp = 124, .temperature = 37.2,
                    .respiratory_rate = 17},
    };

    const auto result = AnalyzeMeasurements(measurements);

    EXPECT_DOUBLE_EQ(result.stats.heart_rate.average, 80.0);
    EXPECT_EQ(result.trends.heart_rate, Trend::Rising);
    EXPECT_EQ(result.trends.spo2, Trend::Falling);
}

TEST(AnalyzeMeasurementsTest, AnomaliesAreBasedOnMostRecentMeasurement) {
    const std::vector<Measurement> measurements{
        Measurement{.heart_rate = 70, .spo2 = 98, .systolic_bp = 120, .temperature = 37.0,
                    .respiratory_rate = 16},
        Measurement{.heart_rate = 140, .spo2 = 98, .systolic_bp = 120, .temperature = 37.0,
                    .respiratory_rate = 16},
    };

    const auto result = AnalyzeMeasurements(measurements);

    ASSERT_EQ(result.anomalies.size(), 1u);
    EXPECT_EQ(result.anomalies[0].code, "tachycardia");
}
