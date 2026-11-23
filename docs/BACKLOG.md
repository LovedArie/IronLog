# Backlog — IronLog

*The one queue. Read it at the start of every working session; it answers
"what next?". Everything else in `docs/` is a **record**. This file is the
opposite: an item exists in order to be **deleted**.*

---

## How this file works

Same rules as TickTimer's backlog (TickTimer = the owner's earlier Qt timer app, also on the VPS), because they worked:

- **One item per bullet.** When an item is picked up, its reasoning goes in
  `arc42.md` §8 (concept) or a new record in [`adr/`](adr/README.md) — never here.
- **Delete the line when it ships.** No "done" section; `git log -p
  docs/BACKLOG.md` is the archive.
- **Order is the ranking**, highest first. Rank by **risk**, then
  **coverage**, then **criticality** (Larman, Ch. 8 §8.3).
- **Ids are permanent and never reused.** `F7` is always the planner.
- **Cap: ~30 open items.** More than that is a wish list. Parked ideas go
  to *Later* (not counted) and come back if they still matter.
- **Size**: S ≈ one evening · M ≈ a weekend · L ≈ a week or more.
- 👤 = needs the owner (accounts, keys, the VPS, a phone in hand).

---

## Roadmap

| Milestone | Theme | Items | Why in this order |
|---|---|---|---|
| **v0.2** | Foundation | I4 F1 F2 F3 F4 | CI/CD is done (I1–I3). Settle the data format once, and answer the riskiest unknown (QML on Android + web) before building on it |
| **v0.3** | Coach | I7 F5 F6 F7 F11 F8 F9 F10 F23 | The heart of the app: *Train now*. New domain logic → highest design risk |
| **v0.4** | Game & body | F12 F13 F14 F18 F21 | Motivation loop + measuring the main goal (lean); depends on activity logging (F11) |
| **v0.5** | Online | I5 F15 F16 F17 I6 | Proven tech from TickTimer → lower risk, so later |
| **v0.6** | Progress+ | F19 F20 F22 H1 | Polish and retire the old desktop UI |

---

## Now — v0.2 Foundation

*In the order they will be done. Delete this section when v0.2 ships.*

| # | Id | Item |
|---|---|---|
| 1 | I4 | QML spike on the phone |
| 2 | F1 | Schema v2 + migration |
| 3 | F2 | Catalog v2 |
| 4 | F3 | Edit a set + undo |
| 5 | F4 | Backups + CSV export |

---

## Infrastructure

- **I4** Spike in `prototypes/`: one QML screen + one C++ view model, built
  as an Android APK and a WASM page, run on the owner's phone, using a
  `Theme.qml` token singleton (design system v1, ui-research.md). Decides ADR-012. Also proves **camera capture** works on the phone (for F21). *(M)* 👤
- **I5** 👤 VPS: `ironlog.<domain>` DNS record, Caddy block, `ironlog.service`,
  backup timer, `ufw` allowing 22/80/443 only. *(S)*
- **I7** Release workflow also builds the Android APK and the web app
  bundle (RELEASING "Coming later"). Needs I4. *(M)*
- **I6** CD deploy: copy server + web files over SSH, restart, then fail the
  job unless `GET /version` answers the new version. *(M)* 👤 (deploy key)

## Features — data and logging

- **F1** Schema v2 + migration v1→v2. One migration, not five. *(L)*
  - Every record: id, `updatedAt`, tombstone (deleted marker).
  - Sets: weight + unit as entered (kg/lb), RIR, set type, note; **timed
    sets** (plank, wall sit, balance holds) and distance.
  - New record kinds: cardio, mobility, body check-in, pause period,
    rest-token use, user reward, progress photo, **session review**,
    **game-mode change** (ADR-010, arc42 §8.1).
- **F2** Catalog v2: exercise ids and metadata (equipment, primary +
  secondary muscles, movement pattern, knee stress, stage, rep range);
  seed with the owner's home-kit exercises and stretch routines. *(M)*
- **F3** Edit a set (kg, reps, RIR, type, note) + undo/redo via `QUndoStack`. *(M)*
- **F4** Rotating daily backups (keep 7) + CSV export of all records. *(S)*
- **F11** Log cardio (treadmill time/speed/incline), badminton, and stretch
  routines, alongside strength sets. *(M)*

## Features — coach

- **F5** Profile + onboarding: goal program, units, equipment per place
  (home / gym / limited), injuries → starting stages, weekly availability,
  "Play it like a game?" checkbox. *(M)*
- **F6** Goal programs as data files: Badminton singles, Lean & toned (upper
  focus), Lean & toned (lower focus), General — each with weekly sessions and
  per-muscle weekly set targets. *(M)*
- **F7** Planner v1: `planner::buildSession()` with rules P1–P7 and a reason
  per item; property test "never exceeds the user's knee stage". *(L)*
- **F8** *Train now* flow (QML). *(M)*
  - Steps: time → situation → gym/home or nearby equipment → plan with XP preview.
  - *Sore* asks which body parts. "Same as last time" skips the steps.
  - A **Why?** sheet on every number.
  - After 3 cramming days in a week, Today offers "Start a pause?".
- **F9** Guided session: step through the plan, log each set inline (one tap
  "repeat last set", +/− steppers), hold timer for timed sets, rest timer. *(M)*
- **F10** **Session review** ("How was it?") at the end of a session. *(M)*
  - Tap chips for overall, energy and joints, **pre-picked from the
    session's data** (one tap to confirm), plus a note (voice through the
    phone keyboard's mic).
  - "Joints: pain" starts the pain check (P7). Answers feed the next plan.
  - Progression gates (P6) are shown as **unlocks**.
- **F23** Exercise pictures in the app (ADR-025). Needs F2. *(M)*
  - **Done:** 17 how-to pictures, 15 muscle diagrams and 17 clips in
    `assets/exercises/`, listed in `exercises.json`.
  - **To do:** bundle them as Qt resources; thumbnail on plan rows, card in
    the session, how-to sheet with 3 form cues; plank + dead-bug diagrams
    (ChatGPT's filter refused them).

## Features — game

- **F12** Character sheet: 8 skills, XP from sets/cardio/mobility, level
  curve, total level — all derived from the log (`game::computeSheet`); shown
  as a **body map** where muscles light up by level. **Game mode is a
  checkbox** (onboarding + settings): off hides XP, levels, streaks and tokens
  and pauses decay; turning it back on loses nothing (ADR-021). *(M)*
- **F13** Game rules v2 (arc42 §8.6). *(M)*
  - Decay starts after 14 days: −0.5 %/day, 3-day warning, never below 50 % of peak.
  - Comeback bonus ×2.
  - **Pause** with a reason (exams / sick / injured / travel), each with a cap.
  - "Sets to next level"; rules carry a version number.
- **F14** Weekly quest and streak. *(M)*
  - Quest done → **rest token** (bank max 2, protects the streak) + **your
    own reward** text. Quest card lists the week's planned sessions.
  - Level-up and quest-complete moments.
  - **Weekly streak**: rest days count, pauses keep it, back-dated logs
    restore a week. Training heat map.
  - *You* tab, game off: program, this week, calendar, "New exercises", Pause.

## Features — online

- **F15** `ironlog-server` (QHttpServer on 127.0.0.1). Live test against a real process. *(L)*
  - Register with an invite code.
  - `/prelogin` + login with a **derived login key** (the server never sees
    the password); device tokens.
  - `GET/PUT /log` with revisions; 409 = conflict, merge and retry.
  - Photo and envelope endpoints; `/version`.
- **F16** Sync client: dirty flag on `logChanged()`, push/pull, `sync::merge`
  by record id on 409; status shown in the UI; decide whether the log blob
  is encrypted too (arc42 §8.14). *(L)*
- **F17** Partner flow: invite (server admin command) → register → onboarding
  → her own plans and levels, fully separate data. *(S)*

## Features — progress

- **F18** Weekly body check-in (weight + waist) with a 4-week trend line —
  the "lean" signal. *(S)*
- **F19** More PR types (heaviest weight, best set volume, most reps at a
  weight) + rep-max table per exercise. *(M)*
- **F20** Plate and warm-up calculator using the user's real equipment
  (dumbbell jumps, plates, kg/lb). *(S)*
- **F21** **Progress photos** (Must). *(L)*
  - Front / side / back at the weekly check-in.
  - Side-by-side compare (week 1 vs now) with a pose-alignment overlay.
  - Stored on the device; synced with **envelope encryption** (ADR-020,
    arc42 §8.14). A **recovery code** is shown at the first photo (and in
    settings). Previews are encrypted too.
  - Never in exports or the repo.
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
- Windows installer (Inno Setup) — the zip is enough while the phone is the
  main client ([DEVELOPING §5](DEVELOPING.md#5-windows-installer-inno-setup))

## Skipped (decided 2026-11-23, after feature research)

*Not parked — deliberately not doing. Reopen only with new evidence.*

| Skipped | Why |
|---|---|
| Programmable progression rules | Planner rules cover it |
| wger exercise library import | Each entry has its own licence |
| Choice of 1RM formula | One labelled estimate is enough |
| openweight export | Not needed |
| Social feed | Not needed |
| AI-generated workouts | Rule-based planner instead (ADR-013) |
| Nutrition logging | Not needed |
