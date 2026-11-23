# ADR-025: Exercise pictures generated with ChatGPT (anime character + muscle diagram)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner (decision) + Claude (production)
- **Supersedes:** ADR-024 (code-drawn pictures)

## Context

Every exercise needs a **how** picture and a **muscles** picture (ADR-024
context). Claude built code-drawn pictures (simple, then anime style). The
owner tried them and said: *"no, I don't like it. use chatgpt"* — then asked
for the muscle picture as an anatomy view, not the character, and for videos.

## Decision

- **How:** one ChatGPT image per exercise: an original anime-style character
  (fixed description, same chat for consistency), side view, start pose →
  end pose with an arrow. 1200 × 800 JPEG.
- **Muscles:** one ChatGPT image per exercise: a flat grey front/back muscle
  diagram (one shared base), main muscles dark blue, helpers light blue.
  ChatGPT drew them red/orange; they were recoloured locally to the design
  system's blue (`#2F5BD0` / `#8FAAF0`) because red vs orange fails the UI
  research's colour-blind check (`docs/research/ui-research.md`, design
  system v1). Dark vs light of one hue passes.
- **Videos:** OpenAI's video tool (Sora) was shut down in March 2026 and
  ChatGPT offers no video option on this account. Short clips are made from
  the two pictures with ffmpeg (start ↔ end pose flip at the exercise's tempo,
  then the muscle diagram with the muscle names). See backlog F23.
- Files live in `assets/exercises/` with `exercises.json` and a provenance
  file (`CREDITS.md`: tool, date, prompts, checks).

## Consequences

- Motivating, consistent art the owner likes; realistic enough to read form.
- Every image must be **checked by eye** (form, fingers, right muscles) — AI
  images of people often contain errors. Done for the first 32 images.
- Still pictures, not motion: the clips flip between two poses.
- Adding an exercise = two prompts + a check + one `exercises.json` line;
  someone (owner or Claude in the owner's browser) must run ChatGPT.
- ChatGPT's content filter refused the plank and dead-bug diagrams (false
  positives); those two use the text legend until resolved.
- Assets are binary files in git (~3 MB for 32 images). Fine at this size;
  revisit (Git LFS) past ~50 MB.

## Alternatives considered

- **Code-drawn pictures (ADR-024)** — owner rejected the look.
- **Licensed photo/GIF packs** — unclear licences (research round 2, §E3).
- **Other AI video tools** (e.g. Google Veo, Runway) — not on the owner's
  accounts; revisit if the owner wants real motion later.

## Sources

- docs/adr/ADR-024-exercise-pictures-drawn-by-code.md
- assets/exercises/CREDITS.md
- NPR / AP, 2026-03-25: OpenAI shuts down Sora
