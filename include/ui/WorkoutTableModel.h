// WorkoutTableModel.h — the sets of ONE day, shaped for a QTableView.
//
// Qt model/view: the model answers "how many rows? what's in cell (r, c)?"
// and the view draws. Same idea as the finance tracker's TransactionModel,
// but this model shows a SLICE (one date) of the data, chosen by setDate().
#pragma once

#include <QAbstractTableModel>
#include <QDate>

#include <vector>

#include "core/SetEntry.h"

namespace ironlog {

class TrainingController;

class WorkoutTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum Column { Number, ExerciseCol, Weight, Reps, Volume, E1rm, Record, ColumnCount };

    explicit WorkoutTableModel(const TrainingController& controller, QObject* parent = nullptr);

    void setDate(const QDate& date);
    QDate date() const { return m_date; }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    double totalVolume() const;
    bool isRecord(int row) const;

public slots:
    void reload();

private:
    const TrainingController& m_controller;
    QDate m_date;

    // A SNAPSHOT (copy) of the day's sets, not a pointer into the log.
    // TrainingLog keeps workouts in a std::vector; adding a new day can
    // reallocate it and leave any pointer/reference dangling. A copy can't.
    std::vector<SetEntry> m_rows;
    std::vector<bool> m_isRecord; // parallel to m_rows
};

} // namespace ironlog
