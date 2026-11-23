# ADR-002: Own `Date` type in core instead of `QDate`

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

`core/` must not include Qt (ADR-001). `std::chrono::year_month_day` is C++20; the project uses C++17.

## Decision

A small `Date` struct in `core/` using Howard Hinnant's public-domain day-number algorithms. Qt types are converted at one boundary (`services/QtConvert.h`).

## Consequences

- Core stays Qt-free and portable.
- We own date math, so it has its own tests (round trips, leap years, week boundaries).
- One conversion step at the Qt boundary.

## Alternatives considered

- `QDate` everywhere — breaks ADR-001.
- C++20 chrono calendar — would raise the language version.
- Boost.Date_Time — a heavy dependency for little code.

## Sources

- Hinnant, *chrono-Compatible Low-Level Date Algorithms*: https://howardhinnant.github.io/date_algorithms.html
