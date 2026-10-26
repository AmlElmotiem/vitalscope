#pragma once

#include "vitalscope/measurement.hpp"

namespace vitalscope {

// A simplified NEWS2-inspired early warning score, based on respiratory
// rate, SpO2, systolic blood pressure, heart rate and temperature. This
// does NOT include consciousness level or supplemental-oxygen use, so it
// is only loosely modeled after the real clinical NEWS2 score, not a
// certified implementation of it.
int ComputeEarlyWarningScore(const Measurement& measurement);

}  // namespace vitalscope
