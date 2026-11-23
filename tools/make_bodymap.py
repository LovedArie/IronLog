#!/usr/bin/env python3
"""Make the game body map from the grey exercise muscle diagram.

Input : assets/exercises/ironlog-muscles-base.jpg (front + back, 1200 x 800)
Output: assets/game/ironlog-bodymap-key.png  colour key, 8 exact colours
        assets/game/bodymap-paths.json       one SVG path per skill (viewBox 0 0 1200 800)

How it works
1. Grey pixels are muscle, white pixels are background or the lines between
   muscles. Connected grey areas = one muscle each (scipy label).
2. SEEDS below put each muscle into a skill: the muscle under a seed point
   gets that skill. NOT_SKILL seeds mark the head, hands and feet. Any other
   small piece without a seed joins the nearest skill.
3. Two cuts fix muscles the drawing joins to a hand or foot:
   arm pixels below the wrist line and leg pixels below the ankle line are
   not a skill.
4. The key PNG paints each skill one flat colour (assets/game/README.md).
   The JSON holds simplified outlines (OpenCV contours) for SVG use.

Usage: python3 tools/make_bodymap.py            (run from the repo root)
"""
import json
from pathlib import Path

import cv2
import numpy as np
from PIL import Image
from scipy import ndimage as nd

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "assets/exercises/ironlog-muscles-base.jpg"
OUT_KEY = ROOT / "assets/game/ironlog-bodymap-key.png"
OUT_PATHS = ROOT / "assets/game/bodymap-paths.json"

# Key colours (assets/game/README.md). Order = drawing order.
KEY = {
    "Arms": (0, 0, 255), "Shoulders": (255, 255, 0), "Chest": (0, 255, 0),
    "Back": (0, 255, 255), "Core": (255, 0, 255), "Legs": (255, 0, 0),
}
NON_SKILL = (128, 128, 128)
WHITE = (255, 255, 255)

# One point inside each muscle, in base-image pixels (x, y).
# Front figure: x < 600. Back figure: x > 600.
SEEDS = {
    "Back": [(364, 128),                                  # front: upper traps
             (293, 232), (435, 233),                      # front: lats at the armpit
             (809, 138), (857, 138), (777, 187), (887, 187), (815, 196), (850, 195),
             (792, 252), (872, 252), (820, 321), (845, 321)],
    "Shoulders": [(270, 169), (458, 169), (740, 170), (924, 170)],
    "Chest": [(327, 186), (402, 186)],
    "Arms": [(260, 238), (468, 238), (237, 233), (505, 286), (219, 298), (246, 295), (224, 340),
             (488, 316), (512, 342), (731, 235), (933, 235), (707, 242), (957, 242),
             (681, 308), (983, 308), (957, 314), (675, 367)],
    "Core": [(315, 252), (413, 252), (346, 239), (382, 239), (346, 270), (382, 270),
             (347, 304), (381, 304), (351, 350), (377, 350), (316, 309), (412, 309),
             (778, 323), (885, 309), (888, 341)],
    "Legs": [(296, 365), (432, 365), (341, 399), (386, 399), (319, 475), (412, 430),
             (291, 463), (437, 464), (393, 493), (414, 548), (292, 542), (435, 543),
             (288, 628), (439, 628), (315, 635), (405, 612), (420, 677),
             (795, 385), (870, 385), (755, 446), (909, 447), (815, 447), (848, 447),
             (776, 483), (888, 482), (798, 510), (866, 510), (780, 557), (883, 557),
             (760, 607), (791, 613), (873, 613), (903, 607), (765, 721), (899, 721)],
}
HEADS_FEET = [(360, 57), (829, 55), (275, 750), (445, 750)]
HANDS = [(190, 403), (536, 403), (1010, 393)]   # a hand piece may run up the forearm
WRIST_Y = 372   # arm pixels below this are hand
ANKLE_Y = 738   # leg pixels below this are foot


def label_muscles(rgb):
    g = rgb.mean(axis=2)
    grey = (g < 200) & (g > 60)
    grey = nd.binary_opening(grey, iterations=1)
    lab, _ = nd.label(grey)
    return grey, lab


def component_at(lab, x, y, r=15):
    """Label under (x, y), or the nearest one within r pixels."""
    if lab[y, x]:
        return lab[y, x]
    ys, xs = np.nonzero(lab[y - r:y + r + 1, x - r:x + r + 1])
    if len(ys) == 0:
        raise SystemExit(f"seed ({x}, {y}) is not on a muscle")
    i = np.argmin((ys - r) ** 2 + (xs - r) ** 2)
    return lab[y - r + ys[i], x - r + xs[i]]


def main():
    rgb = np.asarray(Image.open(BASE).convert("RGB")).astype(int)
    grey, lab = label_muscles(rgb)
    h, w = lab.shape
    yy = np.arange(h)[:, None].repeat(w, axis=1)

    masks = {}
    for skill, pts in SEEDS.items():
        masks[skill] = np.isin(lab, list({component_at(lab, x, y) for x, y in pts}))
    # Unseeded pieces (except head, hands, feet) join the nearest skill.
    names = list(SEEDS)
    owner = np.zeros(lab.shape, dtype=np.int8)          # 0 = none, i+1 = names[i]
    for i, n in enumerate(names):
        owner[masks[n]] = i + 1
    hands = np.isin(lab, list({component_at(lab, x, y) for x, y in HANDS}))
    not_skill = np.isin(lab, list({component_at(lab, x, y) for x, y in HEADS_FEET}))
    not_skill |= hands & (yy > WRIST_Y)                 # forearm part of a hand piece is Arms
    loose = grey & (owner == 0) & ~not_skill
    _, (iy, ix) = nd.distance_transform_edt(owner == 0, return_indices=True)
    owner[loose] = owner[iy[loose], ix[loose]]
    for i, n in enumerate(names):
        masks[n] = owner == i + 1
    for skill in names:
        m = masks[skill]
        if skill == "Arms":
            m &= yy <= WRIST_Y
        if skill == "Legs":
            m &= yy <= ANKLE_Y
        masks[skill] = m

    key = np.full((h, w, 3), WHITE, dtype=np.uint8)
    key[grey] = NON_SKILL
    for skill, m in masks.items():
        key[m] = KEY[skill]
    OUT_KEY.parent.mkdir(parents=True, exist_ok=True)
    Image.fromarray(key).save(OUT_KEY, optimize=True)

    # SVG outlines: one path per skill, plus the non-skill parts.
    def to_path(mask):
        cs, _ = cv2.findContours(mask.astype(np.uint8), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        parts = []
        for c in cs:
            if cv2.contourArea(c) < 40:
                continue
            c = cv2.approxPolyDP(c, 1.6, True).reshape(-1, 2)
            parts.append("M" + " ".join(f"{x} {y}" for x, y in c) + "Z")
        return "".join(parts)

    any_skill = np.zeros_like(grey)
    for m in masks.values():
        any_skill |= m
    paths = {skill: to_path(m) for skill, m in masks.items()}
    paths["none"] = to_path(grey & ~any_skill)
    OUT_PATHS.write_text(json.dumps({"viewBox": f"0 0 {w} {h}", "source": BASE.name,
                                     "paths": paths}, indent=1))
    sizes = {k: len(v) for k, v in paths.items()}
    print(f"key: {OUT_KEY.relative_to(ROOT)}  paths: {OUT_PATHS.relative_to(ROOT)}  chars: {sizes}")


if __name__ == "__main__":
    main()
