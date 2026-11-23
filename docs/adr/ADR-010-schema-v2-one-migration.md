# ADR-010: Schema v2 in one migration

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Sync needs stable record ids and tombstones; research found RPE/RIR, set types, notes, secondary muscles and timed sets all change the file format.

## Decision

One migration to schema v2: ids + `updatedAt` + `deleted` on every record; weight stored with its unit; RIR, set type, note; timed sets; new records (cardio, mobility, body check-in, progress photo, session review, pause, rest-token use, user reward, game-mode change). Drops "one session per day".

## Consequences

- One migration to write and test instead of five.
- Sync can merge by id (ADR-015).
- The v1 file is kept as `ironlog.v1.json` (copied, never moved).
- F1 is a large item.

## Alternatives considered

- Five small schema bumps as features arrive.

## Sources

- docs/research/feature-research.md ("one schema v2 migration")
