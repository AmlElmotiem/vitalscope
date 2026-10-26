#include "vitalscope/news2.hpp"

#include <gtest/gtest.h>

using vitalscope::ComputeEarlyWarningScore;
using vitalscope::Measurement;

TEST(ComputeEarlyWarningScoreTest, NormalMeasurementScoresZero) {
    const Measurement measurement{
        .heart_rate = 75,
        .spo2 = 98,
        .systolic_bp = 120,
        .temperature = 37.0,
        .respiratory_rate = 16,
    };
    EXPECT_EQ(ComputeEarlyWarningScore(measurement), 0);
}

TEST(ComputeEarlyWarningScoreTest, CriticalMeasurementScoresHigh) {
    const Measurement measurement{
        .heart_rate = 145,      // 3
        .spo2 = 88,             // 3
        .systolic_bp = 85,      // 3
        .temperature = 34.5,    // 3
        .respiratory_rate = 6,  // 3
    };
    EXPECT_EQ(ComputeEarlyWarningScore(measurement), 15);
}
