# IronLog — Question Bank

Self-review questions about this project, each with an answer. For review
on your own, or to pull from when you *ask* to be quizzed.

**Depth key**: 🟢 recall · 🟡 explain · 🔴 trade-off / spot the issue

**Sections**: [F Features](#f--features) · [A Architecture](#a--architecture) ·
[D Design decisions](#d--design-decisions) · [C C++](#c--c-language) ·
[Q Qt](#q--qt) · [T Testing](#t--testing) · [G Diagrams](#g--diagrams) ·
[P Process & docs](#p--process--docs) · [S Spot the issue](#s--spot-the-issue) ·
[V Vision v2 & architecture](#v--vision-v2--architecture-planning-2026-11-23)

---

## F — Features

**F1 🟢 What does IronLog do, in one sentence?**
> Logs bodybuilding sets (exercise, kg, reps) per day and shows PRs, weekly
> sets per muscle vs. a target, and estimated-1RM progress over time.

**F2 🟢 What formula gives the estimated 1RM, and what's the special case?**
> Epley: `kg × (1 + reps / 30)`. For 1 rep, e1RM = the weight itself.
> Example: 100 kg × 10 → 100 × 1.333 = **133.3 kg**.

**F3 🟡 Exactly when is a set marked PR?**
> For each exercise in a session, take the **first** set with the day's top
> e1RM. It's a PR if that e1RM is **strictly greater** than the best from
> every **earlier day**. The exercise's first-ever session is never a PR.

**F4 🟡 Why isn't the first-ever session of an exercise a PR?**
> There's nothing to beat. Flagging every set on day one would make the
> badge meaningless. A PR should mean "you improved."

**F5 🟢 What week does the Weekly volume tab show?**
> The Monday–Sunday week containing the day selected on the calendar.

**F6 🟡 How does a set count toward weekly volume? What's skipped?**
> +1 for its exercise's *primary* muscle group. Sets whose exercise isn't in
> the library are skipped, because the muscle can't be known.

**F7 🟡 Where does the 10–20 sets target come from, and is it fixed?**
> Reviews find better growth above ~10 weekly sets per muscle; one suggests
> 12–20. It's a common starting range, stored as `stats::VolumeTarget` so it
> can become a user setting — not hard-coded logic.

**F8 🟢 Why can't you click a future date on the calendar?**
> `setMaximumDate(QDate::currentDate())`. Logging the future would make
> "best before this day" comparisons misleading.

**F9 🟡 What pre-fills when you pick an exercise, and from where?**
> Weight and reps of the most recent set of that exercise
> (`stats::lastSet`). No history → 20 kg (empty bar) and 10 reps.

**F10 🟡 Why can sample data only load into an empty log?**
> So it can never overwrite real training. The rule lives in the controller
> (`loadSampleData` returns false), not just the UI, and a test pins it.

---

## A — Architecture

**A1 🟢 Name the three layers, their folders, and what each may include.**
> `core/` pure C++17, no Qt · `services/` Qt Core, no Widgets · `ui/` Qt
> Widgets. Dependencies point down only: ui → services → core.

**A2 🟡 How does CMake enforce the layering?**
> Each layer is its own target. `ironlog_core` links nothing Qt;
> `ironlog_services` links only `Qt6::Core`; only `IronLog` links
> `Qt6::Widgets`. Including a Widgets header in services fails to compile.

**A3 🟡 If arrows only point down, how does the UI learn that data changed?**
> Qt signals. `TrainingController` emits `logChanged()` without knowing who
> listens; UI objects connect to it. That's the observer pattern — the
> dependency is still UI → services.

**A4 🔴 The models call `stats::` (core) directly. Is that a layering violation?**
> It skips the services layer — SAiP calls this **layer bridging**. It's
> allowed in "relaxed" layering but too much hurts modifiability. Acceptable
> here because it's read-only; every write still goes through the controller.

**A5 🟡 Trace UC-01 "Log a set" through the classes.**
> MainWindow → `TrainingController::addSet` → `ExerciseLibrary::find` →
> `SetEntry::make` → `TrainingLog::workoutOn().addSet` → `JsonStore::save` →
> `emit logChanged()` → models reload, window refreshes → MainWindow checks
> `isRecord(lastRow)` → status bar.

**A6 🔴 Why does MainWindow see the new row right after `addSet()` returns?**
> The signal connections are **direct** (same thread), so all slots run
> *inside* `emit`, before `addSet()` returns. With a queued connection
> (e.g. cross-thread) the model wouldn't have reloaded yet.

**A7 🟡 What would you change to store data in SQLite instead of JSON?**
> Only `services/`: a new store with the same `load()`/`save()` shape, and
> the controller uses it. Core and UI don't change. That's the payoff of the
> Pure Fabrication + layers.

**A8 🟡 What does the core gain from having no Qt?**
> Builds and tests anywhere (`-DIRONLOG_BUILD_APP=OFF`), compiles fast,
> and can be reused by another UI (QML, mobile) or a command-line tool.

---

## D — Design decisions

**D1 🟡 Which GRASP pattern is `TrainingController`, and what does it buy?**
> Controller (Larman Ch. 17). One entry point per user action: validate →
> update → save → signal. Views can't drift out of sync because none of them
> changes data.

**D2 🟡 Which GRASP pattern is `JsonStore`? Why not `TrainingLog::save()`?**
> Pure Fabrication (Larman Ch. 25). Expert would put saving in the domain
> class, but that couples it to a file format and Qt's JSON — low cohesion,
> high coupling. A made-up "store" class keeps the domain pure and swappable.

**D3 🟡 Where does Information Expert show up?**
> `SetEntry` computes its own volume and e1RM; `Workout` sums its sets'
> volume (like Larman's `Sale.getTotal()`); `ExerciseLibrary` answers "does
> this exercise exist?"

**D4 🔴 Expert says `TrainingLog` should compute stats. Why free functions instead?**
> Piling every report onto one class makes it a low-cohesion "god class" —
> Larman lists this as Expert's contraindication. The functions only need the
> public interface, so they live in `stats::` and `TrainingLog` keeps one job:
> its sorted, one-per-day invariant.

**D5 🟡 Which GRASP pattern explains `TrainingLog::workoutOn()` creating workouts?**
> Creator: the log *contains* workouts, so the log creates them. It also
> inserts in sorted position to keep its invariant.

**D6 🔴 Why `double` for kg here, but integer cents in the finance tracker?**
> Money needs exact sums and equality, and floats can't represent 0.10
> exactly. Weights are never compared for exact equality and are shown
> rounded, so `double` is fine. NaN/∞ are rejected in `make()`.

**D7 🔴 Exercises are linked by name. What breaks, and what's the fix?**
> Renaming an exercise would orphan its old sets (R-04). Fix: stable IDs in
> the file + schema v2 migration. Rename isn't offered until then.

**D8 🟡 Why one session per day?**
> Simplest model; the calendar maps one day → one session. Two-a-days merge.
> Documented assumption in the Vision.

**D9 🟡 Why does the app refuse a file with a newer `schemaVersion`?**
> It can't know what the new fields mean. Guessing then auto-saving could
> silently drop data. Refuse, move it aside, tell the user.

**D10 🟡 "Strict about the file, tolerant about items" — what does that mean?**
> Bad JSON/shape/version → reject the whole file. One bad set inside a good
> file → skip it, count it, load the rest. One typo shouldn't cost a year.

**D11 🔴 Why hand-paint the chart instead of QtCharts?**
> QtCharts is an optional add-on (being replaced by QtGraphs) — one more
> install step that can break a fresh build. The chart is ~150 lines of
> `QPainter`. Trade-off: no zoom/tooltips for free.

**D12 🟡 Why does the chart's X axis use dates, not point indexes?**
> So time gaps are real: two weeks off shows as a wide gap, not a flat step.

**D13 🟡 Why is the org name not set in `main.cpp`?**
> `AppDataLocation` is `<org>/<app>`; both "IronLog" → doubled
> `IronLog/IronLog/`. Setting only the app name gives `AppData/Roaming/IronLog/`.

---

## C — C++ language

**C1 🟢 Why is `Date` a `struct` but `SetEntry` a `class`?**
> Stroustrup §16.2.4: `struct` for simple data structures, `class` for a
> type with an invariant. `Date` is plain data; `SetEntry` must keep
> 0 ≤ kg ≤ 1000 and 1 ≤ reps ≤ 100.

**C2 🟡 How does `SetEntry` guarantee an invalid set can't exist?**
> Private constructor + `static std::optional<SetEntry> make(...)`. Only
> `make()` can construct one, and it checks first. Stroustrup §17.6.3.2:
> establish the invariant at construction.

**C3 🔴 Why `std::optional` instead of throwing an exception?**
> Bad form input is *expected*, not exceptional. `optional` puts "may be
> refused" in the return type, so callers must check — unlike TickTimer's
> empty-string id, which was easy to ignore.

**C4 🟢 What does `enum class` give over a plain `enum`?**
> Scoped names (`MuscleGroup::Chest`, no namespace pollution) and no
> implicit conversion to `int`.

**C5 🟡 What is the erase–remove idiom (`TrainingLog::pruneEmpty`)?**
> `remove_if` moves keepers to the front and returns the new logical end;
> `erase` deletes from there to the real end. Two steps because algorithms
> can't change a container's size.

**C6 🟡 Why is `lower_bound` used in `TrainingLog`?**
> Binary search on the sorted vector: O(log n) to find a date or its
> insertion point, keeping the vector sorted without ever calling `sort()`.

**C7 🔴 `auto& w = log.workoutOn(d1); log.workoutOn(d2); w.addSet(s);` — bug?**
> Yes. The second `workoutOn` may insert into the `std::vector` and
> reallocate it, leaving `w` dangling → undefined behaviour. Re-fetch after
> any insert. (This is why the table model stores a *copy* of the sets.)

**C8 🟡 How does `TrainingLog::find()` avoid duplicating code for const and non-const?**
> The non-const version calls the const one and `const_cast`s the result.
> Safe because `*this` is non-const in that call.

**C9 🟢 Why `std::isfinite` in `SetEntry::make`?**
> Rejects NaN and ±∞. A NaN would poison sums and make every comparison
> false (`NaN > x` is always false), breaking PR logic silently.

**C10 🟡 Why are the date algorithms in a `.cpp` but `addDays()` inline in the header?**
> Short one-liners are fine inline in headers; longer definitions go in
> `.cpp` files (Stroustrup Ch. 15 — headers hold declarations and small
> inline definitions). Keeps rebuilds small when the algorithm changes.

---

## Q — Qt

**Q1 🟢 Which three functions must a `QAbstractTableModel` subclass implement?**
> `rowCount()`, `columnCount()`, `data()`. (`headerData()` is optional but
> usual.)

**Q2 🟡 Why do `rowCount`/`columnCount` return 0 when `parent.isValid()`?**
> A valid parent means "children of a cell." Tables have none. Without the
> check, a tree view would think every cell has children.

**Q3 🟡 What do `beginResetModel()`/`endResetModel()` tell the view? Trade-off?**
> "Forget everything, re-ask me." Simple and can't go out of sync. Cost:
> selection and scroll position reset. Fine at ≤ 50 rows.

**Q4 🔴 The Delete button listens to `modelReset` *and* `selectionChanged`. Why both?**
> A model reset clears the selection **without** emitting
> `selectionChanged`. Listening only to that would leave the button enabled
> with nothing selected.

**Q5 🟡 What does `QSaveFile` do differently from `QFile`?**
> Writes to a temporary file and only replaces the real file on
> `commit()`. A crash mid-write leaves the old file intact.

**Q6 🟢 Why must every `Q_OBJECT` header be listed in CMake?**
> AUTOMOC only runs `moc` on files it can see. Unlisted → no generated
> vtable/signal code → "undefined reference to vtable" at link time.

**Q7 🟡 Why `update()` and not calling `paintEvent()` directly?**
> `update()` schedules a repaint; Qt merges multiple requests and calls
> `paintEvent` with a ready `QPainter` target. Calling it yourself skips
> that machinery.

**Q8 🟡 Why do the chart and tables use palette colours or translucent fills?**
> So they stay readable in light and dark themes. Hard-coded black text on a
> dark palette would vanish.

**Q9 🟡 Why `blockSignals(true)` while refilling the combo boxes?**
> Each `addItem`/`clear` can emit `currentIndexChanged`, triggering a
> refresh per item. Blocking avoids N redundant refreshes; we refresh once.

**Q10 🟡 Why doesn't `tr("%n set(s)", nullptr, n)` pluralise here?**
> Qt picks the plural form from a loaded translation file. With none, it
> prints the source text — `(s)` included. We use `countOf()` instead.

**Q11 🟢 What does `WIN32_EXECUTABLE ON` do?**
> Builds a Windows GUI-subsystem exe: no extra console window behind the app.

**Q12 🟡 Why do widgets created with `new` and no `delete` not leak?**
> They get a parent (directly or via a layout). Qt's parent–child ownership
> deletes children when the parent is destroyed.

---

## T — Testing

**T1 🟢 What do the two test executables cover?**
> `test_core`: date math, SetEntry rules, e1RM, library, log ordering, PRs,
> weekly sets, sample data. `test_services`: JSON round-trip, bad files,
> item skipping, controller actions, corrupt-file backup.

**T2 🟡 Which test proves the app never overwrites a corrupt file? How?**
> "Controller: a corrupt file is moved aside, never overwritten." It writes
> garbage, opens a controller, checks a `*.unreadable-*` backup exists with
> the **same bytes**, then checks a new save produces a valid file.

**T3 🟡 Why is Catch2's `main()` in its own OBJECT library?**
> `catch.hpp` is huge. Compiling `main()` once and sharing the object file
> means editing a test doesn't recompile Catch2's runner.

**T4 🔴 Which boundaries does the weekly-sets test pin, and why those?**
> Sunday *before* (excluded), Monday (included), Sunday (included), next
> Monday (excluded), plus an unknown exercise (skipped). Off-by-one at week
> edges is the classic bug.

**T5 🟡 Why can the core tests never fail "at a certain time of day"?**
> No core function reads the clock; the date is always a parameter.
> (TickTimer had a test that failed after 5 PM because one path did.)

**T6 🟡 Why does `test_services` not need `QT_QPA_PLATFORM=offscreen`?**
> It links Qt Core only — no GUI, so no platform plugin is loaded.

---

## G — Diagrams

**G1 🟢 What do solid vs. dotted arrows mean in `architecture.puml`?**
> Solid = "uses" (compile-time dependency, always downward). Dotted =
> Qt signal notification (runtime, the sender doesn't know the receiver).

**G2 🟢 What does the filled diamond mean in `class_diagram.puml`?**
> Composition: the part lives and dies with the whole (a `Workout` owns its
> `SetEntry`s; destroying the workout destroys them).

**G3 🟡 Why is `SetEntry → Exercise` a dotted "refers to by name" arrow, not an association?**
> `SetEntry` stores a name string, not a pointer/reference to an
> `Exercise` object. It's a loose, lookup-based link — which is exactly why
> renaming is risky (R-04).

**G4 🟡 In `sequence_add_set.puml`, what does the "signal fan-out" box show?**
> One `emit logChanged()` calls three receivers in turn (two models and the
> window) — synchronously, before `addSet` returns.

**G5 🟡 Which extensions does the sequence diagram list, and which use-case steps do they map to?**
> UnknownExercise → 3a (exercise not in the list), InvalidNumbers → 6a
> (kg/reps out of range), saveFailed → 7a (disk full / no permission).
> Diagram and use case use the same numbers so they can be read together.

---

## P — Process & docs

**P1 🟢 Which docs were written first, and why the Vision?**
> Vision first (Larman Ch. 7): a short "what and why" before details.
> Then use-case names, supplementary spec, glossary, risks.

**P2 🟡 Why are only UC-01 and UC-04 "fully dressed"?**
> Larman (Ch. 4 §4.3): during inception, name most use cases but detail only
> 10–20% — the riskiest or most central ones.

**P3 🟡 Why does `08_DevelopmentCase.md` opt out of a separate Architecture Document?**
> UP artifacts are optional (Ch. 2 §2.12, "pharmacy" analogy). At v0.1 size,
> one `09_DesignNotes.md` held architecture, design and data model. Once a
> server, three platforms and CI/CD were planned, it was replaced by
> `arc42.md` (archived copy kept) — see V12.

**P4 🟡 Which risks did iteration 1 attack, and why first?**
> R-01 data loss, R-02 wrong stats, R-05 toolchain. UP is risk-driven
> (Ch. 8 §8.3): attack what could sink the project before adding features.

**P5 🟢 What are the four parts of a troubleshooting entry?**
> SYMPTOM (exact text) · CAUSE · FIX · PREVENT.

---

## S — Spot the issue

**S1 🔴 A teammate adds `#include <QDate>` to `core/Workout.h`. Problem?**
> Breaks the layering: core must not depend on Qt. `test_core` and
> `-DIRONLOG_BUILD_APP=OFF` would fail on a machine without Qt. Use
> `ironlog::Date` and convert in `QtConvert.h`.

**S2 🔴 `stats::weeklySets` changes to use `QDate::currentDate()` instead of an `anyDay` parameter. Problem?**
> Hidden clock dependency: tests become time-dependent, and the user can no
> longer view a past week. Keep time as a parameter.

**S3 🔴 A new feature saves by calling `JsonStore::save()` from `MainWindow`. Problem?**
> Bypasses the controller: no `logChanged()` signal, so other views go stale,
> and validation might be skipped. All writes go through `TrainingController`.

**S4 🔴 `WorkoutTableModel` is changed to store `const Workout*` instead of copying the sets. What can go wrong?**
> Adding a different day can reallocate `TrainingLog`'s vector → the pointer
> dangles → crash or garbage, maybe only sometimes. Copy, or re-find on use.

**S5 🔴 A new `Q_OBJECT` class `RestTimer` is added with only its `.cpp` in CMake. Symptom?**
> Link error: `undefined reference to vtable for RestTimer` (moc never ran on
> the header). Fix: add the `.h` to the target's sources.

**S6 🔴 On load failure, the controller just starts with an empty log (no rename). Why is that dangerous?**
> The first save overwrites the unreadable file → the user's history is
> gone. Moving the file aside first turns "can't read" into "kept safe."

**S7 🔴 PR check changed from `top > *previous` to `top >= *previous`. Effect?**
> Matching your old best would count as a PR. Repeating last week's top
> set every week would show a trophy every week — the badge loses meaning.

**S8 🔴 What would you change next, and what would it touch?**
> Example: Undo/redo — wrap each controller action in a `QUndoCommand`
> (services only); views already reload on `logChanged()`, so UI barely
> changes. Command pattern, and it lowers the cost of the no-confirm Delete.

---

## V — Vision v2 & architecture (planning, 2026-11-23)

*Added after the planning session. Sources: `docs/01_Vision.md`,
`docs/arc42.md`, `docs/BACKLOG.md`.*

**V1 🟢 What is IronLog v2 in one sentence?**
> A phone-first training coach that plays like an MMO: plans today's
> workout around time, place and situation, levels up 8 body skills, and
> rewards consistency — synced to the owner's VPS.

**V2 🟡 Walk through *Train now* for a cramming night.**
> Time (e.g. 25 min) → situation *Cramming* → "what's nearby?" checklist
> (pre-ticked from last time) → short, compound-first plan, no PR attempts,
> counted toward the exam-week minimum → *Start* opens the guided session.

**V3 🔴 Why is the planner rule-based instead of AI?**
> Safety (QG1), explainability (QG4) and testability. The owner's decision
> report is already a rule set (pain rule, gates, exam minimum). A pure
> function can be tested to *never* break a constraint. AI can come later
> to explain or tune plans *inside* those rules (ADR-013).

**V4 🟡 Name the planner rules P1–P7 in a few words each.**
> P1 session type from what the week needs · P2 time budget (warm-up, main,
> stretch) · P3 equipment ⊆ available, knee stress ≤ stage, compound first ·
> P4 double progression · P5 situation modifiers · P6 progression gates ·
> P7 pain check drops a stage for 2 weeks.

**V5 🔴 Why are XP and levels derived from the log instead of stored?**
> Nothing to sync or corrupt; every device computes the same sheet; the
> balance table can change and history is re-scored. Cost: recompute on
> open, which is trivial (ADR-014).

**V6 🔴 Decay could punish the owner during exams. How is that prevented?**
> *(Updated for game rules v2.)* 14-day grace per skill with a 3-day
> warning, a Pause (exams ≤ 21 days, also sick / injured / travel) stops
> decay, a floor at 50 % of peak XP, and the comeback bonus doubles XP while
> climbing back. During an exams pause the quest drops to the exam minimum.

**V7 🟡 How does a rest token get earned, and why a bank cap of 2?**
> *(Updated for game rules v2 — it replaced the cheat-meal token.)*
> Completing the weekly quest earns one; it's spent automatically to keep a
> missed week in the streak. The cap keeps it a reward for consistency, not
> a stockpile — the same "bank max 2" Duolingo uses for streak freezes.

**V8 🔴 TickTimer asks a human on every sync conflict. Why does IronLog merge automatically?**
> A planner is *edited*; a training log is *appended*. Two devices adding
> different sets is not a real conflict — the union is correct. Only the
> same record edited on both sides needs a rule (newer `updatedAt` wins;
> tombstones beat older edits). Same wire protocol, smarter client (ADR-015).

**V9 🟡 Why must schema v2 (ids, tombstones) come before sync?**
> Merge needs a stable identity per record to tell "same set" from "new
> set", and deletes must travel as tombstones. The research also advised
> one migration instead of five (RPE, set types, notes, ids, secondary muscles).

**V10 🔴 Why Qt Quick (QML) for the new UI, and why is ADR-012 only "proposed"?**
> Phone-first touch UI, animations, runs on Android, WASM and desktop; core
> and services stay unchanged. TickTimer showed Widgets can work on phones
> but needed a long responsive-layout effort. "Proposed" until the I4 spike
> proves it on the owner's actual phone and browser.

**V11 🔴 Qt's docs warn against exposing QHttpServer to the internet. Why is it OK here?**
> It isn't exposed: it binds 127.0.0.1, and Caddy is the only client. Caddy
> handles TLS and parses public traffic. That's the "trusted environment"
> the docs require (ADR-016, R-17).

**V12 🟡 Why replace `09_DesignNotes.md` with arc42?**
> The project grew a server, three platforms and CI/CD. arc42's fixed
> sections (context, deployment, concepts, decision log, quality scenarios)
> make sure none of those is forgotten. The old notes are archived because
> ADR-001–009 summarise them.

**V13 🟡 Where does IronLog run on the VPS, and what's new compared with TickTimer?**
> Same box and pattern: `ironlog.<domain>` → Caddy → `127.0.0.1:8081`
> (TickTimer is on 8080), own systemd unit and user, own data folder, own
> backup timer. New: CD deploys it and verifies `/version`.

**V14 🔴 TickTimer deliberately had no automatic deploy. What changed?**
> Its reason: the value is in the verification after the copy. IronLog's CD
> makes the verification part of the pipeline — the job fails unless
> `/version` reports the new version.

**V15 🟡 Why is the server scheduled *after* the planner and game?**
> Risk-driven ordering (Larman Ch. 8): the server is proven tech from
> TickTimer; QML on phones and the planner rules are the real unknowns.

**V16 🟢 How is "lean" tracked without food logging?**
> A weekly body check-in: body weight + waist, shown as a trend (F18).

**V17 🟡 Why store weight "as entered" with its unit?**
> The owner's dumbbells are in lb. Storing 25 lb as 11.34 kg and converting
> back invites rounding drift and kg/lb mix-ups (a common import bug).
> Analysis converts with 0.45359237; display uses the user's unit.

**V18 🔴 Spot the issue: someone adds the owner's Physical Decision Report PDF to `docs/`.**
> The repo is public; it contains personal health details. ADR-019 keeps
> personal documents out. Rules derived from it are written generically
> (e.g. "knee stage") in code and docs.

**V19 🔴 Spot the issue: the planner reads `QDate::currentDate()` to decide what's due this week.**
> Breaks the "no clock in core" rule (arc42 §8.12): untestable, and plans
> differ by time of day. Pass `today` in.

**V20 🟡 What's in milestone v0.2, and why is the QML spike third?**
> I1 repo, I2 CI, I4 spike, F1 schema v2, F2 catalog v2, F3 edit/undo, F4
> backups/CSV, I3 release CD. CI first protects everything; the spike comes
> before more code is built on a UI decision that might change.

**V21 🟡 Why are progress photos encrypted on the device rather than just sent over HTTPS?**
> HTTPS protects photos in transit only; on the VPS they'd sit readable by
> anyone with root (including the owner, for the partner's photos). Encrypting
> on the device with a key from the user's password means the server stores
> ciphertext only (ADR-020) — the same "dumb server" rule as the log.

**V22 🔴 Trade-off of end-to-end encrypted photos?**
> Forgotten password = photos on the server can't be recovered, and the
> server can't make thumbnails. Same trade-off EyesOff accepted.

**V23 🟡 Why do timed sets matter for this owner specifically?**
> His plan has about ten timed exercises (planks, side planks, wall sits,
> balance holds, McGill holds). With reps-only logging they can't be recorded,
> earn XP, or show progress.

**V24 🟡 Why did "keyboard-first entry" become "one-tap repeat last set"?**
> The research assumed a desktop app. Phone-first changes the fastest input:
> big +/− steppers and repeating the last set beat keyboard shortcuts.

**V25 🟢 Name three research features deliberately skipped, and one reason each.**
> Programmable progression rules (planner rules cover it) · AI-generated
> workouts (black box; ADR-013) · nutrition logging (scope creep, users
> dislike it in workout apps).

**V26 🟡 How does "How was it?" stay a one-tap habit?**
> `review::suggest()` pre-picks answers from the session (hit rate, PRs, exam
> mode); the user just confirms. Fast enough to do every time, so the
> planner gets readiness data every session.

**V27 🔴 Why is "Joints: pain" never pre-picked, and why store which answers were suggested?**
> Pain must be a deliberate report — it changes the plan (pain check P7),
> and a wrong suggestion either hides real pain or invents it. Storing
> `prefilled` separates "confirmed a suggestion" from "chose it", so the
> suggestions can be judged later instead of grading themselves.

**V28 🟡 Why is voice logging two features, one now and one later?**
> Voice *notes* are free today: the phone keyboard's mic turns speech into
> text in any field. Voice *set logging* ("eighty for eight" → a set) needs
> real speech-to-text plus a parser — parked as an AI-track item
> (whisper.cpp on the VPS or phone + a rule-based parser in `core/`).

**V29 🟡 What does turning game mode off change, and what does it never change?**
> Off: XP, levels, body-map colours, streak and tokens are hidden, and decay
> is paused. Never changed: plans, logging, progress charts, and the safety
> rules (progression gates, pain check) — they don't depend on the game.

**V30 🔴 Why can game mode be toggled with zero data migration?**
> Game state is derived from the log (ADR-014). Off/on is a view choice plus
> `GameModeChange` records that mark decay-free intervals. Turning it back on
> recomputes the sheet — every set trained while off still counts.

**V31 🔴 Spot the issue: "when game mode is off, skip the progression-gate check to keep things simple".**
> Gates are a safety rule from the training plan (e.g. no reverse lunges
> before 3 pain-free weeks of step-ups), presented *as* unlocks when the game
> is on. Tying them to the toggle would let a user bypass safety by
> unchecking a box.

**V32 🔴 Why was the cheat-meal token replaced?**
> Research round 2: cheat meals are linked to eating-disorder symptoms in
> young adults (Ganson 2022; Murray 2018, preliminary), dietitians advise
> dropping "cheat" language, and an expected reward earned for a task is the
> kind most likely to undermine intrinsic motivation. A rest token keeps the
> mechanic; the user can still write their own reward — the app just never
> suggests food or says "cheat" (ADR-022).

**V33 🟡 Why did decay go from 10 days / −1 % to 14 days / −0.5 %?**
> In a detraining study no group lost maximal strength in 3 weeks off and
> the group lost about 1.2 % in 6 weeks. v1 removed ~13 % of XP by day 24 —
> harsher than the body. A game that lies about the body loses trust.

**V34 🟡 What changed when "exam mode" became "Pause"?**
> Same mechanism (stops decay, changes the quest) with a reason and a cap:
> exams ≤ 21 days, sick ≤ 14, injured ≤ 42, travel ≤ 21. Sick/injured
> suspend the quest; travel lowers it to one session. Record: `PausePeriod`.

**V35 🟡 Name four ways the weekly streak is forgiving.**
> Counted per week (not per day); rest days count; paused and rest-token
> weeks keep it; logging a forgotten set for a past day restores that week.
> And no shaming messages.

**V36 🔴 Why version the game rules?**
> XP is derived, so changing the balance table re-scores history and levels
> can shift. A version number + "Game rules updated — levels recalculated"
> tells the user why, instead of numbers silently moving.

**V37 🟡 What is envelope encryption, and why use it for photos?**
> One random photo key encrypts every photo; that key is then locked twice
> (lock key from the password → envelope A; recovery code → envelope B).
> Changing the password re-locks one small envelope instead of re-encrypting
> thousands of photos, and there's a second way in if the password is lost.

**V38 🔴 How can the server check a login without ever seeing the password?**
> The client runs Argon2id on the password and splits the result with a KDF
> into a login key (sent; the server stores a hash of it) and a lock key
> (never sent). The server can verify the login key but can't turn it back
> into the password or the lock key. Contrast: TickTimer sends the password.

**V39 🟡 How does a photo taken on Android show up on the iPhone web app?**
> Same C++ code and libsodium on every platform. The iPhone logs in, derives
> the lock key, downloads envelope A, opens the photo key, then downloads and
> decrypts photos and previews in the browser — treating them as a cache.

**V40 🟡 Why does the app upload an encrypted preview with each photo?**
> The server only holds ciphertext, so it can't make thumbnails. The device
> makes one before encrypting, so galleries load quickly everywhere.

**V41 🔴 Spot the issue: "derive the photo key directly from the password".**
> Then every password change means re-encrypting and re-uploading every
> photo, and a forgotten password loses everything — no recovery code
> possible. Envelope encryption separates "the key that encrypts" from "the
> keys that unlock it".

**V42 🔴 Why does `/prelogin` return a fake salt for unknown users?**
> The client needs the salt before it can derive the login key. Returning
> "no such user" would let anyone check who has an account; a stable fake
> salt makes known and unknown names look the same.

**V43 🟡 Why one file per ADR, and why never rewrite an accepted one?**
> A decision is a moment in time. One file each gives every decision its own
> git history; superseding (new ADR, old one marked "Superseded by ADR-0NN")
> keeps the record of *why* we once chose otherwise — rewriting would erase it.

**V44 🟢 Which ADRs are still "Proposed", and what evidence will settle them?**
> ADR-012 (Qt Quick for the UI) — the I4 spike on a real phone and browser.
> ADR-016 (QHttpServer behind Caddy) — building and live-testing the server (F15).

**V45 🟡 Why is *Train now* a button on Today and not a tab?**
> Tabs are for navigation, actions are buttons (Apple HIG). It's the one
> primary action, so it sits big and fixed at the bottom of Today; "Same as
> last time" gives repeat users a 1-tap path (ADR-023).

**V46 🔴 With the game off, where do pauses and unlocks go — and why must they still exist?**
> "Lighter weeks" on *You* (a pause also lowers the planner's weekly target)
> and a "New exercises" card + NEW tag on the plan (gates are safety rules).
> Both still affect the plan, so hiding them with the game would break it.

**V47 🟡 What is the purpose of the HTML prototype, given the real UI will be QML?**
> To test whether the 4-tab layout houses every feature without clutter —
> on a real phone, cheaply — before writing QML. It's throwaway
> (`prototypes/`): it answers a question, then gets deleted.

---

## W — Workflow & platforms (2026-11-23)

**W1 🟢 Where do your PC copy and Claude's copy of the code meet?**
> On GitHub (`main`). Each side only sees the other's work after a `push`,
> so the rule is *pull before you start, push when a step works*
> (docs/DEVELOPING.md §1).

**W2 🟡 Two people changed the same file. When does `git pull` merge by itself, and when does it stop?**
> Different lines (or different files) merge automatically. The same lines
> changed differently → a *merge conflict*: git writes both versions into the
> file between `<<<<<<<` and `>>>>>>>` markers; you keep the right one, then
> `git add` + `git commit`.

**W3 🟡 Why does the iPhone get a web app instead of a native iOS app?**
> A native iOS build needs a Mac with Xcode and an Apple developer account.
> Qt for WebAssembly runs the same C++/QML code in Safari, served by Caddy
> from the VPS on the same address as the API (no CORS). ADR-011.

**W4 🟢 What's the difference between `build/` and `dist/`?**
> `build/` = CMake's compiled output on your PC. `dist/` = what the Release
> workflow makes on GitHub's machine: the `.exe` plus every Qt DLL copied in
> by `windeployqt`, then zipped. Neither goes into git.

**W5 🟡 Why is Inno Setup parked, and what would adding it take?**
> The phone is the main client and the zip already runs on a PC with no Qt.
> An installer only adds a Start-menu entry and an uninstaller. Adding it =
> one `.iss` script + one `iscc` step in `release.yml` after `windeployqt`.

**W6 🟡 In the HTML prototype, why do buttons carry `data-a` / `data-v` instead of each having its own `onclick`?**
> Event delegation: one click listener on the document looks up the action
> by name. The screen is redrawn often, so per-button listeners would have
> to be re-attached after every render; one listener never does.

**W7 🔴 Spot the issue: a rest timer that does `left = left - 1` every second. What breaks on a phone?**
> When the app goes to the background, the browser/OS pauses or slows
> timers, so the countdown freezes and is wrong when you come back. Store
> the **end time** (`restEnds = now + 90 s`) and compute `left = restEnds − now`
> on each tick — gym usability rule 10.

---

## M — Exercise pictures (2026-11-23; M1–M10 are about ADR-024, later rejected; M11+ about ADR-025)

**M1 🟢 What two pictures does every exercise get, and where are they shown?**
> *How* (an animated side-view figure) and *which muscles* (front + back
> muscle map). Shown as a thumbnail on plan rows, a card in the guided
> session, and a big how-to sheet with 3 form cues.

**M2 🟡 Why draw the pictures with code instead of AI image generation?**
> Form must be exactly right (AI images of people often have body errors —
> nearly half in a 2025 benchmark), animation teaches movements better than
> stills, one look across 60+ exercises comes for free, and there's no
> licence question. Adding an exercise is data, not drawing.

**M3 🟡 What is 2-bone inverse kinematics, in one sentence?**
> Given where a limb starts (hip), where its end must be (ankle) and the two
> bone lengths, find the middle joint (knee) with the law of cosines —
> there are two answers, so a "bend side" picks one.

**M4 🔴 Spot the issue: the knee IK picks the wrong bend side. What does the picture show?**
> A knee bending backward (or an elbow through the floor in a push-up). It
> looks "almost right" in motion, which is why a gallery test renders every
> exercise as stills to check each pose by eye.

**M5 🟡 Why is a pose stored as targets (hip, foot, hand) and not as joint angles?**
> Feet must stay planted and hands must stay on the floor or bench. With
> targets, interpolating between poses keeps them there; with angles, the
> errors add up along the limb and feet slide or float.

**M6 🟢 What does *Reduce motion* change for exercise pictures?**
> No loop. One still: the start pose faint and the end pose solid, so the
> movement is still visible.

**M7 🟡 Why does the muscle map always have a text legend?**
> Colour alone fails for colour-blind users and in sunlight. "Main: Quads,
> Glutes · Helps: Hamstrings" says the same thing in words.

**M8 🟡 How can the same 17 exercises switch between a simple and an anime style with no new pose data?**
> The pose data (key poses → joints by IK) is separate from drawing.
> `drawSimple()` and `drawAnime()` take the same joints; a setting picks the
> renderer. That's the Strategy pattern: one interface, swappable algorithms.

**M9 🔴 Why draw the anime head in its own local frame (forward = +x, up = −y) and then `rotate()` it?**
> The head must follow the spine (standing, lying, upside-down in a push-up).
> Drawing it once in a fixed frame and rotating by `spine angle + 90°`
> (plus a mirror for face-up poses) is far simpler than recomputing every
> eye and hair point for each angle.

**M10 🟢 Where do the anime speed lines come from?**
> The pose a moment ago (t − 0.09 s) vs now: the joint that moved furthest
> gets three short lines behind it, opposite its direction of motion.

**M11 🟢 Which approach did the project finally choose for exercise pictures, and who decided?**
> ChatGPT-generated images (ADR-025): an anime character how-to picture and
> a grey muscle diagram with dark-blue/light-blue highlights per exercise. The owner
> decided after trying the code-drawn version (ADR-024, rejected).

**M12 🟡 Why are all muscle diagrams coloured from one grey base diagram?**
> So every diagram has the same figure, pose and framing — only the colours
> change. That makes them comparable and lets new ones match later (the base
> is saved as `ironlog-muscles-base.jpg`).

**M13 🔴 Spot the issue: a script downloads 32 images one after another from the same page. What happens in Chrome or Edge?**
> Only the first downloads. The browser's "multiple automatic downloads"
> protection blocks the rest without an error on the page. Fix: pack them
> into one zip (one download), or the user allows the site in settings.

**M14 🟡 Why does `assets/exercises/CREDITS.md` exist for AI-generated images?**
> Provenance: which tool, when, which prompts, what was checked. It records
> the rights situation, and it lets anyone make new images that match.

**M15 🟢 Why are the clips made with ffmpeg instead of an AI video tool?**
> OpenAI shut down its video tool (Sora) in March 2026 and ChatGPT offers no
> video option on this account; ffmpeg can build clips locally from the two
> pictures.

**M16 🔴 Spot the issue: main muscles in red, helper muscles in orange. Who can't tell them apart, and what's the fix?**
> Many colour-blind people (red–green deficiency) see red and orange as
> nearly the same. The UI research's rule: one hue, light vs dark (here dark
> blue = main, light blue = helpers), plus the muscle names as text.

**M17 🟡 Three review agents found ~96 problems. How do we avoid paying for that again?**
> Save the findings with evidence and a status (`docs/reviews/`), turn the
> repeat mistakes into a checklist (`DOC-RULES.md`), and next time review
> only the docs changed since the last reviewed commit, skipping facts
> already confirmed.

**M18 🔴 Spot the issue: a doc says "OpenAI shut down Sora in March 2026", sourced to a news story.**
> March was the announcement (2026-03-24). OpenAI's own help page says the
> app closed 2026-04-26 and the API ends 2026-09-24. Rule: use the first-hand
> source for dates.

**M19 🟡 Why did the docs stop calling "knees past toes" wrong form?**
> Fry et al. 2003: keeping knees back lowered knee torque a little but
> raised hip torque about tenfold. Knees past toes is normal for most
> people; vertical shins are only a choice for users on a knee stage.
