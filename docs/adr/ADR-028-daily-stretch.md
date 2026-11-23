# ADR-028: Daily stretch — static in the morning, moving before play

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The owner's plan has a daily morning stretch routine (12 stretches) and 12
add-ons. Its Decision Report (D4, D9) explains why: static holds bring back
range of motion, but they can lower power for a short time. So they are kept
away from explosive play, and dynamic warm-ups come before play. None of
this was a feature (F24). The routine must not cost the streak on a missed
morning, and it must work hands-free.

## Decision

Ship the **daily stretch** as data plus a guided player.

- **Morning routine**, every day (~12 min), kept in an exams pause.
- **Add-ons** after training or in the evening, picked for body areas not
  stretched yet this week.
- Holds start at 30 s per side, with a switch-sides cue, and move on
  automatically. Tight spots grow by ±15 s, remembered, up to 60 s.
- A **weekly reach test** after the towel stretch: hand gap in cm per side.
- **Planner rule P8:** static holds never go in a warm-up or right before a
  sport; warm-ups are dynamic (RAMP).
- Stretching gives Mobility XP. It is **not** a weekly-quest item.

## Consequences

- The plan's most frequent habit (7 days a week) is in the app.
- The reach test turns "flexibility" into a number the user can see move.
- Hands-free timing needs reliable sounds and vibration on the phone. The
  QML spike (I4) must check them.
- Pictures for 24 stretches are still missing. They're text-only until
  made with the ADR-025 pipeline.

## Alternatives considered

- **Stretching as quest items**: a missed morning would cost the streak,
  which punishes the habit.
- **A plain checklist, no timer**: no per-side timing, so holds get rushed.
- **Static stretching in the warm-up**: blunts power before play (D4, D9).

## Sources

- [arc42 §8.17](../arc42.md#817-daily-stretch-f24) · [plan-to-features](../research/plan-to-features.md)
- Owner's plan Part 1, 1b and Decision Report D4, D9 (private, ADR-019)
