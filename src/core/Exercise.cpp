// Exercise.cpp

#include "core/Exercise.h"

#include <algorithm>
#include <cctype>

namespace ironlog {

namespace {

// Case-insensitive compare so "bench press" and "Bench Press" collide.
bool equalsIgnoreCase(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const auto ca = static_cast<unsigned char>(a[i]);
        const auto cb = static_cast<unsigned char>(b[i]);
        if (std::tolower(ca) != std::tolower(cb)) return false;
    }
    return true;
}

std::string trimmed(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

} // namespace

const std::array<MuscleGroup, kMuscleGroupCount>& allMuscleGroups() {
    static const std::array<MuscleGroup, kMuscleGroupCount> kAll = {
        MuscleGroup::Chest, MuscleGroup::Back, MuscleGroup::Shoulders,
        MuscleGroup::Biceps, MuscleGroup::Triceps, MuscleGroup::Quads,
        MuscleGroup::Hamstrings, MuscleGroup::Glutes, MuscleGroup::Calves,
        MuscleGroup::Abs};
    return kAll;
}

std::string_view toString(MuscleGroup m) {
    switch (m) {
    case MuscleGroup::Chest:      return "Chest";
    case MuscleGroup::Back:       return "Back";
    case MuscleGroup::Shoulders:  return "Shoulders";
    case MuscleGroup::Biceps:     return "Biceps";
    case MuscleGroup::Triceps:    return "Triceps";
    case MuscleGroup::Quads:      return "Quads";
    case MuscleGroup::Hamstrings: return "Hamstrings";
    case MuscleGroup::Glutes:     return "Glutes";
    case MuscleGroup::Calves:     return "Calves";
    case MuscleGroup::Abs:        return "Abs";
    }
    return "Unknown"; // unreachable; keeps -Wreturn-type quiet
}

bool sameExerciseName(std::string_view a, std::string_view b) {
    return equalsIgnoreCase(a, b);
}

std::optional<MuscleGroup> muscleGroupFromString(std::string_view s) {
    for (MuscleGroup m : allMuscleGroups())
        if (equalsIgnoreCase(toString(m), s)) return m;
    return std::nullopt;
}

ExerciseLibrary ExerciseLibrary::withDefaults() {
    using M = MuscleGroup;
    ExerciseLibrary lib;
    const Exercise defaults[] = {
        {"Bench Press", M::Chest},          {"Incline Dumbbell Press", M::Chest},
        {"Barbell Row", M::Back},           {"Pull-up", M::Back},
        {"Lat Pulldown", M::Back},          {"Overhead Press", M::Shoulders},
        {"Lateral Raise", M::Shoulders},    {"Barbell Curl", M::Biceps},
        {"Hammer Curl", M::Biceps},         {"Tricep Pushdown", M::Triceps},
        {"Skull Crusher", M::Triceps},      {"Back Squat", M::Quads},
        {"Leg Press", M::Quads},            {"Romanian Deadlift", M::Hamstrings},
        {"Leg Curl", M::Hamstrings},        {"Hip Thrust", M::Glutes},
        {"Standing Calf Raise", M::Calves}, {"Cable Crunch", M::Abs},
    };
    for (const Exercise& e : defaults) lib.add(e);
    return lib;
}

bool ExerciseLibrary::add(Exercise e) {
    e.name = trimmed(e.name);
    if (e.name.empty() || find(e.name) != nullptr) return false;
    m_exercises.push_back(std::move(e));
    return true;
}

const Exercise* ExerciseLibrary::find(std::string_view name) const {
    auto it = std::find_if(m_exercises.begin(), m_exercises.end(),
                           [&](const Exercise& e) { return sameExerciseName(e.name, name); });
    return it == m_exercises.end() ? nullptr : &*it;
}

} // namespace ironlog
