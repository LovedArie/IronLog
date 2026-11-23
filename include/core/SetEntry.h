// SetEntry.h — one logged set: exercise, weight, reps.
#pragma once

#include <optional>
#include <string>

namespace ironlog {

// A SetEntry HAS an invariant: weight >= 0 and 1 <= reps <= kMaxReps.
// So it is a class with private data, and the only way to build one is the
// checked factory make(). Stroustrup §17.6.3.2: "Establish an invariant in a
// constructor." Here the constructor is private and make() decides first,
// because a constructor can't "return nothing" without throwing.
//
// Why std::optional instead of throwing? Bad input from a form is EXPECTED,
// not exceptional. optional puts "this can be refused" in the type, so the
// caller can't forget to check. (Contrast: TickTimer's doors refused by
// returning an empty id string — easy to miss.)
class SetEntry {
public:
    static constexpr int kMaxReps = 100;
    static constexpr double kMaxWeightKg = 1000.0;

    static std::optional<SetEntry> make(std::string exercise, double weightKg, int reps);

    const std::string& exercise() const { return m_exercise; }
    double weightKg() const { return m_weightKg; }
    int reps() const { return m_reps; }

    // Information Expert (Larman Ch. 17): the set has the weight and reps,
    // so the set computes its own volume and estimated 1RM.
    double volume() const { return m_weightKg * m_reps; }   // "tonnage", kg
    double estimatedOneRepMax() const;

private:
    SetEntry(std::string exercise, double weightKg, int reps)
        : m_exercise(std::move(exercise)), m_weightKg(weightKg), m_reps(reps) {}

    std::string m_exercise;
    double m_weightKg = 0.0;
    int m_reps = 1;
};

// Epley formula: e1RM = w * (1 + reps / 30). For a single rep, the lift
// IS the 1RM, so we return w unchanged. Free function so tests and the
// chart can use it on raw numbers too.
double epleyOneRepMax(double weightKg, int reps);

} // namespace ironlog
