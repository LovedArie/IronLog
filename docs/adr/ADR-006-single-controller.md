# ADR-006: All writes through one `TrainingController` + signals

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Several views (tables, chart, calendar) show the same data. If each edits it, they drift apart.

## Decision

Every user action is one call on `TrainingController`: validate → update → save → emit `logChanged()`. Views only listen.

## Consequences

- Views can't disagree; one place to validate and save.
- Same observer shape as the finance tracker's `ledgerChanged`.
- The controller grows with features — split by use case if it gets large.

## Alternatives considered

- Widgets editing the domain directly.

## Sources

- Larman, Ch. 17 (GRASP Controller)
