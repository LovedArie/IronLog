# Backlog — IronLog

*The one queue. Read it at the start of every working session; it answers
"what next?". Everything else in `docs/` is a **record**. This file is the
opposite: an item exists in order to be **deleted**.*

---

## How this file works

Same rules as TickTimer's backlog, because they worked:

- **One line per item.** Reasoning goes in `arc42.md` (§8 concept or §9 ADR)
  when the item is picked up, never here.
- **Delete the line when it ships.** No "done" section; `git log -p
  docs/BACKLOG.md` is the archive.
- **Order is the ranking**, highest first. Rank by **risk**, then
  **coverage**, then **criticality** (Larman, Ch. 8 §8.3).
- **Ids are permanent and never reused.** `F7` is always the planner.
- **Cap: ~25 open items.** More than that is a wish list. Parked ideas go
  to *Later* (not counted) and come back if they still matter.
- **Size**: S ≈ one evening · M ≈ a weekend · L ≈ a week or more.
- 👤 = needs the owner (accounts, keys, the VPS, a phone in hand).

---

## Roadmap

| Milestone | Theme | Items | Why in this order |
|---|---|---|---|
| **v0.2** | Foundation | I4 F1 F2 F3 F4 | Protect the code (CI), settle the data format once, and answer the riskiest unknown (QML on Android + web) before building on it |
| **v0.3** | Coach | F5 F6 F7 F11 F8 F9 F10 | The heart of the app: *Train now*. New domain logic → highest design risk |
| **v0.4** | Game & body | F12 F13 F14 F18 F21 | Motivation loop + measuring the main goal (lean); depends on activity logging (F11) |
| **v0.5** | Online | I5 F15 F16 F17 I6 | Proven tech from TickTimer → lower risk, so later |
| **v0.6** | Progress+ | F19 F20 F22 H1 | Polish and retire the old desktop UI |

---

## Now — v0.2 Foundation

*Ids only, in the order they will be done. Delete this section when v0.2 ships.*

- **I4** · **F1** · **F2** · **F3** · **F4**

---

## Infrastructure

- **I4** Spike in `prototypes/`: one QML screen + one C++ view model, built
  as an Android APK and a WASM page, run on the owner's phone. Decides ADR-012. Also proves **camera capture** works on the phone (for F21). *(M)* 👤
- **I5** 👤 VPS: `ironlog.<domain>` DNS record, Caddy block, `ironlog.service`,
  backup timer, `ufw` allowing 22/80/443 only. *(S)*
- **I6** CD deploy: copy server + web files over SSH, restart, then fail the
  job unless `GET /version` answers the new version. *(M)* 👤 (deploy key)

## Features — data and logging

- **F1** Schema v2 + migration v1→v2: record ids, `updatedAt`, tombstones,
  weight + unit as entered (kg/lb), RIR, set type, note; new record kinds
  (cardio, mobility, body check-in, exam period, token spend, progress photo);
  **timed sets** (plank, wall sit, balance holds) and distance. One migration,
  not five. *(L)*
- **F2** Catalog v2: exercise ids and metadata (equipment, primary +
  secondary muscles, movement pattern, knee stress, stage, rep range);
  seed with the owner's home-kit exercises and stretch routines. *(M)*
- **F3** Edit a set (kg, reps, RIR, type, note) + undo/redo via `QUndoStack`. *(M)*
- **F4** Rotating daily backups (keep 7) + CSV export of all records. *(S)*
- **F11** Log cardio (treadmill time/speed/incline), badminton, and stretch
  routines, alongside strength sets. *(M)*

## Features — coach

- **F5** Profile + onboarding: goal program, units, equipment per place
  (home / gym / limited), injuries → starting stages, weekly availability. *(M)*
- **F6** Goal programs as data files: Badminton singles, Lean & toned (upper
  focus), Lean & toned (lower focus), General — each with weekly sessions and
  per-muscle weekly set targets. *(M)*
- **F7** Planner v1: `planner::buildSession()` with rules P1–P5 and a reason
  per item; property test "never exceeds the user's knee stage". *(L)*
- **F8** *Train now* flow (QML): time → situation → gym/home or nearby
  equipment → plan with XP preview; *Sore* asks which body parts. *(M)*
- **F9** Guided session: step through the plan, log each set inline (one tap
  "repeat last set", +/− steppers), hold timer for timed sets, rest timer. *(M)*
- **F10** End-of-session **"How was it?"** check-in: quick-tap chips for overall,
  energy and joints, **pre-picked from the session's data** (one tap to confirm),
  plus a note field (voice via the phone keyboard's mic). "Joints: pain" starts
  the pain check (P7); answers feed the next plan. Progression gates (P6) shown
  as **unlocks**. *(M)*

## Features — game

- **F12** Character sheet: 8 skills, XP from sets/cardio/mobility, level
  curve, total level — all derived from the log (`game::computeSheet`); shown
  as a **body map** where muscles light up by level. *(M)*
- **F13** Decay after 10 days, 50 % peak floor, muscle memory ×2, exam mode
  (≤ 21 days, pauses decay, lowers the quest). *(M)*
- **F14** Weekly quest → cheat-meal token (bank max 2), spend a token,
  level-up and quest-complete moments; **streak + training heat map**
  (exam weeks don't break a streak). *(M)*

## Features — online

- **F15** `ironlog-server` (QHttpServer, 127.0.0.1): register with invite,
  login, device tokens, `GET/PUT /log` with revisions + 409, `/version`.
  Live test against a real process. *(L)*
- **F16** Sync client: dirty flag on `logChanged()`, push/pull, `sync::merge`
  by record id on 409; status shown in the UI. *(L)*
- **F17** Partner flow: invite → register → onboarding → her own plans and
  levels, fully separate data. *(S)*

## Features — progress

- **F18** Weekly body check-in (weight + waist) with a 4-week trend line —
  the "lean" signal. *(S)*
- **F19** More PR types (heaviest weight, best set volume, most reps at a
  weight) + rep-max table per exercise. *(M)*
- **F20** Plate and warm-up calculator using the user's real equipment
  (dumbbell jumps, plates, kg/lb). *(S)*
- **F21** **Progress photos** (Must): front/side/back at the weekly check-in,
  side-by-side compare (week 1 vs now) with a pose-alignment overlay;
  stored on the device, synced **encrypted end-to-end** (ADR-020), never in
  exports or the repo. *(L)*
- **F22** Stall detection: e1RM falling over several sessions or sets above
  the target → planner suggests a deload week. *(M)*

## Housekeeping

- **H1** Retire the Qt Widgets UI once the QML UI covers everything it does. *(S)*

---

## Later (parked — not counted in the cap)

- AI coach: an LLM that explains or tunes plans **inside** the planner's rules
- **Voice set logging** (AI track): say "eighty for eight" or "same again" →
  a logged set. Speech-to-text with whisper.cpp (on the VPS or on the phone) +
  a rule-based parser in `core/`
- Party view: see your partner's levels (opt-in)
- Achievements beyond unlocks (streaks, milestones)
- Import from Strong / Hevy CSV
- Supersets / exercise grouping
- Weekly / monthly report (PDF)
- Template editor (build your own program in the app)
- Reminders / notifications
- Automated off-box backups (VPS → elsewhere)

## Skipped (decided 2026-11-23, after feature research)

*Not parked — deliberately not doing. Reopen only with new evidence.*
Programmable progression rules (planner rules cover it) · wger exercise
library import (per-entry licences) · choice of 1RM formula (one labelled
estimate is enough) · openweight export · social feed · AI-generated workouts
(rule-based planner instead, ADR-013) · nutrition logging.
