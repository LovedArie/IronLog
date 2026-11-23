// JsonStore.cpp
//
// File layout (schemaVersion 1):
// {
//   "schemaVersion": 1,
//   "exercises": [ { "name": "Bench Press", "muscle": "Chest" }, ... ],
//   "workouts":  [ { "date": "2026-11-23",
//                    "sets": [ { "exercise": "Bench Press", "kg": 100, "reps": 5 } ] } ]
// }

#include "services/JsonStore.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSaveFile>

#include "services/QtConvert.h"

namespace ironlog {

QByteArray JsonStore::toJson(const AppData& data) {
    QJsonArray exercises;
    for (const Exercise& e : data.library.all()) {
        exercises.append(QJsonObject{
            {"name", toQt(e.name)},
            {"muscle", toQt(toString(e.primary))},
        });
    }

    QJsonArray workouts;
    for (const Workout& w : data.log.workouts()) {
        if (w.isEmpty()) continue; // an empty day carries no information
        QJsonArray sets;
        for (const SetEntry& s : w.sets()) {
            sets.append(QJsonObject{
                {"exercise", toQt(s.exercise())},
                {"kg", s.weightKg()},
                {"reps", s.reps()},
            });
        }
        workouts.append(QJsonObject{
            {"date", toQt(w.date().toIso())},
            {"sets", sets},
        });
    }

    const QJsonObject root{
        {"schemaVersion", kSchemaVersion},
        {"exercises", exercises},
        {"workouts", workouts},
    };
    // Indented: the file is human-readable and diff-able in git.
    return QJsonDocument(root).toJson(QJsonDocument::Indented);
}

JsonStore::LoadResult JsonStore::fromJson(const QByteArray& bytes) {
    LoadResult result;

    QJsonParseError parseError{};
    const QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        result.status = LoadStatus::Unreadable;
        result.error = QStringLiteral("Not valid JSON (%1 at byte %2).")
                           .arg(parseError.errorString())
                           .arg(parseError.offset);
        return result;
    }

    const QJsonObject root = doc.object();
    const int version = root.value("schemaVersion").toInt(-1);
    if (version < 1 || version > kSchemaVersion) {
        result.status = LoadStatus::Unreadable;
        result.error = version > kSchemaVersion
            ? QStringLiteral("File was written by a newer IronLog (schema %1).").arg(version)
            : QStringLiteral("Missing or invalid schemaVersion.");
        return result;
    }

    // Policy: be strict about the FILE (above), tolerant about ITEMS (below).
    // One bad set shouldn't cost you a year of history — skip it and count it.
    for (const QJsonValue& v : root.value("exercises").toArray()) {
        const QJsonObject o = v.toObject();
        const auto muscle = muscleGroupFromString(toStd(o.value("muscle").toString()));
        if (!muscle || !result.data.library.add({toStd(o.value("name").toString()), *muscle}))
            ++result.skippedItems;
    }

    for (const QJsonValue& v : root.value("workouts").toArray()) {
        const QJsonObject o = v.toObject();
        const auto date = Date::fromIso(toStd(o.value("date").toString()));
        if (!date) { ++result.skippedItems; continue; }

        Workout& w = result.data.log.workoutOn(*date);
        for (const QJsonValue& sv : o.value("sets").toArray()) {
            const QJsonObject so = sv.toObject();
            // toInt(0)/toDouble(-1): missing fields become values make() rejects.
            auto set = SetEntry::make(toStd(so.value("exercise").toString()),
                                      so.value("kg").toDouble(-1.0),
                                      so.value("reps").toInt(0));
            if (set) w.addSet(std::move(*set));
            else ++result.skippedItems;
        }
    }
    result.data.log.pruneEmpty();

    result.status = LoadStatus::Ok;
    return result;
}

JsonStore::LoadResult JsonStore::load() const {
    QFile file(m_path);
    if (!file.exists()) {
        LoadResult first;
        first.status = LoadStatus::NoFile;
        first.data.library = ExerciseLibrary::withDefaults();
        return first;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        LoadResult failed;
        failed.status = LoadStatus::Unreadable;
        failed.error = file.errorString();
        return failed;
    }
    return fromJson(file.readAll());
}

bool JsonStore::save(const AppData& data, QString* error) const {
    QSaveFile file(m_path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (error) *error = file.errorString();
        return false;
    }
    const QByteArray bytes = toJson(data);
    if (file.write(bytes) != bytes.size() || !file.commit()) {
        // No commit -> QSaveFile discards the temp file; the old log survives.
        if (error) *error = file.errorString();
        return false;
    }
    return true;
}

} // namespace ironlog
