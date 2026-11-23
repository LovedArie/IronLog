// SetEntry.cpp

#include "core/SetEntry.h"

#include <cmath>

namespace ironlog {

double epleyOneRepMax(double weightKg, int reps) {
    if (reps <= 1) return weightKg;
    return weightKg * (1.0 + reps / 30.0);
}

std::optional<SetEntry> SetEntry::make(std::string exercise, double weightKg, int reps) {
    // std::isfinite rejects NaN and infinity — a NaN would poison every sum
    // and comparison downstream (NaN > x is always false).
    if (exercise.empty()) return std::nullopt;
    if (!std::isfinite(weightKg) || weightKg < 0.0 || weightKg > kMaxWeightKg)
        return std::nullopt;
    if (reps < 1 || reps > kMaxReps) return std::nullopt;
    return SetEntry(std::move(exercise), weightKg, reps);
}

double SetEntry::estimatedOneRepMax() const {
    return epleyOneRepMax(m_weightKg, m_reps);
}

} // namespace ironlog
