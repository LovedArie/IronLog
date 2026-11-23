# ADR-024: Exercise pictures drawn by code (form animation + muscle map)

- **Status:** Proposed — waiting for the owner's verdict on the prototype demo
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

The owner: "Every workout needs pictures. Words alone will not motivate me."
Each exercise needs two pictures: **how** to do it, and **which muscles** it
works. The owner's first idea was AI image generation.

Facts that shaped the choice:

- Pictures are expected; unclear instructions are a top complaint in
  mobility apps (research round 2, `feature-research-2.md` §E).
- Free exercise image sets have unclear or per-entry licences (same
  research, §E3), so bundling them is risky.
- AI images of people often have body errors: a 2025 benchmark of four
  text-to-image models found distortions in nearly 50 % of human images
  (Ma et al., arXiv 2503.00811). A wrong joint angle in a form picture is a
  safety problem, and a novice can't spot it.
- Animation beats still pictures most when learning a **movement**
  (procedural-motor knowledge, d = 1.06, Höffler & Leutner 2007).
- AI images would be made by hand, ~2 per exercise, and must keep one look
  across 60+ exercises.

## Decision

Draw both pictures in code, from data in the exercise catalog (F2):

1. **Form figure** — a side-view body. Each exercise stores 2–3 **key
   poses** (hip position, spine angle, foot and hand targets). Limbs are
   placed by 2-bone **inverse kinematics**; the app eases between poses in a
   loop with a tempo label ("Down 2 s"). With *Reduce motion* on, one still
   shows the start pose faint and the end pose solid.
2. **Muscle map** — front + back body, one shape per muscle group; main
   muscles solid, helpers light, plus a text legend (never colour alone).

Shown as a thumbnail on each plan row, a card in the guided session, and a
full how-to sheet with 3 form cues.

Demo: `prototypes/ui-prototype/` (17 exercises).

## Consequences

- Same look for every exercise, light and dark theme, crisp at any size,
  tiny, works offline. All assets are ours — no licence records needed.
- Adding an exercise = adding key poses + muscles to the catalog (data, not
  drawing). Poses must be checked by eye once (a gallery test renders them all).
- Side view only: sideways movements (lateral raise, side plank) need a
  front-view figure later.
- Less realistic than photos or video. Motivation must come from the
  movement, the muscle highlight and progress — not from a model's physique.
- In QML: `Shape`/`Canvas` + a C++ `PoseAnimator` view model; the IK and
  pose maths can live in `core/` and be unit-tested.

## Alternatives considered

- **AI-generated images** (owner's first idea): rejected for form pictures
  (body errors, still images, hand work, consistency); could still serve as
  optional "hero" art later. Not possible from this session either.
- **Licensed photo/GIF packs**: unclear licences (research §E3).
- **Filming the owner**: real, but slow to make and redo; privacy.

## Sources

- docs/research/feature-research-2.md §E3 (media licences) and §E (complaints)
- Höffler & Leutner (2007), *Instructional animation versus static pictures: a meta-analysis*, Learning and Instruction 17(6)
- Ma et al. (2025), *Evaluating and Predicting Distorted Human Body Parts for Generated Images*, arXiv 2503.00811
