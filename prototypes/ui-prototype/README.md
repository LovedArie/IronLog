# UI prototype — 4-tab phone layout

**Question it answers:** does the 4-tab layout ([ADR-023](../../docs/adr/ADR-023-four-tab-navigation.md))
house every feature without clutter, on a real phone, before any QML is written?

**Status:** throwaway. Delete it once the QML UI (backlog I4, then v0.2+)
has answered the same question.

## Run it

| Where | How |
|---|---|
| PC | Double-click `index.html` (opens in your browser) |
| Phone, same Wi-Fi | On the PC: `python -m http.server 8000` in this folder → on the phone: `http://<PC's IP>:8000` (find the IP with `ipconfig`) |
| Phone, anywhere | The published link Claude shared in the chat |

One file, no internet, no install. **Fake data, nothing is saved** — reload to
start over.

## Try this tour

1. **Train now** → 30 min → Normal → Home → plan → **Start session**. Log sets
   with ±, watch the rest timer, do the plank hold. **Finish** → *How was it?* → Save.
2. **You → ⚙ Settings** → turn *Play it like a game* off. Look at Today and You.
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

## How it's built (for reading the code)

- **One state object `S`** holds everything. Each `view…()` function turns
  `S` into HTML. `render()` redraws the screen from `S`.
- **Buttons carry `data-a` (action) and `data-v` (value).** One click handler
  looks up `A[action](value)`. This is *event delegation*: one listener for
  the whole app instead of one per button.
- **Timers** store an end time (`restEnds`), not a countdown. A 250 ms tick
  updates only the numbers, so the screen doesn't flicker.

That's the same shape the QML app will have: state in C++ view models,
QML screens that only display it, and actions that change the state.

## Tested

Headless Chrome clicked through all the flows above at 390 × 844 (43 screens):
no JavaScript errors, no tap target under 44 px.
