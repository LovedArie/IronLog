// Date.cpp — calendar arithmetic.
//
// toDayNumber/fromDayNumber use Howard Hinnant's public-domain
// "days_from_civil" / "civil_from_days" algorithms
// (https://howardhinnant.github.io/date_algorithms.html). They are exact for
// the proleptic Gregorian calendar and need no lookup tables.
// (General knowledge — not from the project books.)

#include "core/Date.h"

#include <cstdio>

namespace ironlog {

namespace {

bool isLeap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

int daysInMonth(int y, int m) {
    static const int kDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeap(y)) return 29;
    return kDays[m - 1];
}

} // namespace

bool Date::isValid() const {
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    return day <= daysInMonth(year, month);
}

long Date::toDayNumber() const {
    // Shift the year so it starts in March: Feb (with the leap day) becomes
    // the LAST month, so month lengths before it never depend on leap years.
    const int y = month <= 2 ? year - 1 : year;
    const long era = (y >= 0 ? y : y - 399) / 400;
    const long yoe = y - era * 400;                                   // [0, 399]
    const long mp  = (month + 9) % 12;                                // Mar=0 ... Feb=11
    const long doy = (153 * mp + 2) / 5 + day - 1;                    // [0, 365]
    const long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;           // [0, 146096]
    return era * 146097 + doe - 719468;                               // 1970-01-01 = 0
}

Date Date::fromDayNumber(long z) {
    z += 719468;
    const long era = (z >= 0 ? z : z - 146096) / 146097;
    const long doe = z - era * 146097;
    const long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const long mp  = (5 * doy + 2) / 153;
    Date d;
    d.day   = static_cast<int>(doy - (153 * mp + 2) / 5 + 1);
    d.month = static_cast<int>(mp < 10 ? mp + 3 : mp - 9);
    d.year  = static_cast<int>(yoe + era * 400 + (d.month <= 2 ? 1 : 0));
    return d;
}

int Date::weekday() const {
    // 1970-01-01 was a Thursday (index 3 when Monday = 0).
    const long n = toDayNumber();
    const long w = (n + 3) % 7;
    return static_cast<int>(w < 0 ? w + 7 : w);
}

std::string Date::toIso() const {
    char buf[16];
    std::snprintf(buf, sizeof buf, "%04d-%02d-%02d", year, month, day);
    return buf;
}

std::optional<Date> Date::fromIso(std::string_view text) {
    // Strict format: exactly YYYY-MM-DD, digits only.
    if (text.size() != 10 || text[4] != '-' || text[7] != '-') return std::nullopt;
    auto digits = [&](std::size_t from, std::size_t len, int& out) {
        out = 0;
        for (std::size_t i = from; i < from + len; ++i) {
            if (text[i] < '0' || text[i] > '9') return false;
            out = out * 10 + (text[i] - '0');
        }
        return true;
    };
    Date d;
    if (!digits(0, 4, d.year) || !digits(5, 2, d.month) || !digits(8, 2, d.day))
        return std::nullopt;
    if (!d.isValid()) return std::nullopt;
    return d;
}

} // namespace ironlog
