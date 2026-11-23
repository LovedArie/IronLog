// test_core.cpp — unit tests for the pure C++ domain core. No Qt needed.

#include "catch.hpp"

#include <cmath>

#include "core/Date.h"
#include "core/Exercise.h"
#include "core/SampleData.h"
#include "core/SetEntry.h"
#include "core/Stats.h"
#include "core/Workout.h"

using namespace ironlog;

namespace {
Date d(int y, int m, int day) { return Date{y, m, day}; }

SetEntry set(const char* ex, double kg, int reps) {
    auto s = SetEntry::make(ex, kg, reps);
    REQUIRE(s.has_value()); // fail loudly at the source, not three calls later
    return *s;
}
} // namespace

// ---- Date ------------------------------------------------------------------

TEST_CASE("Date: day numbers round-trip and hit known anchors", "[date]") {
    CHECK(d(1970, 1, 1).toDayNumber() == 0);
    CHECK(d(2000, 3, 1).toDayNumber() == 11017);
    for (long n : {-1000L, -1L, 0L, 59L, 60L, 11016L, 20415L, 30000L})
        CHECK(Date::fromDayNumber(n).toDayNumber() == n);
}

TEST_CASE("Date: leap years and validity", "[date]") {
    CHECK(d(2024, 2, 29).isValid());
    CHECK_FALSE(d(2026, 2, 29).isValid());
    CHECK_FALSE(d(1900, 2, 29).isValid()); // divisible by 100, not 400
    CHECK(d(2000, 2, 29).isValid());       // divisible by 400
    CHECK_FALSE(d(2026, 13, 1).isValid());
    CHECK(d(2026, 2, 28).addDays(1) == d(2026, 3, 1));
}

TEST_CASE("Date: weekday and Monday of week", "[date]") {
    CHECK(d(2026, 11, 23).weekday() == 0); // Monday
    CHECK(d(2026, 11, 29).weekday() == 6); // Sunday
    CHECK(d(2026, 11, 29).mondayOfWeek() == d(2026, 11, 23));
    CHECK(d(2026, 11, 30).mondayOfWeek() == d(2026, 11, 30));
    CHECK(d(2027, 1, 1).mondayOfWeek() == d(2026, 12, 28)); // crosses a year
}

TEST_CASE("Date: ISO text in and out", "[date]") {
    CHECK(d(2026, 3, 7).toIso() == "2026-03-07");
    REQUIRE(Date::fromIso("2026-03-07").has_value());
    CHECK(*Date::fromIso("2026-03-07") == d(2026, 3, 7));
    CHECK_FALSE(Date::fromIso("2026-02-30").has_value()); // impossible date
    CHECK_FALSE(Date::fromIso("2026-3-7").has_value());   // wrong width
    CHECK_FALSE(Date::fromIso("2026/03/07").has_value()); // wrong separator
    CHECK_FALSE(Date::fromIso("").has_value());
}

// ---- SetEntry --------------------------------------------------------------

TEST_CASE("SetEntry: invariant is enforced by make()", "[set]") {
    CHECK(SetEntry::make("Bench Press", 100.0, 5).has_value());
    CHECK(SetEntry::make("Pull-up", 0.0, 8).has_value());      // bodyweight
    CHECK_FALSE(SetEntry::make("", 100.0, 5).has_value());
    CHECK_FALSE(SetEntry::make("Bench Press", -1.0, 5).has_value());
    CHECK_FALSE(SetEntry::make("Bench Press", 100.0, 0).has_value());
    CHECK_FALSE(SetEntry::make("Bench Press", 100.0, SetEntry::kMaxReps + 1).has_value());
    CHECK_FALSE(SetEntry::make("Bench Press", std::nan(""), 5).has_value());
}

TEST_CASE("SetEntry: volume and Epley e1RM", "[set]") {
    const SetEntry s = set("Bench Press", 100.0, 10);
    CHECK(s.volume() == Approx(1000.0));
    CHECK(s.estimatedOneRepMax() == Approx(133.333).epsilon(1e-4)); // 100 * (1 + 10/30)
    CHECK(set("Back Squat", 140.0, 1).estimatedOneRepMax() == Approx(140.0)); // a single IS the 1RM
}

// ---- Exercise library ------------------------------------------------------

TEST_CASE("ExerciseLibrary: defaults cover every muscle group", "[library]") {
    const auto lib = ExerciseLibrary::withDefaults();
    for (MuscleGroup m : allMuscleGroups()) {
        bool found = false;
        for (const Exercise& e : lib.all()) found = found || e.primary == m;
        INFO("muscle group: " << toString(m));
        CHECK(found);
    }
}

TEST_CASE("ExerciseLibrary: names are unique, trimmed, case-insensitive", "[library]") {
    ExerciseLibrary lib;
    CHECK(lib.add({"  Cable Fly ", MuscleGroup::Chest}));
    CHECK(lib.find("cable fly") != nullptr);
    CHECK(lib.find("Cable Fly")->name == "Cable Fly"); // stored trimmed
    CHECK_FALSE(lib.add({"CABLE FLY", MuscleGroup::Chest}));
    CHECK_FALSE(lib.add({"   ", MuscleGroup::Chest}));
    CHECK(lib.size() == 1);
}

TEST_CASE("MuscleGroup: text round-trip and enum order", "[library]") {
    const auto& all = allMuscleGroups();
    for (std::size_t i = 0; i < all.size(); ++i) {
        CHECK(stats::indexOf(all[i]) == i); // weeklySets relies on this
        CHECK(muscleGroupFromString(toString(all[i])) == all[i]);
    }
    CHECK_FALSE(muscleGroupFromString("Forearms").has_value());
}

// ---- Workout / TrainingLog -------------------------------------------------

TEST_CASE("Workout: total volume and set removal", "[workout]") {
    Workout w(d(2026, 11, 23));
    w.addSet(set("Bench Press", 100.0, 5));  // 500
    w.addSet(set("Bench Press", 90.0, 8));   // 720
    CHECK(w.totalVolume() == Approx(1220.0));
    CHECK_FALSE(w.removeSet(5));
    CHECK(w.removeSet(0));
    CHECK(w.totalVolume() == Approx(720.0));
}

TEST_CASE("TrainingLog: get-or-create keeps sessions sorted and unique", "[log]") {
    TrainingLog log;
    log.workoutOn(d(2026, 11, 25)).addSet(set("Back Squat", 100, 5));
    log.workoutOn(d(2026, 11, 20)).addSet(set("Back Squat", 95, 5));
    log.workoutOn(d(2026, 11, 22)).addSet(set("Back Squat", 97.5, 5));
    log.workoutOn(d(2026, 11, 22)).addSet(set("Back Squat", 97.5, 5)); // same day

    REQUIRE(log.workouts().size() == 3);
    CHECK(log.workouts()[0].date() == d(2026, 11, 20));
    CHECK(log.workouts()[2].date() == d(2026, 11, 25));
    CHECK(log.find(d(2026, 11, 22))->sets().size() == 2);
    CHECK(log.find(d(2026, 11, 21)) == nullptr);
}

TEST_CASE("TrainingLog: removal and pruning of empty days", "[log]") {
    TrainingLog log;
    log.workoutOn(d(2026, 11, 20)).addSet(set("Leg Curl", 40, 12));
    log.workoutOn(d(2026, 11, 21)); // created but never given a set
    log.pruneEmpty();
    CHECK(log.workouts().size() == 1);
    CHECK(log.removeWorkout(d(2026, 11, 20)));
    CHECK_FALSE(log.removeWorkout(d(2026, 11, 20)));
    CHECK(log.isEmpty());
}

// ---- Stats -----------------------------------------------------------------

TEST_CASE("Stats: e1RM history is one best point per session, oldest first", "[stats]") {
    TrainingLog log;
    auto& a = log.workoutOn(d(2026, 11, 2));
    a.addSet(set("Bench Press", 80, 8));   // 101.33
    a.addSet(set("Bench Press", 85, 5));   // 99.17
    log.workoutOn(d(2026, 11, 9)).addSet(set("Bench Press", 85, 8)); // 107.67
    log.workoutOn(d(2026, 11, 5)).addSet(set("Back Squat", 100, 5)); // other lift

    const auto h = stats::e1rmHistory(log, "bench press"); // case-insensitive
    REQUIRE(h.size() == 2);
    CHECK(h[0].date == d(2026, 11, 2));
    CHECK(h[0].bestE1rm == Approx(80 * (1 + 8 / 30.0)));
    CHECK(h[1].bestE1rm == Approx(85 * (1 + 8 / 30.0)));
    CHECK(stats::allTimeBestE1rm(log, "Bench Press") == Approx(h[1].bestE1rm));
    CHECK_FALSE(stats::allTimeBestE1rm(log, "Leg Press").has_value());
}

TEST_CASE("Stats: lastSet finds the newest set of an exercise", "[stats]") {
    TrainingLog log;
    log.workoutOn(d(2026, 11, 9)).addSet(set("Bench Press", 90, 8));
    auto& older = log.workoutOn(d(2026, 11, 2));
    older.addSet(set("Bench Press", 80, 8));
    older.addSet(set("Bench Press", 85, 6));
    CHECK(stats::lastSet(log, "Bench Press")->weightKg() == Approx(90));
    log.find(d(2026, 11, 9))->addSet(set("Bench Press", 92.5, 5));
    CHECK(stats::lastSet(log, "Bench Press")->weightKg() == Approx(92.5));
    CHECK_FALSE(stats::lastSet(log, "Leg Press").has_value());
}

TEST_CASE("Stats: personal records", "[stats]") {
    TrainingLog log;
    log.workoutOn(d(2026, 11, 2)).addSet(set("Bench Press", 100, 5)); // 116.67

    SECTION("first-ever session of an exercise is not a PR") {
        CHECK(stats::personalRecordIndices(log, *log.find(d(2026, 11, 2))).empty());
    }
    SECTION("beating the previous best flags the day's top set only") {
        auto& w = log.workoutOn(d(2026, 11, 9));
        w.addSet(set("Bench Press", 100, 5));  // 116.67 — ties, not a PR
        w.addSet(set("Bench Press", 105, 5));  // 122.50 — top set -> PR
        w.addSet(set("Bench Press", 105, 5));  // equal to top, but not first
        w.addSet(set("Back Squat", 140, 5));   // first-ever squat -> not a PR
        const auto prs = stats::personalRecordIndices(log, w);
        REQUIRE(prs.size() == 1);
        CHECK(prs[0] == 1);
    }
    SECTION("matching the previous best is not a PR") {
        auto& w = log.workoutOn(d(2026, 11, 9));
        w.addSet(set("Bench Press", 100, 5));
        CHECK(stats::personalRecordIndices(log, w).empty());
    }
    SECTION("only EARLIER days count as the bar to beat") {
        // A later, heavier session must not stop an earlier day being a PR.
        log.workoutOn(d(2026, 11, 9)).addSet(set("Bench Press", 105, 5));
        log.workoutOn(d(2026, 11, 16)).addSet(set("Bench Press", 110, 5));
        CHECK(stats::personalRecordIndices(log, *log.find(d(2026, 11, 9))).size() == 1);
    }
}

TEST_CASE("Stats: weekly sets per muscle respect Monday–Sunday bounds", "[stats]") {
    const auto lib = ExerciseLibrary::withDefaults();
    TrainingLog log;
    log.workoutOn(d(2026, 11, 22)).addSet(set("Bench Press", 80, 8));   // Sunday BEFORE
    log.workoutOn(d(2026, 11, 23)).addSet(set("Bench Press", 80, 8));   // Monday
    log.workoutOn(d(2026, 11, 23)).addSet(set("Barbell Row", 70, 8));
    log.workoutOn(d(2026, 11, 29)).addSet(set("Bench Press", 80, 8));   // Sunday (in)
    log.workoutOn(d(2026, 11, 30)).addSet(set("Bench Press", 80, 8));   // next Monday
    log.workoutOn(d(2026, 11, 25)).addSet(set("Mystery Move", 10, 10)); // not in library

    const auto counts = stats::weeklySets(log, lib, d(2026, 11, 26)); // a Thursday
    CHECK(counts[stats::indexOf(MuscleGroup::Chest)] == 2);
    CHECK(counts[stats::indexOf(MuscleGroup::Back)] == 1);
    CHECK(counts[stats::indexOf(MuscleGroup::Quads)] == 0);
}

TEST_CASE("Stats: volume classification edges", "[stats]") {
    CHECK(stats::classify(9) == stats::VolumeStatus::Below);
    CHECK(stats::classify(10) == stats::VolumeStatus::InRange);
    CHECK(stats::classify(20) == stats::VolumeStatus::InRange);
    CHECK(stats::classify(21) == stats::VolumeStatus::Above);
    CHECK(stats::classify(5, {4, 6}) == stats::VolumeStatus::InRange);
}

// ---- Sample data -----------------------------------------------------------

TEST_CASE("SampleData: deterministic, never in the future, shows progress", "[sample]") {
    const Date today = d(2026, 11, 25); // a Wednesday
    const TrainingLog log = sample::demoLog(today);
    REQUIRE_FALSE(log.isEmpty());
    CHECK(log.workouts().back().date() <= today);
    CHECK(sample::demoLog(today).workouts().size() == log.workouts().size());

    const auto h = stats::e1rmHistory(log, "Bench Press");
    REQUIRE(h.size() >= 2);
    CHECK(h.back().bestE1rm > h.front().bestE1rm); // progressive overload shows

    // Every exercise used must exist in the default library.
    const auto lib = ExerciseLibrary::withDefaults();
    for (const Workout& w : log.workouts())
        for (const SetEntry& s : w.sets()) {
            INFO(s.exercise());
            CHECK(lib.find(s.exercise()) != nullptr);
        }
}
