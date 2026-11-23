// WeeklyVolumeModel.cpp

#include "ui/WeeklyVolumeModel.h"

#include <QBrush>
#include <QColor>

#include "services/QtConvert.h"
#include "services/TrainingController.h"

namespace ironlog {

WeeklyVolumeModel::WeeklyVolumeModel(const TrainingController& controller, QObject* parent)
    : QAbstractTableModel(parent), m_controller(controller), m_anyDay(QDate::currentDate()) {
    connect(&m_controller, &TrainingController::logChanged, this, &WeeklyVolumeModel::reload);
    // A new exercise can change which muscle a logged set counts toward.
    connect(&m_controller, &TrainingController::libraryChanged, this, &WeeklyVolumeModel::reload);
    reload();
}

void WeeklyVolumeModel::setWeekContaining(const QDate& anyDay) {
    // Same week -> same numbers; skip the reset so the view doesn't flicker
    // or lose its selection while the user clicks around inside one week.
    if (monday() == toQt(toDomain(anyDay).mondayOfWeek())) return;
    m_anyDay = anyDay;
    reload();
}

QDate WeeklyVolumeModel::monday() const { return toQt(toDomain(m_anyDay).mondayOfWeek()); }

void WeeklyVolumeModel::reload() {
    beginResetModel();
    m_counts = stats::weeklySets(m_controller.log(), m_controller.library(), toDomain(m_anyDay));
    endResetModel();
}

int WeeklyVolumeModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(kMuscleGroupCount);
}

int WeeklyVolumeModel::columnCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant WeeklyVolumeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= rowCount()) return {};
    const MuscleGroup muscle = allMuscleGroups()[static_cast<std::size_t>(index.row())];
    const int sets = m_counts[stats::indexOf(muscle)];
    const stats::VolumeStatus status = stats::classify(sets, m_target);

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Muscle: return toQt(toString(muscle));
        case Sets:   return sets;
        case Target: return QStringLiteral("%1–%2").arg(m_target.low).arg(m_target.high);
        case Status:
            switch (status) {
            case stats::VolumeStatus::Below:   return tr("Below target");
            case stats::VolumeStatus::InRange: return tr("On target");
            case stats::VolumeStatus::Above:   return tr("Above target");
            }
        }
        return {};
    }

    if (role == Qt::BackgroundRole && index.column() == Status) {
        // Translucent fills: readable on light AND dark palettes.
        switch (status) {
        case stats::VolumeStatus::Below:   return QBrush(QColor(255, 152, 0, 90));  // amber
        case stats::VolumeStatus::InRange: return QBrush(QColor(76, 175, 80, 110)); // green
        case stats::VolumeStatus::Above:   return QBrush(QColor(33, 150, 243, 90)); // blue
        }
    }

    if (role == Qt::TextAlignmentRole && (index.column() == Sets || index.column() == Target))
        return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);

    return {};
}

QVariant WeeklyVolumeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
    case Muscle: return tr("Muscle group");
    case Sets:   return tr("Sets this week");
    case Target: return tr("Target");
    case Status: return tr("Status");
    }
    return {};
}

} // namespace ironlog
