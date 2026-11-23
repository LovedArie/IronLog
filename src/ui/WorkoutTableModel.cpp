// WorkoutTableModel.cpp

#include "ui/WorkoutTableModel.h"

#include <QBrush>
#include <QColor>
#include <QFont>

#include "services/QtConvert.h"
#include "services/TrainingController.h"
#include "core/Stats.h"

namespace ironlog {

WorkoutTableModel::WorkoutTableModel(const TrainingController& controller, QObject* parent)
    : QAbstractTableModel(parent), m_controller(controller), m_date(QDate::currentDate()) {
    // Observer: whenever the log changes, re-read our slice.
    connect(&m_controller, &TrainingController::logChanged, this, &WorkoutTableModel::reload);
    reload();
}

void WorkoutTableModel::setDate(const QDate& date) {
    if (date == m_date) return;
    m_date = date;
    reload();
}

void WorkoutTableModel::reload() {
    // beginResetModel/endResetModel tell every attached view "throw away
    // what you know and re-ask me". Blunt but correct for ~30 rows.
    beginResetModel();
    m_rows.clear();
    m_isRecord.clear();
    if (const Workout* w = m_controller.log().find(toDomain(m_date))) {
        m_rows = w->sets();
        m_isRecord.assign(m_rows.size(), false);
        for (std::size_t i : stats::personalRecordIndices(m_controller.log(), *w))
            m_isRecord[i] = true;
    }
    endResetModel();
}

int WorkoutTableModel::rowCount(const QModelIndex& parent) const {
    // Table models have no children: a valid parent means "rows under a
    // cell", which is always 0. Forgetting this check confuses tree views.
    return parent.isValid() ? 0 : static_cast<int>(m_rows.size());
}

int WorkoutTableModel::columnCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant WorkoutTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= rowCount()) return {};
    const SetEntry& s = m_rows[static_cast<std::size_t>(index.row())];
    const bool record = m_isRecord[static_cast<std::size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case Number:      return index.row() + 1;
        case ExerciseCol: return toQt(s.exercise());
        case Weight:      return formatKg(s.weightKg());
        case Reps:        return s.reps();
        case Volume:      return formatKg(s.volume());
        case E1rm:        return QString::number(s.estimatedOneRepMax(), 'f', 1);
        case Record:      return record ? tr("PR") : QString();
        }
        return {};

    case Qt::TextAlignmentRole:
        if (index.column() == ExerciseCol) return {};
        // As int: every Qt 6 delegate version reads alignment this way.
        return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::FontRole:
        if (record && index.column() == Record) {
            QFont bold;
            bold.setBold(true);
            return bold;
        }
        return {};

    case Qt::BackgroundRole:
        // Semi-transparent gold reads well on both light and dark themes.
        if (record && index.column() == Record) return QBrush(QColor(255, 193, 7, 140));
        return {};

    case Qt::ToolTipRole:
        if (record) return tr("Personal record: best estimated 1RM you've hit for this exercise.");
        if (index.column() == E1rm)
            return tr("Estimated one-rep max (Epley): weight × (1 + reps / 30)");
        return {};
    }
    return {};
}

QVariant WorkoutTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
    case Number:      return tr("#");
    case ExerciseCol: return tr("Exercise");
    case Weight:      return tr("Weight (kg)");
    case Reps:        return tr("Reps");
    case Volume:      return tr("Volume (kg)");
    case E1rm:        return tr("Est. 1RM");
    case Record:      return tr("PR");
    }
    return {};
}

double WorkoutTableModel::totalVolume() const {
    double total = 0.0;
    for (const SetEntry& s : m_rows) total += s.volume();
    return total;
}

bool WorkoutTableModel::isRecord(int row) const {
    return row >= 0 && row < rowCount() && m_isRecord[static_cast<std::size_t>(row)];
}

} // namespace ironlog
