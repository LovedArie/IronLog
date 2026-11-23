# 01 — Vision

> **What this is**: The 3-minute summary of the project — goals, users,
> scope, constraints. (Larman, *Applying UML and Patterns*, Ch. 4 §4.3,
> Table 4.1; written first per Ch. 7.) Version 2 replaces the v0.1 "desktop
> logbook" vision after the 2026-11-23 planning session.

---

## In one sentence

**IronLog is a phone-first training coach that plays like an MMO:** it plans
today's workout around your time, place and situation, levels up your body
part by part, and rewards consistency — synced to your own server.

---

## Problem

| Pain | Today |
|---|---|
| **Starting is the hard part.** A student's day changes constantly (classes, cramming, gym or no gym). A fixed plan breaks on the first busy day. | Decide on the spot, often decide "skip". |
| **Motivation fades.** Progress in the body is slow and invisible week to week. | Nothing rewards showing up. |
| **Plans ignore your situation.** Generic apps don't know your equipment, your injuries, or your sport. | Hand-written plans in PDFs. |
| **Logbooks are passive.** They record what you did; they don't tell you what to do next. | IronLog v0.1. |

## Users

| Persona | Wants | Needs from the app |
|---|---|---|
| **Owner** — student, badminton singles player, returning to training, home kit + sometimes a gym, an old injury to train around | Lean, strong, badminton-ready body; fit training around school | Plans that respect time, equipment and knee rules; a game that keeps him going |
| **Partner** — wants a toned, good-looking body; no interest in configuring anything | Pick a goal, follow the plan | Simple onboarding, her own account and data; may prefer the game **off** |
| **Developer** (also the owner) | Portfolio project: C++, Qt, server, CI/CD | Clean architecture, tests, docs |

## Goals

| # | Goal | Measured by |
|---|---|---|
| G1 | **Starting a workout is effortless** | *Train now* → a ready plan in ≤ 4 answers and < 30 seconds |
| G2 | **Keep showing up** | Weekly quest completed ≥ 3 of 4 weeks; levels rising month over month |
| G3 | **Train safely toward the goal** | Generated plans never break a constraint (tested); no "too much too soon" jumps |
| G4 | **See progress** | Strength, stamina, mobility and body check-ins all charted |
| G5 | **Own your data** | Local-first, synced to the owner's VPS, exportable |

---

## Feature modules

| Module | What it does |
|---|---|
| **Coach** | **Goal programs** (Badminton singles · Lean & toned, upper focus · Lean & toned, lower focus · General fitness). **Train now**: asks time → today's situation → gym or home / what's nearby, then builds a session. **Guided session**: follow the plan, log as you go, rest timer. **"How was it?"** check-in at the end: quick-tap answers pre-picked from the session (overall, energy, joints) + a voice-friendly note. Progression and pain checks. |
| **Game** *(optional — a checkbox)* | **Character sheet** (a **body map** that lights up by level) with 8 skills: Arms, Shoulders, Chest, Back, Core, Legs, Stamina, Mobility. XP from every set, minute of cardio and stretch. **Decay** after a grace period, paused by **exam mode**; **muscle memory** doubles XP while you climb back. **Weekly quest** → **cheat-meal token**. **Unlocks**: harder exercises open when progression gates are passed. **Streak + heat map** (exam weeks don't break it). |
| **Log** | Strength sets (weight, reps, RIR, set type, note), **timed holds** (plank, wall sit, balance), cardio (treadmill time/speed/incline), badminton, stretching. One-tap "repeat last set". Edit + undo. |
| **Progress** | PRs, estimated-1RM charts, weekly sets per muscle, weekly body weight + waist trend, **progress photos** (private, encrypted) with side-by-side compare, stall → deload hint. |
| **Online** | Accounts on the owner's VPS (invite-only), sync across devices, works offline. |
| **Platforms** | Android app, web app (for iPhone), Windows desktop — one codebase. |

### The *Train now* flow (the heart of the app)

```
Train now
 └─ How much time?            15 · 20 · 30 · 45 · 60 · 90 min
     └─ How's today?          Normal day · Cramming · Sore / tired
         ├─ Normal  → Gym or home?        → uses that place's equipment
         ├─ Cramming → What's nearby?     → quick equipment checklist
         └─ Sore     → recovery session   (mobility + easy cardio)
             └─ Session plan: warm-up → main block → stretch,
                each exercise with a one-line "why"
```

---

## What success looks like

- The owner trains **≥ 3 times a week** through a whole semester, exam weeks included.
- The partner can create an account and start a plan **without help**.
- Every number the app shows (XP, levels, PRs, plan choices) can explain
  **where it came from**.
- A phone lost or replaced costs **zero** logged sets.

## Out of scope (for v1)

- **Food logging and calorie counting.** "Lean" is tracked with weekly body
  weight + waist + progress photos instead. Cheat-meal tokens reward consistency; they are
  not a diet tool.
- Social feeds, leaderboards with strangers, public sharing.
- Wearables / heart-rate devices.
- App-store publishing (APK + web app + zip, like TickTimer).
- An AI coach. Planned as a **later** layer on top of the rule-based planner
  (and a good fit for the developer's AI career goal).

## Constraints

- **Tech:** C++17 + Qt 6 (developed on 6.11), CMake. One codebase for
  Android, WebAssembly and Windows. Server in C++ on the existing Hetzner VPS
  behind Caddy, run by systemd (same setup as TickTimer).
- **People and time:** one student developer; portfolio pace.
- **Safety:** the app gives training advice. Plans follow written rules
  (pain rule, gradual progression, exam-week minimum) and say they are not
  medical advice.
- **Privacy:** the repo is public. Personal health details stay in the
  user's own data, never in the repo.

## Key assumptions

- Users train with **free weights, bodyweight, bands, a treadmill** at home,
  and sometimes a commercial gym.
- Weights are logged in **kg or lb** (per user); the owner's dumbbells are in lb.
- Several activities can happen on one day (morning stretch + strength +
  badminton). *(v0.1 assumed one session per day; v2 drops that.)*

---

## Revision history

| Version | Date | Description | Author |
|---------|------|-------------|--------|
| Inception draft | 2026-11-23 | v0.1: desktop logbook with PRs, weekly volume, chart | Claude (pairing) |
| v2 | 2026-11-23 | Coach + game + online, phone-first, after feature research and owner interview | Claude (pairing) with owner |
| v2.3 | 2026-11-23 | Game mode is optional: a per-user checkbox; off hides the game and pauses decay, nothing is lost | Claude (pairing) with owner |
| v2.2 | 2026-11-23 | "How was it?" session check-in with pre-picked choices; voice notes via keyboard dictation; voice set logging parked as AI track | Claude (pairing) with owner |
| v2.1 | 2026-11-23 | Research triage: progress photos (Must), timed holds, fast phone logging, body map, streak, stall/deload | Claude (pairing) with owner |
