# 09 — Design Notes (v0.1, archived)

> **Archived 2026-11-23.** Superseded by [../arc42.md](../arc42.md). Kept
> because ADR-001 to ADR-009 in arc42 §9 summarise the decisions below in
> one line each; the full reasoning lives here. Links in this file are
> relative to the old location and may be stale.

> **What this is**: How IronLog is built and *why*: layers, class
> responsibilities, data flow, file format, and the decisions behind them.
> This is the project's architecture + design + data-model doc in one file
> (see `08_DevelopmentCase.md` for why one file is enough).

Diagrams (PNG versions in `diagrams/rendered/`):

| Diagram | Shows |
|---|---|
| [architecture.puml](../diagrams/architecture.puml) | The three layers and which way dependencies point |
| [class_diagram.puml](../diagrams/class_diagram.puml) | Core + services classes, composition, the SetEntry invariant |
| [sequence_add_set.puml](../diagrams/sequence_add_set.puml) | UC-01 step by step, including the signal fan-out |

---

## 1. Layers

```
ui/        Qt Widgets   MainWindow, table models, ProgressChart, dialog
  │ uses
services/  Qt Core      TrainingController (application), JsonStore (persistence)
  │ uses
core/      pure C++17   Date, Exercise, SetEntry, Workout, TrainingLog, stats::
```

**Rule**: arrows only point down. `core/` never includes Qt. `services/`
never includes Widgets. CMake enforces it: each layer is its own target and
only links what's below it.

**Why**: Larman's Layers pattern (Ch. 13) separates UI work from domain work
so domain logic can be reused and changed without ripple effects. *Software
Architecture in Practice* (Ch. 8, "Layers Pattern") adds the key constraint:
the allowed-to-use relation must be **unidirectional** — upward use isn't
allowed.

**How the UI hears about changes without an upward dependency**: Qt
signals. `TrainingController` emits `logChanged()`; it doesn't know who's
listening. The UI connects to it. That's the observer pattern — the same
shape as the finance tracker's `DatabaseManager::ledgerChanged`.

**One deliberate shortcut — layer bridging**: the table models and the
window call `stats::` (core) directly for read-only queries, skipping the
services layer. SAiP names this "layer bridging" and warns that too much of
it hurts modifiability. It's acceptable here because it's **read-only**;
every *write* still goes through the controller.

---

## 2. Class responsibilities

| Class | Layer | Responsibility | Pattern / reason |
|---|---|---|---|
| `Date` | core | Calendar math: validity, day numbers, weekday, Monday-of-week, ISO text | `struct` — plain value, no invariant to guard (Stroustrup §16.2.4) |
| `Exercise` | core | Name + primary muscle | `struct` — plain data |
| `ExerciseLibrary` | core | Knows all exercises; enforces unique, trimmed names | Information Expert for "does this exercise exist?" |
| `SetEntry` | core | One set; knows its volume and e1RM | `class` with invariant; private ctor + `make()` (Stroustrup §17.6.3.2) |
| `Workout` | core | One day's sets; knows its total volume | Information Expert — like Larman's `Sale.getTotal()` |
| `TrainingLog` | core | All workouts, sorted, one per day; get-or-create | Creator — it contains workouts, so it makes them |
| `stats::` | core | e1RM history, PRs, last set, weekly sets, classification | Free functions — keeps `TrainingLog` small (see §4.3) |
| `sample::demoLog` | core | Deterministic 8-week demo history | Pure function of `today` → testable |
| `JsonStore` | services | Save/load the whole `AppData` as JSON | Pure Fabrication (Larman Ch. 25) |
| `TrainingController` | services | Every write: validate → update → save → signal | GRASP Controller (Larman Ch. 17) |
| `QtConvert` | services | QDate↔Date, QString↔std::string, number formatting | Adapter seam — conversions live in one file |
| `WorkoutTableModel` | ui | One day's sets as table rows, PR flags | Qt model/view |
| `WeeklyVolumeModel` | ui | 10 muscle rows with counts and status | Qt model/view |
| `ProgressChart` | ui | Paints the e1RM line chart | Custom `paintEvent()` |
| `AddExerciseDialog` | ui | Collects name + muscle | Modal `QDialog` |
| `MainWindow` | ui | Layout, menus, turns clicks into controller calls | UI only — no training logic |

---

## 3. Data flow — logging a set (UC-01)

See `sequence_add_set.puml`. In short:

1. `MainWindow` → `TrainingController::addSet(date, name, kg, reps)`
2. Controller looks up the exercise (gets the library's spelling).
3. `SetEntry::make()` validates. Empty `optional` → `InvalidNumbers`, stop.
4. `TrainingLog::workoutOn(date).addSet(set)` — get-or-create keeps order.
5. `JsonStore::save()` — atomic write via `QSaveFile`.
6. `emit logChanged()` → both models reload, window refreshes calendar,
   headers, chart.
7. Back in `MainWindow`: is the last row a PR? → status-bar message.

Signals here are **direct connections** (same thread), so all of step 6
finishes before `addSet()` returns. That's why step 7 can read the
freshly-reloaded model.

---

## 4. Key decisions

### 4.1 Own `Date` type instead of `QDate`

- **Chosen**: a 40-line `Date` struct in core.
- **Alternative**: use `QDate` everywhere.
- **Why**: keeps core free of Qt → builds and tests anywhere, reusable.
  `std::chrono::year_month_day` would do it, but that's C++20.
- **Trade-off**: a conversion at the boundary (`QtConvert.h`), and our own
  date math to test (done: round-trip, leap years, week boundaries).

### 4.2 `SetEntry`: private constructor + `std::optional` factory

- **Chosen**: `static std::optional<SetEntry> make(...)`.
- **Alternatives**: public constructor that throws; public struct with an
  `isValid()` check.
- **Why**: bad form input is *expected*, not exceptional, so no exceptions.
  A plain struct would let invalid sets exist. With `make()`, an invalid
  `SetEntry` **cannot be created**, and `optional` forces callers to check.
- **Contrast**: TickTimer's "doors" refused by returning an empty id string —
  easy to ignore. `optional` puts the refusal in the type.

### 4.3 Analysis as free functions in `stats::`

- **Chosen**: `stats::personalRecordIndices(log, workout)` etc.
- **Alternative**: methods on `TrainingLog`.
- **Why**: Information Expert points at `TrainingLog`, but Larman lists the
  contraindication: piling unrelated work on one class lowers cohesion.
  These functions only need the public interface, so they don't need to be
  members. `TrainingLog` stays small, with one job: keep its invariant.

### 4.4 Persistence as a Pure Fabrication

- **Chosen**: `JsonStore`, a class that isn't a training concept.
- **Alternative**: `TrainingLog::saveTo(path)`.
- **Why**: Larman Ch. 25 uses this exact case (saving a `Sale`): putting
  persistence in the domain class couples it to a storage technology and
  lowers cohesion. A fabricated store keeps the domain pure and makes
  storage swappable (SQLite later, same interface).

### 4.5 One controller for all writes

- **Chosen**: every change goes through `TrainingController`.
- **Why**: one place to validate, save, and announce. Views can't get out
  of sync, because none of them changes data — they only listen.
- **Trade-off**: the controller grows with each feature. If it gets big,
  split by use case (e.g. a separate `ExerciseController`).

### 4.6 Models hold a snapshot, not a pointer

- `WorkoutTableModel` **copies** the day's sets on `reload()`.
- **Why**: `TrainingLog` stores workouts in a `std::vector`. Adding a new
  day can reallocate it, which would leave a stored pointer dangling. A
  copy can't dangle. Cost: ~30 small objects per reload — nothing.

### 4.7 Full model reset instead of row-level updates

- `beginResetModel()` / `endResetModel()` on every change.
- **Alternative**: `beginInsertRows()` for the one new row.
- **Why**: simpler and impossible to get out of sync. At ≤ 50 rows the
  speed difference is invisible. Revisit if a model ever shows thousands.

### 4.8 Hand-painted chart instead of QtCharts

- **Why**: QtCharts is an optional add-on module (and being replaced by
  QtGraphs) — another install step that can break a fresh build. The chart
  is ~150 lines of `QPainter`, and writing it shows how every widget draws.
- **Detail**: X positions use the **date**, not the point index, so a
  two-week break shows as a real gap.

### 4.9 `double` for kilograms (contrast: integer cents in finance tracker)

- Money needs exact sums, so the finance tracker stored integer cents.
- Weights are never compared for exact equality and only displayed rounded,
  so `double` is fine. `SetEntry::make()` rejects NaN/infinity, which would
  otherwise poison every comparison.

### 4.10 Exercises referenced by name

- **Why**: simplest possible link, readable JSON.
- **Risk**: renaming would orphan old sets (R-04). Rename isn't offered yet.
  When it is: add stable IDs and migrate with schema v2.
- All name comparisons go through `sameExerciseName()` (case-insensitive) —
  one rule, one place.

### 4.11 No wall clock inside the core

- Functions take the date as a parameter (`weeklySets(log, lib, anyDay)`,
  `demoLog(today)`). Only `ui/` reads `QDate::currentDate()`.
- **Why**: TickTimer had a test that failed only after 5 PM because one
  path read the real clock. Passing time in makes every rule testable at
  any hour.

### 4.12 Strict about the file, tolerant about items

- Bad JSON, wrong shape, or a newer `schemaVersion` → refuse the whole file
  (`Unreadable`), move it aside, start fresh, tell the user.
- One bad set or exercise inside a good file → skip it, count it, load the rest.
- **Why**: guessing at a file you don't understand is how apps destroy data.
  But one typo shouldn't cost a year of history.

### 4.13 Application name only, no organization name

- `QStandardPaths::AppDataLocation` is `<organization>/<application>`.
  Setting both to "IronLog" would give `.../IronLog/IronLog/` (TickTimer
  hit this). So `main.cpp` sets only the application name.
- Consequence: the app name **is** the data folder name. Renaming the app
  needs a migration that copies the old file (Risk R-10).

---

## 5. Domain rules (exact)

| Rule | Definition | Where |
|---|---|---|
| Set volume | `kg × reps` | `SetEntry::volume()` |
| e1RM (Epley) | `kg × (1 + reps/30)`; for 1 rep, `kg` | `epleyOneRepMax()` |
| Session best | highest e1RM among that day's sets of the exercise | `stats::` (internal `dayBest`) |
| PR | the **first** set with the day's top e1RM, if it is **strictly greater** than the best of every **earlier day**. No earlier day → not a PR. | `stats::personalRecordIndices()` |
| Week | Monday–Sunday containing the chosen day | `Date::mondayOfWeek()` |
| Weekly sets | count of sets whose exercise's primary muscle = M, inside the week. Unknown exercises skipped. | `stats::weeklySets()` |
| Status | `< low` Below; `> high` Above; else On target. Default 10–20. | `stats::classify()` |

**About the 10–20 default.** A 2017 meta-analysis (Schoenfeld et al.) found
better hypertrophy with more than ~9 weekly sets per muscle, and a 2022
systematic review (Baz-Valle et al.) suggests 12–20 weekly sets as a
reasonable range. 10–20 is a common starting point, not a rule — it's a
parameter (`stats::VolumeTarget`) so it can become a user setting.

---

## 6. File format — schema v1

```json
{
  "schemaVersion": 1,
  "exercises": [
    { "name": "Bench Press", "muscle": "Chest" }
  ],
  "workouts": [
    { "date": "2026-11-23",
      "sets": [ { "exercise": "Bench Press", "kg": 100, "reps": 5 } ] }
  ]
}
```

- `muscle` is one of the 10 `MuscleGroup` names.
- `date` is strict ISO `YYYY-MM-DD`.
- Empty days are never written.
- **Versioning**: bump `kSchemaVersion` when the layout changes, and teach
  `fromJson()` to read the old one. Files with a *newer* version are refused.

---

## 7. Extension seams

| Future feature | Touches | Notes |
|---|---|---|
| Undo/redo | `services/` | Wrap each controller action in a `QUndoCommand`. Views unchanged — they already reload on `logChanged()`. |
| Edit a set | `core/Workout`, controller, `WorkoutTableModel` | Add `Workout::replaceSet()`; model gets `flags()` + `setData()`. |
| SQLite storage | `services/` only | New store class with the same `load()`/`save()` shape. Core and UI unchanged. |
| Secondary muscles | `core/Exercise`, `stats::weeklySets` | Counts become `double` (half-sets). Schema v2. |
| Mobile UI (QML) | new `qml/` layer | Reuses `core/` and `services/` untouched. |

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.): Ch. 13 (Layers pattern,
  Fig. 13.4); Ch. 17 (Information Expert, Creator, Controller, Low Coupling);
  Ch. 25 §25.2 (Pure Fabrication — "saving a Sale in a database").
- Bass, Clements, Kazman, *Software Architecture in Practice* (4th ed.),
  Ch. 8 §8.4 (Layers pattern: unidirectional allowed-to-use, layer bridging).
- Stroustrup, *The C++ Programming Language* (4th ed.): §16.2.4 (class vs
  struct), §17.6.3.2 (establish invariants in constructors), §15.4 (`main`).
- Date algorithms: Howard Hinnant, *chrono-Compatible Low-Level Date
  Algorithms* — https://howardhinnant.github.io/date_algorithms.html
- Weekly volume: Baz-Valle et al. (2022), *J Hum Kinet* 81:199–210,
  https://pubmed.ncbi.nlm.nih.gov/35291645/ ; Schoenfeld et al. (2017) as
  summarised in https://pmc.ncbi.nlm.nih.gov/articles/PMC8884877/
- Qt specifics (QSaveFile, model/view, QStandardPaths): Qt 6 documentation
  (general knowledge, not from the project books).
