# ADR-026: Body limitations replace the knee-only stage

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)
- **Supersedes:** —

## Context

The planner has one joint rule: "knee stress ≤ current knee stage" (arc42
§8.5, P3). It came from the owner's plan, which was written around one knee.
Other users will have a shoulder, lower back, ankle or wrist that needs
care. A knee-only design would need new code for each joint. The pain check
is yes/no, so "mild ache, gone by morning" and "sharp pain, swollen next
day" get the same answer. Analysis: [plan-to-features §2 F25](../research/plan-to-features.md#f25-body-limitations-the-knee-scaled).

## Decision

Model a **limitation** as data: joint, side, stage. Every exercise rates its
**stress per joint** (low / medium / high) and flags kneeling, impact, deep
bend and overhead. **Stage ladders** per joint (data files) say what each
stage allows and what unlocks. The planner's safety filter checks every
limitation before anything else, and nothing can override it. Replace the
yes/no pain check with **pain monitoring** (0–10 during, plus a next-morning
check; 0–3 OK, 4–5 hold, > 5 or worse next day → down a stage for 2 weeks).
**Red flags** (swelling, heat, locking, giving way, can't take weight) stop
the exercise and point to a professional; the joint stays out of plans until
the user marks it "cleared by a professional". **Flare-ups** can be reported
any time, outside a session; the same rules then act on the joint's stage.
Every report goes into a per-joint **pain log**, shown in the body part's
detail and as a number on the body map.

## Consequences

- A new joint is a new row in the ladders and stress columns. It needs no new code.
- The knee rules become one instance, so existing tests keep their meaning.
- Catalog work grows: every exercise needs stress ratings for 7 joints (F2).
- Pain is more nuanced, but needs a next-morning question (one notification).
- New risk R-24: someone whose condition the rules don't fit follows the plan.
  Mitigation: PAR-Q+ link at onboarding, red flags, "not medical advice".

## Alternatives considered

- **Knee only, add joints later**: new code per joint, and the planner's
  safety tests would be rewritten each time.
- **Free-text injuries**: the planner can't use free text, so the safety
  filter couldn't be tested.
- **Diagnosis-based profiles** ("ACL", "rotator cuff"): this is a medical
  claim that the app can't verify. It also needs clinical review.

## Sources

- [plan-to-features.md](../research/plan-to-features.md) (F25, sources list)
- Silbernagel et al. (2007), pain-monitoring model:
  https://search.pedro.org.au/search-results/record-detail/18389
- NHS knee pain: https://www.nhs.uk/symptoms/knee-pain/
- PAR-Q+: https://doi.org/10.14288%2Fhfjc.v12i4.295
- Owner's plan and Decision Report, C1 and D3 (private, ADR-019)
