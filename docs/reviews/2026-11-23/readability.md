# IronLog docs: readability review

> **Agent report, kept as written** (2026-11-23). Reviewed commit
> `5939378`. File:line references point to that commit, so many have moved
> since. What happened to each finding: [`../2026-11-23-doc-review.md`](../2026-11-23-doc-review.md).

**Main reader:** the owner: knows Java, is learning C++, Qt and AI, and needs short, scannable docs.
**Scope:** 19 docs (see the score table). Research, archive and question-bank.md were skipped. No repo file was changed.

## Summary

1. **What works well:** DEVELOPING, RELEASING, ADR-023 and the assets README already suit him. They use tables, numbered steps and short lines.
2. **What doesn't:** BACKLOG, arc42, the Use-Case Model and ADR-020/022. Their lists are long run-on sentences of 30–73 words, and about 45 terms are used before they are explained. For example, "TickTimer" appears 17 times in arc42 and is never explained.
3. **Fix first:**
   - Some concepts have more than one name. Two names even clash: "lighter week(s)" means both Pause and deload, and "check-in" means both the body check-in and the session review.
   - The Use-Case Model has a broken duplicate table.
   - Some numbering runs out of order.

---

## Scores (1 = hard, 5 = easy)

| Doc | Score | Why |
|---|---|---|
| README.md | 3 | Good tables. But the top has a 73-word sentence, and 1RM, MMO, CI/CD and QML are not explained. |
| docs/01_Vision.md | 3 | Great one-line summary and *Train now* diagram. But two table cells run to ~100 words, and the history is out of order. |
| docs/02_UseCaseModel.md | 2 | A broken duplicate table at lines 48–58. Method jargon is not explained ("inception", "fully dressed", ⭐, "5a"). |
| docs/04_Glossary.md | 3 | Definitions are short and concrete. But it is one 74-row table with no groups, tech words are missing, and "lighter week" clashes with deload. |
| docs/05_RiskList.md | 3 | Bold risk names help. But it says "ranked" and isn't, the IDs jump around, and the H/M/L key is at the bottom. |
| docs/arc42.md | 2 | Good "how to read it" map. But it is 598 lines, §8 is out of order, and it has the most jargon in the repo (GRASP, Argon2id, RAMP, TickTimer). |
| docs/BACKLOG.md | 2 | The rules and roadmap table are clear. But each item is a 30–62-word run-on line. |
| docs/DEVELOPING.md | 4 | Has a diagram, a rule table and steps. One 32-word sentence. |
| docs/RELEASING.md | 4 | Numbered steps, each with a "Proof". Some table cells are dense. It says "Run workflow", where DEVELOPING says "Release button". |
| docs/adr/README.md | 4 | Short, with numbered rules. "Pure Fabrication" in the index is not explained. |
| ADR-020 (photo encryption) | 2 | All the crypto is in 3 sentences. Argon2id, KDF, salt and secretstream are not explained. |
| ADR-021 (optional game) | 4 | Short and clear. |
| ADR-022 (game rules v2) | 2 | The decision is one 55-word chain of semicolons. It should be a table. |
| ADR-023 (four tabs) | 4 | Lists and numbered "homes". "Left rail" and "bottom sheet" are not explained. |
| ADR-024 (code-drawn pictures) | 3 | Well argued but long. IK, cel shading and "d = 1.06" are not explained. |
| ADR-025 (ChatGPT pictures) | 3 | Clear bullets. Uses "how picture" and "muscles picture" where other docs use other names. |
| assets/exercises/README.md | 4 | Short, table first, concrete steps. Could show one real file name (`ironlog-goblet-how.jpg`). |
| assets/exercises/CREDITS.md | 3 | Clear table. One 46-word sentence, and notes are mixed in with the prompts. |
| prototypes/ui-prototype/README.md | 3 | Great opener ("Question it answers") and tour. But the "Real" column lists a rejected design, and the code notes are dense. |

---

## Findings (most important first)

| # | File:line or section | Problem | Rewrite |
|---|---|---|---|
| 1 | 02_UseCaseModel.md:48–58 | A broken leftover table starts with `----\|------\|`. It repeats UC-01 to UC-10 with old values ("Future", "Follow a Program Template", "iteration 1"). The reader sees two answers for the same IDs. | Delete lines 48–58. |
| 2 | README.md:15–21 | One sentence is 73 words: "The plan (see Vision and Backlog) turns it into a phone-first training coach that plays like an MMO: *Train now* builds a workout …; 8 skills level up …; a weekly quest …; every exercise has …; data syncs …" | v0.1 is a desktop logbook. The goal is a **phone-first training coach**:<br>• **Train now** builds today's workout from your time, place and equipment.<br>• **Game (optional):** 8 skills level up as you train. They fade slowly if you stop. A Pause protects exam weeks.<br>• **Weekly quest:** finish it to earn a rest token and your own reward.<br>• **Pictures:** each exercise has a how-to picture and a muscle diagram.<br>• **Sync:** your data goes to your own server. |
| 3 | 01_Vision.md:51–52 | The Coach cell (106 words) and the Game cell (100 words) are walls of text inside a table. | In the module table, change the Game cell to "8 skills that level up. Details below." Then add a small table:<br>Character sheet → body map; muscles light up by level<br>XP → earned per set, cardio minute, stretch minute<br>Decay → a skill fades after 2 weeks off; you are warned first<br>Pause → stops decay (exams, sick, injured, travel)<br>Comeback bonus → double XP while you climb back<br>Weekly quest → done = rest token + your own reward<br>Streak → weeks in a row; rest days and pauses count<br>Do the same for Coach: one row each for Goal programs, Train now, Guided session, Session review, Exercise pictures. |
| 4 | 04_Glossary.md:47 and :74 | One name, two meanings. "**Lighter weeks** — the game-off name for a Pause" vs "**Deload** — a planned lighter week after a stall". | Delete the "Lighter weeks" row. In the Pause row, add: "Same name with the game off." In ADR-023:24 and BACKLOG:110, write `"Pause" row on *You*`. If a softer label is wanted, use "Break", never "lighter". |
| 5 | 04_Glossary.md:62; 01_Vision.md:51; BACKLOG.md:89 | "Check-in" means two things: the weekly **body** check-in (weight + waist) and the end-of-session "How was it?" questions. | Use "check-in" only for the body check-in.<br>Vision:51 → `**Session review ("How was it?")** at the end: …`<br>BACKLOG:89 → `**F10** Session review ("How was it?"): …`<br>Glossary:62 alias → `How was it?` |
| 6 | arc42.md (17 uses, first :74); 01_Vision.md:88; 05_RiskList.md:21; BACKLOG.md:11; RELEASING.md:4 | "TickTimer" is never explained in any doc in scope. | At first use in each doc: `TickTimer (the owner's earlier Qt app, with its own sync server)`. Also add it to the Glossary. |
| 7 | 04_Glossary.md (whole) | One 74-row table, neither grouped nor alphabetical. Five columns, and most Format and Validation cells are "—". Rejected-prototype terms (Key pose, IK) are mixed in. "Lock key" (:67) uses "envelope A" before Envelope A/B is defined (:70). | Split it into short tables with headings: **Training** · **Coach** · **Game** · **Photos and keys** (Envelope encryption first, then Envelope A/B, then the keys) · **Sync and server** · **Exercise pictures** · **Prototype only**. Keep 2 columns: Term, Meaning. Move Format and Validation into one "Data rules" table for code readers. |
| 8 | 04_Glossary.md (missing) | Tech words used across the docs are not in the Glossary: MMO, QML, WASM, VPS, CI/CD, ADR, arc42, UP, spike, APK, TickTimer, Caddy, systemd, Argon2id, KDF, GRASP, view model. | Add a "## Tech words" table. Each row is the term plus a definition of 5 words or fewer, e.g. `QML → Qt's language for app screens`. The ready-made list is in the Appendix below. |
| 9 | BACKLOG.md:105–110 (F14) | 62-word run-on: "Weekly quest → rest token (bank max 2, protects the streak) + your own reward text; level-up and quest-complete moments; weekly streak (…) + training heat map; quest card lists …; *You* tab has a game-off version (…)" | **F14** Weekly quest and streak *(M)*<br>• Quest card lists the week's planned sessions.<br>• Quest done → +1 rest token (max 2) + your own reward.<br>• Weekly streak: rest days count, pauses keep it, logging a past day can restore a week.<br>• Heat map of training days.<br>• Level-up and quest-done moments.<br>• Game-off *You* tab: program, this week, calendar, New exercises, Pause. |
| 10 | BACKLOG.md:133–138 (F21) | 58-word run-on about progress photos and encryption. | **F21** Progress photos (Must = top priority) *(L)*<br>• Front, side and back photo at the weekly check-in.<br>• Compare week 1 with now, side by side, with a pose guide.<br>• Stored on the phone. Synced encrypted (ADR-020).<br>• Ask for the recovery code at the first photo.<br>• Never in exports or the repo. |
| 11 | BACKLOG.md:83–86 (F8); same problem in F1 :59, F13 :102, F15 :114 | 49-word run-on: "time → situation → gym/home or nearby equipment → plan with XP preview; *Sore* asks …; "Same as last time" skips the wizard; reusable **Why?** sheet …; after 3 cramming days …" | **F8** Train now screens (QML) *(M)*<br>• Questions: time → how's today → gym/home or what's nearby.<br>• Shows the plan with an XP preview.<br>• *Sore* asks which body parts.<br>• "Same as last time" skips the questions.<br>• Why? sheet on every number.<br>• 3 cramming days in a week → Today offers "Start a pause?"<br>Use the same sub-bullets for F1, F13 and F15. |
| 12 | adr/ADR-022:13 (also :9, 34 words) | The decision is one 55-word chain: "Decay after 14 days at −0.5 %/day with a 3-day warning; 50 % floor; Comeback bonus ×2; Pause with a reason (…); weekly forgiving streak; rest token …" | Make it a 2-column table (Rule → Value):<br>Decay starts → after 14 days off<br>Decay speed → −0.5 % XP a day<br>Warning → 3 days before<br>Floor → never below 50 % of peak<br>Comeback bonus → XP ×2<br>Pause limits → exams 21 · sick 14 · injured 42 · travel 21 days<br>Streak → counted in weeks, forgiving<br>Quest reward → rest token (max 2) + your own reward<br>Rules file → has a version number |
| 13 | adr/ADR-020:9–13 | Dense crypto, with Argon2id, KDF, salt and secretstream not explained. Worst sentence (36 words): "It is locked twice: envelope A with a lock key derived from the password (Argon2id → KDF; never leaves the device) and envelope B with a recovery code shown once …" | **Envelope encryption** = one key locks the photos; other keys lock that key.<br>1. A random **photo key** encrypts every photo.<br>2. The photo key is locked twice:<br>  • **Envelope A** is locked by a key made from your password. That key never leaves the phone.<br>  • **Envelope B** is locked by a **recovery code**, shown once.<br>3. The server gets a **login key**, never the password.<br>*(Argon2id = slow password-hashing method. KDF = makes keys from a secret.)* |
| 14 | arc42.md §8 (:394, :399, :415, :462, :480); arc42.md §10 (:551–554); 01_Vision.md:114–125 | Numbers run out of order in three places:<br>• arc42 §8 headings go 8.12 → **8.15 → 8.14 → 8.13** → 8.16.<br>• arc42 §10 rows go Q8 → **Q10 → Q9** → Q11.<br>• Vision revisions go v0.1, v2, **v2.5, v2.4, v2.3, v2.2, v2.1**.<br>Readers who scan by number get lost. | • Put the arc42 §8 headings in number order: 8.12, 8.13 (Training rules v0.1), 8.14 (Photos), 8.15 (How was it?), 8.16. Add a mini-index under "## 8.": `8.1 Data · 8.5 Planner · 8.6 Game · 8.14 Photos · 8.16 Pictures`.<br>• Sort §10 rows Q1–Q11.<br>• Vision history: newest first (v2.5 → … → Inception draft). |
| 15 | 05_RiskList.md:3, :28–37 | The opener says "ranked", but the rows are not sorted by risk. The IDs jump (R-16, R-18, R-20, R-21, R-19, R-22, R-23, R-17). The key `H = High, M = Medium, L = Low` is under the table. | Move the key above the table. Then sort: Open/Watching first, by Impact then Likelihood (R-07 H/H first). Mitigated/Accepted go last. If you won't sort, change the opener to: "Sorted by ID. *Status* shows what's still open." |
| 16 | README.md:5–7, :89 | "1RM" is in the first paragraph and never explained. "e1RM" appears in the test table and is not linked to "estimated 1RM". The first sentence is 26 words. | A desktop **training log** in C++17 and Qt 6.<br>• Log your sets.<br>• See personal records (PRs) the moment you hit them.<br>• Check weekly sets per muscle group.<br>• Watch your **e1RM** climb on a chart.<br>*(e1RM = estimated one-rep max: the most you could lift once.)* |
| 17 | README.md:25–29 (roadmap) | The first table hits five new terms at once: "CI/CD, data format v2, QML/Android/web spike … XP … server on the VPS". | v0.2 → `Foundation: automatic build and release (CI/CD), data format v2, a test build of the phone UI (QML spike)`<br>v0.4 → `Game: skills, XP (experience points), slow decay, pauses, quests, streak, photos`<br>v0.5 → `Online: server on the owner's rented server (VPS), accounts, sync` |
| 18 | 01_Vision.md:12–14; README.md:16–17; arc42.md:35 | "Plays like an MMO" is never explained. The game is also optional (ADR-021) and has no multiplayer (Vision:86). The Vision sentence is 37 words. | **IronLog is a phone-first training coach.** It plans today's workout around your time, place and situation. An optional game levels up your body part by part, like skills in an MMO (online role-playing game). Everything syncs to your own server. |
| 19 | README.md:29, :135; arc42.md:575 | Three stale facts contradict other docs:<br>• README v0.6 lists "body check-ins", but BACKLOG:33 puts F18 in v0.4.<br>• README says "prototypes/ … (empty)", but ui-prototype/ exists.<br>• arc42 says "No CI yet (backlog I2)", but CI runs and I2 is not in the backlog. | • README:29 → `v0.6 — Progress+: more PR types, plate calculator, stall → deload hint`<br>• README:135 → `├── prototypes/   throwaway experiments (ui-prototype/)`<br>• arc42:575 → delete the line. |
| 20 | arc42.md §8.14, :415–431 | The section jumps straight into a 6-row key table. libsodium, `crypto_pwhash`, `crypto_kdf`, Argon2id and salt are not explained. "EyesOff" is never explained either. | Add before the table:<br>**Idea in one line:** a random photo key locks the photos. That key is kept in two locked "envelopes": one opens with your password, one with a recovery code.<br>**Words:** libsodium = crypto library · Argon2id = slow password hash · KDF = makes sub-keys · salt = random extra input per user.<br>For EyesOff, say what it is in 5 words or drop the name. |
| 21 | arc42.md:433–436 | 51-word run-on: "What the server stores (…): hash of the login key · hash of the recovery auth key · envelope A (…) · envelope B (…) · encrypted photos and encrypted previews." | **What the server stores** (useless without the password or code):<br>• hash of the login key<br>• hash of the recovery key<br>• envelope A: the photo key, locked by your password<br>• envelope B: the photo key, locked by the recovery code<br>• encrypted photos and previews |
| 22 | arc42.md:331–336 | A 50-word italic paragraph: "*Game rules v2 …, changed after research round 2 (…): decay was gentler than v1 (…), cheat-meal token replaced (…), streak made weekly and forgiving (…).*" Italics also make long text harder to read. | **Why v2 changed the game (research round 2):**<br>• Gentler decay. Strength barely drops in 3 weeks off.<br>• No cheat-meal token. "Cheat" wording breeds guilt and is linked to eating-disorder symptoms.<br>• Weekly, forgiving streak, like Duolingo's streak freeze. |
| 23 | arc42.md:150–161 | The "Pattern / reason" column uses names with no meaning given: "GRASP Controller", "Information Expert", "Creator", "Pure Fabrication", "private ctor", "Stroustrup §16.2.4". | Add a key under the table:<br>**Pattern names** (GRASP = Larman's rules for which class does what):<br>• Information Expert: the data owner does the work.<br>• Creator: a container makes its parts.<br>• Pure Fabrication: a helper class, not from the domain.<br>• Controller: one entry point for all actions.<br>• ctor = constructor. |
| 24 | arc42.md:294–302 (planner rules) | P2 uses "RAMP" and "isolation" with no meaning given. P3 uses "compound". The Source column cites "C8", "D3, D7" from a private report the reader can't open. | P2 → `Warm-up 5–8 min (RAMP: raise, activate, mobilise, potentiate) …; drop one-joint (isolation) exercises first`.<br>Under the table: `Compound = multi-joint exercise. C8, D3, D7 = sections of the owner's private Decision Report.` |
| 25 | 01_Vision.md:3–6; 02_UseCaseModel.md:3–5; 04_Glossary.md:3–4; 05_RiskList.md:3–5 | Each opener puts book references in the first lines ("Larman, *Applying UML and Patterns*, Ch. 4 §4.3, Table 4.1; written first per Ch. 7"). None says who should read it, or when. | Use the same 2-line opener in every doc, e.g. for the Vision:<br>`> **What:** the 3-minute summary: goals, users, scope.`<br>`> **Read it:** first, before any other doc.`<br>Move the Larman references to "Sources" at the end. |
| 26 | 02_UseCaseModel.md:4–5, :20, :62, :88 | Method jargon with no meaning given: "During inception, name most use cases but write only 10–20% in detail", "(fully dressed)", the ⭐ column, and extension IDs like "6a". | • Under the table: `⭐ = written out in full below ("fully dressed": every step + what can go wrong).`<br>• Above the first Extensions list: `**Extensions** = what happens when a step goes differently. "6a" = another path at step 6.`<br>• Drop the inception sentence, or add "(inception = the first planning phase)". |
| 27 | 02_UseCaseModel.md:142–144, :164 | A 29-word success guarantee. "zone-2" is not explained. | **Success guarantee:** a plan is shown that:<br>• fits the time,<br>• uses only the equipment you have,<br>• stays inside your limits (e.g. knee stage),<br>• gives a one-line reason for each exercise.<br>Nothing is logged yet.<br>:164 → `easy zone-2 cardio (you can still talk)` |
| 28 | README.md:174 (Docs table) | The docs map is near the end (line 174 of 201). Nothing tells the owner which docs to read first. | Add after line 7:<br>`> **Start here:** [Vision](docs/01_Vision.md) (what and why) → [BACKLOG](docs/BACKLOG.md) (what's next) → [DEVELOPING](docs/DEVELOPING.md) (how we work).` |
| 29 | BACKLOG.md:173–177 ("Skipped") | A 40-word run-on joined with "·". The reasons sit in brackets, and some items have none. | Make it a table (Skipped → Why):<br>Programmable progression rules → planner rules cover it<br>wger exercise import → per-entry licences<br>Choice of 1RM formula → one estimate is enough<br>openweight export → *(add a reason)*<br>Social feed → out of scope<br>AI-generated workouts → rule-based planner (ADR-013)<br>Nutrition logging → out of scope (Vision) |
| 30 | BACKLOG.md:41–43 ("Now") | The section lists bare IDs: "**I4** · **F1** · **F2** · **F3** · **F4**". The reader has to scroll to decode them. | Make it a table (Order · ID · What):<br>1 · I4 · QML test build on the phone<br>2 · F1 · Data format v2<br>3 · F2 · Exercise catalog v2<br>4 · F3 · Edit a set + undo<br>5 · F4 · Daily backups + CSV export |
| 31 | prototypes/ui-prototype/README.md:35, :23 | The column headed "Real (same as the planned app)" lists "Exercise pictures (ADR-024, **rejected** …)". The tour (:23) still says "muscle map". This contradicts ADR-025. | Take that item out of the Real column. Add a line under the table:<br>`**Not in the real app:** the animated figures (ADR-024, rejected). The app uses the ChatGPT pictures in assets/exercises/ (ADR-025).`<br>:23 → `(old code-drawn figure, muscle diagram, form cues)` |
| 32 | prototypes/ui-prototype/README.md:41–57, :64 | Dense code notes. One is 45 words: "`drawAnime()` adds cel shading (`cap()` = outline + fill + shade strip), a profile head drawn in a local frame and rotated to the spine, and effects from joint velocity." :64 is 39 words. | Make it a table (Piece → Code → How):<br>State → `S` → one object holds everything<br>Screens → `view…()`, `render()` → turn `S` into HTML; redraw<br>Buttons → `data-a`, `data-v` → one click handler for all (event delegation)<br>Figures → `MEDIA`, `ik()`, `poseAt()` → 2–3 poses; `ik()` bends knees and elbows<br>Timers → `restEnds` → store the end time; a 250 ms tick updates the numbers<br>:64 → **Tested:** headless Chrome (no window) clicked every flow on a 390 × 844 screen. A script drew every exercise, light and dark. No errors, no tap target under 44 px. |
| 33 | RELEASING.md:13–14, :28, :36–38 | Table cells chain 5–6 steps with "·". "LTS" and "smoke-tests" are not explained. This doc says "Run workflow", where DEVELOPING says "Release button". 27-word sentence: "It refuses to publish if the version and `CMakeLists.txt` disagree, if the version isn't `X.Y.Z`, or (button) if that tag already exists — fix and start again." | :36 → It stops (no release) if:<br>• the version ≠ `CMakeLists.txt`,<br>• the version isn't `X.Y.Z` (e.g. `0.2.0`),<br>• you used the button and the tag already exists.<br>Fix it, then start again.<br>:28 → `**Release button:** Actions → Release → Run workflow`.<br>:13 → `Qt 6.8 LTS (long-term support)`, `smoke-test (quick "does it start" check)`. |
| 34 | DEVELOPING.md:31–34 | 32-word sentence: "If we both changed the **same lines**, `git pull` stops with a *merge conflict*: git marks both versions in the file, you keep the right one, then `git add` + `git commit`." | **Merge conflict** = we both changed the same lines.<br>1. `git pull` stops and marks both versions in the file.<br>2. Keep the right version and delete the markers.<br>3. `git add` + `git commit`.<br>Different lines merge on their own. |
| 35 | adr/ADR-024:40–45; assets/exercises/CREDITS.md:23–26 | Two list-sentences:<br>• ADR-024 (42 words): "Default: **Anime** — an original cel-shaded character (ink outline, flat colour + one shade, spiky hair, headband, big eye) with manga effects: …"<br>• CREDITS (46 words): "key form points asked for: shins vertical / knees over feet (squats, lunge), flat back (rows), …" | ADR-024 → Default style: **Anime**.<br>• Look: ink outline, flat colour + one shade, spiky hair, headband.<br>• Effects: speed lines, sweat drop on holds, sparkle per rep.<br>CREDITS → a table (Form point → Exercises):<br>Shins vertical, knees over feet → squats, lunge<br>Flat back → rows<br>Straight body line → push-up, plank, ring row<br>Lower back on the mat → dead bug<br>Knees not locked → leg press |

---

## Terminology: use one name

| Concept | Use this | Other names found | Files |
|---|---|---|---|
| Picture of the muscles an exercise works | **muscle diagram** | muscle map; muscle picture; muscles picture | Glossary:82 (alias); ADR-024 title, :37; adr/README:53; arc42:532; ui-prototype README:23, :35; ADR-025:10, :12; assets README:10 |
| Picture showing how to do an exercise | **how-to picture** | how picture; form picture; form figure; "How" | Glossary:81 (alias); ADR-025:10; ADR-024:21, :32, :69; ui-prototype README:35 |
| Game body that lights up by skill level | **body map** (game only) | Easily confused with "muscle diagram" or "muscle map". Overlaps "character sheet". | Vision:52; BACKLOG:99; arc42:321, :345; Glossary:73. Add "Not the exercise muscle diagram" to Glossary:73. |
| Time off that stops decay | **Pause** (in both game modes) | Lighter weeks; exams pause; exam mode; pause period. Clashes with deload ("lighter week"). | Glossary:47, :56, :74; BACKLOG:61, :110; ADR-023:24, :25; arc42:408 |
| Game on/off setting | **game mode** (UI label: "Play it like a game?") | game layer; game on/off; gamification; "Game (optional — a checkbox)" | ADR-021 title; adr/README:50; arc42:529; ADR-023:21; BACKLOG:77; ui-prototype README:25; Glossary:48; RiskList:24; Vision:52 |
| End-of-session questions | **session review ("How was it?")** | check-in; "How was it?" check-in; end-of-session check-in | Glossary:62; Vision:51; BACKLOG:89 |
| Weekly weight + waist entry | **body check-in** | check-in; weigh-in | Glossary:61; BACKLOG:127, :133; README:27, :29 |
| Sets per muscle per week | **weekly sets** | weekly volume (also the tab name). "Volume" also means weight × reps. | README:37, :43; 02_UseCaseModel:25, :102; Glossary:17–19. Rename the tab "Weekly sets", or explain it under the README screenshot. |
| Estimated one-rep max | **e1RM** (define once) | estimated 1RM; Est. 1RM; estimated max; estimated-1RM | README:7, :36; RiskList:18; Glossary:22; Vision:54 |
| Goal-based training plan file | **goal program** | program template; program; plan | 02_UseCaseModel:57 (dead fragment); Vision:32 |
| iPhone/browser version of the app | **web app** | web version; web build; WASM page; web bundle; Qt WebAssembly bundle | DEVELOPING:64, :97; BACKLOG:50; arc42:220, :228; RELEASING:58 |
| Button that starts a release | **Release button** | Run workflow button | RELEASING:14, :28; DEVELOPING:17, :68, :114 |
| The *Train now* questions | **Train now** (+ "the questions") | Train now wizard; wizard; flow. "Wizard" is jargon. | arc42:125; ADR-023:18; BACKLOG:85; Glossary:39 |
| Being logged in | **login session** | session; session tokens; browser session. Clashes with training "session". | arc42:367, :455; Glossary:83 |
| Exam-week rule (minimum training) | **exam-week minimum** | exam minimum | arc42:300; Vision:99; 02_UseCaseModel:163 |
| The app's cartoon athlete | **IronLog character** | anime character; anime-style character | Vision:51; arc42:483; ADR-025 title, :17; Glossary:81; assets README:7 |

---

## Appendix: short definitions to paste at first use

Each definition is 5 words or fewer. "Define first in" lists the first use in each doc.

| Term | Definition | Define first in |
|---|---|---|
| 1RM | max weight for one rep | README:7 |
| e1RM | estimated 1RM, from a set | README:89; RiskList:18; arc42:41 |
| RIR | reps you had left | Vision:53; BACKLOG:60 |
| XP | experience points (game score) | README:27; Vision:52; 02:155; RiskList:15 |
| MMO | online role-playing game | README:17; Vision:12; arc42:35 |
| QML | Qt's language for app screens | README:25; RiskList:25; BACKLOG:49 |
| WASM / WebAssembly | runs C++ in a browser | Vision:95; arc42:45; RiskList:25; RELEASING:58 |
| CI / CD | automatic build+test / automatic release | README:25; Vision:33; arc42:22 |
| VPS | rented server online | README:28; Vision:43; 02:12 |
| spike | small throwaway test build | README:25; RiskList:25; DEVELOPING:69 |
| APK | Android app install file | Vision:88; DEVELOPING:69 |
| UP | Unified Process (Larman's method) | README:134; RiskList:3; arc42:77 |
| arc42 | 12-part architecture doc template | README:134 |
| ADR | Architecture Decision Record | README:143 |
| TickTimer | owner's earlier Qt app | Vision:88; RiskList:21; arc42:74; BACKLOG:11; RELEASING:4 |
| Caddy | web server; adds HTTPS | Vision:96; arc42:74 |
| systemd | Linux service manager | Vision:96 |
| loopback / 127.0.0.1 | reachable only inside server | RiskList:27; arc42:97 |
| 409 | server reply: "conflict" | arc42:187; BACKLOG:116 |
| tombstone | "deleted" mark kept for sync | BACKLOG:59; arc42:256 |
| GRASP | Larman's class-responsibility rules | arc42:159 |
| Pure Fabrication | helper class, not domain | adr/README:34; arc42:158 |
| envelope encryption | key locked by other keys | BACKLOG:135; ADR-020:9; arc42:415 |
| Argon2id | slow password-hashing method | ADR-020:13; arc42:426 |
| KDF | makes keys from a secret | ADR-020:13 |
| salt | random extra input per user | ADR-020:13; arc42:426 |
| zone 2 | easy cardio; you can talk | 02:164; arc42:296 |
| deload | planned easy recovery week | Vision:54; BACKLOG:146 |
| compound / isolation | multi-joint / one-joint exercise | 02:162; RiskList:15; arc42:297 |
| RAMP | four-step warm-up method | arc42:297 |
| hypertrophy | muscle growth | arc42:474 |
| IK (inverse kinematics) | maths that bends limb joints | ADR-024:34; ui-prototype README:35 |
| cel shading | flat cartoon-style colouring | ADR-024:41; ui-prototype README:50 |
| inception | first planning phase | 02:4 |
| fully dressed | full step-by-step use case | 02:62 |
| Must | top priority (MoSCoW) | Vision:124; BACKLOG:133 |
| LTS | long-term support version | RELEASING:13 |
| smoke test | quick "does it start" check | RELEASING:14 |
| property test | test over many random inputs | RiskList:23; BACKLOG:82; arc42:548 |
| view model | C++ data behind a screen | BACKLOG:49 |
| bottom sheet | panel that slides up | ADR-023:28 |
| left rail | side tab bar (wide screens) | ADR-023:16 |
| provenance | where it came from | ADR-025:27 |
| headless | runs without a screen | README:116; ui-prototype README:64 |
| CORS | browser cross-site rule | arc42:220 |
