# UI prototype — 4-tab phone layout

**Question it answers:** does the 4-tab layout ([ADR-023](../../docs/adr/ADR-023-four-tab-navigation.md))
house every feature without clutter, on a real phone, before any QML is written?

**Status:** throwaway. Delete it once the QML UI (backlog I4, then v0.2+)
has answered the same question.

## Run it

| Where | How |
|---|---|
| PC | Double-click `index.html` (opens in your browser) |
| Phone, same Wi-Fi | On the PC, in the **repo root**: `python -m http.server 8000` → on the phone: `http://<PC's IP>:8000/prototypes/ui-prototype/` (find the IP with `ipconfig`). The root is needed because the pictures live in `assets/exercises/` |
| Phone, anywhere | The published link Claude shared in the chat |

One page, no internet, no install. It loads the exercise pictures and clips
from `assets/exercises/` (two folders up). **Fake data, nothing is saved** —
reload to start over.

## Try this tour

1. **Train now** → 30 min → Normal → Home → plan. Tap a **thumbnail** for the
   how-to sheet (clip, how-to picture, muscle diagram, form cues). **Start session**. Log sets
   with ±, watch the rest timer, do the plank hold. **Finish** → *How was it?* → Save.
2. **You**: the body map — darker = higher level, the striped back is fading. Tap a part. Then **⚙ Settings** → turn *Play it like a game* off. Look at Today and You.
3. **Train now → Cramming** (the 3rd this week) → back on Today: the pause offer.
4. **Progress → Photos** → unlock → *Take photos* (recovery-code prompt) → Compare.
5. **Log** → Sun 22 → Strength B → delete a set → **Undo**.
6. Settings → Theme *Dark*, *Reduce motion*, units *kg*.

## What's real, what's fake

| Real (same as the planned app) | Fake (stand-ins) |
|---|---|
| Design tokens: colours, type sizes, 48 px targets, light/dark ([ui-research.md](../../docs/research/ui-research.md)) | Planner: a few hard-coded rules instead of `planner::buildSession()` |
| Screen flow, one primary button per screen, bottom-aligned | XP and levels: simplified, not game rules v2 exactly |
| Timers from stored timestamps (survive switching apps) | Camera and photos: drawn outlines, no real camera |
| Snackbar + Undo, Why? sheets, pre-picked answers | Fingerprint unlock, recovery code, sync |
| Exercise pictures and clips: the real files from `assets/exercises/` (ADR-025) | Cues and muscle lists are typed into `MEDIA`, not read from the catalog (F2) |

## How it's built (for reading the code)

| Piece | Code | How it works |
|---|---|---|
| State | `S` | One object holds everything |
| Screens | `view…()` | Each turns `S` into HTML; `render()` redraws from `S` |
| Buttons | `data-a`, `data-v`, `A` | One click handler runs `A[action](value)` — *event delegation*: one listener for the whole app |
| Timers | `restEnds` | Store an end time, not a countdown. A 250 ms tick updates only the numbers |
| Exercise pictures | `PIC`, `howImg()`, `clipVideo()`, `muscleImg()` | `PIC` is the folder: `assets/exercises/` when published, `../../assets/exercises/` in the repo |
| Clip | `<video autoplay muted loop playsinline>` | Muted, so phones allow autoplay. Reduce motion → still, with a play button |
| Body map | `BODY`, `bodyMap()` | Outlines from `assets/game/bodymap-paths.json`; fill = level shade, or stripes when fading; tap → `skillTap` snackbar |
| Missing diagram | `NO_DIAGRAM` | Plank and dead bug show a box; the muscle names below still say it all |

*The code-drawn pictures (ADR-024, rejected) were removed on 2026-11-23.
They are in git history (commit `5e66649` and earlier).*

That's the same shape the QML app will have: state in C++ view models,
QML screens that only display it, and actions that change the state.

## Tested

Headless Chrome clicked through all the flows above at 390 × 844 (44 screens),
and a gallery script rendered every exercise at 5 points of its loop (light + dark):
no JavaScript errors, no tap target under 44 px.
