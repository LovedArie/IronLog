# IronLog: Feature Research

> **Date:** 2026-11-23 · **Scope:** popular lifting apps, open-source trackers, user feedback, training science
> **Read this first:** Prices are US App Store / vendor prices as shown when fetched (Nov 2026). They change often.
> Many comparison pages are written by **competing vendors** (JEFIT, Boostcamp, Alpha Progression, Cora, Setgraph, SensAI). Each is marked *(vendor)* in Sources.
> Reddit could not be read directly, so Reddit sentiment comes from aggregator sites. Treat it as a direction, not a measurement.
> Citations look like [[12]][s12]. Each one links to its source.

---

## Summary

- **Fast logging is the main battleground.** "Too many taps per set" is the most cited complaint, and fast logging is the most cited reason people switch apps [[32]][s32] [[35]][s35]. A desktop app can win here with the keyboard.
- **Paywalls show up in the same places.** These are routine caps (Strong 3, Hevy 4), history limits (Hevy free shows 3 months), charts and analytics, muscle heat maps, auto-progression, and plate and warm-up calculators [[1]][s1] [[9]][s9] [[14]][s14] [[17]][s17].
- **Subscriptions are the #2 complaint category** across top health and fitness apps (24% of negative reviews in one analysis) [[36]][s36]. Free apps like FitNotes are liked precisely because they have no upsells [[26]][s26] [[32]][s32].
- **Smart progression is what people pay for.** This means per-set targets from RIR (Alpha), feedback-driven mesocycles (RP), and auto-increases (Boostcamp Pro, Hevy Trainer) [[17]][s17] [[20]][s20] [[14]][s14] [[18]][s18]. But users distrust black-box AI suggestions [[13]][s13] [[34]][s34].
- **People care about owning their data.** Lifters lose years of logs [[38]][s38], app-to-app imports drop supersets and mix up units and dates [[7]][s7] [[8]][s8], and an open JSON format (openweight) now exists [[31]][s31].
- **The science backs what IronLog already does** (weekly sets per muscle) and points to next steps: counting secondary muscles as half-sets [[42]][s42], logging RIR [[40]][s40], and flagging stalls for a reactive deload [[43]][s43].
- **Native desktop is nearly empty.** Every tracker I found was mobile or web. Flexify (Flutter, MIT) is the one open-source option on the Microsoft Store [[29]][s29]. This is an observation from searching, not a market study.
- **Quick wins are basics.** Users either expect these or competitors charge for them: editing and undo, keyboard-fast entry, export and backups, more PR types, and plate and warm-up calculators.

---

## App-by-app table

| App | Standout features | Pricing (US) | What users complain about |
|---|---|---|---|
| **Hevy** | Shows last session's numbers while logging; set types (warm-up/drop/failure/superset); rest timer, plate calc, RPE; body-map muscle view; calendar with weekly streak; monthly report; 14 body measurements + photos; social feed [[2]][s2] [[3]][s3] | Free: 4 routines, 7 custom exercises, 3 months of stats. Pro $2.99/mo, $23.99/yr, $74.99 lifetime [[1]][s1] [[11]][s11] | 3-month history and routine caps; lifetime price feels steep; social side divides users; one user hit kg/lb mix-ups and dates shifted by a day after a Strong import; rest-timer alerts while in other apps [[8]][s8] [[32]][s32] |
| **Strong** | Very simple, fast UI; auto rest timer; supersets; warm-up/failure/drop tags; plate + warm-up calculators; CSV export; Apple Watch [[9]][s9] | Free: unlimited workouts, 3 routines. PRO $4.99/mo, $29.99/yr, $99.99 "Forever" (other SKUs also listed) [[9]][s9] | Routine cap; charts and measurements are PRO (per a competitor) [[11]][s11]; custom exercises can be hidden but not deleted; built-ins can't be renamed; plate calc lacks unusual bar weights; few new features [[9]][s9] [[10]][s10] |
| **JEFIT** | 1,400+ exercises; coach-built plans; body measurements; Apple Watch [[12]][s12] | Free with ads. Elite $12.99/mo, $69.99/yr [[11]][s11] | Heavy ads in free tier [[12]][s12]; seen as dated next to Hevy [[32]][s32]; best features need Elite; 7 free custom exercises; no plate calculator [[11]][s11] |
| **Fitbod** | Generates each workout from goals, equipment, and muscle recovery; recovery heat map [[13]][s13] | No free tier (7-day trial). $15.99/mo, $95.99/yr [[13]][s13] | No free version; tester says it guesses at weights; no periodisation or deloads; exercises change often [[13]][s13] [[19]][s19] |
| **Boostcamp** | Huge program library; free tracker with RPE, supersets, auto rest timer, warm-up templates, custom program builder [[14]][s14] [[15]][s15] | Pro $14.99/mo or $59.99/yr. Analytics, muscle heat map, auto-progression are Pro; plate calc limited to 10 free uses [[14]][s14] | Monthly Pro is pricey; progression depends on the chosen program; light form guidance [[19]][s19] |
| **Alpha Progression** | Per-set weight/rep targets from RIR + history; plan generator with periodisation and deloads; multiple gyms with their own equipment; CSV export [[17]][s17] | Pro $12.99/mo, $79.99/yr (14-day trial) [[17]][s17] | Periodisation is paywalled; a user wants a 1RM estimate, not only 10RM; no Apple Watch app [[17]][s17] [[19]][s19] |
| **RP Hypertrophy** | Mesocycle plans with weekly targets; adjusts from pump, soreness, and workload feedback [[20]][s20] | $34.99/mo, $299.99/yr (lower promo SKUs also listed). Free tier: unverified [[20]][s20] | Expensive; no in-workout rest timer; no per-side logging; no calendar view of past workouts; no cardio [[20]][s20] |
| **Liftosaur** | Open source; progression written as scripts (Liftoscript); 61 built-in programs; weight rounding to your plates; Hevy CSV import [[21]][s21] [[23]][s23] | Free core. Premium $4.99/mo, $39.99/yr, $99.99 lifetime for graphs, watch app, API [[23]][s23] | Steep learning curve; crashes and sync problems after its 2026 rewrite; no Strong importer [[23]][s23] |
| **StrongLifts 5×5** | Fully automatic weight increases; adjustable increment rules; deload rule after repeated failures [[24]][s24] [[25]][s25] | Pro $11.99/mo, $59.99/yr, multi-year plans. Exact free vs Pro split: unverified [[24]][s24] | Little complaint data in the sources I could read. One reviewer quotes an older, lower yearly price [[24]][s24] |
| **FitNotes** (Android) | Free, no ads, no account, offline; volume-by-muscle chart; best-set highlighting; calendar; CSV export; backup to Drive/Dropbox [[26]][s26] [[27]][s27] | Free, no in-app purchases [[26]][s26] | Android only; no automatic sync, so backups are manual; a log, not a coach; not recently updated [[26]][s26] [[35]][s35] |

**Open-source trackers worth studying**

| Project | What it does | Idea worth borrowing |
|---|---|---|
| **wger** (AGPL-3.0) | Self-hostable web app with routines, auto weight progression, nutrition, body weight, measurements, progress photos, REST API [[28]][s28] | Exercise and ingredient **data** is Creative Commons (licence varies per entry), so a library could be imported if credited [[28]][s28] |
| **Liftosaur** (open source) | PWA + mobile wrappers; progression and deload logic as scripts [[21]][s21] | Clean model: **Program → Day → Exercise → Set**, with state variables that persist between workouts and a "finish day" script that updates them [[22]][s22]. The author regrets using DynamoDB and would choose a relational DB [[21]][s21] |
| **Flexify** (MIT) | Flutter, offline-first, plans, rest timers, graphs; ships on Windows via the Microsoft Store [[29]][s29] | Proof that a "keep your data on your device" desktop lifting log has a place |
| **LibreFit** (GPL-3.0+) | Privacy-focused Android tracker, 800+ exercises, offline, local storage [[30]][s30] | Equipment / muscle / difficulty filters for the exercise picker |
| **openweight** (Apache-2.0) | Open JSON format: WorkoutLog, WorkoutTemplate, Program, LifterProfile. Its CLI converts Strong and Hevy CSV [[31]][s31] | An **export target**: IronLog's data could move into any tool that reads openweight |

---

## Feature catalogue

**Key:** Demand = High / Med / Low (evidence cited). Fit = for a desktop, local, C++/Qt app. Effort: S = a day or two, M = about a week, L = several weeks (for this codebase). Layer: `core` = pure C++ domain, `services` = controller/persistence, `ui` = Qt widgets.
*The "Teaches" column is my own general knowledge, not from the sources.*

### A. Logging speed and correctness

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| Edit / delete sets and past days | All major apps (baseline) | **High**. Clumsy logging is the top reason to switch [[32]][s32]; Hevy users ask to edit a routine mid-workout [[8]][s8] | High (seam already planned in Design Notes §7) | S | core, services, ui | Editable `QAbstractTableModel` (`flags()`, `setData()`) |
| Undo / redo | Not advertised by the apps checked (unverified) | **Med** (inferred as a safety net for edits; no direct evidence) | High | S | services | Command pattern via `QUndoStack` |
| Keyboard-first entry (Tab/Enter flow, exercise autocomplete, shortcuts) | Mobile apps can't match this; spreadsheet users rely on it [[37]][s37] | **High**. "Too many taps" is the #1 complaint [[32]][s32] [[35]][s35]; Hevy users ask for a "Next field" button [[8]][s8] | Very high (desktop's natural edge) | S | ui | `QCompleter`, focus chain, `QShortcut`, event filters |
| Set types: warm-up / drop / failure | Hevy, Strong, Boostcamp [[2]][s2] [[9]][s9] [[15]][s15] | **Med–High** | High (warm-ups should not count toward PRs or weekly sets) | S–M | core, services (schema v2), ui | `enum class`, schema migration, `QStyledItemDelegate` with combo box |
| RPE / RIR per set | Hevy, Boostcamp (free), Alpha; Strong CSV has an RPE column [[2]][s2] [[16]][s16] [[17]][s17] [[47]][s47] | **High** for hypertrophy lifters. RPE-based loading gave similar gains to %1RM loading [[40]][s40] | High | S–M | core, ui | `std::optional<double>`, input validation, delegates |
| Notes (per workout; sticky per exercise) | Strong, Alpha [[9]][s9] [[17]][s17] | **Med**. Hevy users want notes that persist across workouts [[8]][s8] | High | S | core, ui | Schema evolution, `QPlainTextEdit` |
| Supersets / exercise grouping | Strong, Hevy, Boostcamp [[9]][s9] [[2]][s2] [[15]][s15] | **Med**. Supersets are lost in Hevy's Strong import [[7]][s7] | Med (desktop logging often happens after the session) | M | core, ui | Group IDs; tree model vs flat table |
| Timed / distance / assisted sets | Strong (duration, assisted bodyweight) [[9]][s9] | **Med**. Strong users can't combine weight and time; Hevy users want a timed mode per exercise [[9]][s9] [[8]][s8] | Med | M | core | `std::variant` for different set kinds |
| Rest timer (auto-start, per-exercise length, alert) | Strong, Hevy, Boostcamp, Liftosaur, Flexify [[9]][s9] [[2]][s2] [[15]][s15] [[23]][s23] [[29]][s29] | **High**. RP users ask for one; Hevy users want alerts while in other apps [[20]][s20] [[8]][s8] [[35]][s35] | Med (only useful if the laptop is at the gym) | S | ui | `QTimer`, `QSystemTrayIcon::showMessage`, `QSoundEffect` |

### B. Planning and progression

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| Templates / routines | All; free caps are Strong 3, Hevy 4 [[9]][s9] [[1]][s1] | **High** (it is the main paywall lever [[32]][s32]) | High | M | core, services, ui | Value-type copying (Prototype idea), list models |
| Progression hint: double progression (top of rep range at target RIR → add load) | Hevy Trainer (Pro), Alpha, Boostcamp Pro [[18]][s18] [[14]][s14] | **High**. Users want to be told what to do next [[34]][s34] | High (pure, testable rule; method from [[45]][s45]) | M | core | Pure functions + unit tests |
| Programmable progression rules | Liftosaur, StrongLifts [[21]][s21] [[24]][s24] | **Med** (power users) | Med | L | core | Strategy pattern, or a tiny interpreter |
| Stall / deload flag (e1RM falling over N sessions; sets above MRV) | Alpha, RP schedule deloads [[17]][s17] [[20]][s20] | **Med**. In a survey of 246 athletes, 54% of reactive deloads were triggered by stalled or declining performance [[43]][s43]; a drop across several workouts (not one bad day) is a common sign [[44]][s44] | High (analytics on data IronLog already has) | M | core | Trend detection, moving averages, tests |
| Readiness feedback (pump, soreness, joints) | RP [[20]][s20] | **Med** (RP-style lifters) | Med | M | core, ui | Form design, enums |
| Per-muscle volume targets (MEV / MRV per muscle) | RP's model [[41]][s41]; Boostcamp Pro muscle tracker [[14]][s14] | **Med** | High (`stats::VolumeTarget` is already a parameter) | S | core, ui | Editable settings, `QSpinBox` delegate |
| Secondary muscles counted as half-sets | Hevy tags primary + secondary muscles [[4]][s4] | **Med**. Half-credit counting had the strongest support in a 2024 meta-regression (preprint) [[42]][s42] | High (seam planned in Design Notes §7) | M | core, services (schema v2) | Schema migration; `double` counts |

### C. Calculators

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| Plate calculator (your own bar + plate inventory) | Strong, Hevy, Liftosaur, Alpha (Pro), Boostcamp (10 free uses) [[9]][s9] [[2]][s2] [[23]][s23] [[17]][s17] [[14]][s14] | **Med–High**. Strong lacks unusual bars; Hevy users want per-side plate entry [[9]][s9] [[8]][s8] | High | S | core, ui | Greedy algorithm, saved settings |
| Warm-up calculator | Strong, Hevy (Pro), Alpha (Pro), Boostcamp, Liftosaur [[9]][s9] [[1]][s1] [[17]][s17] [[15]][s15] [[21]][s21] | **Med** | High | S (after plate calc) | core | Rounding to available plates; reuse |
| Choice of 1RM formula + "estimate" label | Most apps show e1RM [[2]][s2] [[9]][s9] | **Low**. But estimates can miss the true 1RM by 10%+, and Epley and Brzycki differ below 10 reps [[46]][s46] | High | S | core | `std::function` / Strategy |

### D. Body tracking

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| Body weight log + chart | Hevy (free), FitNotes, Liftosaur, wger, Alpha [[3]][s3] [[26]][s26] [[21]][s21] [[28]][s28] [[17]][s17] | **Med** | High (reuse `ProgressChart`) | S–M | core, ui | Reusing one widget for a second data series |
| Measurements + progress photos | Hevy (14 measurements, photos), wger [[3]][s3] [[28]][s28] | **Med** | Med | M | core, ui | `QPixmap`, storing files beside the JSON |

### E. Analytics

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| More PR types (heaviest, best set volume, session volume, most reps) + rep-max table | Hevy [[5]][s5] | **Med–High**. PR tracking is a frequently praised feature [[33]][s33] [[8]][s8] | High | S | core | `std::map` aggregation, `<algorithm>` |
| Calendar + training heat map + weekly streak | Hevy (weekly streak), FitNotes calendar [[3]][s3] [[27]][s27]; RP users miss one [[20]][s20] | **Med** | High | S–M | ui | `QCalendarWidget::setDateTextFormat`, custom `QPainter` grid |
| Muscle body map | Fitbod, Hevy, Boostcamp Pro, Liftosaur [[13]][s13] [[3]][s3] [[14]][s14] [[21]][s21] | **Med–High** (often paywalled) | High | M | ui | `QSvgRenderer` / `QPainterPath`, colour scales |
| Volume trend charts (tonnage; sets per muscle by week) | Strong, Hevy, Alpha [[9]][s9] [[3]][s3] [[17]][s17] | **Med** | High (reuse chart) | S–M | core, ui | Weekly aggregation; chart reuse |
| Weekly / monthly report | Hevy monthly (Pro), Boostcamp weekly (free) [[3]][s3] [[14]][s14] | **Med** | High | M | core, ui | `QTextDocument` → PDF with `QPdfWriter` |

### F. Data ownership

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Layer | What it teaches |
|---|---|---|---|---|---|---|
| CSV export | Strong (free), Alpha, Hevy, FitNotes, StrongLifts [[11]][s11] [[17]][s17] [[6]][s6] [[27]][s27] [[24]][s24] | **High**. Data that is locked in or hard to export is a recurring complaint [[35]][s35] | High | S | services | CSV quoting/escaping, `QSaveFile` |
| Import Strong / Hevy CSV | Hevy imports Strong; Liftosaur imports Hevy [[6]][s6] [[23]][s23] | **High**. People switch apps, and current imports lose data [[7]][s7] [[8]][s8] | High (file-based desktop) | M | services, ui (exercise-mapping dialog) | Adapter pattern, parsing, unit conversion |
| openweight JSON export | openweight CLI (Strong/Hevy → openweight) [[31]][s31] | **Low** (niche and new) | High | M | services | Mapping to an external schema |
| Rotating automatic backups | FitNotes (manual only) [[26]][s26] [[27]][s27] | **High**. Lifters lose years of data [[38]][s38] | High (atomic save already exists) | S | services | `QDir`, retention policy, timestamped files |
| kg / lb display toggle | Strong (mixed units) [[9]][s9] | **Med**. Unit mix-ups are a real bug class [[8]][s8] | High | S–M | core, ui | Convert only at the edges; strong types |
| Rename / merge / delete exercises (stable IDs) | Strong users complain they can't [[9]][s9] | **Med** | High (fixes known risk R-04) | M | core, services (schema v2) | Stable identity, data migration |
| Exercise library from wger data | wger (CC-licensed data) [[28]][s28] | **Med**. Limited exercise variety is the biggest complaint about workout apps [[36]][s36] | Med (licence varies per entry) | M | services | Data import, licence hygiene |

### G. Probably skip (for now)

| Feature | Which apps have it | User demand (evidence) | Fit | Effort | Why skip |
|---|---|---|---|---|---|
| Cloud sync / mobile app | All major apps | High | Low for a local-first v1 | L | Needs a server and conflict handling; Design Notes already reserves a QML path |
| Social feed | Hevy [[2]][s2] | Divisive [[32]][s32] [[35]][s35] | Low | L | Pushy social features are a listed complaint |
| AI-generated workouts | Fitbod [[13]][s13] | Mixed: guesses at weights [[13]][s13]; AI progression called unreliable [[34]][s34] | Low | L | Black box; hard to test |
| Nutrition | wger, Caliber [[28]][s28] [[39]][s39] | Users dislike workout apps that add nutrition [[35]][s35] | Low | L | Scope creep |

---

## Differentiators

1. **Nothing paywalled, ever.** Unlimited templates, full history, all charts, plate and warm-up calculators. These are exactly the items competitors cap [[1]][s1] [[9]][s9] [[14]][s14] [[17]][s17].
2. **Local-first, open, documented data.** Readable JSON schema, CSV export, optional openweight export, and rotating backups. This answers both the lock-in and the data-loss complaints [[31]][s31] [[35]][s35] [[38]][s38].
3. **Bring your history.** Import Strong and Hevy CSV with a clear exercise-mapping step, and keep units and dates correct. These are the places existing imports go wrong [[6]][s6] [[7]][s7] [[8]][s8].
4. **Spreadsheet speed with app analytics.** Keyboard-first entry for the lifters who still prefer spreadsheets [[37]][s37], plus the charts that spreadsheets lack (the TechRadar author admits apps win on graphs) [[37]][s37].
5. **Transparent, evidence-based analytics.** Show the formula behind every number, count secondary muscles as half-sets [[42]][s42], use editable MEV/MRV targets [[41]][s41], and flag stalls as a deload hint [[43]][s43]. This is a rule you can read, unlike a black box [[34]][s34].

---

## Pitfalls

- **Capping core features.** Routine limits and history limits draw the most resentment [[32]][s32] [[36]][s36] [[8]][s8].
- **Slow logging.** Every extra click per set costs users [[32]][s32] [[35]][s35] [[36]][s36].
- **Losing data.** Never rely on a single file. Keep backups automatic, not manual [[38]][s38] [[26]][s26].
- **Rigid exercise lists.** Let users rename, merge, and delete exercises and mix weight with time [[9]][s9].
- **Lossy or buggy import.** Watch units, time zones (one user saw dates shifted by a day), and supersets [[8]][s8] [[7]][s7].
- **Overstating accuracy.** Label e1RM as an estimate. Inaccurate data was the #1 negative-review theme in one analysis [[36]][s36] [[46]][s46].
- **Forcing a training model.** Rigid programming is disliked; suggestions should be optional [[32]][s32] [[37]][s37].
- **Bloat.** Avoid social features, gamification, and nutrition [[35]][s35].
- **Silent timers.** A rest timer that doesn't alert in the background is a known complaint [[35]][s35] [[8]][s8].

---

## Ranked picks for IronLog

**Score = demand × fit ÷ effort.** Values: High = 3, Med–High = 2.5, Med = 2, Low = 1. Effort: S = 1, S–M = 1.5, M = 2, L = 3. Ties are broken by strength of evidence.

| # | Feature | Score | Why |
|---|---|---|---|
| 1 | Edit / delete sets + undo | 9 | Baseline expectation; seam already designed; teaches `QUndoStack` |
| 2 | Keyboard-first fast entry | 9 | #1 complaint everywhere; desktop's biggest advantage |
| 3 | CSV export | 9 | Answers lock-in; tiny effort |
| 4 | Rotating automatic backups | 9 | Answers data loss; builds on atomic save |
| 5 | More PR types + rep-max table | 7.5 | Pure `core` work on existing data |
| 6 | Plate calculator (custom inventory) | 7.5 | Small algorithm; often paywalled elsewhere |
| 7 | RPE / RIR per set | 6 | High demand among hypertrophy lifters |
| 8 | Rest timer with tray alert | 6 | High demand; fit is Med on desktop |
| 9 | Warm-up calculator | 6 | Reuses the plate calculator |
| 10 | Per-muscle volume targets (MEV/MRV) | 6 | Extends IronLog's signature feature |

**Next tier:** set types (5), notes (6, but bundled with RPE), templates (4.5), Strong/Hevy import (4.5), double-progression hint (4.5).

**Tip:** RPE, set types, notes, stable exercise IDs, and secondary muscles all change the file format. Do them as **one schema v2 migration**, not five.

---

## Sources

**App vendors and help centres**
1. [Hevy Pro Subscription: How to get Pro and What Does It Include (Hevy Help)][s1]
2. [Everything You Need to Know About the Hevy App: 2025 Features Guide (Hevy Help)][s2]
3. [Hevy: Gym Progress features][s3]
4. [Hevy: Sets per muscle group per week][s4]
5. [Personal Records (PRs) and Set Records Explained (Hevy Help)][s5]
6. [How to Import Strong App CSV Files and Export Your Data in Hevy (Hevy Help)][s6]
7. [Tutorial: Log Previous Workouts and Import CSV (Hevy Help)][s7]
8. [Hevy: Workout Tracker Gym Log Reviews (JustUseApp)][s8]
9. [Strong Workout Tracker Gym Log (App Store)][s9]
10. [Strong App Review 2024: Think less. Lift more. (HotelGyms)][s10]
11. [Fitbod vs Strong vs Jefit vs Hevy: Which Workout Tracker Is Best in 2026? (JEFIT blog, *vendor*)][s11]
12. [JEFIT review (Garage Gym Reviews)][s12]
13. [Fitbod review (Garage Gym Reviews)][s13]
14. [Is Boostcamp free? (Boostcamp, *vendor*)][s14]
15. [Boostcamp features (Boostcamp, *vendor*)][s15]
16. [Best apps for RPE/RIR (Boostcamp, *vendor*)][s16]
17. [Gym Workout Alpha Progression (App Store)][s17]
18. [Alpha Progression vs Hevy (Alpha Progression blog, *vendor*)][s18]
19. [Alpha Progression vs Fitbod vs Hevy vs Boostcamp (FitnessDrum)][s19]
20. [RP Hypertrophy (App Store)][s20]
21. [Liftosaur README (GitHub)][s21]
22. [Liftosaur / Liftoscript documentation (GitHub Gist)][s22]
23. [Liftosaur Review 2026: Price, Free vs Premium (SensAI, *vendor*)][s23]
24. [Stronglifts 5x5 Workout (App Store)][s24]
25. [Stronglifts app: Free vs Pro (StrongLifts)][s25]
26. [FitNotes Is the Gym Log I Wish I'd Found Years Ago (Android Authority / WhistleOut)][s26]
27. [Keep a Gym Workout Log with FitNotes for Android (gHacks)][s27]

**Open source**

28. [wger README (GitHub)][s28]
29. [Flexify (GitHub)][s29]
30. [LibreFit (GitHub)][s30]
31. [openweight: Open data format for strength training (GitHub)][s31]

**User sentiment (aggregators; Reddit not read directly)**

32. [Best Workout Tracker: What Reddit Recommends (Cora, *vendor*)][s32]
33. [Best workout app on Reddit (GummySearch)][s33]
34. [Best fitness tracking apps: Reddit report (Discury)][s34]
35. [Best Workout Tracker App Reddit (Setgraph, *vendor*)][s35]
36. [Health & Fitness App Reviews: What Users Really Want in 2026 (Unstar)][s36]
37. [I've never found a better solution than a spreadsheet (TechRadar)][s37]
38. [Training log apps (Barbell Medicine forum)][s38]
39. [Why People Are Switching to This Free Workout App (AOL)][s39]

**Training science**

40. [Reps in Reserve (Stronger by Science)][s40]
41. [Training Volume Landmarks for Muscle Growth (Renaissance Periodization)][s41]
42. [Pelland et al.: The Resistance Training Dose-Response (SportRxiv preprint)][s42]
43. [Bell et al. 2024: Deloading practices in strength and physique sports (PMC)][s43]
44. [Deload Week: When to Back Off (StrengthLog)][s44]
45. [Progression (RippedBody)][s45]
46. [One-repetition maximum (Wikipedia)][s46]
47. [Strong CSV template (RepStack)][s47]

[s1]: https://help.hevyapp.com/hc/en-us/articles/35119778922263-Hevy-Pro-Subscription-How-to-get-Pro-and-What-Does-It-Include
[s2]: https://help.hevyapp.com/hc/en-us/articles/33106320824727-Everything-You-Need-to-Know-About-the-Hevy-App-2025-Features-Guide
[s3]: https://www.hevyapp.com/features/gym-progress/
[s4]: https://www.hevyapp.com/features/sets-per-muscle-group-per-week/
[s5]: https://help.hevyapp.com/hc/en-us/articles/35649367857175-Personal-Records-PRs-and-Set-Records-Explained-How-They-Work-in-the-Hevy-App
[s6]: https://help.hevyapp.com/hc/en-us/articles/38001424401943-How-to-Import-Strong-App-CSV-Files-and-Export-Your-Data-in-Hevy
[s7]: https://help.hevyapp.com/hc/en-us/articles/35687878672663-Tutorial-Log-Previous-Workouts-and-Import-CSV
[s8]: https://justuseapp.com/en/app/1458862350/hevy-gym-trainingstagebuch/reviews
[s9]: https://apps.apple.com/app/strong/id464254577
[s10]: https://www.hotelgyms.com/blog/the-strong-app-review-think-less-lift-more
[s11]: https://www.jefit.com/blog/fitbod-vs-strong-vs-jefit-vs-hevy
[s12]: https://www.garagegymreviews.com/equipment/jefit
[s13]: https://www.garagegymreviews.com/fitbod-review
[s14]: https://www.boostcamp.app/is-boostcamp-free
[s15]: https://www.boostcamp.app/features
[s16]: https://www.boostcamp.app/best/rpe-rir
[s17]: https://apps.apple.com/us/app/gym-workout-alpha-progression/id1462277793
[s18]: https://alphaprogression.com/en/blog/alpha-progression-vs-hevy
[s19]: https://fitnessdrum.com/alpha-progression-vs-fitbod-vs-hevy-vs-boostcamp/
[s20]: https://apps.apple.com/us/app/rp-hypertrophy/id1555614554
[s21]: https://github.com/astashov/liftosaur
[s22]: https://gist.github.com/astashov/b38bb3c1b637738fae62f9b4e41fb981
[s23]: https://www.sensai.fit/blog/liftosaur-review
[s24]: https://apps.apple.com/us/app/stronglifts-5-5-workout/id488580022
[s25]: https://stronglifts.com/app/pro
[s26]: https://cellphoneplans.androidauthority.com/CellPhones/Guides/fitnotes-app-review
[s27]: https://www.ghacks.net/?p=90924
[s28]: https://github.com/wger-project/wger
[s29]: https://github.com/brandonp2412/flexify
[s30]: https://github.com/odweta/LibreFit
[s31]: https://github.com/radupana/openweight
[s32]: https://www.corahealth.app/blog/best-workout-tracker-reddit
[s33]: https://gummysearch.com/tools/best-products/workout-app/
[s34]: https://discury.io/report/best-fitness-tracking-apps-reddit
[s35]: https://setgraph.app/ai-blog/best-workout-tracker-app-reddit
[s36]: https://unstar.app/blog/health-fitness-app-reviews-what-users-really-want-2026
[s37]: https://www.techradar.com/health-fitness/forget-whoop-apple-garmin-and-all-the-rest-when-it-comes-to-lifting-weights-ive-never-found-a-better-solution-than-a-spreadsheet
[s38]: https://forum.barbellmedicine.com/t/training-log-apps/405
[s39]: https://www.aol.com/articles/why-people-switching-free-workout-143000340.html
[s40]: https://www.strongerbyscience.com/reps-in-reserve
[s41]: https://rpstrength.com/training-volume-landmarks-muscle-growth/
[s42]: https://sportrxiv.org/index.php/server/preprint/view/460
[s43]: https://pmc.ncbi.nlm.nih.gov/articles/PMC10948666
[s44]: https://www.strengthlog.com/deload-week/
[s45]: https://rippedbody.com/progression/
[s46]: https://en.wikipedia.org/wiki/1RM
[s47]: https://repstack.pages.dev/tools/strong-csv-template/
