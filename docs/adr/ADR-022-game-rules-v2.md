# ADR-022: Game rules v2 (evidence-based)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Research round 2: v1 decay was harsher than real detraining; cheat meals are linked to eating-disorder symptoms in young adults and "cheat" wording breeds guilt; forgiving streaks (freezes, pauses, weekly counting) beat punishing ones.

## Decision

| Rule | Value |
|---|---|
| Decay starts | after 14 days without training a skill |
| Decay speed | −0.5 % per day, with a 3-day warning first |
| Floor | never below 50 % of the skill's peak |
| Comeback bonus | ×2 XP until back at the old level |
| Pause (with a reason) | exams ≤ 21 days · sick ≤ 14 · injured ≤ 42 · travel ≤ 21; no decay while paused |
| Streak | counts weeks, not days; forgiving (rest days count, pauses keep it) |
| Weekly quest reward | rest token (bank max 2) + a reward the user writes — replaces the cheat-meal token |
| Shown to the user | "sets to next level" |
| Versioning | the balance table has a version number |

## Consequences

- The game matches what bodies and people actually do.
- The app never suggests food or says "cheat".
- Version bumps tell users why levels changed.

## Alternatives considered

- Keep v1 rules.

## Sources

- docs/research/feature-research-2.md §A, critique section
