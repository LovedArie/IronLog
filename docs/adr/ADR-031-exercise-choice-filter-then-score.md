# ADR-031: Exercise choice — safety filters first, then a score

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The owner's Decision Report chose every part of the plan the same way. It
rated options on six weighted criteria and let **safety win** whenever it
conflicted with speed of results:

| Criterion | Weight |
|---|---|
| K1 Safety | highest |
| K2 Effectiveness | high |
| K3 Sustainability | high |
| K4 Equipment fit | medium |
| K5 Sport transfer | medium |
| K6 Time efficiency | medium |

It also chose compound-first with a little targeted isolation (D8). Planner
rule P3 only filtered and ordered. It didn't say how to choose between
allowed exercises.

## Decision

`planner::buildSession()` chooses exercises in two steps.

1. **Hard filters** that nothing can override:
   - equipment available;
   - every body limitation's stage (ADR-026);
   - gates not yet passed.
2. **A weighted score** on K2–K6 over what's left. Each exercise carries its
   scores in the catalog. The goal program and sport profile can change the
   weights.

Safety is **only** a filter, never a weight, so a high score can't buy back
risk. Order stays compound first, isolation last. Each plan item names its
top reason in the Why? sheet.

## Consequences

- Choices are explainable and testable: the same inputs give the same
  scores.
- Programs differ by weights, not by code. "Time-crunched" can weight K6
  higher.
- The catalog needs scores per exercise, set by hand from the plan's
  reasoning. They are reviewable data.
- Ties need a stable rule (catalog order) so plans don't flicker.

## Alternatives considered

- **Safety as the biggest weight**: a large effectiveness score could
  still outweigh it.
- **Fixed exercise lists per program**: simple, but unaware of equipment,
  limitations and the time budget.
- **AI selection**: not explainable or testable enough for a coach that
  plans around injuries (ADR-013).

## Sources

- [arc42 §8.5](../arc42.md#85-planner-rules-engine) · [plan-to-features §2](../research/plan-to-features.md)
- Owner's Decision Report §4 (K1–K6) and D8 (private, ADR-019)
