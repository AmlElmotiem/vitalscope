#include "vitalscope/news2.hpp"

namespace vitalscope {
namespace {

int ScoreRespiratoryRate(double rate) {
    if (rate <= 8) return 3;
    if (rate <= 11) return 1;
    if (rate <= 20) return 0;
    if (rate <= 24) return 2;
    return 3;
}

int ScoreSpo2(double spo2) {
    if (spo2 <= 91) return 3;
    if (spo2 <= 93) return 2;
    if (spo2 <= 95) return 1;
    return 0;
}

int ScoreSystolicBp(double systolic) {
    if (systolic <= 90) return 3;
    if (systolic <= 100) return 2;
    if (systolic <= 110) return 1;
    if (systolic <= 219) return 0;
    return 3;
}

int ScoreHeartRate(double rate) {
    if (rate <= 40) return 3;
    if (rate <= 50) return 1;
    if (rate <= 90) return 0;
    if (rate <= 110) return 1;
    if (rate <= 130) return 2;
    return 3;
}

int ScoreTemperature(double temperature) {
    if (temperature <= 35.0) return 3;
    if (temperature <= 36.0) return 1;
    if (temperature <= 38.0) return 0;
    if (temperature <= 39.0) return 1;
    return 2;
}

}  // namespace

int ComputeEarlyWarningScore(const Measurement& measurement) {
    return ScoreRespiratoryRate(measurement.respiratory_rate) + ScoreSpo2(measurement.spo2) +
           ScoreSystolicBp(measurement.systolic_bp) + ScoreHeartRate(measurement.heart_rate) +
           ScoreTemperature(measurement.temperature);
}

}  // namespace vitalscope
