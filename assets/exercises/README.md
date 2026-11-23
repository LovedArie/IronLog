# Exercise pictures

Two pictures and one clip per exercise ([ADR-025](../../docs/adr/ADR-025-exercise-pictures-from-chatgpt.md); rules in [arc42 §8.16](../../docs/arc42.md#816-exercise-pictures)):

| File | Shows |
|---|---|
| `ironlog-<id>-how.jpg` | **How**: the IronLog character in side view, start pose → end pose |
| `ironlog-<id>-muscles.jpg` | **Muscles**: front + back muscle diagram; main muscles dark blue, helpers light blue |
| `clips/ironlog-<id>-clip.mp4` | **Clip** (~7 s, 720 × 960): start ↔ end pose twice, then the muscles with their names. Built by `tools/make_clips.py` |

`ironlog-muscles-base.jpg` is the plain grey diagram every muscle picture
was coloured from — paste it into a new ChatGPT chat to make more that match.

`exercises.json` lists every exercise with its two files and its muscles.
The app shows the muscle names as text too (never colour alone).

All images: 1200 × 800 JPEG, made with ChatGPT image generation on
2026-11-23 and checked by eye for form and anatomy errors. How they were
made, and how to make more that match: [CREDITS.md](CREDITS.md).

## Missing

| Exercise | Missing | Why |
|---|---|---|
| Plank, Dead bug | muscle diagram | ChatGPT's content filter refused these two (false positive on the body diagram). Owner to decide: retry later, or use the app's text legend only |

## Adding an exercise

1. In the ChatGPT chat "Create Box Squat Image", ask for the next exercise
   with the same wording as the others (see CREDITS.md).
2. In the chat "Edit Muscle Diagram", ask for the new muscle colours.
3. Check both pictures (form, fingers, right muscles), save as
   `ironlog-<id>-how.jpg` / `ironlog-<id>-muscles.jpg` (1200 × 800), add a
   line to `exercises.json` and the step labels to `STEPS` in
   `tools/make_clips.py`.
4. Run `python3 tools/make_clips.py assets/exercises assets/exercises/clips <id>`.
