// TrainingController.h — the app's single entry point for changing data.
//
// GRASP Controller (Larman Ch. 17): the UI doesn't edit the domain directly.
// Every user action ("add a set", "delete a set", "add an exercise") becomes
// ONE call here. The controller validates, updates the domain, saves, and
// announces the change with a signal. Widgets and models only listen.
//
// Same observer shape as the finance tracker's DatabaseManager::ledgerChanged:
// one signal fans out to every view, so no widget has to know about another.
#pragma once

#include <QDate>
#include <QObject>
#include <QString>

#include "services/JsonStore.h"
#include "core/Exercise.h"
#include "core/Workout.h"

namespace ironlog {

class TrainingController : public QObject {
    Q_OBJECT

public:
    // Loads immediately. A corrupt file is moved aside (never overwritten)
    // and the app starts fresh; startupWarning() explains what happened.
    explicit TrainingController(const QString& dataPath, QObject* parent = nullptr);

    const TrainingLog& log() const { return m_data.log; }
    const ExerciseLibrary& library() const { return m_data.library; }
    QString dataPath() const { return m_store.path(); }
    QString startupWarning() const { return m_startupWarning; }

    enum class AddResult { Ok, UnknownExercise, InvalidNumbers };

    // ---- System operations: one per user action ----
    AddResult addSet(const QDate& date, const QString& exercise, double kg, int reps);
    bool removeSet(const QDate& date, int index);
    bool addExercise(const QString& name, MuscleGroup muscle);

    // Only allowed while the log is empty, so it can never destroy history.
    bool loadSampleData(const QDate& today);

signals:
    void logChanged();      // any workout/set changed -> views reload
    void libraryChanged();  // exercise list changed -> combo boxes reload
    void saveFailed(const QString& message);

private:
    void persist(); // save + emit saveFailed on error

    JsonStore m_store;
    AppData m_data;
    QString m_startupWarning;
};

} // namespace ironlog
