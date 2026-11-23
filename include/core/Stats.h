// Stats.h — read-only analysis of a TrainingLog.
//
// WHY FREE FUNCTIONS, NOT TrainingLog METHODS? Information Expert would say
// "the log has the data, so the log computes." But piling every report onto
// TrainingLog makes it a god class with low cohesion (Larman Ch. 17 lists
// exactly this contraindication). These functions only READ through the
// public interface, so they don't need to be members. Keeping them outside
// keeps TrainingLog small and its invariant easy to audit.
#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>

#include "core/Date.h"
#include "core/Exercise.h"
#include "core/Workout.h"

namespace ironlog::stats {

// ---- Strength progress -----------------------------------------------------

struct ProgressPoint {
    Date date;
    double bestE1rm = 0.0; // the day's best estimated 1RM, kg
};

// One point per session that included the exercise, oldest first.
std::vector<ProgressPoint> e1rmHistory(const TrainingLog& log, std::string_view exercise);

// Best e1RM on any day strictly BEFORE `date`; nullopt if never done before.
std::optional<double> bestE1rmBefore(const TrainingLog& log, std::string_view exercise, Date date);

// Best e1RM ever; nullopt if never done.
std::optional<double> allTimeBestE1rm(const TrainingLog& log, std::string_view exercise);

// The most recent set of this exercise (latest day, last set that day).
// The UI uses it to pre-fill weight and reps: "what did I do last time?"
std::optional<SetEntry> lastSet(const TrainingLog& log, std::string_view exercise);

// Which sets in `w` are personal records (PRs)?
// Rule: for each exercise in the session, take the FIRST set with that day's
// top e1RM. It is a PR if it strictly beats every EARLIER session.
// An exercise's very first session is NOT a PR — there's nothing to beat,
// and flagging every set on day one would make the trophy meaningless.
std::vector<std::size_t> personalRecordIndices(const TrainingLog& log, const Workout& w);

// ---- Weekly volume per muscle group ----------------------------------------

// Default target: 10–20 sets per muscle per week. A commonly used starting
// range for hypertrophy (reviews find benefits from ~10+ weekly sets; see
// docs/arc42.md §8.13 for sources). Deliberately a parameter, not a law.
struct VolumeTarget {
    int low = 10;
    int high = 20;
};

enum class VolumeStatus { Below, InRange, Above };

VolumeStatus classify(int sets, VolumeTarget target = {});

// Index into a per-muscle array. Relies on allMuscleGroups() listing the
// enum in declaration order (a test pins this).
inline std::size_t indexOf(MuscleGroup m) { return static_cast<std::size_t>(m); }

using MuscleCounts = std::array<int, kMuscleGroupCount>;

// Sets per muscle group in the Monday–Sunday week containing `anyDay`.
// Sets whose exercise is not in the library are skipped (can't classify them).
MuscleCounts weeklySets(const TrainingLog& log, const ExerciseLibrary& library, Date anyDay);

} // namespace ironlog::stats
