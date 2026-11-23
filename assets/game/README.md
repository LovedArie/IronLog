# Game assets

> **What this is**: the pictures the game layer uses. **Read it when** you
> change the body map or add a game picture.

## Body map (You tab, game on)

Both files are made by [`tools/make_bodymap.py`](../../tools/make_bodymap.py)
from the grey exercise diagram
[`ironlog-muscles-base.jpg`](../exercises/ironlog-muscles-base.jpg), so the
game and the exercise pictures show the same body.

| File | Used by | What it is |
|---|---|---|
| `ironlog-bodymap-key.png` | the Qt app (planned, F12) | **Colour key**, 1200 × 800 PNG, 8 exact colours. The app reads it pixel by pixel and repaints each skill's area by level |
| `bodymap-paths.json` | the UI prototype (`BODY`) | One SVG outline per skill (+ `none` for head, hands, feet), viewBox `0 0 1200 800` |

| Key colour | Skill |
|---|---|
| Blue `#0000FF` | Arms (biceps, triceps, forearms) |
| Yellow `#FFFF00` | Shoulders |
| Green `#00FF00` | Chest |
| Cyan `#00FFFF` | Back (traps, rhomboids, lats, lower back) |
| Magenta `#FF00FF` | Core (abs, obliques) |
| Red `#FF0000` | Legs (glutes, quads, hamstrings, calves) |
| Grey `#808080` | Head, hands, feet — not a skill |
| White | Background and the lines between muscles |

The key colours are never shown to the user. On screen each area uses one
purple hue (`xp` token): darker = higher level, stripes = fading soon
(design system v1, `docs/research/ui-research.md`). Stamina and Mobility
have no body area; they show as bars under the map.

**How the script decides:** grey pixels = muscle, white = lines. Each
connected grey area is one muscle. A seed point per muscle names its skill;
head, hands and feet are marked as not a skill; any leftover piece joins
the nearest skill. Change a skill's area by moving or adding a seed in
`SEEDS`, then run `python3 tools/make_bodymap.py` from the repo root.

*Changed 2026-11-23:* a ChatGPT-made key was planned first. The script
replaced it: same look, exact alignment with the muscle diagrams, and it
can be re-run.
