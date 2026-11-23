# 02 — Use-Case Model

> **What this is**: What users do with the system, as scenarios. Functional
> requirements live here. (Larman, Ch. 5 §5.5, Ch. 6.) During inception,
> name most use cases but write only 10–20% in detail (Ch. 4 §4.3).

---

## Actors

- **Lifter** — the person training (the owner, the partner). Each has an account.
- **Server admin** — the owner, managing invite codes and the VPS.
- **ironlog-server** — supporting actor: stores and syncs each lifter's log.
- **File system** — supporting actor: stores `ironlog.json` on each device.

---

## Use cases — by name

| ID | Name | Primary actor | Detailed? | Status | Backlog |
|----|------|---------------|-----------|--------|---------|
| UC-01 | Log a Set | Lifter | ⭐ | Done (v0.1) | — |
| UC-02 | Delete a Set | Lifter | | Done | — |
| UC-03 | Review a Session | Lifter | | Done | — |
| UC-04 | Review Weekly Volume | Lifter | ⭐ | Done | — |
| UC-05 | Track Exercise Progress | Lifter | | Done | — |
| UC-06 | Add Custom Exercise | Lifter | | Done | — |
| UC-07 | Load Sample Data | Lifter | | Done | — |
| UC-08 | Edit a Set | Lifter | | Planned | F3 |
| UC-09 | Follow a Goal Program | Lifter | | Planned | F6 |
| UC-10 | Export / Back Up Log | Lifter | | Planned | F4 |
| UC-11 | Train Now (plan today's session) | Lifter | ⭐ | Planned | F7, F8 |
| UC-12 | Follow a Guided Session | Lifter | | Planned | F9 |
| UC-13 | Log Cardio, Badminton or Stretching | Lifter | | Planned | F11 |
| UC-14 | Set Up Profile (onboarding) | Lifter | | Planned | F5 |
| UC-15 | View Character Sheet | Lifter | | Planned | F12 |
| UC-16 | Start / End Exam Mode | Lifter | | Planned | F13 |
| UC-17 | Redeem a Cheat-Meal Token | Lifter | | Planned | F14 |
| UC-18 | Report Pain on an Exercise | Lifter | | Planned | F10 |
| UC-19 | Weekly Body Check-In | Lifter | | Planned | F18 |
| UC-20 | Register / Log In | Lifter | | Planned | F15 |
| UC-21 | Sync Devices | Lifter (+ ironlog-server) | | Planned | F16 |
| UC-22 | Invite a New Lifter | Server admin | | Planned | F17 |

----|------|---------------|-----------|--------|
| UC-01 | Log a Set | Lifter | ⭐ | Done (iteration 1) |
| UC-02 | Delete a Set | Lifter | | Done |
| UC-03 | Review a Session | Lifter | | Done |
| UC-04 | Review Weekly Volume | Lifter | ⭐ | Done |
| UC-05 | Track Exercise Progress | Lifter | | Done |
| UC-06 | Add Custom Exercise | Lifter | | Done |
| UC-07 | Load Sample Data | Lifter | | Done |
| UC-08 | Edit a Set | Lifter | | Future |
| UC-09 | Follow a Program Template | Lifter | | Future |
| UC-10 | Export / Back Up Log | Lifter | | Future |

---

## UC-01: Log a Set (fully dressed)

**Primary actor**: Lifter

**Stakeholders and interests**:
- *Lifter*: wants to log in seconds between sets, and to know right away
  if the set was a personal record.
- *Lifter (future self)*: wants every logged set kept safely and correctly.

**Preconditions**: The app is open. At least one exercise exists.

**Success guarantee**: The set is stored on the chosen day, saved to disk,
and every view (table, weekly volume, chart, calendar) shows it.

**Main success scenario**:
1. Lifter selects a day on the calendar (default: today).
2. System shows that day's sets and pre-selects the day's last exercise.
3. Lifter picks an exercise.
4. System pre-fills weight and reps from the most recent set of that exercise.
5. Lifter adjusts weight and reps and clicks *Add set* (or Ctrl+Enter).
6. System validates the numbers and records the set.
7. System saves the log to disk.
8. System refreshes all views and tells the Lifter if the set is a new PR.

Lifter repeats steps 3–5 for each set.

**Extensions**:
- 1a. Lifter picks a future day: the calendar doesn't allow it.
- 3a. Exercise isn't in the list: Lifter adds it first (UC-06).
- 6a. Weight outside 0–1000 kg or reps outside 1–100: System refuses and
  says the numbers are out of range. Nothing changes.
- 7a. Save fails (disk full, no permission): System keeps the set on screen,
  shows "NOT SAVED" in the status bar, and warns once per session.

**Special requirements**: Step 5→8 must feel instant (< 100 ms).

**Frequency**: 15–30 times per session, 3–6 sessions a week.

---

## UC-04: Review Weekly Volume (fully dressed)

**Primary actor**: Lifter

**Stakeholders and interests**:
- *Lifter*: wants to see which muscles are under- or over-trained this week.

**Preconditions**: None (an empty week shows zeros).

**Success guarantee**: Nothing changes; the Lifter sees a correct summary.

**Main success scenario**:
1. Lifter selects any day and opens the *Weekly volume* tab.
2. System finds the Monday–Sunday week containing that day.
3. System counts every set in that week toward its exercise's primary muscle.
4. System shows one row per muscle: sets, target (10–20), and status
   *Below / On / Above target*, colour-coded.

**Extensions**:
- 3a. A set's exercise isn't in the library (e.g. hand-edited file):
  System skips it rather than guessing a muscle.

**Frequency**: once or twice a week, usually when planning the next session.

---

## UC-11: Train Now (fully dressed)

**Primary actor**: Lifter

**Stakeholders and interests**:
- *Lifter*: wants a workout that fits **today** (time, place, energy) in
  under 30 seconds, without thinking.
- *Lifter (future self)*: wants steady progress without injury — the plan
  must respect constraints (e.g. knee stage) and progression gates.
- *Developer*: wants every plan reproducible and explainable (pure function).

**Preconditions**: Lifter has a profile (UC-14): goal program, equipment per
place, constraints.

**Success guarantee**: A session plan is shown that fits the time budget,
uses only available equipment, never exceeds the lifter's constraint
stages, and gives a one-line reason per exercise. Nothing is logged yet.

**Main success scenario**:
1. Lifter taps *Train now* on the Today screen.
2. System asks how much time is available (15 / 20 / 30 / 45 / 60 / 90 min).
3. Lifter picks a time.
4. System asks how today is: *Normal*, *Cramming*, or *Sore / tired*.
5. Lifter picks *Normal*.
6. System asks: gym or home?
7. Lifter picks one; System loads that place's equipment set.
8. System builds the plan (planner rules P1–P5) and shows warm-up, main
   block and stretch, each item with its reason, plus an XP preview.
9. Lifter taps *Start*; System opens the guided session (UC-12).

**Extensions**:
- 5a. *Cramming*:
    1. System asks what's nearby, pre-ticking the last "limited" equipment set.
    2. Lifter confirms or edits the checklist.
    3. System builds a short, compound-first plan with no PR attempts and
       counts it toward the exam-week minimum. Continue at step 8.
- 5b. *Sore / tired*: System builds a recovery session (mobility, easy zone-2
  cardio, light core). Continue at step 8.
- 8a. No exercise fits (e.g. 15 min, no equipment, knee stage 1): System
  offers a bodyweight + mobility session and says why.
- 8b. Lifter dislikes an exercise: Lifter taps *Swap*; System offers the
  next-best candidate with the same movement pattern and constraints.
- 9a. Lifter leaves without starting: nothing is logged; the plan is kept
  for the rest of the day.

**Special requirements**: works offline; plan appears < 1 s after step 7;
the "not medical advice; stop on sharp pain" note is always visible.

**Frequency**: once per training day (3–6 times a week).

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 4 §4.3, Ch. 5–6.
