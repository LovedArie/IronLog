# ADR-027: One engine for gates, goal tests and triggers

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The owner's plan has many "do X when Y" rules: stage gates (2 clean weeks),
unlocks (reverse lunge after 3–4 pain-free weeks), a phase-2 checklist at
week 8, jogging after a long easy walk plus 8 pain-free weeks, a split change
after 3–4 months, and re-tests every 4 weeks. The backlog had them spread
over P6, F22 (stall → deload) and ad-hoc notices. Every user's plan will
have rules like these. Analysis: [plan-to-features §2 F26](../research/plan-to-features.md#f26-gates-goal-tests-and-triggers-the-revisit-when-scaled).

## Decision

Build **one rules engine**, with its rules in data files.

- A **gate** is criteria plus a minimum time. It unlocks an exercise, a stage or a phase.
- A **trigger** is a condition over derived stats, a message and an offer. The user accepts or
  dismisses it. It never acts on its own.
- A **goal test** is a scheduled measurement (default every 4 weeks), charted in Progress.

The engine only reads derived state (ADR-014). It is a pure function of
(log, profile, rules, today). F22 (stall → deload) becomes one trigger.

## Consequences

- Plans from the owner, a coach or a physio become data, rules included.
- Every rule can be tested with a fixed log and a fixed date.
- Both time **and** criteria, as the plan does. Rehab research says time
  alone doesn't show readiness.
- A small condition language is needed (comparisons over derived stats,
  "for N weeks"). Keep it closed: no scripting.
- Too many notices would be noise. Show at most one trigger on Today
  (the same rule as the notice card).

## Alternatives considered

- **Hard-code each rule**: fast at first, but every new plan means new code.
- **Time-only gates**: simple, but ignore pain and performance.
- **Automatic progression without asking**: takes control away from the
  user, and would be wrong when the log is incomplete.

## Sources

- [plan-to-features.md](../research/plan-to-features.md) (F26)
- Criteria- vs time-based progression: https://pmc.ncbi.nlm.nih.gov/articles/PMC8811519/
  · https://pubmed.ncbi.nlm.nih.gov/28078610/
- Owner's plan and Decision Report, D1, D2, D6, D7, §13 (private, ADR-019)
