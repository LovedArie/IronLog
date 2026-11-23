// TrainingController.cpp

#include "services/TrainingController.h"

#include <QDateTime>
#include <QFile>

#include "services/QtConvert.h"
#include "core/SampleData.h"

namespace ironlog {

TrainingController::TrainingController(const QString& dataPath, QObject* parent)
    : QObject(parent), m_store(dataPath) {
    JsonStore::LoadResult loaded = m_store.load();

    switch (loaded.status) {
    case JsonStore::LoadStatus::Ok:
        m_data = std::move(loaded.data);
        if (loaded.skippedItems > 0)
            m_startupWarning = tr("Skipped %1 in your log; everything else loaded.")
                                   .arg(countOf(loaded.skippedItems, tr("damaged entry"),
                                                tr("damaged entries")));
        break;

    case JsonStore::LoadStatus::NoFile:
        m_data = std::move(loaded.data); // defaults: empty log + starter exercises
        break;

    case JsonStore::LoadStatus::Unreadable: {
        // NEVER autosave over a file we couldn't read — that turns "the app
        // can't parse my data" into "the app deleted my data". Move it aside.
        const QString backup = dataPath + ".unreadable-" +
                               QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss");
        const bool moved = QFile::rename(dataPath, backup);
        m_data.library = ExerciseLibrary::withDefaults();
        m_startupWarning = tr("Your log could not be read: %1\n\n").arg(loaded.error) +
            (moved ? tr("It was kept as:\n%1\nIronLog started a fresh log.").arg(backup)
                   : tr("It could not be moved aside, so changes will NOT be saved "
                        "until the file is fixed or removed."));
        if (!moved) m_store = JsonStore(QString()); // empty path: every save fails loudly
        break;
    }
    }
}

TrainingController::AddResult TrainingController::addSet(const QDate& date, const QString& exercise,
                                                          double kg, int reps) {
    // Use the library's spelling, so "bench press" typed by hand is stored
    // as "Bench Press" and every later comparison sees one name.
    const Exercise* known = m_data.library.find(toStd(exercise));
    if (!known || !date.isValid()) return AddResult::UnknownExercise;

    auto set = SetEntry::make(known->name, kg, reps);
    if (!set) return AddResult::InvalidNumbers;

    m_data.log.workoutOn(toDomain(date)).addSet(std::move(*set));
    persist();
    emit logChanged();
    return AddResult::Ok;
}

bool TrainingController::removeSet(const QDate& date, int index) {
    Workout* w = m_data.log.find(toDomain(date));
    if (!w || index < 0 || !w->removeSet(static_cast<std::size_t>(index))) return false;
    m_data.log.pruneEmpty(); // last set gone -> the day is no longer "trained"
    persist();
    emit logChanged();
    return true;
}

bool TrainingController::addExercise(const QString& name, MuscleGroup muscle) {
    if (!m_data.library.add({toStd(name), muscle})) return false;
    persist();
    emit libraryChanged();
    return true;
}

bool TrainingController::loadSampleData(const QDate& today) {
    if (!m_data.log.isEmpty() || !today.isValid()) return false;
    m_data.log = sample::demoLog(toDomain(today));
    persist();
    emit logChanged();
    return true;
}

void TrainingController::persist() {
    QString error;
    if (!m_store.save(m_data, &error))
        emit saveFailed(error.isEmpty() ? tr("Unknown error") : error);
}

} // namespace ironlog
