# ADR-023: Four-tab navigation (Today · Log · Progress · You)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

The feature list grew (coach, game, photos, check-ins, pauses). The owner's
concern: features are useless if the UI has no clear place for them.
UI research (`docs/research/ui-research.md`) mapped every feature to a
screen and found 7 features with no home and 3 screens at risk of crowding.

## Decision

- **Tab bar:** Today · Log · Progress · You (a left rail on wide screens).
- *Train now* is the primary **button** on Today, not a tab; "Same as last
  time" skips the questions.
- The guided session is full-screen and can shrink to a **mini bar** above
  the tabs.
- *You* is never hidden: game on = character sheet; game off = plain
  progress. Settings is a gear on *You*.
- The 7 homeless features get these homes:
  1. Pause with the game off → "Lighter weeks" row on *You*
  2. Cramming vs an exams pause → after 3 cramming days in a week, Today offers "Start a pause?"
  3. Unlocks with the game off → "NEW" tag on the plan + "New exercises" card
  4. "Every number explains itself" → one reusable **Why?** bottom sheet
  5. This week's planned sessions → listed inside the quest card
  6. Photo recovery code → prompt at the first photo (not only in settings)
  7. Inviting a lifter → a server admin command
- Crowding limits: Today shows at most 3 cards; rare actions behind "⋯";
  celebrations only after "How was it?", one screen, tap to close.

## Consequences

- Every Vision/Backlog feature has a named home (feature → screen map).
- One primary action per screen, in the bottom 40 % (thumb reach).
- A clickable HTML prototype (`prototypes/ui-prototype/`) tests the layout
  before any QML is written.

## Alternatives considered

- 5 tabs with *Train now* as a tab — Apple: tabs are for navigation, not actions.
- A drawer menu — hides features; worse discoverability.
- Hiding *You* when the game is off — Apple: never hide tabs.

## Sources

- docs/research/ui-research.md (navigation, feature → screen map, wireframes)
