# ADR-018: Risk-driven milestone order

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Many features; one developer. Some parts are new (QML on phones, planner rules), some are proven (the server).

## Decision

Order milestones by risk first: v0.2 Foundation (CI, schema v2, QML + camera spike) → v0.3 Coach → v0.4 Game & body → v0.5 Online → v0.6 Progress+.

## Consequences

- Unknowns are found early, when changing course is cheap.
- Sync arrives after some phone-only use (data safety relies on local backups until then).

## Alternatives considered

- Server first because the VPS exists.

## Sources

- Larman, Ch. 8 §8.3 (risk-driven iterations)
