// WeeklyVolumeModel.h — sets per muscle group for one Monday–Sunday week.
//
// Bodybuilding programs are usually judged per MUSCLE per WEEK ("am I doing
// enough chest?"), not per session. This model turns the log into that view:
// one row per muscle group, coloured below / on target / above.
#pragma once

#include <QAbstractTableModel>
#include <QDate>

#include "core/Stats.h"

namespace ironlog {

class TrainingController;

class WeeklyVolumeModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum Column { Muscle, Sets, Target, Status, ColumnCount };

    explicit WeeklyVolumeModel(const TrainingController& controller, QObject* parent = nullptr);

    void setWeekContaining(const QDate& anyDay);
    QDate monday() const;
    stats::VolumeTarget target() const { return m_target; }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

public slots:
    void reload();

private:
    const TrainingController& m_controller;
    QDate m_anyDay;
    stats::VolumeTarget m_target; // defaults 10–20
    stats::MuscleCounts m_counts{};
};

} // namespace ironlog
