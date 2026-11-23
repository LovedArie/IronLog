// Date.h — a tiny calendar date for the domain core.
//
// WHY NOT QDate? The core is pure C++17 so it compiles and unit-tests with
// no Qt installed (same layering rule as EyesOff's core/). The Qt layer
// converts QDate <-> Date at the boundary (see app/QtConvert.h).
//
// WHY NOT std::chrono::year_month_day? That calendar API is C++20; the
// template pins C++17.
//
// Date is a "simple data structure" with value semantics, so it is a struct
// (Stroustrup §16.2.4: struct for plain data, class for a type with an
// invariant). Use isValid() before trusting a Date built from user input.
#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace ironlog {

struct Date {
    int year  = 1970;
    int month = 1;   // 1..12
    int day   = 1;   // 1..31 (depends on month)

    // True if this is a real calendar date (handles leap years).
    bool isValid() const;

    // Days since 1970-01-01. Turning a date into ONE integer makes
    // "difference in days" and "add N days" plain integer arithmetic.
    long toDayNumber() const;
    static Date fromDayNumber(long days);

    Date addDays(long n) const { return fromDayNumber(toDayNumber() + n); }

    // 0 = Monday ... 6 = Sunday (ISO 8601 order: training weeks start Monday).
    int weekday() const;
    Date mondayOfWeek() const { return addDays(-weekday()); }

    // "2026-11-23" <-> Date. fromIso returns nullopt for malformed text
    // or impossible dates like 2026-02-30.
    std::string toIso() const;
    static std::optional<Date> fromIso(std::string_view text);
};

// Comparison operators: C++17 has no <=>, so we define the set by hand.
// Comparing day numbers is simplest and obviously correct.
inline bool operator==(const Date& a, const Date& b) {
    return a.year == b.year && a.month == b.month && a.day == b.day;
}
inline bool operator!=(const Date& a, const Date& b) { return !(a == b); }
inline bool operator<(const Date& a, const Date& b) {
    return a.toDayNumber() < b.toDayNumber();
}
inline bool operator>(const Date& a, const Date& b) { return b < a; }
inline bool operator<=(const Date& a, const Date& b) { return !(b < a); }
inline bool operator>=(const Date& a, const Date& b) { return !(a < b); }

} // namespace ironlog
