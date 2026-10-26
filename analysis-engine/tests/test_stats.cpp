#include "vitalscope/stats.hpp"

#include <gtest/gtest.h>

using vitalscope::ComputeStats;
using vitalscope::ComputeTrend;
using vitalscope::Trend;

TEST(ComputeStatsTest, EmptyInputReturnsZeroedStats) {
    const auto stats = ComputeStats({});
    EXPECT_DOUBLE_EQ(stats.average, 0.0);
    EXPECT_DOUBLE_EQ(stats.minimum, 0.0);
    EXPECT_DOUBLE_EQ(stats.maximum, 0.0);
    EXPECT_DOUBLE_EQ(stats.std_dev, 0.0);
}

TEST(ComputeStatsTest, ComputesAverageMinMax) {
    const auto stats = ComputeStats({1.0, 2.0, 3.0, 4.0, 5.0});
    EXPECT_DOUBLE_EQ(stats.average, 3.0);
    EXPECT_DOUBLE_EQ(stats.minimum, 1.0);
    EXPECT_DOUBLE_EQ(stats.maximum, 5.0);
}

TEST(ComputeStatsTest, SingleValueHasZeroStdDev) {
    const auto stats = ComputeStats({42.0});
    EXPECT_DOUBLE_EQ(stats.std_dev, 0.0);
}

TEST(ComputeTrendTest, FewerThanTwoValuesIsStable) {
    EXPECT_EQ(ComputeTrend({}), Trend::Stable);
    EXPECT_EQ(ComputeTrend({5.0}), Trend::Stable);
}

TEST(ComputeTrendTest, DetectsRisingTrend) {
    EXPECT_EQ(ComputeTrend({60, 65, 70, 75, 80}), Trend::Rising);
}

TEST(ComputeTrendTest, DetectsFallingTrend) {
    EXPECT_EQ(ComputeTrend({100, 95, 90, 85, 80}), Trend::Falling);
}

TEST(ComputeTrendTest, DetectsStableTrend) {
    // Symmetric fluctuation around 70: the regression slope is exactly 0.
    EXPECT_EQ(ComputeTrend({70, 71, 69, 71, 70}), Trend::Stable);
}
