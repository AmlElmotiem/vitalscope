#include "vitalscope/analysis.hpp"

#include <stdexcept>

#include "vitalscope/news2.hpp"

namespace vitalscope {
namespace {

std::vector<double> Extract(const std::vector<Measurement>& measurements,
                             double Measurement::*field) {
    std::vector<double> values;
    values.reserve(measurements.size());
    for (const auto& measurement : measurements) {
        values.push_back(measurement.*field);
    }
    return values;
}

}  // namespace

AnalysisResult AnalyzeMeasurements(const std::vector<Measurement>& measurements,
                                    const config::Thresholds& thresholds) {
    if (measurements.empty()) {
        throw std::invalid_argument("AnalyzeMeasurements requires at least one measurement");
    }

    AnalysisResult result;

    const auto heart_rates = Extract(measurements, &Measurement::heart_rate);
    const auto spo2s = Extract(measurements, &Measurement::spo2);
    const auto systolics = Extract(measurements, &Measurement::systolic_bp);
    const auto diastolics = Extract(measurements, &Measurement::diastolic_bp);
    const auto temperatures = Extract(measurements, &Measurement::temperature);
    const auto resp_rates = Extract(measurements, &Measurement::respiratory_rate);

    result.stats.heart_rate = ComputeStats(heart_rates);
    result.stats.spo2 = ComputeStats(spo2s);
    result.stats.systolic_bp = ComputeStats(systolics);
    result.stats.diastolic_bp = ComputeStats(diastolics);
    result.stats.temperature = ComputeStats(temperatures);
    result.stats.respiratory_rate = ComputeStats(resp_rates);

    result.trends.heart_rate = ComputeTrend(heart_rates);
    result.trends.spo2 = ComputeTrend(spo2s);
    result.trends.systolic_bp = ComputeTrend(systolics);
    result.trends.diastolic_bp = ComputeTrend(diastolics);
    result.trends.temperature = ComputeTrend(temperatures);
    result.trends.respiratory_rate = ComputeTrend(resp_rates);

    const Measurement& latest = measurements.back();
    result.anomalies = DetectAnomalies(latest, thresholds);
    result.early_warning_score = ComputeEarlyWarningScore(latest);

    return result;
}

}  // namespace vitalscope
