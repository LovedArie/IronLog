// Workout.h — one training session (one per calendar day) and the full log.
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "core/Date.h"
#include "core/SetEntry.h"

namespace ironlog {

// One day's session: a date plus its sets, in the order they were done.
// Assumption for iteration 1: at most ONE session per day (see Vision).
class Workout {
public:
    explicit Workout(Date date) : m_date(date) {}

    Date date() const { return m_date; }
    const std::vector<SetEntry>& sets() const { return m_sets; }
    bool isEmpty() const { return m_sets.empty(); }

    void addSet(SetEntry s) { m_sets.push_back(std::move(s)); }

    // false if index is out of range (nothing changes).
    bool removeSet(std::size_t index);

    // Information Expert again: the workout holds the sets, so it knows the
    // total. Same shape as Larman's Sale.getTotal() summing its line items.
    double totalVolume() const;

private:
    Date m_date;
    std::vector<SetEntry> m_sets;
};

// The whole training history. Invariant: workouts are sorted by date and
// no two share a date. Every mutation goes through methods that keep that
// true, so readers (charts, stats) can rely on chronological order.
class TrainingLog {
public:
    // Get-or-create (GRASP Creator, Larman Ch. 17: the log CONTAINS
    // workouts, so the log creates them). Inserts in sorted position.
    Workout& workoutOn(Date d);

    // nullptr if no session that day.
    const Workout* find(Date d) const;
    Workout* find(Date d);

    // Removes the whole session. false if there was none.
    bool removeWorkout(Date d);

    // Drops sessions with zero sets (e.g. after deleting the last set),
    // so an empty day doesn't show up as "trained" on the calendar.
    void pruneEmpty();

    const std::vector<Workout>& workouts() const { return m_workouts; }
    bool isEmpty() const { return m_workouts.empty(); }

private:
    std::vector<Workout> m_workouts; // sorted ascending by date
};

} // namespace ironlog
