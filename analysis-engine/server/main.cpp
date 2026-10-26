// Thin HTTP layer around the vitalscope analysis library. This file is the
// ONLY place in analysis-engine that knows about HTTP or JSON — the library
// itself (vitalscope_lib) stays free of both, so it can be unit-tested and
// reused without a server.
#include <cstdint>
#include <string>
#include <vector>

#include <crow.h>
#include <nlohmann/json.hpp>

#include "vitalscope/analysis.hpp"

using nlohmann::json;
using namespace vitalscope;

namespace {

Measurement MeasurementFromJson(const json& j) {
    Measurement measurement;
    measurement.timestamp_unix = j.value("timestamp", static_cast<std::int64_t>(0));
    measurement.heart_rate = j.at("heart_rate").get<double>();
    measurement.spo2 = j.at("spo2").get<double>();
    measurement.systolic_bp = j.at("systolic_bp").get<double>();
    measurement.diastolic_bp = j.at("diastolic_bp").get<double>();
    measurement.temperature = j.at("temperature").get<double>();
    measurement.respiratory_rate = j.at("respiratory_rate").get<double>();
    return measurement;
}

const char* TrendToString(Trend trend) {
    switch (trend) {
        case Trend::Rising:
            return "rising";
        case Trend::Falling:
            return "falling";
        case Trend::Stable:
            return "stable";
    }
    return "stable";
}

const char* SeverityToString(Severity severity) {
    switch (severity) {
        case Severity::Info:
            return "info";
        case Severity::Warning:
            return "warning";
        case Severity::Critical:
            return "critical";
    }
    return "info";
}

json StatsToJson(const Stats& stats) {
    return json{
        {"average", stats.average},
        {"minimum", stats.minimum},
        {"maximum", stats.maximum},
        {"std_dev", stats.std_dev},
    };
}

json ResultToJson(const AnalysisResult& result) {
    json anomalies = json::array();
    for (const auto& anomaly : result.anomalies) {
        anomalies.push_back(json{
            {"code", anomaly.code},
            {"message", anomaly.message},
            {"severity", SeverityToString(anomaly.severity)},
        });
    }

    return json{
        {"stats",
         {
             {"heart_rate", StatsToJson(result.stats.heart_rate)},
             {"spo2", StatsToJson(result.stats.spo2)},
             {"systolic_bp", StatsToJson(result.stats.systolic_bp)},
             {"diastolic_bp", StatsToJson(result.stats.diastolic_bp)},
             {"temperature", StatsToJson(result.stats.temperature)},
             {"respiratory_rate", StatsToJson(result.stats.respiratory_rate)},
         }},
        {"trends",
         {
             {"heart_rate", TrendToString(result.trends.heart_rate)},
             {"spo2", TrendToString(result.trends.spo2)},
             {"systolic_bp", TrendToString(result.trends.systolic_bp)},
             {"diastolic_bp", TrendToString(result.trends.diastolic_bp)},
             {"temperature", TrendToString(result.trends.temperature)},
             {"respiratory_rate", TrendToString(result.trends.respiratory_rate)},
         }},
        {"anomalies", anomalies},
        {"early_warning_score", result.early_warning_score},
    };
}

}  // namespace

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    .methods(crow::HTTPMethod::GET)([] { return crow::response(200, R"({"status": "ok"})"); });

    CROW_ROUTE(app, "/analyze")
    .methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (const json::parse_error&) {
            return crow::response(400, R"({"error": "invalid JSON"})");
        }

        if (!body.contains("measurements") || !body["measurements"].is_array() ||
            body["measurements"].empty()) {
            return crow::response(400,
                                   R"({"error": "'measurements' must be a non-empty array"})");
        }

        std::vector<Measurement> measurements;
        try {
            for (const auto& item : body["measurements"]) {
                measurements.push_back(MeasurementFromJson(item));
            }
        } catch (const json::exception& e) {
            json error{{"error", std::string("invalid measurement: ") + e.what()}};
            return crow::response(400, error.dump());
        }

        const auto result = AnalyzeMeasurements(measurements);
        crow::response response{ResultToJson(result).dump()};
        response.set_header("Content-Type", "application/json");
        return response;
    });

    app.port(8081).multithreaded().run();
    return 0;
}
