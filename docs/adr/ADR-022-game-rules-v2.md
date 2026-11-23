# ADR-022: Game rules v2 (evidence-based)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Research round 2: v1 decay was harsher than real detraining; cheat meals are linked to eating-disorder symptoms in young adults and "cheat" wording breeds guilt; forgiving streaks (freezes, pauses, weekly counting) beat punishing ones.

## Decision

Decay after 14 days at −0.5 %/day with a 3-day warning; 50 % floor; Comeback bonus ×2; Pause with a reason (exams 21, sick 14, injured 42, travel 21 days); weekly forgiving streak; rest token (bank max 2) + a reward the user writes, replacing the cheat-meal token; "sets to next level"; balance table versioned.

## Consequences

- The game matches what bodies and people actually do.
- The app never suggests food or says "cheat".
- Version bumps tell users why levels changed.

## Alternatives considered

- Keep v1 rules.

## Sources

- docs/research/feature-research-2.md §A, critique section
