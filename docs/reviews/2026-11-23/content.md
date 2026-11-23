# IronLog docs — content validation

> **Agent report, kept as written** (2026-11-23). Reviewed commit
> `5939378`. File:line references point to that commit, so many have moved
> since. What happened to each finding: [`../2026-11-23-doc-review.md`](../2026-11-23-doc-review.md).

**Confirmed: 36** items in "Verified OK" below. Each item bundles several checks against the code, build runs, repo files and rule consistency, and 13 of the items are outside facts.
**Wrong: 31** findings: **8 High**, **8 Medium**, **15 Low**. Of the outside facts asked about, 3 are wrong: the Sora date (#3), "knees past toes" (#4) and the Qt LGPL scope (#12).
**Unverifiable: 7** (listed at the end). Nothing in the repo was edited. All builds ran in `/tmp`, and those folders are deleted.

Severity key: **High** = false, or would mislead a reader or break a build. **Medium** = imprecise, stale or incomplete. **Low** = nit.

## Findings

| # | Severity | File:line | Claim | Evidence | Fix |
|---|---|---|---|---|---|
| 1 | High | README.md:60 | `cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"   # Windows; omit on Linux` | No generator is named and the Qt prompt isn't mentioned. On Windows, CMake then picks MSVC/VS or fails. The repo's own TROUBLESHOOTING.md:164-180 ("CMake picks the wrong compiler") and ci.yml:105 both use `-G "MinGW Makefiles"`. | `cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"   # Windows: run from the "Qt 6.11.0 (MinGW 64-bit)" prompt. Linux: cmake -S . -B build` |
| 2 | High | docs/arc42.md:575 | "No CI yet (backlog I2)." | `.github/workflows/ci.yml` has 3 jobs. 06_IterationPlan.md:85 marks I2 as done ("✅ 2026-11-23, green"). arc42's own §10 Q7 (line 550) says "✅ CI job 1". | Delete the line, or: "- CI covers desktop only (core, Linux Qt 6.4, Windows Qt 6.8); no Android/WASM jobs yet (v0.3)." |
| 3 | High | docs/05_RiskList.md:34; docs/adr/ADR-025-exercise-pictures-from-chatgpt.md:22, :53 | "OpenAI shut down its video tool (Sora) in March 2026"; source line "NPR / AP, 2026-03-25" | OpenAI **announced** it on 2026-03-24 (AP story dated 24 Mar: https://www.opb.org/article/2026/03/24/openai-pulls-the-plug-on-sora-the-viral-ai-video-app-that-sparked-deepfake-concerns). OpenAI's help article says: "The Sora web and app experiences were discontinued on April 26, 2026"; "The Sora API will be discontinued on September 24, 2026" (https://help-lb.openai.com/en/articles/20001152-what-to-know-about-the-sora-discontinuation). | R-23: "OpenAI announced the end of Sora on 24 March 2026 (app and web closed 26 April 2026; API ends 24 September 2026)". ADR-025:22: "…(Sora) is discontinued (announced March 2026, app closed April 2026)…". ADR-025:53: "AP, 2026-03-24: OpenAI pulls the plug on Sora; OpenAI Help Center: What to know about the Sora discontinuation" |
| 4 | High | docs/05_RiskList.md:33 (also assets/exercises/CREDITS.md:57) | R-22 gives "knees past toes" as an example of **wrong form**. CREDITS asks for "shins vertical / knees over feet (squats, lunge)". | Knees past toes is not a form error for most people. Fry et al. 2003 (JSCR 17(4):629-633): limiting forward knee travel cut knee torque 22% but raised hip torque about 1070%. Physio Network: "no need to restrict forward knee movement altogether"; less knee travel only helps people with anterior knee pain (https://www.physio-network.com/?p=257). The novice-safety risk entry teaches a myth. | R-22: "…(extra fingers, a rounded back on a row, knees caving in, wrong muscle coloured)…". CREDITS:57: "shins near vertical on the box squat (knee-friendly variation for the owner's knee rule; not a general form rule)" |
| 5 | High | README.md:122-137 | Layout says "`prototypes/` throwaway experiments (empty)". The tree has no `assets/`, and `docs/` lists only "UP artifacts 01–08, arc42, BACKLOG, research/, troubleshooting". | `prototypes/ui-prototype/` exists (index.html is 136 KB, plus a README). `assets/exercises/` holds 33 JPEGs and exercises.json. `docs/adr/`, DEVELOPING.md, RELEASING.md and `archive/` are missing from the tree. | `├── assets/exercises/       how-to pictures + muscle diagrams, exercises.json (ADR-025)` · `├── docs/                   UP artifacts 01–08, arc42, adr/, BACKLOG, DEVELOPING, RELEASING, TROUBLESHOOTING, research/, archive/` · `├── prototypes/             throwaway experiments (ui-prototype/: clickable 4-tab phone layout)` |
| 6 | High | README.md:29 | "v0.6 \| Progress+: body check-ins, more PRs, calculators" | BACKLOG.md:33-35 puts F18 (body check-in) in **v0.4**. v0.6 = F19 F20 F22 H1. README's own v0.4 row (line 27) already says "check-ins". | `\| v0.6 \| Progress+: more PRs, plate/warm-up calculator, stall → deload hint, retire the desktop UI \|` |
| 7 | High | docs/02_UseCaseModel.md:48-58 | A second, orphaned use-case table: broken header `----\|------\|…`, UC-09 "Follow a **Program Template**", UC-08–10 "**Future**", UC-01 "Done (iteration 1)" | It contradicts the table directly above it (lines 20-46): UC-09 "Follow a Goal Program", status "Planned" with backlog ids. It looks like a leftover from an edit. | Delete lines 48-58. |
| 8 | High | docs/adr/ADR-004-stats-free-functions.md:17 | "`TrainingLog` keeps one job: its sorted, one-record-per-id invariant." | include/core/Workout.h:37-38: "Invariant: workouts are sorted by date and no two share a date." v0.1 records have no ids (ids come with schema v2, F1). | "`TrainingLog` keeps one job: its invariant (workouts sorted by date, at most one per date)." |
| 9 | Medium | docs/arc42.md:227-228 (§7.3) | CI runs on "every push and pull request". CD on "tag `vX.Y.Z`" builds "Windows zip, Android APK…, web bundle… Deploy… `/version` must return X.Y.Z". | ci.yml:13-17 runs on push to `main`, PRs, and manual. release.yml has a tag **or** a Run-workflow button, and builds, smoke-tests and publishes **only** the Windows zip. Its own header (lines 137-138) says APK/web come in v0.3 and deploy in v0.5. The row isn't marked "planned". | CI: "every push to `main`, every pull request, manual". CD: "tag `vX.Y.Z` or *Run workflow* button. ✅ Windows zip (windeployqt, smoke test with no Qt on PATH) → GitHub Release. Planned: Android APK + web bundle (v0.3); VPS deploy + `/version` check (v0.5, I6)." |
| 10 | Medium | README.md:74-76 with docs/DEVELOPING.md:54 | Core-only build uses `-B build-core`; the daily routine is `git add -A` | `.gitignore` ignores `build/` and `cmake-build-*/` but not `build-core/`. A simulated run under that .gitignore left `build-core/test_core` (binary) and `build-core/Testing/Temporary/CTestCostData.txt` untracked, so `git add -A` would commit them. | README: `cmake -S . -B build/core -DIRONLOG_BUILD_APP=OFF` (and matching `--build` / `--test-dir build/core`), or add `build-*/` to `.gitignore`. |
| 11 | Medium | docs/arc42.md:308, :310, :331-333 | Heading "Game rules (balance table **v1** …)", column "**v1** value". The note says "decay **was gentler than v1**". | The table holds the **v2** rules (14 days, −0.5 %/day; ADR-022 "Game rules v2"). Everywhere else, "v1" means the old rules (Glossary:56, 58: "v1 name", "v1 cheat-meal token"). "Was gentler" says the opposite of what was meant. | Heading: "### 8.6 Game rules v2 (balance table, rules version 1 — tunable, one file)". Column: "Value". Note: "decay made gentler than v1 (strength barely changes in 3 weeks off)" |
| 12 | Medium | docs/03_SupplementarySpecification.md:63 | "Qt used under LGPL (dynamic linking)." | True for v0.1 Widgets. Not true for the planned v2 parts: Qt HTTP Server "is available under commercial licenses… In addition… GNU General Public License, version 3" (no LGPL) (https://doc.qt.io/qt-6/qthttpserver-index.html). And "Qt for WebAssembly uses static linking by default" (https://doc.qt.io/qt-6.11/wasm.html). | "Qt used under LGPLv3 with dynamic linking on desktop and Android. The WebAssembly build links Qt statically (LGPL relinking duties apply). `ironlog-server` uses Qt HTTP Server, which is GPLv3/commercial only." |
| 13 | Medium | docs/04_Glossary.md:80 | IK format: `pose::solveLimb` | No such identifier anywhere in the repo. The prototype's IK is the JS function `ik()` (prototypes/ui-prototype/index.html). ADR-024:64 names only a planned `PoseAnimator`. | Format column: "`ik()` in prototypes/ui-prototype/index.html" |
| 14 | Medium | docs/arc42.md:261 (§8.2) | "One JSON document per user; `schemaVersion` 2." | include/services/JsonStore.h:29 has `kSchemaVersion = 1`, and a file written by the app has `"schemaVersion": 1`. §8.2 has no "planned" marker, unlike §5. | "…`schemaVersion` 2 (planned, F1; v0.1 writes 1 ✅)." |
| 15 | Medium | docs/04_Glossary.md:44; docs/arc42.md:302 | Pain check: "sharp pain, pinching or next-day swelling → drops it one stage for 2 weeks" | This reads as an automatic injury protocol, with no framing and no escalation. The "not medical advice" note lives only in §8.9 and UC-11. QG1 (safety) is the top quality goal, and injured users are in scope. | Append: "…for 2 weeks. If it happens again or doesn't settle, the app suggests stopping that exercise and seeing a health professional. General guidance, not medical advice." |
| 16 | Medium | docs/arc42.md:325 vs :300 | Exams pause quest = "2 strength + 1 cardio". P5 exam minimum = "2 strength + 1–2 cardio". | The two rules give different cardio numbers for the same exam week. | §8.6: "exams → the exam minimum (2 strength + 1 cardio)". P5: "weekly target = exam minimum (2 strength + 1 cardio)" (or use 1–2 in both) |
| 17 | Low | docs/arc42.md:430; docs/adr/ADR-020…:13; docs/02_UseCaseModel.md:45 vs docs/adr/ADR-023…:29; docs/BACKLOG.md:137 | Recovery code shown "when photo sync is turned on" vs "prompt at the first photo" | Two different trigger moments for the same one-time code. | State it once: "shown once at the first photo, which turns on photo sync (also reachable in Settings)". |
| 18 | Low | README.md:124; docs/DEVELOPING.md:16 | "ci.yml (every push) · release.yml (on version tag)"; mermaid "every push → CI" | CI runs on push to `main` + PRs. Release also has a button (workflow_dispatch). | README: "ci.yml (push to main, PRs) · release.yml (version tag or Run-workflow button)". Mermaid label: "push to main / PR". |
| 19 | Low | docs/RELEASING.md:141 | Release "smoke-tests the zip with no Qt on PATH" | release.yml:236-248 runs `dist/IronLog/IronLog.exe`, the folder that gets zipped, not an extracted zip. | "smoke-tests the packaged folder (the zip's contents) with no Qt on PATH" |
| 20 | Low | docs/06_IterationPlan.md:80, :91, :96 | "*I3 moved up… so it's tested next.*" I3 is listed last in the table. DoD says "written into arc42 ADR-012". | I3 is already ✅ (line 91). ADRs live in `docs/adr/`. | Delete line 80. Move the I3 row after I2. "…written into ADR-012 (docs/adr/ADR-012-qml-ui.md)". |
| 21 | Low | docs/arc42.md:489 | Images "~80–130 KB each" | Actual range is 65.8 KB (pushup-how) to 138.2 KB (legpress-how). | "~65–140 KB each" |
| 22 | Low | docs/03_SupplementarySpecification.md:60; README.md:62 | v0.1 platforms include macOS; run with `./build/IronLog` | No CI job builds macOS. CMakeLists.txt:105 sets `MACOSX_BUNDLE ON`, so on macOS the binary is `build/IronLog.app`. | 03: "macOS (should build; not tested)". README: `./build/IronLog   # macOS: open build/IronLog.app; Windows: build\IronLog.exe` |
| 23 | Low | docs/adr/ADR-012-qml-ui.md:28 | "docs/research/ui-research.md (when available)" | The file exists. | "docs/research/ui-research.md" |
| 24 | Low | docs/BACKLOG.md:13 | "Reasoning goes in `arc42.md` (§8 concept or §9 ADR)" | ADRs are now separate files in `docs/adr/` (arc42 §9 is a summary). | "…(`arc42.md` §8 concept, or a new `docs/adr/ADR-0NN-*.md`)" |
| 25 | Low | docs/BACKLOG.md:20 | "Cap: ~25 open items." | 27 open ids are listed (I4–I6, F1–F23, H1). | Park two items in *Later*, or say "~25 (currently 27)". |
| 26 | Low | docs/02_UseCaseModel.md:154; docs/BACKLOG.md:81 vs docs/adr/ADR-013…:13 | UC-11 builds the plan with "planner rules P1–P5" | ADR-013 and arc42 §8.5 say plans follow P1–P7. P6 gates and P7 pain also shape every plan (§8.9). | UC-11 step 8: "planner rules P1–P7" |
| 27 | Low | docs/DEVELOPING.md:69-70 | Android/iPhone: "spike I4, then v0.2+" | RELEASING.md:185 and release.yml:137 put APK + web in v0.3. | "spike **I4** (v0.2), shipped from v0.3" |
| 28 | Low | docs/adr/ADR-024-…:3 | Status "Rejected (2026-11-23) … superseded by ADR-025" | The ADR README (line 24) treats `Rejected` and `Superseded by` as separate status values. A never-accepted ADR is just rejected. | "**Status:** Rejected (2026-11-23) — owner disliked the look; see ADR-025" |
| 29 | Low | docs/arc42.md:394-462, 549-555; docs/05_RiskList.md:29-35; docs/01_Vision.md:118-124 | Ordering | §8 runs 8.12 → 8.15 → 8.14 → 8.13 → 8.16. Q10 comes before Q9. Risks go R-18, R-20, R-21, R-19, …, R-17. Vision revisions go v2.5 before v2.4. | Renumber or reorder in ascending order. |
| 30 | Low | assets/exercises/CREDITS.md:41 | "OpenAI's Terms of Use assign rights in outputs to the user." | Confirmed, but the clause has limits: "We hereby assign to you all our right, title, and interest, **if any**…" and it "does not extend to other users' output" (https://openai.com/policies/terms-of-use/, effective 2026-01-01). | "OpenAI's Terms of Use (eff. 2026-01-01) say the user owns Output and assign OpenAI's rights, if any, to the user. Re-check if the terms change." |
| 31 | Low | README.md:37 | "marked *below / on / above* a 10–20 set target" | The range is reasonable (see Verified OK), but in the README it reads as a prescription. arc42 §8.13 frames it properly. | "…a 10–20 set target (a common starting range, not a prescription; general guidance, not medical advice)." |

## Verified OK

**Build and commands**
- The core-only build (`-DIRONLOG_BUILD_APP=OFF`) configures, builds and passes ctest, with **0 warnings** under GCC 13.3 `-Wall -Wextra -Wpedantic`. `test_core` gives "799 assertions in 18 test cases".
- The full build against **Qt 6.4.2** (backs "Qt 6.4 or newer") gives 0 warnings and 2/2 ctest passing. `test_services`: 11 cases.
- Test counts: 18 `TEST_CASE` in test_core.cpp and 11 in test_services.cpp, matching 06_IterationPlan.md:62-63. README's test table (what each covers, "Qt Core only") matches the CMake link lines.
- CLI options (main.cpp, `--help`): `--data`, `--demo`, `--date`, `--tab session|week|progress`, `--screenshot`, plus `--version` ("IronLog 0.1.0"). Both README:111-112 commands and the ci.yml smoke command exit 0 and write a PNG under `QT_QPA_PLATFORM=offscreen`.
- Linux data path `~/.local/share/IronLog/ironlog.json` confirmed by a run with a temp HOME. Only the application name is set, never an organization.
- RELEASING step 1 `grep -n "VERSION 0"` finds CMakeLists.txt:15. The sed in release.yml extracts `0.1.0`. Tag `v0.1.0` exists locally.

**Code names and rules**
- `SetEntry::make()` uses a private ctor and returns `std::optional`, checking 0–1000 kg, 1–100 reps, finite and non-empty. `epleyOneRepMax()` returns w(1+r/30), and w when r≤1. `SetEntry::volume()` exists.
- These functions exist and behave as described: `stats::personalRecordIndices()` (first set with the day's top e1RM, strictly greater than every earlier day, first session never a PR), `stats::weeklySets()` (Mon–Sun, unknown exercises skipped), `stats::classify()`, `stats::VolumeTarget{10,20}`, `Date::mondayOfWeek()`, `stats::lastSet`.
- `MuscleGroup` has 10 values. Exercise names are trimmed and compared case-insensitively.
- `TrainingController` validates, then `persist()`, then `emit logChanged()`. `saveFailed` shows "NOT SAVED: …" plus one dialog per session. An unreadable file is renamed `<path>.unreadable-yyyyMMdd-HHmmss` and never overwritten. `schemaVersion` is 1, with refusal outside 1..current.
- UI claims hold:
  - Ctrl+Enter adds a set and Delete removes one; the calendar's max date is today.
  - Selecting a day pre-selects its last exercise.
  - "Est. 1RM" header, PR tooltip, gold PR fill, and "Below / On / Above target" labels.
  - Sample data is 8 weeks of PPL, loadable only into an empty log.
  - The status bar shows the data path; Help → About shows the version and the path.
  - There is no `setStyleSheet`.
- CMake: targets `ironlog_core` / `ironlog_services` / `IronLog` / `test_core` / `test_services`. `find_package(Qt6 6.4)`, CMake ≥ 3.20, every Q_OBJECT header listed. Catch2 is v2.13.10 under the Boost Software License 1.0.

**CI/CD vs docs**
- ci.yml has 3 jobs: core without Qt; Linux with apt Qt 6.4, offscreen and a smoke screenshot; Windows with Qt 6.8.3 and MinGW 13.1. Every job has `timeout-minutes`, ctest runs with `--timeout 120`, and Windows tests run under pwsh. This matches RELEASING's table, 06 I2, and the two TROUBLESHOOTING CI entries.
- release.yml matches RELEASING steps 3–4:
  - Triggers: tag `v*.*.*` or the Run-workflow button.
  - Refuses unless the version is X.Y.Z, refuses an existing tag, and checks tag = `project(VERSION)`.
  - Runs `windeployqt --compiler-runtime` and a smoke test with only system32 on PATH.
  - Publishes with `gh release create --target $GITHUB_SHA`, as `IronLog-vX.Y.Z-windows.zip`.

**Repo facts**
- Every relative Markdown link in the in-scope docs resolves.
- `.gitignore` covers `build/`, `dist/` and `*.local.*` (ADR-019; `docs/How_to/SERVER_GUIDE.local.md` is ignored and untracked).
- exercises.json lists 17 exercises, and every file it names exists. The folder has 17 `-how` + 15 `-muscles` + 1 base = 33 JPEGs. Only the base file is unlisted, and that is documented.
- "17 how-to pictures + 15 muscle diagrams" is correct. Plank and dead bug have `muscles: null`, as documented.
- All 33 JPEGs are 1200 × 800, and the total is 2.83 MB (backs "~3 MB").
- The revlunge, stepup and legpress muscle images are byte-identical (backs CREDITS "share one diagram").
- ui-prototype: `MEDIA` has 17 exercises. `S`, `A`, `render()`, `joints()`, `ik()`, `poseAt()`, `drawSimple()`, `drawAnime()`, `cap()`, `restEnds`, `data-a`/`data-v` and `requestAnimationFrame` all exist, and the tick is `setInterval(…, 250)`.
- ui-research.md lists 7 features without a home and 3 crowded screens (backs ADR-023). archive/design-notes-v0.1.md §4 exists. feature-research-2.md has §A, §E3 and a "Critique" section, as the ADRs cite.

**Rule consistency**
- These game-rule values agree across arc42 §8.6, ADR-022, the Glossary, BACKLOG F13/F14 and R-12, and the Vision's "2 weeks" agrees: decay after 14 days at −0.5 %/day, 3-day warning, 50 % peak floor, comeback ×2, pause caps (exams 21 / sick 14 / injured 42 / travel 21), rest token bank max 2, level L→L+1 = 50 × L, 8 skills.
- The secondary-muscle +5 XP (half of +10) matches R-03's "half-sets".
- UI rules agree across ADR-023, arc42 §5.1, BACKLOG F8/F14 and the Glossary: 4 tabs (Today · Log · Progress · You), *Train now* as a button not a tab, the mini bar, the Why? sheet, "Lighter weeks", and the "3 cramming days" pause offer.

**Outside facts — Confirmed**
- Qt 6.11 for Android needs JDK 21 and NDK 27.2.12479018 (r27c): https://doc.qt.io/qt-6.11/android.html
- Qt 6.8 for Android used JDK 17: https://doc.qt.io/qt-6.8/android.html
- Epley formula: 1RM = weight × (1 + reps/30), most accurate at 2–10 reps (backs R-06): https://www.topendsports.com/testing/calculators/1repmax.htm
- 10–20 sets per muscle per week is a common hypertrophy range. Baz-Valle et al. 2022 suggest "12-20 weekly sets per muscle group" (https://pmc.ncbi.nlm.nih.gov/articles/PMC8884877), and Human Kinetics gives "the minimum… per week appears to be 10" (https://us.humankinetics.com/blogs/strength-conditioning-fitness/sets-and-repetitions-to-build-muscle). The arc42 citation is correct: *J Hum Kinet* 81:199–210, PMID 35291645.
- Höffler & Leutner 2007: overall d = 0.37, procedural-motor d = 1.06, *Learning and Instruction* 17(6):722–738: https://www.leibniz-ipn.de/en/research/publications/instructional-animation-versus-static-pictures-a-meta-analysis
- arXiv 2503.00811 (Ma et al. 2025): 4 T2I models, "nearly 50 %" of human images distorted: https://arxiv.org/abs/2503.00811
- OpenAI terms: the user owns Output, and OpenAI assigns its rights (see #30 for the limits): https://openai.com/policies/terms-of-use/
- Qt HTTP Server: "only for local connections or in a trusted network, and do not expose the ports to the internet" (backs ADR-016 and arc42 §8.8): https://doc.qt.io/qt-6/qthttpserver-index.html
- Qt 6.11 on Windows uses MinGW-w64 13.1 (backs ci.yml and TROUBLESHOOTING "same family"): https://doc.qt.io/qt-6.11/supported-platforms.html
- GitHub's windows-2025 runner includes Inno Setup (6.5.4 added in image 20251014) (backs DEVELOPING §5): https://newreleases.io/project/github/actions/runner-images/release/win25%2F20251014.59
- Qt targets one Emscripten version per minor release (4.0.7 for 6.11) (backs DEVELOPING): https://doc.qt.io/qt-6.11/wasm.html
- Hinnant's date algorithms: "Consider these donated to the public domain" (backs ADR-002): https://howardhinnant.github.io/date_algorithms.html
- `-1073741515` = `0xC0000135` (checked with signed 32-bit arithmetic).

**Health/safety framing present.** "General guidance, not medical advice; stop on sharp pain" appears in 03:16-17, arc42 §8.9, UC-11 and the Vision constraints. The no-food-reward and no-"cheat" rules are consistent across the Vision, 03, arc42, ADR-022 and the Glossary.

## Unverifiable

| Claim | Why |
|---|---|
| arc42:473 Schoenfeld et al. 2017: "better hypertrophy above ~9 weekly sets" | Paper not reachable. The citation exists (J Sports Sci 35:1073-1082), but the set categories couldn't be confirmed. |
| 06:84-91: CI "green", v0.1.0 GitHub Release "published" | Not queried on GitHub, to avoid sending project details out. Only the local tag was checked. |
| TROUBLESHOOTING:255-261: aqtinstall 3.3.0 folder-layout cause | Needs the historical CI log. |
| arc42:422: "libsodium, the library EyesOff already uses" | Owner's other repo. |
| ADR-025:22: "ChatGPT offers no video option on this account" | Account-specific. |
| CREDITS:43: JPEG quality 0.88; CREDITS:73-75: which chat made which diagram | Process claims; only the dimensions were checked. |
| 03:47-49 and UC-01: < 100 ms per action, startup < 1 s | No benchmark run. Not measured. |
