# 01 — Vision

> **What this is**: the 3-minute summary of the project: goals, users,
> scope, constraints. **Read it when** you start, or before deciding if a
> feature belongs. Version 2 (now v2.8) replaced the v0.1 "desktop logbook"
> vision on 2026-11-23. *(Method: Larman, Applying UML and Patterns, Ch. 4
> §4.3, Table 4.1.)*

---

## In one sentence

**IronLog is a phone-first training coach that plays like an MMO:** it plans
today's workout around your time, place, situation and **body's limits**,
levels up your body part by part, and rewards consistency — synced to your
own server.

*MMO = online role-playing game. IronLog borrows its levelling, not its
multiplayer.*

---

## Problem

| Pain | Today |
|---|---|
| **Starting is the hard part.** A student's day changes constantly (classes, cramming, gym or no gym). A fixed plan breaks on the first busy day. | Decide on the spot, often decide "skip". |
| **Motivation fades.** Progress in the body is slow and invisible week to week. | Nothing rewards showing up. |
| **Plans ignore your situation.** Generic apps don't know your equipment, your injuries, or your sport. | Hand-written plans in PDFs. |
| **Logbooks are passive.** They record what you did; they don't tell you what to do next. | IronLog v0.1. |
| **Coming back is risky.** After a long break, or with a joint that needs care, "too much too soon" is the most common way to get hurt. | Guess, or don't start. |
| **"Revisit later" never happens.** Plans say "jog once the walk is easy", "lunge after pain-free weeks", "re-test every 4 weeks", but nobody watches for it. | A note in a PDF. |

## Users

| Persona | Wants | Needs from the app |
|---|---|---|
| **Owner** — student, badminton singles player, returning to training, home kit + sometimes a gym, an old injury to train around | Lean, strong, badminton-ready body; fit training around school | Plans that respect time, equipment and knee rules; a game that keeps him going |
| **Partner** — wants a toned, good-looking body; no interest in configuring anything | Pick a goal, follow the plan | Simple onboarding, her own account and data; may prefer the game **off** |
| **Any user the owner's plan describes**, scaled: a joint that needs care, a sport to protect, a long break behind them, a low starting fitness | Train without making it worse | Plans inside their limits; a slow ramp back; their sport days respected; a clear, explained next step ([plan-to-features](research/plan-to-features.md)) |
| **Developer** (also the owner) | Portfolio project: C++, Qt, server, CI/CD | Clean architecture, tests, docs |

## Goals

| # | Goal | Measured by |
|---|---|---|
| G1 | **Starting a workout is effortless** | *Train now* → a ready plan in ≤ 4 answers and < 30 seconds |
| G2 | **Keep showing up** | Weekly quest completed ≥ 3 of 4 weeks; levels rising month over month |
| G3 | **Train safely toward the goal** | Plans never break any body limitation or equipment limit (property test); pain > 5/10 or a red flag always steps back; return ramp after breaks |
| G4 | **See progress** | Strength, stamina, mobility and body check-ins all charted |
| G5 | **Own your data** | Local-first, synced to the owner's VPS, exportable |
| G6 | **Know the next step** | Every "ready for…" offer and every gate shows its reason; each goal has a test, repeated every 4 weeks |

**Goal horizons.** Each user goal is written as short (about 8 weeks),
medium and long term, and each gets a goal test. The owner's, as an
example: last a whole badminton game → a strong, lean, flexible body → an
explosive singles player. Explosive (jump) training waits for a readiness
gate (phase 2).

---

## Feature modules

### Coach

| Feature | What it does | Backlog |
|---|---|---|
| **Goal programs** | Badminton singles · Lean & toned (upper) · Lean & toned (lower) · General. Weekly sessions and set targets, as data files | F6 |
| **Train now** | Time → today's situation → gym, home or what's nearby → a session plan, each item with its reason | F8 |
| **Guided session** | Follow the plan, log as you go, rest and hold timers | F9 |
| **Session review** | "How was it?" at the end: answers pre-picked from the session, plus a voice-friendly note | F10 |
| **Body limitations** | Pick the joint that needs care (knee, shoulder, back…), its side and stage. Every plan stays inside it. Pain is rated 0–10, during and the next morning, so the plan steps back when needed. Red flags → see a professional | F25 |
| **Exercise pictures** | How-to picture (the IronLog character, start → end), muscle diagram (dark blue = main, light blue = helpers), and a short clip | F23 |
| **Daily stretch** | A guided morning routine (12 stretches, ~12 min, timer per side), add-ons after training or in the evening, and a weekly reach test | F24 |
| **Ready for…** | The app watches the log and offers the next step (a harder exercise, intervals, a new phase) when time **and** criteria are met. Goal tests every 4 weeks | F26 |
| **Cardio program** | A talk-test baseline, then a weekly table; one change at a time; intervals after a base | F27 |
| **Sport and prehab** | Sport days respected (no hard legs the day before), a sport warm-up before play, short knee-care and stability blocks | F28 |

### Game *(optional — a checkbox)*

| Feature | What it does |
|---|---|
| **Character sheet** | A **body map** that lights up by level, 8 skills: Arms, Shoulders, Chest, Back, Core, Legs, Stamina, Mobility |
| **XP** | From every set, cardio minute and stretch minute; "sets to next level" always shown |
| **Gentle decay** | Warned first, after 2 weeks off a skill; stopped by a **Pause** (exams, sick, injured, travel); a **Comeback bonus** doubles XP while you climb back |
| **Weekly quest** | → **rest token** (protects your streak) + **your own reward**, written by you |
| **Unlocks** | Harder exercises open when their gates are passed |
| **Streak** | Weekly, with a heat map; rest days count, a pause never breaks it |

### Other modules

| Module | What it does |
|---|---|
| **Log** | Strength sets (weight, reps, RIR, set type, note), **timed holds** (plank, wall sit, balance), cardio (treadmill time/speed/incline), badminton, stretching, pain reports. One-tap "repeat last set". Edit + undo |
| **Progress** | PRs, estimated-1RM charts, weekly sets per muscle, weekly body weight + waist, reach test, goal tests, **progress photos** (private, encrypted) with side-by-side compare |
| **Online** | Accounts on the owner's VPS (invite-only), sync across devices, works offline |
| **Platforms** | Android app, web app (for iPhone), Windows desktop — one codebase |

### The *Train now* flow (the heart of the app)

```
Train now
 └─ How much time?            15 · 20 · 30 · 45 · 60 · 90 min
     └─ How's today?          Normal day · Cramming · Sore / tired · Before my sport
         ├─ Normal   → Gym or home?        → uses that place's equipment
         ├─ Cramming → What's nearby?      → quick equipment checklist
         ├─ Sore     → recovery session    (mobility + easy cardio)
         └─ Sport    → sport warm-up only  (moving stretches, no holds)
             └─ Safety filters: body limitations + equipment (never overridden)
                 └─ Session plan: warm-up → main block → cool-down stretch,
                    each exercise with a one-line "why"
```

---

## What success looks like

- The owner trains **≥ 3 times a week** through a whole semester, exam weeks included.
- The partner can create an account and start a plan **without help**.
- Every number the app shows (XP, levels, PRs, plan choices) can explain
  **where it came from**.
- A phone lost or replaced costs **zero** logged sets.
- No generated plan ever breaks a user's body limitation (tested), and a
  red flag always stops the exercise.

## Out of scope (for v1)

- **Food logging and calorie counting.** "Lean" is tracked with weekly body
  weight + waist + progress photos instead. The app never suggests food as a
  reward and never uses the word "cheat" (game rules v2, research round 2).
- Social feeds, leaderboards with strangers, public sharing.
- Wearables / heart-rate devices (cardio intensity uses the talk test).
- **Diagnosing or treating injuries**, or replacing a physio. The app uses
  self-reported limitations and points to a professional on red flags.
- **Explosive (jump) training** before a readiness gate is passed (phase 2,
  after v1).
- App-store publishing (APK + web app + zip, like TickTimer (the owner's earlier Qt timer app, with its own sync server on the same VPS)).
- An AI coach. Planned as a **later** layer on top of the rule-based planner
  (and a good fit for the developer's AI career goal).

## Constraints

- **Tech:** C++17 + Qt 6 (developed on 6.11), CMake. One codebase for
  Android, WebAssembly and Windows. Server in C++ on the existing Hetzner VPS
  behind Caddy, run by systemd (same setup as TickTimer).
- **People and time:** one student developer; portfolio pace.
- **Safety:** the app gives training advice. Plans follow written rules
  (body limitations, pain monitoring 0–10, red flags, return ramp, gradual
  progression, exam-week minimum), link the PAR-Q+ readiness questionnaire
  at onboarding, and say they are not medical advice.
- **Privacy:** the repo is public. Personal health details stay in the
  user's own data, never in the repo.

## Key assumptions

- Users train with **free weights, bodyweight, bands, a treadmill** at home,
  and sometimes a commercial gym.
- Weights are logged in **kg or lb** (per user); the owner's dumbbells are in lb.
- Most users have no heart-rate strap: "can talk, can't sing" (the talk
  test) sets easy-cardio pace.
- Several activities can happen on one day (morning stretch + strength +
  badminton). *(v0.1 assumed one session per day; v2 drops that.)*

---

## Revision history

| Version | Date | Description | Author |
|---------|------|-------------|--------|
| Inception draft | 2026-11-23 | v0.1: desktop logbook with PRs, weekly volume, chart | Claude (pairing) |
| v2 | 2026-11-23 | Coach + game + online, phone-first, after feature research and owner interview | Claude (pairing) with owner |
| v2.1 | 2026-11-23 | Research triage: progress photos (Must), timed holds, fast phone logging, body map, streak, stall/deload | Claude (pairing) with owner |
| v2.2 | 2026-11-23 | Session review ("How was it?") with pre-picked choices; voice notes via keyboard dictation; voice set logging parked as AI track | Claude (pairing) with owner |
| v2.3 | 2026-11-23 | Game mode is optional: a per-user checkbox; off hides the game and pauses decay, nothing is lost | Claude (pairing) with owner |
| v2.4 | 2026-11-23 | Game rules v2 from research round 2: gentler decay, Pause with a reason, weekly streak, rest token + user-written reward (replaces cheat-meal token) | Claude (pairing) with owner |
| v2.5 | 2026-11-23 | Exercise pictures: how-to picture + muscle diagram per exercise, made with ChatGPT (ADR-025) | Claude (pairing) with owner |
| v2.6 | 2026-11-23 | Daily stretch (F24): guided morning routine + add-ons + weekly reach test, from the owner's fitness plan | Claude (pairing) with owner |
| v2.7 | 2026-11-23 | Plan scaled to every user: body limitations (any joint), pain monitoring, gates and triggers, goal tests, cardio program, sport profile + prehab (plan-to-features, ADR-026/027) | Claude (pairing) with owner |
| v2.8 | 2026-11-23 | Full read of the owner's plan and Decision Report: new problems (coming back, "revisit later"), scaled persona, G3 sharpened, G6 + goal horizons, module tables split, "Before my sport" in Train now, out-of-scope (diagnosis, early jump training), safety constraint | Claude (pairing) with owner |
