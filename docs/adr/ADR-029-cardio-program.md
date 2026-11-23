# ADR-029: Cardio program from a talk-test baseline

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The owner's plan builds stamina with zone-2 incline walking: a week-by-week
table, one change at a time, and intervals only from week 5 (Decision
Report D1). Running is held back because of knee impact. Every user starts
from a different fitness level, and most have no heart-rate strap.

## Decision

Build a **cardio program** (F27).

- **Baseline:** a 10-minute talk test (you can talk but not sing). That
  speed and incline is week 1.
- **Weekly table:** 2 weeks per step. Add minutes first, then speed, then
  incline, **one change at a time** (P10).
- **Intervals** unlock through a gate after 4 weeks of base.
- **Jogging** is a trigger (ADR-027). It appears only if no body
  limitation forbids impact (ADR-026).

## Consequences

- Works without any device. The talk test is a validated stand-in for
  the ventilatory threshold.
- The table is data, so a bike or a rower is a new file, not new code.
- The baseline is self-judged, so it can be off. The first 2 weeks act as a
  check: "Too easy / About right / Too hard" after each session adjusts the
  next step.

## Alternatives considered

- **Heart-rate zones**: need a strap, which the out-of-scope list excludes.
- **HIIT first**: faster VO2max gains, but higher strain and recovery cost
  for a detrained body (D1).
- **A fixed table with no baseline**: wrong for most starting points.

## Sources

- Talk test: https://pubmed.ncbi.nlm.nih.gov/21774751/ ·
  https://pubmed.ncbi.nlm.nih.gov/25536539/
- [arc42 §8.20](../arc42.md#820-cardio-program-f27-adr-029) · [plan-to-features F27](../research/plan-to-features.md)
- Owner's plan Part 2 and Decision Report D1 (private, ADR-019)
