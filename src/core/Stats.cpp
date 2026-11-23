// Stats.cpp

#include "core/Stats.h"

#include <algorithm>

namespace ironlog::stats {

namespace {

// The day's best e1RM for one exercise, or nullopt if not done that day.
std::optional<double> dayBest(const Workout& w, std::string_view exercise) {
    std::optional<double> best;
    for (const SetEntry& s : w.sets()) {
        if (!sameExerciseName(s.exercise(), exercise)) continue;
        const double e = s.estimatedOneRepMax();
        if (!best || e > *best) best = e;
    }
    return best;
}

} // namespace

std::vector<ProgressPoint> e1rmHistory(const TrainingLog& log, std::string_view exercise) {
    std::vector<ProgressPoint> points;
    // workouts() is already date-sorted (TrainingLog's invariant), so the
    // result is chronological for free.
    for (const Workout& w : log.workouts())
        if (auto best = dayBest(w, exercise)) points.push_back({w.date(), *best});
    return points;
}

std::optional<double> bestE1rmBefore(const TrainingLog& log, std::string_view exercise, Date date) {
    std::optional<double> best;
    for (const Workout& w : log.workouts()) {
        if (w.date() >= date) break; // sorted: everything after is later too
        if (auto b = dayBest(w, exercise); b && (!best || *b > *best)) best = b;
    }
    return best;
}

std::optional<double> allTimeBestE1rm(const TrainingLog& log, std::string_view exercise) {
    std::optional<double> best;
    for (const Workout& w : log.workouts())
        if (auto b = dayBest(w, exercise); b && (!best || *b > *best)) best = b;
    return best;
}

std::optional<SetEntry> lastSet(const TrainingLog& log, std::string_view exercise) {
    // Walk BACKWARDS (newest day first, last set first) and stop at the
    // first match: crbegin/crend are reverse iterators.
    const auto& days = log.workouts();
    for (auto w = days.crbegin(); w != days.crend(); ++w)
        for (auto s = w->sets().crbegin(); s != w->sets().crend(); ++s)
            if (sameExerciseName(s->exercise(), exercise)) return *s;
    return std::nullopt;
}

std::vector<std::size_t> personalRecordIndices(const TrainingLog& log, const Workout& w) {
    std::vector<std::size_t> prs;
    const auto& sets = w.sets();
    for (std::size_t i = 0; i < sets.size(); ++i) {
        const SetEntry& s = sets[i];

        // Only judge each exercise once: at its first appearance today.
        bool seenEarlierToday = false;
        for (std::size_t j = 0; j < i; ++j)
            if (sameExerciseName(sets[j].exercise(), s.exercise())) { seenEarlierToday = true; break; }
        if (seenEarlierToday) continue;

        const auto previous = bestE1rmBefore(log, s.exercise(), w.date());
        if (!previous) continue; // first-ever session: nothing to beat

        // Find the FIRST set today with the day's top e1RM for this exercise.
        std::size_t topIndex = i;
        double top = s.estimatedOneRepMax();
        for (std::size_t j = i + 1; j < sets.size(); ++j) {
            if (!sameExerciseName(sets[j].exercise(), s.exercise())) continue;
            if (sets[j].estimatedOneRepMax() > top) { top = sets[j].estimatedOneRepMax(); topIndex = j; }
        }
        if (top > *previous) prs.push_back(topIndex);
    }
    std::sort(prs.begin(), prs.end());
    return prs;
}

VolumeStatus classify(int sets, VolumeTarget target) {
    if (sets < target.low) return VolumeStatus::Below;
    if (sets > target.high) return VolumeStatus::Above;
    return VolumeStatus::InRange;
}

MuscleCounts weeklySets(const TrainingLog& log, const ExerciseLibrary& library, Date anyDay) {
    MuscleCounts counts{}; // {} zero-initialises every element
    const Date monday = anyDay.mondayOfWeek();
    const Date nextMonday = monday.addDays(7);
    for (const Workout& w : log.workouts()) {
        if (w.date() < monday) continue;
        if (w.date() >= nextMonday) break; // sorted -> done
        for (const SetEntry& s : w.sets())
            if (const Exercise* e = library.find(s.exercise())) ++counts[indexOf(e->primary)];
    }
    return counts;
}

} // namespace ironlog::stats
