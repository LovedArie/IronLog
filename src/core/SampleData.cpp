// SampleData.cpp

#include "core/SampleData.h"

#include <vector>

namespace ironlog::sample {

namespace {

struct Planned {
    const char* exercise;
    int sets;
    double baseKg;     // week-0 weight
    double stepKg;     // added each week (progressive overload)
    int reps;
};

// Push / Pull / Legs. Pull is deliberately high-volume for Back so the
// weekly-volume tab shows all three statuses (below / in range / above).
const std::vector<Planned> kPush = {
    {"Bench Press", 4, 80.0, 2.5, 8},  {"Incline Dumbbell Press", 3, 28.0, 1.0, 10},
    {"Overhead Press", 3, 50.0, 1.25, 8}, {"Lateral Raise", 4, 10.0, 0.5, 15},
    {"Tricep Pushdown", 3, 30.0, 1.25, 12},
};
const std::vector<Planned> kPull = {
    {"Barbell Row", 4, 70.0, 2.5, 8},  {"Lat Pulldown", 4, 60.0, 2.5, 10},
    {"Pull-up", 3, 0.0, 0.0, 8},       {"Barbell Curl", 3, 30.0, 1.25, 10},
    {"Hammer Curl", 3, 14.0, 0.5, 12},
};
const std::vector<Planned> kLegs = {
    {"Back Squat", 4, 100.0, 2.5, 6},  {"Romanian Deadlift", 3, 90.0, 2.5, 8},
    {"Leg Curl", 3, 40.0, 1.25, 12},   {"Hip Thrust", 3, 100.0, 5.0, 10},
    {"Standing Calf Raise", 4, 60.0, 2.5, 12}, {"Cable Crunch", 3, 40.0, 2.5, 15},
};

void addSession(TrainingLog& log, Date day, const std::vector<Planned>& plan, int week) {
    Workout& w = log.workoutOn(day);
    for (const Planned& p : plan) {
        const double kg = p.baseKg + p.stepKg * week;
        for (int s = 0; s < p.sets; ++s) {
            int reps = p.reps;
            if (week == 4) reps -= 2;          // a rough week: visible e1RM dip
            if (s == p.sets - 1) reps -= 1;    // last set is the hardest
            if (auto set = SetEntry::make(p.exercise, kg, reps)) w.addSet(*set);
        }
    }
}

} // namespace

TrainingLog demoLog(Date today) {
    TrainingLog log;
    const Date thisMonday = today.mondayOfWeek();
    constexpr int kWeeks = 8;
    for (int week = 0; week < kWeeks; ++week) {
        const Date monday = thisMonday.addDays(-7L * (kWeeks - 1 - week));
        // Mon push, Tue pull, Thu legs, Fri push, Sat pull.
        const struct { int offset; const std::vector<Planned>* plan; } days[] = {
            {0, &kPush}, {1, &kPull}, {3, &kLegs}, {4, &kPush}, {5, &kPull}};
        for (const auto& d : days) {
            const Date day = monday.addDays(d.offset);
            if (day > today) continue; // never invent future sessions
            addSession(log, day, *d.plan, week);
        }
    }
    return log;
}

} // namespace ironlog::sample
