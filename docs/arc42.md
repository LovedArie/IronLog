# IronLog — Architecture (arc42)

> **What this is**: the single architecture document for IronLog, in the
> [arc42](https://arc42.org) template's 12 sections. It replaces
> `09_DesignNotes.md` (v0.1), now archived at
> [archive/design-notes-v0.1.md](archive/design-notes-v0.1.md) for the
> detailed v0.1 reasoning. It describes the **target** architecture (v1)
> and marks what **exists today** (v0.1.0) with ✅.
>
> **How to read it**: sections 1, 3, 4 and 5 give the big picture in ten
> minutes. Sections 8 and 9 hold the reasoning. Diagrams live in
> `diagrams/` (PlantUML) with PNGs in `diagrams/rendered/`.

| # | Section | Answers |
|---|---|---|
| 1 | [Introduction and goals](#1-introduction-and-goals) | What must it do, and what quality matters most? |
| 2 | [Constraints](#2-constraints) | What can't we choose? |
| 3 | [Context and scope](#3-context-and-scope) | Who and what does it talk to? |
| 4 | [Solution strategy](#4-solution-strategy) | The few big ideas |
| 5 | [Building block view](#5-building-block-view) | The parts and their jobs |
| 6 | [Runtime view](#6-runtime-view) | Key scenarios step by step |
| 7 | [Deployment view](#7-deployment-view) | Where it runs; CI/CD |
| 8 | [Crosscutting concepts](#8-crosscutting-concepts) | Rules that apply everywhere |
| 9 | [Architecture decisions](#9-architecture-decisions) | ADR log |
| 10 | [Quality requirements](#10-quality-requirements) | Testable quality scenarios |
| 11 | [Risks and technical debt](#11-risks-and-technical-debt) | What could hurt |
| 12 | [Glossary](#12-glossary) | Words |

---

## 1. Introduction and goals

### 1.1 Requirements overview

IronLog is a **phone-first training coach that plays like an MMO**. Full
vision: [01_Vision.md](01_Vision.md). Use cases: [02_UseCaseModel.md](02_UseCaseModel.md).

| Module | Core capability | Status |
|---|---|---|
| Log | Strength sets, cardio, badminton, stretching; edit + undo | ✅ sets only |
| Progress | PRs, e1RM chart, weekly sets per muscle, body check-ins, progress photos | ✅ first three |
| Coach | Goal programs, *Train now* planner, guided session, session review, body limitations + pain monitoring, daily stretch, exercise pictures; later (v0.6): "ready for…" gates and goal tests, cardio program, sport + prehab | planned (prototype ✅) |
| Game | 8 skills, XP, gentle decay, pauses, weekly quest, rest token + own reward, weekly streak, unlocks | planned |
| Online | Accounts, sync via the owner's VPS | planned |
| Platforms | Android, web (WASM), Windows from one codebase | ✅ Windows/Linux desktop |

### 1.2 Quality goals (top 5, in priority order)

| # | Quality | Why it matters here | Scenario |
|---|---|---|---|
| QG1 | **Safety of advice** | The app tells people with a joint that needs care what to lift. | Q4, Q12, Q13 |
| QG2 | **Data durability** | Months of training history must survive crashes, phone loss and sync conflicts. | Q3, Q8 |
| QG3 | **Speed of use** | "Too many taps per set" is the #1 complaint about other apps (research). | Q1, Q2 |
| QG4 | **Fair, explainable game rules** | A game that punishes exam weeks or can't explain its numbers kills motivation. | Q5 |
| QG5 | **Maintainability / testability** | One student developer; portfolio code that can be read and changed. | Q7 |

### 1.3 Stakeholders

| Who | Expects |
|---|---|
| Owner (student, badminton) | Plans that fit his day and body; motivation; works offline |
| Partner | Simple onboarding, own private data |
| Any user with a limitation, a sport or a long break | Plans inside their limits, a slow ramp back, their sport days respected |
| Developer (owner) | Clean layers, tests, docs; skills for a C++/AI career |
| Future readers (recruiters) | A codebase that shows good engineering judgment |

---

## 2. Constraints

| Type | Constraint | Consequence |
|---|---|---|
| Technical | C++17, Qt 6 (dev: 6.11, MinGW on Windows), CMake ≥ 3.20 | One language from server to phone |
| Technical | Targets: Android, WebAssembly (iPhone via browser), Windows | Phone-first UI; no native iOS app |
| Technical | Existing **Hetzner CX23** VPS (Ubuntu, x86_64), **Caddy**, **systemd**, no Docker, shared with TickTimer (the owner's earlier Qt timer app, with its own sync server on the same VPS) | New app = subdomain + loopback port + systemd unit + Caddy block (TickTimer `ROLLOUT.md` §2i) |
| Organizational | One developer, student schedule | Small iterations; risk-driven order |
| Organizational | Public GitHub repo | No personal health data or secrets in the repo |
| Regulatory / ethical | General training guidance, **not** a medical device | No diagnosis; self-reported limitations; red flags point to a professional; PAR-Q+ linked at onboarding (§8.9) |
| Conventions | UP artifacts (Larman), arc42, TickTimer-style backlog and troubleshooting log | See `08_DevelopmentCase.md` |

---

## 3. Context and scope

![Context](../diagrams/rendered/context.png)

### Business context

| Partner | In | Out |
|---|---|---|
| Owner, Partner | Time, situation, equipment, logged sets, check-ins | Session plans, levels, rewards, progress |
| GitHub | Code pushes, version tags | CI results, release files (zip, APK) |

### Technical context

| Channel | Protocol | Notes |
|---|---|---|
| App ↔ Caddy | HTTPS (TLS by Caddy, Let's Encrypt) | `ironlog.<domain>`; JSON API + `/app` web files |
| Caddy ↔ ironlog-server | HTTP on 127.0.0.1:8081 | Never reachable from outside |
| GitHub Actions → VPS | SSH (deploy key in GitHub secrets) | CD only; followed by a `/version` check |

---

## 4. Solution strategy

| Goal / problem | Approach | Section |
|---|---|---|
| Phone + web + desktop from one codebase | **Qt Quick (QML) UI** over the existing C++ layers | ADR-012 |
| Safe, explainable plans | **Rule-based planner** as a pure C++ function; rules come from the owner's written training decisions | 8.5, ADR-013 |
| Safe for any joint, not just a knee | **Body limitations as data**; the safety filter runs first and can't be overridden | 8.18, ADR-026 |
| Plans that adapt over weeks | **One engine** for gates, goal tests and "ready for…" triggers, over derived state | 8.19, ADR-027 |
| New programs without new code | **Training content as data files**: catalog, programs, ladders, routines, sports, rules | 8.22 |
| Fair game that can be rebalanced | **XP and levels are derived** from the log, never stored | 8.6, ADR-014 |
| Works offline, syncs safely | **Local-first** + TickTimer's revisioned-document protocol + **record-level merge** on the client | 8.3, ADR-015 |
| Reuse a proven server setup | Same VPS pattern as TickTimer; **QHttpServer** bound to loopback behind Caddy | 7, ADR-016 |
| Testability | Layers `core → services → ui`; every rule a pure function with "now" passed in | 5, 8.12 |

---

## 5. Building block view

### 5.1 Level 1 — the layers (target)

![Building blocks](../diagrams/rendered/building_blocks.png)

| Block | Tech | Responsibility | Status |
|---|---|---|---|
| `core/` | Pure C++17 | All training knowledge: log, catalog, stats, body limitations, planner, rules engine, game, sync merge | ✅ log, catalog, stats |
| `data/` | JSON, bundled as Qt resources | Training content: exercise catalog, goal programs, stage ladders, stretch routines, sport profiles, gates and triggers (§8.22) | planned |
| `services/` | Qt Core + Network | Controller (every write), JSON store, sync, auth, view models | ✅ controller, store |
| `ui-qml/` | Qt Quick | Phone-first screens: tabs **Today · Log · Progress · You** (ADR-023); Train now questions, guided session + mini bar, "How was it?", Why? sheet | planned |
| `ui/` | Qt Widgets | v0.1 desktop UI; retired when QML reaches parity | ✅ |
| `server/` | Qt Core + HttpServer | Accounts, sessions, revisioned log storage | planned |

**Rule**: dependencies point down only (`ui → services → core`). The server
reuses `core/` only for validation, never the UI. (Larman Ch. 13 Layers;
*Software Architecture in Practice* Ch. 8: the allowed-to-use relation must
be one-way.)

### 5.2 Level 2 — `core/` modules

| Module | Main types | Job | Status |
|---|---|---|---|
| `log` | `SetEntry`, `Workout`, `TrainingLog` → v2: `CardioEntry`, `MobilityEntry`, `BodyCheckIn`, `PausePeriod`, `RestTokenUse`, `UserReward`, `PainReport`, `GoalTest`, `TriggerResponse` | What happened | ✅ sets |
| `catalog` | `Exercise`, `ExerciseLibrary`, `MuscleGroup` → v2: equipment, secondary muscles, movement pattern, **stress per joint** + kneeling / impact / deep-bend / overhead flags, stage, variants | What exists | ✅ basic |
| `body` | `Limitation`, `StageLadder`, `currentStage()`, `painAction()` | What the body allows today (ADR-026) | planned |
| `stats` | free functions | PRs, e1RM history, weekly sets, last set | ✅ |
| `planner` | `Profile`, `TrainContext`, `SessionPlan`, `buildSession()`: safety filters → score → order → progress (P1–P11) | What to do today | planned |
| `rules` | `Gate`, `Trigger`, `GoalTestSpec`, `evaluate(log, profile, rules, today)` | What's next: unlocks, offers, tests due (ADR-027) | planned |
| `game` | `SkillSheet`, `xpFor()`, `applyDecay()`, `weeklyQuest()` | Levels and rewards | planned |
| `sync` | `merge(local, remote)` | Combine two copies of a log | planned |
| `Date` | value type | Calendar math | ✅ |

### 5.3 Level 2 — v0.1 classes (as built)

![v0.1 architecture](../diagrams/rendered/architecture.png)

| Class | Layer | Responsibility | Pattern / reason |
|---|---|---|---|
| `Date` | core | Calendar math, ISO text | `struct`: plain value (Stroustrup §16.2.4) |
| `ExerciseLibrary` | core | Unique, trimmed exercise names | Information Expert |
| `SetEntry` | core | One set; volume and e1RM | Invariant via private ctor + `make()` (Stroustrup §17.6.3.2) |
| `Workout` | core | One day's sets; total volume | Information Expert (Larman's `Sale.getTotal()`) |
| `TrainingLog` | core | Sorted workouts; get-or-create | Creator |
| `stats::` | core | Analysis | Free functions keep `TrainingLog` cohesive |
| `JsonStore` | services | Save/load JSON atomically | Pure Fabrication (Larman Ch. 25) |
| `TrainingController` | services | Every write → validate, save, signal | GRASP Controller (Larman Ch. 17) |
| `WorkoutTableModel`, `WeeklyVolumeModel` | ui | Table views | Qt model/view |
| `ProgressChart` | ui | e1RM line chart | Custom `paintEvent()` |

**Pattern names** (GRASP = Larman's rules for which class does what):
Information Expert = the data owner does the work · Creator = a container
makes its parts · Pure Fabrication = a helper class not from the domain ·
Controller = one entry point for all actions · ctor = constructor.

---

## 6. Runtime view

### R1 — *Train now* (cramming night)

![Train now](../diagrams/rendered/sequence_train_now.png)

1. Three answers: time, situation, equipment (pre-filled from last time).
2. `planner::buildSession()` — pure function — first applies the **safety
   filters** (equipment, every body limitation's stage), then scores the
   remaining exercises, fits the time budget, orders compound-first, sets
   loads (one change at a time), adds limitation cues ("pad your knee"), and
   attaches a "why" to each item.
3. `game::previewXp()` shows what the session is worth.
4. *Start* opens the guided session; every logged set takes the UC-01 path.

### R2 — Log a set (exists)

See `diagrams/sequence_add_set.puml`: controller validates → updates log →
saves atomically → emits `logChanged()` → all views reload.

### R3 — Sync after training offline

1. App marks the log *dirty* on every change (rides `logChanged()`, as TickTimer).
2. When online: `PUT /log` with `baseRevision`.
3. **No conflict** → server stores, returns new revision.
4. **409 conflict** → `GET /log`, `sync::merge(local, remote)` by record id
   (pure function), `PUT` again with the server's revision.
5. Only if a record was **edited differently on both sides** does the user
   choose (rare: sets are mostly appended).

### R5 — Pain, then the next morning

1. In the session, "Joints: pain" opens the pain sheet: which exercise,
   0–10, any red flag.
2. A red flag → that exercise is removed today, and the sheet suggests a
   professional. Nothing else is decided automatically.
3. Next app open, the next morning: "How's your knee this morning? 0–10".
4. `body::painAction(during, nextMorning)` (pure): 0–3 and settled → keep;
   4–5 or still there → hold the load; > 5 or worse → that exercise drops a
   stage for 2 weeks. The result is a `PainReport`; stages are derived.

### R6 — A "ready for…" offer

1. On open, `rules::evaluate(log, profile, rules, today)` lists gates that
   just passed and triggers whose conditions hold (all derived).
2. Today shows **one** offer, with its reason: "4 weeks of zone 2 done.
   Add one interval session a week?" → Accept / Not now.
3. The answer is stored as a `TriggerResponse`; "Not now" hides it for 7
   days. Accepting changes the program from the next plan on.

### R4 — Opening the app on a new day

Nothing is "ticked" or stored. The character sheet is recomputed:
`game::computeSheet(log, pauses, today)` applies XP, decay and the comeback
bonus up to today. Same inputs → same levels on every device.

---

## 7. Deployment view

![Deployment](../diagrams/rendered/deployment.png)

### 7.1 On the VPS (same pattern as TickTimer)

| Item | Value |
|---|---|
| Hostname | `ironlog.<domain>` (A record → VPS IP; DNS on Cloudflare) |
| Caddy | New site block: API → `reverse_proxy 127.0.0.1:8081`; `/app*` → web files; `/download/*` → APK |
| Service | `/etc/systemd/system/ironlog.service`, user `ironlog`, `--bind 127.0.0.1 --port 8081 --data /var/lib/ironlog --invite …`, systemd hardening as TickTimer's unit |
| Data | `/var/lib/ironlog/{accounts.json, devices.json, logs/<user>.json, photos/<user>/}` — all writes via `QSaveFile` |
| Backups | `ironlog-backup.timer` nightly 03:00 → `/var/backups/ironlog`, 30 days; off-box copy manual for now |
| Firewall | `ufw` allow 22, 80, 443 only (open item on the shared box, also TickTimer's) |

### 7.2 Clients

| Platform | Artifact | Distribution |
|---|---|---|
| Android | Signed APK (Qt for Android, arm64-v8a) | GitHub Release + `https://ironlog.<domain>/download/` |
| iPhone / browsers | Qt WebAssembly bundle | `https://ironlog.<domain>/app/` (same origin as the API → no CORS) |
| Windows | `windeployqt` zip | GitHub Release |

### 7.3 CI/CD (GitHub Actions)

| Pipeline | Trigger | Status | Steps |
|---|---|---|---|
| **CI** | every push to `main`, every pull request | ✅ I2 |  (1) Linux, core only, no Qt: build + `test_core` — fast feedback. (2) Linux + Qt: full build, `ctest` with `QT_QPA_PLATFORM=offscreen`. (3) Windows + Qt MinGW: full build + tests. |
| **CD** | tag `vX.Y.Z` or the Release button | ✅ Windows zip (I3) · planned: APK + web bundle, VPS deploy + `/version` check (I6, v0.5) | Build Windows zip, Android APK (keystore from secrets), web bundle → GitHub Release. Deploy server + web files to the VPS over SSH → restart service → **`GET https://ironlog.<domain>/version` must return X.Y.Z**, else the job fails. |

The last step answers TickTimer's reason for *not* automating deploys
(`design-addendum-deployment.md` §H: "the value is in the verification").
Here the verification is part of the pipeline.

---

## 8. Crosscutting concepts

### 8.1 Domain model v2 (schema v2)

| Record | Key fields | Notes |
|---|---|---|
| `StrengthSet` | `id`, `at` (date+time), `exerciseId`, `weight`, `unit`, `reps`, `rir?`, `type` (working / warm-up / drop / failure), `note?` | Weight kept **as entered** with its unit |
| `CardioEntry` | `id`, `at`, `kind` (treadmill / walk / run / bike / badminton), `minutes`, `speed?`, `incline?`, `zone?` | Feeds Stamina |
| `MobilityEntry` | `id`, `at`, `routineId` (morning / add-ons / session cool-down), `minutes`, `holds` (stretch id → seconds per side), `reach?` (towel-stretch hand gap in cm, per side) | Feeds Mobility; `reach` feeds the reach chart (§8.17) |
| `BodyCheckIn` | `id`, `date`, `bodyweight`, `unit`, `waist?` | "Lean" trend |
| `ProgressPhoto` | `id`, `date`, `pose` (front / side / back), `fileRef`, `previewRef`, `checkInId?` | Image + preview stored beside the JSON, encrypted (§8.14) |
| `SessionReview` | `id`, `date`, `overall` (great / good / ok / rough), `energy` (high / normal / low), `joints` (fine / twinge / pain), `note?`, `prefilled` (which answers the app suggested) | Planner input; `joints = pain` → pain check |
| `GameModeChange` | `id`, `at`, `on` (true / false) | The off-intervals pause decay; the current value is the last record |
| `TimedSet` | as `StrengthSet` but `seconds` instead of `reps` (plank, wall sit, balance) | Same PR/XP rules on duration |
| `PausePeriod` | `id`, `start`, `end`, `reason` (exams / sick / injured / travel) | Stops decay; changes the quest (§8.6); length capped per reason |
| `RestTokenUse` | `id`, `week` | A rest token spent to keep a missed week in the streak |
| `UserReward` | `id`, `text`, `since` | The reward the user writes for completing a weekly quest (free text, optional) |
| `Profile` | goal program, units, equipment sets per place, weekly availability, sport + usual days, time since regular training | Per user; limitations are their own records |
| `StretchRoutine` | `id`, name, ordered stretch ids, default hold (s) | Data file, like goal programs (§8.17) |
| `Limitation` | `id`, `joint` (knee / shoulder / lower back / ankle / hip / wrist / neck), `side` (left / right / both), `stage`, `since`, `note?` | Per user; drives the safety filter (P3, ADR-026). Note stays on the device |
| `PainReport` | `id`, `at`, `exerciseId`, `limitationId?`, `during` (0–10), `nextMorning?` (0–10), `redFlags` (list) | P7; feeds stages and gates |
| `GoalTest` | `id`, `date`, `testId` (e.g. reach, balance, time-before-gassed), `value`, `unit` | Every 4 weeks (F26) |
| `TriggerResponse` | `id`, `at`, `triggerId`, `accepted` (true / false) | So a dismissed offer isn't repeated every day (ADR-027) |
| `Exercise` | stable `id` slug, name, primary + secondary muscles, equipment set, movement pattern, knee stress (low/med/high), stage, default rep range | Catalog |

Every record also carries `updatedAt` and `deleted` (tombstone) for sync.
**v2 drops "one session per day"**: activities are records with a time.

### 8.2 Persistence and schema versions

- One JSON document per user; `schemaVersion` 2 (planned, F1; v0.1 writes 1 ✅). Strict about the file,
  tolerant about items (v0.1 rule kept).
- **Migration v1 → v2** on first load: generate ids, name → exercise id,
  unit = kg. The v1 file is kept as `ironlog.v1.json` (copy, never move).
- Writes are atomic (`QSaveFile`); unreadable files are moved aside.
- Rotating local backups: last 7 daily copies.

### 8.3 Sync and conflicts

- **Wire protocol = TickTimer's**: `GET /log` → `{revision, data}`;
  `PUT /log` with `baseRevision` → new revision or **409**. The server stores
  an opaque blob and never parses training data.
- **Merge = new for IronLog**: on 409 the client merges by record `id`:
  union of records; same id → newer `updatedAt` wins; tombstones win over
  older edits. `sync::merge()` is a pure core function with exhaustive tests.
- **Why not TickTimer's "a human chooses"**: a training log is append-mostly,
  so most conflicts are two devices adding *different* sets. Union is
  correct for those; asking would be noise.
- Client lessons pre-paid (TickTimer TROUBLESHOOTING): normalize the server
  URL where it's born; branch on HTTP status, not `reply->error()`;
  `clearConnectionCache()` per request.

### 8.4 Derived state

XP, levels, decay, quest progress, **earned** tokens, body-limitation
stages and open "ready for…" offers are computed from the log on demand. Only *inputs* are stored (sets, cardio, pauses,
rest-token uses, the user's reward text). Consequences: nothing to sync or corrupt; rebalancing the
game is a code change that re-scores history; two devices always agree.

### 8.5 Planner (rules engine)

`planner::buildSession(profile, context, history, catalog, today) → SessionPlan`

| Rule | What it does | Source |
|---|---|---|
| P1 Session type | Pick what the weekly plan still needs (strength A/B alternation, zone-2 cardio, mobility); avoid hitting a skill hard < 48 h apart | Owner's plan: A/B full-body ×3 |
| P2 Time budget | Warm-up 5–8 min (RAMP: raise, activate, mobilise, potentiate) + main + 3–5 min stretch; ~3 min per strength set incl. rest; drop one-joint (isolation) exercises first, never the warm-up | Owner's plan: RAMP, 60–90 s rest |
| P3 Exercise choice | **Filters first** (cannot be overridden): equipment ⊆ available; for **every** body limitation, the exercise's stress on that joint ≤ what its stage allows (ADR-026). **Then score** the rest on the Decision Report's criteria (effectiveness, sustainability, equipment fit, sport transfer, time) and pick the best; compound before isolation (ADR-031) | Decision report: K1–K6, pain rule, compound-first |
| P4 Loads | Double progression: all sets at top of rep range → next available weight (e.g. +5 lb dumbbell) | Owner's plan: "picking weights" |
| P5 Situation | **Cramming**: shorter, compound-only, no PR attempts, weekly target = exam minimum (2 strength + 1 cardio). **Sore/tired**: mobility + easy zone 2 + light core. **Before my sport**: the sport's RAMP warm-up only, no static holds (§8.21) | Decision report C8 |
| P6 Gates | Harder variations unlock only after their gate (e.g. reverse lunge after 3–4 pain-free weeks of step-ups; stability stage after 2 clean weeks). Gates are data, evaluated by the rules engine (§8.19) | Decision report D3, D7 |
| P7 Pain monitoring | 0–10 during the exercise and the next morning: 0–3 and settled → OK; 4–5 or still there → hold the load; > 5 or worse next morning → that exercise drops one stage for 2 weeks. Red flags (swelling, heat, locking, giving way, can't take weight) → stop it and suggest a professional (§8.9) | Owner's plan: pain rule; Silbernagel 2007 (stricter: 3/10) |
| P8 Stretch timing | Static holds only in the morning routine, the session cool-down or the evening — never in a warm-up or right before badminton (warm-ups are dynamic, RAMP). The cool-down picks add-on stretches for areas not stretched yet this week | Decision report D4, D9 |
| P9 Return ramp | After a long break or a pause > 3 weeks: week 1 at ~60 % of normal sets, +10 % a week (balance-table defaults) | Decision report C2 |
| P10 One change at a time | A progression step changes one of weight / reps / sets, or time / speed / incline | Owner's plan: treadmill tips |
| P11 Past the heaviest weight | At the top weight and top reps: 3 s lowering, then a pause, then a one-leg or one-arm version | Home rack limit (C5) |

*Compound = a multi-joint exercise. C8, D3, D7 = sections of the owner's
private Decision Report (kept out of the repo, ADR-019).*

Each plan item carries a **reason string** ("Box squat: knee-friendly depth,
compound first"). Goal programs (Badminton singles, Lean & toned upper,
Lean & toned lower, General) are **data files**, not code.

### 8.6 Game rules v2 (balance table — tunable, one file)

| Rule | Value |
|---|---|
| Skills | Arms, Shoulders, Chest, Back, Core, Legs, Stamina, Mobility |
| XP: strength set | +10 primary skill, +5 each secondary (half-set rule) |
| XP: PR | +25 primary skill |
| XP: cardio / badminton | +2 / min zone 2; +3 / min intervals or badminton |
| XP: mobility | +2 / min (daily stretch, add-ons, cool-down) |
| XP: prehab block | its sets count like strength sets; a goal test gives +10 to the skill it tests |
| No XP | for pain reports, red flags or skipped exercises — pain is never rewarded |
| Level curve | level L → L+1 costs 50 × L XP (fast early, slower later) |
| Total level | sum of skill levels |
| Progress display | "N sets to next level" on every skill (competence feedback) |
| Decay | after **14 days** with no XP in a skill: **−0.5 %** of that skill's XP per day |
| Decay warning | 3 days before decay starts: "Back starts fading in 3 days"; the body map greys that muscle |
| Floor | decay never goes below 50 % of the skill's peak XP |
| Comeback bonus | XP ×2 while below peak (retraining is faster than first training) |
| Pause | stops all decay. Reasons and caps: exams ≤ 21 days · sick ≤ 14 · injured ≤ 42 · travel ≤ 21 |
| Weekly quest | the program's planned sessions; during a pause: exams → 2 strength + 1 cardio · travel → 1 session · sick / injured → no quest |
| Rest token | quest complete → +1 rest token (bank max 2); spent automatically to keep a missed week in the streak |
| Your reward | optional text the user writes ("movie night"); shown when the quest completes. The app never suggests food and never says "cheat" |
| Streak | consecutive **weeks** with the quest done; rest days count; paused weeks and rest-token weeks keep it; a set logged later for a past day can restore that week; no shaming messages |
| Rules version | the balance table has a version number; when it changes, the app says "Game rules updated — levels recalculated" |

**Why v2 changed the game** (research round 2, [`feature-research-2.md`](research/feature-research-2.md)):

- **Gentler decay** than v1: strength barely drops in 3 weeks off.
- **No cheat-meal token**: cheat meals are linked to eating-disorder
  symptoms in young adults, and "cheat" wording breeds guilt.
- **Weekly, forgiving streak**, like Duolingo's streak freeze.

**Game mode (on/off, per user — ADR-021).** One checkbox, asked at
onboarding and changeable in settings.

| | Game ON | Game OFF |
|---|---|---|
| XP | earned and shown | still computed from the log, not shown |
| Decay | applies after the grace period | **paused** (like a Pause) |
| Levels, body map colours, streak, tokens | shown | hidden |
| Weekly quest | "quest", earns a rest token + shows your reward | plain "2 of 3 planned sessions done" |
| Progression gates, pain check, plans | active | **active** — safety rules never depend on the game |
| Turning back on | — | levels include everything trained while off; nothing lost |

Possible only because game state is derived (ADR-014): the toggle is a
view choice plus a decay pause, not data to migrate.

### 8.7 Units

Weights are stored **as entered** (`weight` + `unit`). Analysis converts to
kg with the exact factor 0.45359237. Display uses the user's unit.
Equipment (e.g. 5–25 lb dumbbells) is defined in its own unit, so load
suggestions are always weights that physically exist.

### 8.8 Security

- HTTPS only between devices and Caddy; the server binds 127.0.0.1.
  Qt's docs say Qt HTTP Server should not be exposed to the internet —
  Caddy is the only thing that talks to it.
- Invite code on `/register`. **Unlike TickTimer, the password never
  reaches the server**: the client derives a *login key* from it (§8.14)
  and the server stores a salted, stretched hash of that key. Session
  tokens in memory; device tokens stored hashed; login brake in the server
  (stock Caddy has no rate limiting).
- Each user can only read and write their own log.

### 8.9 Safety (health)

The planner never generates something outside the user's constraints (P3,
P6, P7 are tested). Every plan screen shows "General guidance, not medical
advice; stop on sharp pain." Personal plans and reports are **not** in the repo.

| Boundary | How |
|---|---|
| Readiness screen | Onboarding links the PAR-Q+ (not copied). Any "yes" → suggest a doctor or qualified exercise professional first; the app still works with conservative stages |
| Limitations | Self-reported joint + stage (ADR-026). The app never diagnoses or names an injury |
| Red flags | Swelling, heat or redness, locking, giving way, can't take weight → stop that exercise, suggest a professional |
| Pain | Monitored 0–10 (P7); pain is never rewarded with XP or praise |
| Offers, not orders | Stage-ups, jogging, new phases are offers the user accepts (ADR-027) |

Design and sources: [research/plan-to-features.md](research/plan-to-features.md).

### 8.10 Error handling

Expected bad input → `std::optional` or a result enum (no exceptions).
Server errors use distinct codes (`auth`, `forbidden`, `conflict`,
`no_such_user`, unknown → "unexpected reply"), as TickTimer learned.

### 8.11 Testing

| Level | Tool | Examples |
|---|---|---|
| Core rules | Catch2 | planner never breaks constraints; decay stopped during every pause reason; merge is commutative for appends |
| Services | Catch2 + Qt Core | store migration v1→v2; controller signals; sync state machine |
| Server | Catch2 + live process | register/login/409 against a real `ironlog-server` on a temp port |
| UI | manual QA checklist + screenshots | phone layout, web build loads |

### 8.12 Time

No core function reads the clock. `today` / `now` is a parameter
(TickTimer lesson: a test that failed only after 5 PM).

### 8.13 Training rules already in v0.1 ✅

| Rule | Definition | Where |
|---|---|---|
| Set volume | weight × reps | `SetEntry::volume()` |
| e1RM (Epley) | weight × (1 + reps/30); 1 rep → weight | `epleyOneRepMax()` |
| PR | first set with the day's top e1RM, **strictly** greater than every earlier day; first-ever session is not a PR | `stats::personalRecordIndices()` |
| Week | Monday–Sunday containing the chosen day | `Date::mondayOfWeek()` |
| Weekly sets | sets per primary muscle in the week; unknown exercises skipped | `stats::weeklySets()` |
| Volume status | < low → Below; > high → Above; default 10–20 | `stats::classify()` |

**About the 10–20 default.** A 2017 meta-analysis (Schoenfeld, Ogborn &
Krieger, *J Sports Sci* 35(11):1073–1082, https://doi.org/10.1080/02640414.2016.1210197)
found more muscle growth with more weekly sets; groups doing ≥ 9 sets per
muscle grew more (p = 0.076, so not quite significant), and the data only
reached ~10 sets. A 2022 systematic
review (Baz-Valle et al., *J Hum Kinet* 81:199–210,
https://pubmed.ncbi.nlm.nih.gov/35291645/) suggests 12–20. It's a starting
point and a parameter (`stats::VolumeTarget`); goal programs (F6) will set
their own per-muscle targets.

### 8.14 Progress photos (privacy) — envelope encryption

Body photos are the most sensitive data IronLog holds. Design approved by
the owner 2026-11-23 (ADR-020).

**Idea in one line:** a random photo key locks the photos. That key is
kept in two locked "envelopes": one opens with your password, one with a
recovery code.

**Words:** libsodium = crypto library (the owner's earlier password-manager
project EyesOff uses it) · Argon2id = slow password hash · KDF = makes
sub-keys from one key · salt = random extra input per user.

![Photo keys](../diagrams/rendered/photo_keys.png)

**The keys:**

| Key | Made from | Lives | Job |
|---|---|---|---|
| Master key | `crypto_pwhash` (Argon2id) of the password + a per-user salt | memory only | Parent of the two sub-keys |
| Login key | `crypto_kdf` sub-key 1 of the master key | sent at login; server stores only a hash of it | Proves who you are — **the server never sees the password** |
| Lock key | `crypto_kdf` sub-key 2 of the master key | **never leaves the device** | Locks/unlocks envelope A |
| Photo key | random 256-bit, made once | inside the envelopes; in memory while unlocked | Encrypts every photo and preview |
| Recovery code | random, shown **once** at the first photo, which turns on photo sync (also in Settings) (e.g. `K7QF-2MZD-…`) | the user's password manager or paper | Backup way in |
| Recovery keys | Argon2id of the recovery code → auth + unlock sub-keys | memory only | Auth proves the code to the server; unlock opens envelope B |

**What the server stores** (all useless without the password or the code):

- hash of the login key
- hash of the recovery auth key
- envelope A: the photo key, locked by the lock key (from your password)
- envelope B: the photo key, locked by the recovery unlock key
- encrypted photos and encrypted previews

**Flows**

| Situation | What happens |
|---|---|
| New device (Android, web, Windows) | Log in → derive lock key → download envelope A → open it → photos decrypt on the device |
| Change password | New master key → new login key + **re-lock envelope A only**; one request; no photo is re-uploaded |
| Forgot password | Enter recovery code → server checks the recovery auth key → open envelope B → choose a new password → new envelope A |
| Lost password **and** code | Server photos can't be opened by anyone. Photos still on a logged-in device are fine (and can be re-uploaded under a new key) |

**Other rules**

| Rule | How |
|---|---|
| Encryption | Photos: `crypto_secretstream_xchacha20poly1305` (built for large files). Envelopes: `crypto_secretbox` |
| Previews | The server can't make thumbnails, so the device makes a small preview, encrypts it, and uploads it next to the photo — galleries load fast everywhere |
| Synced separately | `PUT/GET /photos/<id>` and `/photos/<id>/preview` as opaque blobs; the log only references photo ids |
| Login salt | `GET /prelogin?user=` returns the salt + Argon2id settings needed before login; unknown users get a stable fake salt so the endpoint doesn't reveal who has an account |
| Web app (iPhone) | Browser storage may be cleared, so decrypted photos are a cache, downloaded again when needed; the password is asked again when opening photos in a new browser session |
| Never exported or committed | Not in CSV export, never in the repo |
| Per user | The partner has her own photo key; the owner, as server admin, cannot read her photos |

**Open for F16:** the training log could be locked with the same machinery
(the server would then store an encrypted log blob). Decide when building sync.

### 8.15 "How was it?" — pre-picked answers

`review::suggest(session, plan, history) → SessionReview` is a pure core
function; the UI shows its picks highlighted and the user confirms or taps
another chip.

| Answer | Pre-picked when |
|---|---|
| Overall | ≥ 90 % of planned sets hit target → *Good*; any PR → *Great*; < 60 % → *Rough*; else *OK* |
| Energy | an exams pause or *Cramming* → *Low*; otherwise last review's value |
| Joints | always *Fine* — **never** pre-pick pain; the user must tap it |

The app stores which answers were suggested (`prefilled`), so later
analysis can tell "confirmed" from "chosen". Voice notes use the phone
keyboard's dictation: no speech code in v1.

### 8.16 Exercise pictures

Every exercise shows two pictures (ADR-025): a **how-to picture** (the
IronLog anime character, start pose → end pose) and a **muscle diagram**
(main muscles dark blue, helpers light blue). Both are images made with ChatGPT and
checked by eye; the code-drawn alternative (ADR-024) was rejected.

| Piece | Where | What it does |
|---|---|---|
| Image files | `assets/exercises/ironlog-<id>-how.jpg`, `…-muscles.jpg` | 1200 × 800 JPEG, ~65–140 KB each |
| `exercises.json` | `assets/exercises/` | Exercise id → files + main/helper muscles; read with the catalog (F2) |
| Qt resource file | `ui-qml/` (planned) | Bundles the images into the app, so they work offline |
| Clip per exercise | built with ffmpeg (planned) | Switches between start and end pose at the exercise's tempo, then the muscle diagram |
| `CREDITS.md` | `assets/exercises/` | Tool, date, prompts, checks — how to make more that match |

Rules: the muscle names are always shown as text too (never colour alone);
a missing diagram falls back to the text legend (plank, dead bug today).

**Three muscle vocabularies, one mapping (F2).** `exercises.json` uses 16
display names; the core has 10 `MuscleGroup` values; the game has 8 skills.
F2 maps each display name to a `MuscleGroup` and a skill, e.g. lats, upper
back, lower back → Back; front/rear shoulders → Shoulders; abs, obliques →
Core; quads, glutes, hamstrings, calves, hip flexors → Legs; biceps,
triceps, forearms → Arms. Stamina and Mobility come from cardio and stretch
minutes, not muscles.

**Body map (game).** `tools/make_bodymap.py` turns the same grey diagram
into `assets/game/ironlog-bodymap-key.png` (one flat colour per skill area,
for the app) and `bodymap-paths.json` (SVG outlines, for the prototype). The
app repaints each area by level (one purple `xp` hue, darker = higher;
stripes = fading — the UI research's sequential rule). Tapping an area shows
its level and sets to the next one. See [`assets/game/README.md`](../assets/game/README.md).


### 8.17 Daily stretch (F24)

From the owner's fitness plan (Part 1 and 1b) and Decision Report D4
(private, ADR-019). Static stretching is for range of motion, not injury
prevention, so it sits away from explosive play.

| Part | When | Content |
|---|---|---|
| Morning routine | every day, ~12 min; kept in an exams pause | 12 stretches: neck tilt, cross-body shoulder, towel stretch, doorway chest, cat-cow, child's pose, kneeling hip flexor, figure-four glute, seated hamstring, standing quad, calf on the slant board, arm circles |
| Add-ons | after a session (cool-down) or evening | 12 more for areas badminton loads: wrist flexors and extensors, overhead triceps/lat, sleeper, butterfly, side lunge, open book, side bend, knees-to-chest, child's pose side reach, kneeling shin, foot roll |
| Reach test | weekly, after the towel stretch | Hand gap behind the back, per side, in cm (0 = touching); pre-filled from last week, ± to change |

**Player rules.** One stretch per screen. Holds are per side, 30 s by
default; tight spots (towel, hip flexor, hamstring) are marked and grow
with ±15 s up to 60 s, remembered per stretch. A beep + buzz says "switch
sides"; the player moves on by itself after the second side, so hands stay
free. Rep-based items (cat-cow, arm circles, open book) show a count and a
Done button instead of a timer. Timers use stored end times, like the rest
timer (§8.12).

**Body coverage.** Each stretch names its body area. The cool-down picks
add-ons whose areas have no stretch yet this week (planner rule P8), so
every area gets covered over a week.

**Game.** Stretch minutes give Mobility XP (+2 per minute, §8.6). The
routine is not a quest item, so missing a morning never costs a streak.


### 8.18 Body limitations and pain monitoring (F25, ADR-026)

The owner's plan was built around one knee. The app generalizes it: a
**limitation** is a joint, a side and a stage. Design and sources:
[plan-to-features F25](research/plan-to-features.md#f25-body-limitations-the-knee-scaled).

| Piece | Rule |
|---|---|
| Joints | knee, shoulder, lower back, ankle, hip, wrist, neck; left / right / both |
| Joint stress | every exercise: low / medium / high per joint, plus flags kneeling, impact, deep bend, overhead |
| Stage ladder | per joint (data): the highest stress each stage allows, and what unlocks next. Example, knee: 1 = box squat, bridge, wall sit, low step-up · 2 = reverse lunge, higher step-up, single-leg RDL · 3 = Bulgarian split squat · 4 = forward lunge, then jumps (after a readiness gate) |
| Safety filter (P3) | stress ≤ the stage's limit for **every** limitation; runs before scoring; can't be overridden |
| Swaps and cues | a filtered exercise is swapped for the closest allowed variant; kneeling → "pad your knee" or the standing version; the swap is named in the plan ("for your knee") |
| Pain monitoring (P7) | 0–10 during and next morning: 0–3 settled → keep · 4–5 or still there → hold · > 5 or worse → down one stage for 2 weeks |
| Red flags | swelling, heat or redness, locking, giving way, can't take weight → stop that exercise, suggest a professional |
| Weekly rating | 0–10 per limitation in the body check-in; used by gates (§8.19) |
| Stage up | a gate: 2 weeks, all planned sessions, no pain > 3, weekly rating ≤ 2 → an **offer**, never automatic |

### 8.19 Gates, goal tests and triggers (F26, ADR-027)

One engine replaces scattered "revisit when" rules (old P6, F22).
`rules::evaluate(log, profile, rules, today) → {passedGates, offers, testsDue}`
is pure, like the game.

| Kind | Example from the owner's plan, scaled | Result |
|---|---|---|
| Gate (time + criteria) | Reverse lunge after 3–4 pain-free weeks of step-ups; stability stage after 2 clean weeks; phase 2 (jumps) after 6–8 weeks, no pain, 3×10 loaded reverse lunges, 30 s single-leg balance | Unlock (game on: an unlock moment) |
| Trigger | Walk 40 min at a steady pace and incline for 2 weeks + 8 pain-free weeks → "try light jogging?"; e1RM falling 3 sessions → "lighter week?"; 3–4 months on full-body A/B → "try an upper/lower split?" | An offer on Today (max one), with its reason |
| Goal test | Time before getting gassed in your sport; reach test; single-leg balance; reps of a test exercise | Due every 4 weeks; charted in Progress |

Rules are data with a small, closed condition language: comparisons over
derived stats, "for N weeks", "and". No scripting.

### 8.20 Cardio program (F27, ADR-029)

From the plan's treadmill part, scaled to any starting point.

| Step | Rule |
|---|---|
| Baseline | 10-minute talk test: raise the pace until you can talk but not sing; that speed + incline is week 1. After each session for 2 weeks: "Too easy / About right / Too hard" adjusts the next step |
| Weekly table | 2 weeks per step: +5 min until 40–45 min, then speed, then incline. **One change at a time** (P10) |
| Session | 5 min easy → main block → 5 min easy; no holding the rails |
| Intervals | a gate after 4 weeks of base: once a week, 6 × (1 min faster + 2 min easy) |
| Jogging | a trigger (§8.19), only if no limitation forbids impact |
| Logged as | `CardioEntry` (minutes, speed, incline, zone); Stamina XP |

### 8.21 Sport profile and prehab blocks (F28, ADR-030)

| Piece | Rule |
|---|---|
| Sport profile (data) | body areas it loads, a RAMP warm-up, its weak spots. Badminton: wrists and forearms, inner thighs, back of the shoulder, spine rotation, knee control |
| Sport days | from the profile; no hard leg session within 24 h before; the sport counts toward Stamina |
| "Before my sport" | Train now situation → the sport's RAMP warm-up only (raise, activate, mobilise, potentiate); no static holds (P8) |
| Prehab block | 10–12 min, from limitations + sport, with stages and a weekly target (e.g. 3×): core (curl-up, side plank, bird dog → suitcase carry, dead bug, Pallof press); knee (single-leg balance, slow step-down → single-leg RDL → split squat); hip (band side steps, side-lying leg raise) |
| Placement | after cardio or on rest days; counts in the weekly plan, not the quest |

### 8.22 Training content as data

Everything a coach would write lives in versioned JSON files under `data/`,
bundled as Qt resources and read by `catalog`, `planner`, `body` and
`rules`.

| File | Holds |
|---|---|
| `exercises.json` | catalog: muscles, equipment, joint stress + flags, stage, variants, cues, pictures (F2, F23) |
| `programs/*.json` | goal programs: weekly sessions, set targets, phases (F6) |
| `ladders/*.json` | one per joint: stage limits and unlocks (F25) |
| `stretches.json` | morning routine and add-ons (F24) |
| `sports/*.json` | sport profiles and warm-ups (F28) |
| `rules/*.json` | gates, triggers, goal tests (F26) |

Each file has a `version`; the app says "rules updated" when one changes,
like the game's balance table. A coach or physio program could ship as
data later, without new code.

---

## 9. Architecture decisions

Each decision has its own record in [`docs/adr/`](adr/README.md) — context,
decision, consequences, alternatives, sources. This table is the summary.
Older, longer reasoning for ADR-001 to 009 is also in
[archive/design-notes-v0.1.md](archive/design-notes-v0.1.md) §4.

| # | Decision | Status | Date |
|---|---|---|---|
| [001](adr/ADR-001-three-layers.md) | Three layers as three CMake targets | Accepted | 2026-11-23 |
| [002](adr/ADR-002-own-date-type.md) | Own `Date` type in core instead of `QDate` | Accepted | 2026-11-23 |
| [003](adr/ADR-003-setentry-invariant.md) | `SetEntry` invariant via private constructor + `std::optional` factory | Accepted | 2026-11-23 |
| [004](adr/ADR-004-stats-free-functions.md) | Analysis as free functions (`stats::`) | Accepted | 2026-11-23 |
| [005](adr/ADR-005-jsonstore-pure-fabrication.md) | Persistence as a Pure Fabrication (`JsonStore`) | Accepted | 2026-11-23 |
| [006](adr/ADR-006-single-controller.md) | All writes through one `TrainingController` + signals | Accepted | 2026-11-23 |
| [007](adr/ADR-007-atomic-save.md) | Atomic saves; never overwrite an unreadable file | Accepted | 2026-11-23 |
| [008](adr/ADR-008-hand-painted-chart.md) | Hand-painted chart (no QtCharts) | Accepted | 2026-11-23 |
| [009](adr/ADR-009-app-name-only.md) | Set only the application name (no organization name) | Accepted | 2026-11-23 |
| [010](adr/ADR-010-schema-v2-one-migration.md) | Schema v2 in one migration | Accepted | 2026-11-23 |
| [011](adr/ADR-011-phone-first.md) | Phone-first: Android, web (WASM) and Windows | Accepted | 2026-11-23 |
| [012](adr/ADR-012-qml-ui.md) | Qt Quick (QML) for the new UI | Proposed | 2026-11-23 |
| [013](adr/ADR-013-rule-based-planner.md) | Rule-based planner; AI later on top | Accepted | 2026-11-23 |
| [014](adr/ADR-014-derived-game-state.md) | Game state derived from the log | Accepted | 2026-11-23 |
| [015](adr/ADR-015-record-level-merge.md) | Record-level merge on top of TickTimer's revisioned protocol | Accepted | 2026-11-23 |
| [016](adr/ADR-016-qhttpserver-behind-caddy.md) | QHttpServer behind Caddy on loopback | Proposed | 2026-11-23 |
| [017](adr/ADR-017-systemd-no-docker.md) | systemd, no Docker, on the existing VPS | Accepted | 2026-11-23 |
| [018](adr/ADR-018-risk-driven-order.md) | Risk-driven milestone order | Accepted | 2026-11-23 |
| [019](adr/ADR-019-personal-docs-out-of-repo.md) | Personal training documents stay out of the public repo | Accepted | 2026-11-23 |
| [020](adr/ADR-020-photo-envelope-encryption.md) | Progress photos: envelope encryption | Accepted | 2026-11-23 |
| [021](adr/ADR-021-optional-game.md) | Game layer is optional per user | Accepted | 2026-11-23 |
| [022](adr/ADR-022-game-rules-v2.md) | Game rules v2 (evidence-based) | Accepted | 2026-11-23 |
| [023](adr/ADR-023-four-tab-navigation.md) | Four-tab navigation (Today · Log · Progress · You) | Accepted | 2026-11-23 |
| [024](adr/ADR-024-exercise-pictures-drawn-by-code.md) | Exercise pictures drawn by code (form animation + muscle map) | Rejected | 2026-11-23 |
| [025](adr/ADR-025-exercise-pictures-from-chatgpt.md) | Exercise pictures generated with ChatGPT (anime character + muscle diagram) | Accepted | 2026-11-23 |
| [026](adr/ADR-026-body-limitations.md) | Body limitations replace the knee-only stage | Proposed | 2026-11-23 |
| [027](adr/ADR-027-gates-and-triggers.md) | One engine for gates, goal tests and triggers | Proposed | 2026-11-23 |
| [028](adr/ADR-028-daily-stretch.md) | Daily stretch — static in the morning, moving before play | Proposed | 2026-11-23 |
| [029](adr/ADR-029-cardio-program.md) | Cardio program from a talk-test baseline | Proposed | 2026-11-23 |
| [030](adr/ADR-030-sport-profile-and-prehab.md) | Sport profiles and prehab blocks as data | Proposed | 2026-11-23 |
| [031](adr/ADR-031-exercise-choice-filter-then-score.md) | Exercise choice — safety filters first, then a score | Proposed | 2026-11-23 |

**Proposed** decisions are waiting for evidence: ADR-012 (QML) on the I4
spike, ADR-016 (QHttpServer) on F15.

---

## 10. Quality requirements

| # | Scenario | Measure | Test |
|---|---|---|---|
| Q1 | Owner logs a repeat set mid-workout | ≤ 3 taps or 1 key combo | QA checklist |
| Q2 | *Train now* on a phone, offline | Plan shown < 1 s after the last answer | Timed in QA |
| Q3 | App killed while saving | Previous file intact | ✅ `test_services` |
| Q4 | Planner with **any** set of body limitations and any input | Never returns an exercise whose stress on a limited joint is above that stage's limit | Property test over many contexts and limitation sets |
| Q5 | 21 days without training during an exams pause | No decay; streak kept | Catch2 |
| Q6 | Port 8081 probed from the internet | Times out | `curl` from mobile data |
| Q7 | Core built on a machine without Qt | Builds and passes | ✅ CI job 1 |
| Q8 | Same day logged on phone and PC offline, then both sync | All sets present once | Catch2 on `sync::merge` + live test |
| Q9 | Someone with root on the VPS opens the photo store | Sees only ciphertext | Inspect `/var/lib/ironlog/photos/` in QA |
| Q10 | Game off for 60 days of training, then on again | Levels reflect all training; no decay applied for the off period | Catch2 on `game::computeSheet` |
| Q11 | User forgets the password, has the recovery code | All photos back on every device; none re-uploaded | Live test: reset via code, open on a 2nd client |
| Q12 | Pain 6/10 during reverse lunges, worse next morning | Reverse lunge drops one stage for 2 weeks; the next plan uses the stage-below exercise | Catch2 on `body::painAction` + planner |
| Q13 | A red flag (swelling) is reported | That exercise is removed today and the "see a professional" message shows; no XP | Catch2 + QA |
| Q14 | User taps "Not now" on a trigger | The same offer doesn't show again for 7 days | Catch2 on `rules::evaluate` |
| Q15 | Back after a 4-week pause | Week 1 plans ~60 % of normal sets, rising each week (P9) | Catch2 on the planner |

---

## 11. Risks and technical debt

Full register: [05_RiskList.md](05_RiskList.md). Top risks:

| Risk | Mitigation |
|---|---|
| Generated plans cause injury | Rules from written decisions; safety filter for every body limitation; pain monitoring + red flags; gates (Q4, Q12, Q13) |
| A user's condition doesn't fit the rules (R-24) | PAR-Q+ link, conservative stages, offers never automatic, "not medical advice" |
| QML + Android + WASM is new territory | Spike first (backlog I4) |
| Sync merge loses data | Ids, tombstones, pure merge with tests, server backups |
| Gamification backfires (decay feels like punishment) | Game rules v2: 14-day grace, warning, pauses, comeback bonus, floor, weekly streak, game can be turned off |
| Scope is large for one student | Milestones, backlog cap, risk-driven order |

**Technical debt (known)**

- Exercise referenced by **name** (fixed by schema v2 ids).
- Widgets UI will be duplicated by QML until retired.
- Full model resets in table models (fine at current sizes).

---

## 12. Glossary

See [04_Glossary.md](04_Glossary.md).

---

## Sources

- arc42 template: https://arc42.org
- [Feature research round 2](research/feature-research-2.md) (game rules v2, pictures), [UI research](research/ui-research.md) (design system v1, 4 tabs) and [plan-to-features](research/plan-to-features.md) (body limitations, gates, cardio, sport).
- Larman, *Applying UML and Patterns* (3rd ed.): Ch. 8 (risk-driven
  iterations), Ch. 13 (Layers), Ch. 17 (Expert, Creator, Controller),
  Ch. 25 (Pure Fabrication).
- Bass, Clements, Kazman, *Software Architecture in Practice* (4th ed.), Ch. 8 §8.4 (Layers).
- Stroustrup, *The C++ Programming Language* (4th ed.), §16.2.4, §17.6.3.2.
- TickTimer repo (owner's): `docs/ROLLOUT.md` §2, §2i; `docs/design-addendum-sync.md`;
  `docs/design-addendum-deployment.md` §H; `docs/SERVER.md`; `deploy/`.
- Owner's *Fitness Plan* and *Physical Decision Report* (2026-11-09) — rule
  sources for §8.5. Kept privately, not in the repo.
- Feature research: [research/feature-research.md](research/feature-research.md).
- Qt HTTP Server limitations: https://doc.qt.io/qt-6/qthttpserver-index.html
