# ADR-013: Rule-based planner; AI later on top

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **See also:** [ADR-026](ADR-026-body-limitations.md), [ADR-027](ADR-027-gates-and-triggers.md), [ADR-031](ADR-031-exercise-choice-filter-then-score.md) refine the rules (now P1–P11). This decision stands.

## Context

The app tells people with injuries what to lift. Plans must be safe, explainable and testable. The owner's training decisions already form a rule set.

## Decision

`planner::buildSession()` is a pure C++ function implementing written rules (P1–P7) and returning a reason per item. An LLM may later explain or tune plans *inside* those rules.

## Consequences

- Every plan can be tested (e.g. never exceeds a constraint stage).
- Every exercise has a "why".
- Less "magic" than AI-generated workouts (which users distrust).

## Alternatives considered

- AI-generated workouts (Fitbod-style).

## Sources

- docs/research/feature-research.md, feature-research-2.md (trust in coach apps)
