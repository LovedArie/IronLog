// test_services.cpp — JsonStore + TrainingController. Qt Core only (no GUI),
// so these run headless, fast, and without a window popping up.

#include "catch.hpp"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

#include "core/SampleData.h"
#include "core/Stats.h"
#include "services/JsonStore.h"
#include "services/TrainingController.h"

using namespace ironlog;

namespace {

AppData smallData() {
    AppData data;
    data.library = ExerciseLibrary::withDefaults();
    data.library.add({"Développé couché", MuscleGroup::Chest}); // non-ASCII on purpose
    auto& w = data.log.workoutOn(Date{2026, 11, 23});
    w.addSet(*SetEntry::make("Bench Press", 102.5, 5));
    w.addSet(*SetEntry::make("Développé couché", 51.25, 8));
    data.log.workoutOn(Date{2026, 11, 20}).addSet(*SetEntry::make("Back Squat", 140, 3));
    return data;
}

} // namespace

TEST_CASE("JsonStore: save then load gives back the same data", "[store]") {
    QTemporaryDir dir; // deleted automatically at the end of the test
    REQUIRE(dir.isValid());
    const JsonStore store(dir.filePath("log.json"));

    const AppData original = smallData();
    QString error;
    REQUIRE(store.save(original, &error));

    const auto loaded = store.load();
    REQUIRE(loaded.status == JsonStore::LoadStatus::Ok);
    CHECK(loaded.skippedItems == 0);
    CHECK(loaded.data.library.size() == original.library.size());
    CHECK(loaded.data.library.find("Développé couché") != nullptr);

    const auto& workouts = loaded.data.log.workouts();
    REQUIRE(workouts.size() == 2);
    CHECK(workouts[0].date() == Date{2026, 11, 20}); // still sorted
    const auto& sets = workouts[1].sets();
    REQUIRE(sets.size() == 2);
    CHECK(sets[0].weightKg() == Approx(102.5));
    CHECK(sets[1].exercise() == "Développé couché");
    CHECK(sets[1].weightKg() == Approx(51.25));
    CHECK(sets[1].reps() == 8);
}

TEST_CASE("JsonStore: missing file means first run with default exercises", "[store]") {
    QTemporaryDir dir;
    const auto r = JsonStore(dir.filePath("nope.json")).load();
    CHECK(r.status == JsonStore::LoadStatus::NoFile);
    CHECK(r.data.log.isEmpty());
    CHECK(r.data.library.size() == ExerciseLibrary::withDefaults().size());
}

TEST_CASE("JsonStore: broken or too-new files are refused, never guessed", "[store]") {
    CHECK(JsonStore::fromJson("{ not json").status == JsonStore::LoadStatus::Unreadable);
    CHECK(JsonStore::fromJson("[1, 2, 3]").status == JsonStore::LoadStatus::Unreadable);
    CHECK(JsonStore::fromJson(R"({"workouts": []})").status == JsonStore::LoadStatus::Unreadable);

    const auto tooNew = JsonStore::fromJson(R"({"schemaVersion": 99})");
    CHECK(tooNew.status == JsonStore::LoadStatus::Unreadable);
    CHECK(tooNew.error.contains("newer"));
}

TEST_CASE("JsonStore: bad ITEMS are skipped and counted, good ones kept", "[store]") {
    const QByteArray json = R"({
        "schemaVersion": 1,
        "exercises": [ {"name": "Bench Press", "muscle": "Chest"},
                       {"name": "Wrist Curl",  "muscle": "Forearms"} ],
        "workouts": [
            {"date": "2026-11-23", "sets": [
                {"exercise": "Bench Press", "kg": 100, "reps": 5},
                {"exercise": "Bench Press", "kg": -5,  "reps": 5},
                {"exercise": "Bench Press", "kg": 100} ]},
            {"date": "2026-02-30", "sets": []},
            {"date": "2026-11-24", "sets": [ {"exercise": "Bench Press", "kg": 0, "reps": 0} ]}
        ]
    })";
    const auto r = JsonStore::fromJson(json);
    REQUIRE(r.status == JsonStore::LoadStatus::Ok);
    // Forearms (unknown muscle), kg -5, missing reps, Feb 30, reps 0.
    CHECK(r.skippedItems == 5);
    CHECK(r.data.library.size() == 1);
    // 11-24 lost its only set, so the empty day was pruned.
    REQUIRE(r.data.log.workouts().size() == 1);
    CHECK(r.data.log.workouts()[0].sets().size() == 1);
}

TEST_CASE("JsonStore: file has the documented shape", "[store]") {
    const QJsonObject root = QJsonDocument::fromJson(JsonStore::toJson(smallData())).object();
    CHECK(root.value("schemaVersion").toInt() == JsonStore::kSchemaVersion);
    CHECK(root.value("exercises").isArray());
    const QJsonObject firstSet =
        root.value("workouts").toArray().at(0).toObject().value("sets").toArray().at(0).toObject();
    CHECK(firstSet.value("exercise").toString() == "Back Squat");
    CHECK(firstSet.value("kg").toDouble() == Approx(140.0));
    CHECK(firstSet.value("reps").toInt() == 3);
}

TEST_CASE("JsonStore: the sample log survives a round trip intact", "[store]") {
    QTemporaryDir dir;
    const JsonStore store(dir.filePath("sample.json"));
    AppData data;
    data.library = ExerciseLibrary::withDefaults();
    data.log = sample::demoLog(Date{2026, 11, 23});
    REQUIRE(store.save(data));

    const auto r = store.load();
    REQUIRE(r.status == JsonStore::LoadStatus::Ok);
    CHECK(r.data.log.workouts().size() == data.log.workouts().size());
    CHECK(stats::allTimeBestE1rm(r.data.log, "Bench Press") ==
          Approx(*stats::allTimeBestE1rm(data.log, "Bench Press")));
}

// ---- TrainingController ----------------------------------------------------

TEST_CASE("Controller: adding a set saves, signals, and uses library spelling", "[controller]") {
    QTemporaryDir dir;
    const QString path = dir.filePath("log.json");
    TrainingController c(path);
    CHECK(c.startupWarning().isEmpty()); // first run is not a problem

    int changes = 0;
    QObject::connect(&c, &TrainingController::logChanged, [&] { ++changes; });

    const QDate day(2026, 11, 23);
    CHECK(c.addSet(day, "bench press", 100.0, 5) == TrainingController::AddResult::Ok);
    CHECK(changes == 1);
    CHECK(c.log().find(Date{2026, 11, 23})->sets()[0].exercise() == "Bench Press");

    // Refusals change nothing and announce nothing.
    CHECK(c.addSet(day, "Unicycle Squat", 50, 5) == TrainingController::AddResult::UnknownExercise);
    CHECK(c.addSet(day, "Bench Press", 100, 0) == TrainingController::AddResult::InvalidNumbers);
    CHECK(changes == 1);

    // Saved to disk: a second controller on the same file sees the set.
    TrainingController reopened(path);
    REQUIRE(reopened.log().find(Date{2026, 11, 23}) != nullptr);
    CHECK(reopened.log().find(Date{2026, 11, 23})->sets().size() == 1);
}

TEST_CASE("Controller: deleting the last set removes the day", "[controller]") {
    QTemporaryDir dir;
    TrainingController c(dir.filePath("log.json"));
    const QDate day(2026, 11, 23);
    c.addSet(day, "Bench Press", 100, 5);
    CHECK_FALSE(c.removeSet(day, 3));
    CHECK(c.removeSet(day, 0));
    CHECK(c.log().isEmpty());
}

TEST_CASE("Controller: sample data never overwrites real training", "[controller]") {
    QTemporaryDir dir;
    TrainingController c(dir.filePath("log.json"));
    CHECK(c.loadSampleData(QDate(2026, 11, 23)));
    const auto sessions = c.log().workouts().size();
    CHECK_FALSE(c.loadSampleData(QDate(2026, 11, 23))); // log not empty now
    CHECK(c.log().workouts().size() == sessions);
}

TEST_CASE("Controller: a corrupt file is moved aside, never overwritten", "[controller]") {
    QTemporaryDir dir;
    const QString path = dir.filePath("log.json");
    const QByteArray garbage = "{ this is not json";
    {
        QFile f(path);
        REQUIRE(f.open(QIODevice::WriteOnly));
        f.write(garbage);
    }

    TrainingController c(path);
    CHECK_FALSE(c.startupWarning().isEmpty());
    CHECK(c.log().isEmpty());

    // The original bytes survive in a backup next to the log...
    const QStringList backups = QDir(dir.path()).entryList({"log.json.unreadable-*"});
    REQUIRE(backups.size() == 1);
    QFile backup(dir.filePath(backups.first()));
    REQUIRE(backup.open(QIODevice::ReadOnly));
    CHECK(backup.readAll() == garbage);

    // ...and new work saves to a fresh, valid file.
    c.addSet(QDate(2026, 11, 23), "Bench Press", 100, 5);
    CHECK(JsonStore(path).load().status == JsonStore::LoadStatus::Ok);
}

TEST_CASE("Controller: custom exercises are unique and persist", "[controller]") {
    QTemporaryDir dir;
    const QString path = dir.filePath("log.json");
    TrainingController c(path);
    int libraryChanges = 0;
    QObject::connect(&c, &TrainingController::libraryChanged, [&] { ++libraryChanges; });

    CHECK(c.addExercise("Cable Fly", MuscleGroup::Chest));
    CHECK_FALSE(c.addExercise("cable fly", MuscleGroup::Chest));
    CHECK(libraryChanges == 1);
    CHECK(TrainingController(path).library().find("Cable Fly") != nullptr);
}
