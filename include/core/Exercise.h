// Exercise.h — muscle groups, exercises, and the exercise library.
#pragma once

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace ironlog {

// enum class (not plain enum): the names don't leak into the namespace and
// there is no silent conversion to int. Same reason as Java's enums being
// real types instead of int constants.
enum class MuscleGroup {
    Chest, Back, Shoulders, Biceps, Triceps,
    Quads, Hamstrings, Glutes, Calves, Abs
};

constexpr std::size_t kMuscleGroupCount = 10;

// Every group in display order. Lets code loop over the enum safely.
const std::array<MuscleGroup, kMuscleGroupCount>& allMuscleGroups();

std::string_view toString(MuscleGroup m);
std::optional<MuscleGroup> muscleGroupFromString(std::string_view s);

// THE rule for "is this the same exercise?" — case-insensitive. Every
// comparison of exercise names goes through here, so the rule lives in
// exactly one place.
bool sameExerciseName(std::string_view a, std::string_view b);

// An exercise is plain data with no invariant to guard -> struct.
// Each exercise counts toward ONE primary muscle group. (Simplification:
// a bench press also works triceps; see Risk R-03 in docs/05_RiskList.md.)
struct Exercise {
    std::string name;
    MuscleGroup primary = MuscleGroup::Chest;
};

// The catalogue of exercises the user can log.
// Invariant: names are non-empty and unique (case-insensitive), so a name
// works as the key that SetEntry uses to point back at its exercise.
class ExerciseLibrary {
public:
    // A starter list covering every muscle group.
    static ExerciseLibrary withDefaults();

    // Returns false (and changes nothing) if the name is blank or taken.
    // A bool return makes the refusal visible to the caller instead of
    // failing silently.
    bool add(Exercise e);

    // nullptr if not found. Returning a pointer (not a copy) is the usual
    // C++ "maybe a reference" idiom; the pointer is valid until the next add().
    const Exercise* find(std::string_view name) const;

    const std::vector<Exercise>& all() const { return m_exercises; }
    std::size_t size() const { return m_exercises.size(); }

private:
    std::vector<Exercise> m_exercises;
};

} // namespace ironlog
