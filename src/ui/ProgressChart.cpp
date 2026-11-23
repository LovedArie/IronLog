// ProgressChart.cpp
//
// Drawing plan:
//   1. Work out the "plot rect" (widget minus margins for labels).
//   2. Map data -> pixels:  x from the DATE (real time, so a two-week break
//      shows as a gap), y from the e1RM.
//   3. Draw grid + labels, then the line, then the points, then highlight
//      the best point.

#include "ui/ProgressChart.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

#include <algorithm>
#include <cmath>

#include "services/QtConvert.h"

namespace ironlog {

namespace {

// Round a raw step up to a "nice" number (1, 2, 2.5, 5 x 10^n) so the axis
// reads 100, 110, 120 instead of 103.7, 114.2, ...
double niceStep(double raw) {
    if (raw <= 0) return 1.0;
    const double magnitude = std::pow(10.0, std::floor(std::log10(raw)));
    for (double m : {1.0, 2.0, 2.5, 5.0, 10.0})
        if (raw <= m * magnitude) return m * magnitude;
    return 10.0 * magnitude;
}

} // namespace

ProgressChart::ProgressChart(QWidget* parent) : QWidget(parent) {
    // Tell Qt we paint every pixel ourselves -> it can skip erasing first.
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void ProgressChart::setSeries(const QString& title, std::vector<stats::ProgressPoint> points) {
    m_title = title;
    m_points = std::move(points);
    update(); // schedule a repaint; never call paintEvent() yourself
}

void ProgressChart::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Colours come from the palette, so light and dark themes both work.
    const QPalette pal = palette();
    const QColor bg = pal.color(QPalette::Base);
    const QColor text = pal.color(QPalette::Text);
    QColor grid = text;
    grid.setAlpha(40);
    const QColor line = pal.color(QPalette::Highlight);
    const QColor best(255, 160, 0);

    p.fillRect(rect(), bg);

    // Title
    QFont titleFont = font();
    titleFont.setBold(true);
    p.setFont(titleFont);
    p.setPen(text);
    p.drawText(QRect(12, 6, width() - 24, 24), Qt::AlignLeft | Qt::AlignVCenter,
               m_title.isEmpty() ? tr("Estimated 1RM") : m_title);
    p.setFont(font());

    if (m_points.size() < 2) {
        QColor hint = text;
        hint.setAlpha(150);
        p.setPen(hint);
        p.drawText(rect(), Qt::AlignCenter,
                   m_points.empty() ? tr("No sessions logged for this exercise yet.")
                                    : tr("Log one more session to see a trend."));
        return;
    }

    // ---- 1. Plot rect ------------------------------------------------------
    const QFontMetrics fm(font());
    const int left = fm.horizontalAdvance(QStringLiteral("0000")) + 16;
    const QRectF plot(left, 40, width() - left - 20, height() - 40 - fm.height() - 18);
    if (plot.width() < 40 || plot.height() < 40) return; // too small to be useful

    // ---- 2. Scales ----------------------------------------------------------
    auto [minIt, maxIt] = std::minmax_element(
        m_points.begin(), m_points.end(),
        [](const auto& a, const auto& b) { return a.bestE1rm < b.bestE1rm; });
    const double step = niceStep((maxIt->bestE1rm - minIt->bestE1rm) / 4.0);
    const double yMin = std::floor(minIt->bestE1rm / step) * step - step;
    const double yMax = std::ceil(maxIt->bestE1rm / step) * step + step;

    const long x0 = m_points.front().date.toDayNumber();
    const long x1 = m_points.back().date.toDayNumber();
    const double xSpan = std::max(1L, x1 - x0);

    auto toPixel = [&](const stats::ProgressPoint& pt) {
        const double fx = (pt.date.toDayNumber() - x0) / xSpan;
        const double fy = (pt.bestE1rm - yMin) / (yMax - yMin);
        return QPointF(plot.left() + fx * plot.width(), plot.bottom() - fy * plot.height());
    };

    // ---- 3a. Grid + Y labels ----------------------------------------------
    for (double v = yMin; v <= yMax + step / 2; v += step) {
        const double y = plot.bottom() - (v - yMin) / (yMax - yMin) * plot.height();
        p.setPen(QPen(grid, 1));
        p.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
        p.setPen(text);
        p.drawText(QRectF(0, y - fm.height() / 2.0, left - 8, fm.height()),
                   Qt::AlignRight | Qt::AlignVCenter, formatKg(v));
    }

    // X labels: first and last date (enough context, never overlapping).
    p.setPen(text);
    const QRectF xLabels(plot.left(), plot.bottom() + 6, plot.width(), fm.height());
    p.drawText(xLabels, Qt::AlignLeft, toQt(m_points.front().date).toString("MMM d"));
    p.drawText(xLabels, Qt::AlignRight, toQt(m_points.back().date).toString("MMM d"));

    // ---- 3b. The line -------------------------------------------------------
    QPainterPath path(toPixel(m_points.front()));
    for (std::size_t i = 1; i < m_points.size(); ++i) path.lineTo(toPixel(m_points[i]));
    p.setPen(QPen(line, 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setBrush(Qt::NoBrush);
    p.drawPath(path);

    // ---- 3c. Points + best -------------------------------------------------
    p.setBrush(bg);
    p.setPen(QPen(line, 2));
    for (const auto& pt : m_points) p.drawEllipse(toPixel(pt), 3.5, 3.5);

    const QPointF bestPx = toPixel(*maxIt);
    p.setBrush(best);
    p.setPen(QPen(best.darker(130), 1.5));
    p.drawEllipse(bestPx, 6, 6);

    const QString label = tr("Best %1 kg").arg(QString::number(maxIt->bestE1rm, 'f', 1));
    const int labelW = fm.horizontalAdvance(label) + 12;
    // Keep the label inside the plot: flip to the left near the right edge.
    const double lx = bestPx.x() + labelW + 10 > plot.right() ? bestPx.x() - labelW - 10
                                                               : bestPx.x() + 10;
    const QRectF box(lx, bestPx.y() - fm.height() - 10, labelW, fm.height() + 6);
    QColor boxFill = best;
    boxFill.setAlpha(60);
    p.setPen(Qt::NoPen);
    p.setBrush(boxFill);
    p.drawRoundedRect(box, 4, 4);
    p.setPen(text);
    p.drawText(box, Qt::AlignCenter, label);
}

} // namespace ironlog
