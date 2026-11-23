// QtConvert.h — the ONE place where Qt types meet domain types.
//
// The core speaks std::string and ironlog::Date; Qt speaks QString and
// QDate. Converting at a single boundary (an "adapter" seam) keeps Qt out
// of the core and keeps conversion rules from being re-invented per file.
#pragma once

#include <QDate>
#include <QString>

#include <string>
#include <string_view>

#include "core/Date.h"

namespace ironlog {

inline Date toDomain(const QDate& d) { return Date{d.year(), d.month(), d.day()}; }
inline QDate toQt(const Date& d) { return QDate(d.year, d.month, d.day); }

// Qt 6's toStdString() is UTF-8, and the core treats strings as UTF-8, so
// accented exercise names ("Développé couché") survive the round trip.
inline std::string toStd(const QString& s) { return s.toStdString(); }
inline QString toQt(std::string_view s) {
    return QString::fromUtf8(s.data(), static_cast<qsizetype>(s.size()));
}

// 100 -> "100", 102.5 -> "102.5", 51.25 -> "51.25". 'g' drops trailing zeros.
inline QString formatKg(double kg) { return QString::number(kg, 'g', 6); }

// "1 set" / "17 sets". Qt's tr("%n set(s)") only pluralises when a
// translation file is loaded; without one it prints "set(s)" literally.
inline QString countOf(int n, const QString& one, const QString& many) {
    return QString::number(n) + ' ' + (n == 1 ? one : many);
}

} // namespace ironlog
