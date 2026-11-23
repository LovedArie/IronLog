// ProgressChart.h — a hand-painted line chart of estimated 1RM over time.
//
// WHY NOT QtCharts? It's an optional add-on module (and Qt is moving to
// QtGraphs), so it's one more install step that can break a build on a new
// machine. A chart this simple is ~150 lines of QPainter — and writing it
// teaches how EVERY Qt widget draws itself: override paintEvent().
#pragma once

#include <QString>
#include <QWidget>

#include <vector>

#include "core/Stats.h"

namespace ironlog {

class ProgressChart : public QWidget {
    Q_OBJECT

public:
    explicit ProgressChart(QWidget* parent = nullptr);

    // Replace the data and repaint. Points must be oldest-first.
    void setSeries(const QString& title, std::vector<stats::ProgressPoint> points);

    QSize sizeHint() const override { return {520, 320}; }
    QSize minimumSizeHint() const override { return {280, 180}; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_title;
    std::vector<stats::ProgressPoint> m_points;
};

} // namespace ironlog
