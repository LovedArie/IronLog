# ADR-021: Game layer is optional per user

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Not everyone wants levels and streaks; the partner may prefer a plain tracker.

## Decision

A per-user "game mode" checkbox (onboarding + settings). Off hides XP, levels, streak and tokens and pauses decay. Safety rules (progression gates, pain check) never depend on it.

## Consequences

- Lossless toggle because game state is derived (ADR-014).
- `GameModeChange` records mark decay-free intervals.
- Lowers the risk of the game backfiring (R-12).

## Alternatives considered

- Game always on.

## Sources

- arc42 §8.6
