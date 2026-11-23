# ADR-014: Game state derived from the log

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

XP, levels, decay and streaks must agree on every device and survive rebalancing.

## Decision

Store only inputs (sets, cardio, pauses, rest-token uses, game-mode changes). Compute the character sheet with `game::computeSheet(log, pauses, today)` on demand.

## Consequences

- Nothing to sync or corrupt; devices always agree.
- Rebalancing re-scores history (rules are versioned, ADR-022).
- Game mode can be toggled losslessly (ADR-021).
- Recomputing on open is cheap at this data size.

## Alternatives considered

- Store XP and levels as fields.

## Sources

- arc42 §8.4
