# Credits and provenance — exercise pictures

| Field | Value |
|---|---|
| Made with | ChatGPT image generation (OpenAI), the owner's account |
| Date | 2026-11-23 |
| Made by | Claude, driving the owner's Chrome at the owner's request |
| Rights | OpenAI's Terms of Use assign rights in outputs to the user. Re-check if the terms change |
| Checked | Every image checked by eye for form errors, extra limbs/fingers and wrong muscles before saving |
| Format | Downscaled in the browser to 1200 × 800 JPEG (quality 0.88) |

## How pictures — character prompt (first message, then "same character")

> Style: Japanese anime illustration, clean confident line art, cel shading,
> bright friendly colors, plain off-white background, no text, no logos.
> Character (keep him exactly the same in every future image): an original
> young athletic man, lean build, short spiky dark-navy hair, white headband,
> red t-shirt, navy shorts, white sneakers.

Each exercise then: *"Next exercise, same character, same style, same
layout (two side views facing right, arrow between). EXERCISE: LEFT = start
pose … RIGHT = end pose … Anatomy must be correct. No text. Landscape 3:2."*
The pose wording per exercise is in `exercises.json` order; key form points
asked for: shins vertical / knees over feet (squats, lunge), flat back
(rows), straight body line (push-up, plank, ring row), lower back on the mat
(dead bug), knees not locked (leg press).

## Muscle diagrams — base prompt

> A muscle diagram for a fitness app, in the clean flat style fitness apps
> use. A stylized athletic human body silhouette, FRONT view left and BACK
> view right, divided into the major muscle groups with anatomically correct
> shapes, separated by thin white lines. A simple diagram, not a realistic
> person. ALL muscle groups light grey. Plain white background. No text.

Each exercise then: *"Start again from the GREY diagram, identical. Color
ONLY: MAIN in bright red = … HELPERS in orange = … All other muscle groups
stay light grey. No text."*

**Recoloured afterwards:** red → `#2F5BD0` (dark blue, main) and orange →
`#8FAAF0` (light blue, helpers), by hue, keeping the white lines, so the
diagrams follow the design system's colour-blind rule (one hue, dark vs
light). New diagrams: generate in red/orange as above, then run the same
recolour (hue < 15° or > 340° → main; 15–50° → helper).

An earlier realistic skin-off anatomy chart was dropped: ChatGPT's content
filter refused most edits of it. Three diagrams (cat-cow, hip flexor,
walk) were made in a second chat from a pasted copy of the same diagram.
Reverse lunge, step-up and leg press share one diagram (same muscles).
