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
   with ±, watch the rest timer, do the plank hold. **Finish** → *How was it?* → Joints: **Pain** →
   pick the exercise, a number 0–10, maybe a red flag → Save.
2. **Today**: the next-morning pain check (0–10). A high number drops that exercise a stage;
   the next plan swaps it ("Swapped for safety").
3. **Today → Ready for…**: an offer (intervals) with **Why?**, *Add it* or *Not now*.
4. **You → Programs → Body limitations**: set Knee stage 1, add Shoulder. Then Train now again:
   reverse lunge becomes step-up; at the gym the bench press is left out.
5. **Train now → Before my sport**: the badminton RAMP warm-up, no static holds.
6. **You**: the body map — darker = higher level, the striped back is fading. Tap a part. Then **⚙ Settings** → turn *Play it like a game* off. Look at Today and You.
7. **Today → Morning stretch → Start**: hold timer per side, a beep to switch sides, then it moves on by itself. Try +15 s on the towel stretch. At the end: the weekly reach test (hand gap in cm). See it, and the **goal tests**, in **Progress → Body**.
8. **Train now → Cramming** (the 3rd this week) → back on Today: the pause offer.
9. **Progress → Photos** → unlock → *Take photos* (recovery-code prompt) → Compare.
10. **Log** → Sun 22 → Strength B → delete a set → **Undo**.
11. Settings → Theme *Dark*, *Reduce motion*, units *kg*.

## What's real, what's fake

| Real (same as the planned app) | Fake (stand-ins) |
|---|---|
| Design tokens: colours, type sizes, 48 px targets, light/dark ([ui-research.md](../../docs/research/ui-research.md)) | Planner: a few hard-coded rules instead of `planner::buildSession()` |
| Screen flow, one primary button per screen, bottom-aligned | XP and levels: simplified, not game rules v2 exactly |
| Timers from stored timestamps (survive switching apps) | Camera and photos: drawn outlines, no real camera |
| Snackbar + Undo, Why? sheets, pre-picked answers | Fingerprint unlock, recovery code, sync |
| Exercise pictures and clips: the real files from `assets/exercises/` (ADR-025) | Cues and muscle lists are typed into `MEDIA`, not read from the catalog (F2) |
| Body limitations: safety filter before anything else, swaps named in the plan, pain 0–10 + next-morning check, red flags (ADR-026) | Joint stress per exercise is typed into `STRESS`; only the knee has a full stage ladder |
| One "ready for…" offer at a time, with its reason (ADR-027) | The offer, goal-test results, cardio week and prehab block are fixed sample data, not computed |

## How it's built (for reading the code)

| Piece | Code | How it works |
|---|---|---|
| State | `S` | One object holds everything |
| Screens | `view…()` | Each turns `S` into HTML; `render()` redraws from `S` |
| Buttons | `data-a`, `data-v`, `A` | One click handler runs `A[action](value)` — *event delegation*: one listener for the whole app |
| Timers | `restEnds` | Store an end time, not a countdown. A 250 ms tick updates only the numbers |
| Exercise pictures | `PIC`, `howImg()`, `clipVideo()`, `muscleImg()` | `PIC` is the folder: `assets/exercises/` when published, `../../assets/exercises/` in the repo |
| Clip | `<video autoplay muted loop playsinline>` | Muted, so phones allow autoplay. Reduce motion → still, with a play button |
| Body limitations | `S.limitations`, `STRESS`, `fitsLimits()`, `painAction()`, `viewLimits()` | `buildPlan()` runs the safety filter first: each exercise's stress per joint ≤ that limitation's stage, else swap to an allowed alternative or leave it out (the note says which) |
| Daily stretch | `STRETCH`, `ROUTINE`, `viewStretch()` | Phases ready → hold → switch → hold → next; a second 250 ms timer moves between them. ±15 s is kept in `holdSec` |
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
