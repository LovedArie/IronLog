// Workout.cpp

#include "core/Workout.h"

#include <algorithm>
#include <numeric>

namespace ironlog {

bool Workout::removeSet(std::size_t index) {
    if (index >= m_sets.size()) return false;
    m_sets.erase(m_sets.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

double Workout::totalVolume() const {
    return std::accumulate(m_sets.begin(), m_sets.end(), 0.0,
                           [](double sum, const SetEntry& s) { return sum + s.volume(); });
}

namespace {
// lower_bound needs "is this workout before date d?" — this is that test.
bool before(const Workout& w, const Date& d) { return w.date() < d; }
} // namespace

Workout& TrainingLog::workoutOn(Date d) {
    // Binary search for the insertion point keeps the vector sorted
    // without ever calling sort(). O(log n) find + O(n) insert — fine for
    // a few thousand sessions (years of training).
    auto it = std::lower_bound(m_workouts.begin(), m_workouts.end(), d, before);
    if (it != m_workouts.end() && it->date() == d) return *it;
    return *m_workouts.insert(it, Workout(d));
}

const Workout* TrainingLog::find(Date d) const {
    auto it = std::lower_bound(m_workouts.begin(), m_workouts.end(), d, before);
    return (it != m_workouts.end() && it->date() == d) ? &*it : nullptr;
}

Workout* TrainingLog::find(Date d) {
    // Reuse the const version instead of duplicating the search.
    // const_cast is safe here: *this is non-const, so the object is too.
    return const_cast<Workout*>(static_cast<const TrainingLog&>(*this).find(d));
}

bool TrainingLog::removeWorkout(Date d) {
    auto it = std::lower_bound(m_workouts.begin(), m_workouts.end(), d, before);
    if (it == m_workouts.end() || it->date() != d) return false;
    m_workouts.erase(it);
    return true;
}

void TrainingLog::pruneEmpty() {
    // erase-remove idiom: remove_if packs the keepers to the front and
    // returns the new logical end; erase chops off the rest.
    m_workouts.erase(std::remove_if(m_workouts.begin(), m_workouts.end(),
                                    [](const Workout& w) { return w.isEmpty(); }),
                     m_workouts.end());
}

} // namespace ironlog
