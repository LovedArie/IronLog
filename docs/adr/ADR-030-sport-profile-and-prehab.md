# ADR-030: Sport profiles and prehab blocks as data

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The owner's plan must coexist with badminton (constraint C7):

- a separate RAMP warm-up before play;
- static stretching after play, not before;
- accessories for the areas badminton loads;
- knee-care add-ons 2–3 times a week;
- a staged stability program 3 times a week (Decision Report D7, D9).

Other users play other sports, and other joints need other prehab.

## Decision

Make **sports** and **prehab blocks** data (F28).

- A **sport profile** holds the body areas it loads, its RAMP warm-up and
  its weak spots.
- Sport days are respected: no hard leg session within 24 h before.
- **Before my sport** is a Train now situation that gives only the sport's
  warm-up.
- A **prehab block** (10–12 min) is chosen from the user's limitations and
  sport. It has stages and a weekly target. Blocks go after cardio or on
  rest days.

## Consequences

- A new sport is one file: running, soccer, climbing.
- Prehab counts in the weekly plan but not in the quest, so it never
  threatens the streak.
- More content to write and check: every sport warm-up and block needs a
  sourced reason.

## Alternatives considered

- **Badminton hard-coded**: the app would serve only the owner.
- **No sport awareness**: heavy legs the day before a match hurt
  performance and raise risk (C7).
- **Unstable-surface gear (BOSU, wobble boards)**: not owned. A folded mat
  gives a similar balance challenge (D7).

## Sources

- [arc42 §8.21](../arc42.md#821-sport-profile-and-prehab-blocks-f28-adr-030) · [plan-to-features F28](../research/plan-to-features.md)
- Owner's plan Parts 3–5 and Decision Report C7, D7, D9 (private, ADR-019)
