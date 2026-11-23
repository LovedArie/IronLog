// JsonStore.h — saves and loads the whole training log as one JSON file.
//
// GRASP Pure Fabrication (Larman Ch. 25): "who saves a Workout?" Expert
// would say Workout (it has the data). But that would couple the domain to
// a file format and to Qt's JSON classes. So we invent a class that is not
// a training concept at all — a store — whose ONLY job is persistence.
// The domain stays pure; the store is cohesive and swappable (SQLite later).
//
// Not a QObject: it has no signals or slots, so it doesn't need moc.
#pragma once

#include <QByteArray>
#include <QString>

#include "core/Exercise.h"
#include "core/Workout.h"

namespace ironlog {

struct AppData {
    TrainingLog log;
    ExerciseLibrary library;
};

class JsonStore {
public:
    // Bump when the file layout changes, and teach fromJson() to read the
    // old layout. A file with a NEWER version is refused, not guessed at.
    static constexpr int kSchemaVersion = 1;

    enum class LoadStatus {
        Ok,          // read successfully (skippedItems may still be > 0)
        NoFile,      // first run: data = defaults
        Unreadable   // corrupt JSON, wrong shape, or newer schema
    };

    struct LoadResult {
        LoadStatus status = LoadStatus::NoFile;
        AppData data;
        int skippedItems = 0; // invalid sets/exercises dropped while reading
        QString error;        // human-readable reason when Unreadable
    };

    explicit JsonStore(QString path) : m_path(std::move(path)) {}

    const QString& path() const { return m_path; }

    LoadResult load() const;

    // Atomic: writes a temp file then renames it over the old one
    // (QSaveFile), so a crash mid-save can never leave a half-written log.
    bool save(const AppData& data, QString* error = nullptr) const;

    // Pure conversions — public so tests can check the format directly.
    static QByteArray toJson(const AppData& data);
    static LoadResult fromJson(const QByteArray& bytes);

private:
    QString m_path;
};

} // namespace ironlog
