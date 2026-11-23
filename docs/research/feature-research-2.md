# IronLog: Feature Research, Round 2

> **Date:** 2026-11-23 · **Scope:** game mechanics, coach-style planners, progress photos, voice logging, mobility players, benchmark tests, exercise media licences, app lock, reminders
> **Builds on:** [`feature-research.md`](feature-research.md), which covers basic loggers (Hevy, Strong, JEFIT, Fitbod, Boostcamp, Alpha, RP, Liftosaur, StrongLifts, FitNotes). This report does not repeat it.
> **Read this first:**
> - Prices and ratings are what the store page showed when I fetched it (Nov 2026). They are US store figures unless noted, and they change often.
> - *(vendor)* marks a source written by the company that sells the product.
> - *(analysis)* marks my own reasoning, with no source behind it.
> - *(general knowledge)* marks a fact I did not check in this round.
> - PubMed and PMC blocked automated reading, so some studies are cited through a news release or a mirror. Each such case is noted.
> - Citations look like [[12]][r12]. Every one links to its source.

---

## Summary

- **Gamification helps a little, and mostly only while it runs.** In a meta-analysis of 16 trials, the effect was g = 0.42 during the intervention and g = 0.15 at follow-up [[3]][r3]. Pokémon GO's extra steps had faded by week 6 [[6]][r6]. Takeaway: keep the game optional and keep adding fresh content.
- **Forgiving apps beat punishing ones.** Apple added a ring pause of up to 90 days that keeps the streak [[11]][r11]. Letting Duolingo users hold 2 streak freezes raised daily users and did not lead to more days off [[9]][r9]. Strava and Hevy count streaks by week [[12]][r12] [[14]][r14]. Gentler Streak counts rest days toward the streak [[15]][r15].
- **Replace the "cheat-meal" token.** Cheat meals are linked to eating-disorder symptoms in young adults [[24]][r24] [[25]][r25], and dietitians advise dropping the "cheat" label [[27]][r27]. A reward you expect, can hold, and earn for a task is the kind most likely to undermine intrinsic motivation [[22]][r22]. A non-food **rest token** that works like a streak freeze keeps the "bank max 2" mechanic.
- **Our decay is harsher than the body.** In one study, no group lost maximal strength in the first 3 weeks off, and over 6 weeks the whole group lost only 1.2% [[30]][r30]. Our rule (−1%/day after 10 days) has already removed about 13% of XP by day 24 *(analysis)*. Make decay gentler. Keep the muscle-memory bonus, because research shows lost gains come back [[31]][r31].
- **Coach apps ask the same three things as "Train now":** time, equipment, and soreness [[32]][r32] [[39]][r39] [[36]][r36]. Trust is the weak spot. Fitbod's CEO says users now want to know *why* [[34]][r34], and some users say they can't trust the suggested weights [[35]][r35]. IronLog's one-line reason per exercise answers this directly, so also show how the user's feedback changed the plan.
- **Progress photos follow a standard pattern:** a ghost overlay, front/side/back poses, side-by-side or swipe comparison, and gentle reminders [[45]][r45] [[46]][r46] [[48]][r48]. None of the photo apps I read mentions encryption, and the main fear users report is **losing their photos** [[46]][r46]. Encrypting before sync sets IronLog apart, but if the user loses the key they lose every photo. Browser storage on iPhone can also be wiped [[52]][r52].
- **Voice logging is a wave of new, barely-rated apps** [[55]][r55] [[56]][r56] [[57]][r57], so demand is unproven. In one study, some users liked voice input and others felt awkward talking [[60]][r60]. The cheapest win is a **gym-phrase parser for typed or dictated text** now. On-device speech recognition can come later.
- **Mobility players are well loved.** Bend is rated 4.8 from about 175K ratings [[67]][r67]. Complaints are about unclear holds, transitions that are too short, no easier versions, and streaks that reset completely after one miss [[67]][r67] [[68]][r68].
- **Benchmarks:** Peloton retests after about 6 weeks [[70]][r70], and a trainer quoted by Nike says every 1–3 months [[71]][r71]. That fits **"boss fights" every 4–6 weeks** built from safe, sub-maximal tests (reps, holds, time).
- **Exercise media licences are messy.** free-exercise-db uses the Unlicense, but its maintainer says image licensing info still needs to be added [[72]][r72] [[73]][r73]. wger's maintainer says some uploaded images may not be free [[77]][r77]. One MIT-licensed dataset ships proprietary GIFs [[80]][r80]. **Store the licence of every asset.**

---

## A. Gamified fitness and habit apps

### A1. Apps

| App | Feature | How it works | What users say |
|---|---|---|---|
| **Habitica** | RPG habit tracker | Tasks earn XP and gold. Missed Dailies cost HP. During boss quests, your missed Dailies damage the whole party. At 0 HP you lose a level, all your gold, and one item [[17]][r17]. Code is GPL-3.0, but art is CC BY-NC-SA 3.0, which is non-commercial, so don't borrow it [[18]][r18] | Reviewer: the game layer is a "double-edged sword", many users won't enjoy it, and it has a steep learning curve [[16]][r16] |
| **Fitocracy** | Social lifting log with points | Points per activity based on its estimated fitness benefit, levels, quests, and badges [[1]][r1] | Reached 1M users in 2013, was sold in 2016, and its site was offline by Aug 2024 [[1]][r1]. **I found no public reason for the shutdown (unverified).** In 2018 a few people still used it to log lifts, and many accounts were dormant [[2]][r2] |
| **Zombies, Run!** | Story-driven running | Audio missions and zombie chases | In interviews with 30 long-term users, the story kept them coming back for years. Interest dropped when the story felt stale. The chases didn't work on treadmills [[8]][r8] |
| **Pokémon GO** | Walking game (AR) | Explore to catch creatures | 1,182 US young adults walked 955 extra steps/day in week 1, back to baseline by week 6 [[6]][r6] [[7]][r7] |
| **Apple Activity rings** | Daily rings + streak | watchOS 11 added a pause of up to 90 days that keeps the streak [[11]][r11] | Before that, one missed day wiped the streak, which could push people to quit [[11]][r11] |
| **Duolingo** *(vendor)* | Daily streak + freezes | Users can hold up to 2 streak freezes at once | +0.38% daily users and no extra days off. Users who reach a 7-day streak are 3.6× more likely to finish a course [[9]][r9] |
| **Strava** | Weekly streak; Local Legends | A streak needs ≥ 1 activity of ≥ 60 s each Mon–Sun, and late uploads restore it [[12]][r12]. Local Legend goes to whoever has the most efforts on a segment in 90 days, not the fastest [[13]][r13] | Rewards showing up, not speed [[13]][r13]. Streak banners can't be turned off [[12]][r12] |
| **Hevy** *(vendor)* | Weekly streak | Needs ≥ 1 workout per week. Back-dating a workout restores a lost week [[14]][r14] | No grace weeks mentioned [[14]][r14] |
| **Gentler Streak** *(vendor)* | "Consistently, not constantly" | Rest days keep the streak alive. Choosing sick, injured, or on a break switches to a Wellbeing Path. Data stays on the device [[15]][r15] | Rated 4.7 from ~8.9K ratings, won a 2024 Apple Design Award, and is praised as sustainable [[15]][r15] |
| **Ascend: Fitness RPG Gym Log** *(vendor)* | Lifting RPG | 5 stats: Strength, Conditioning, "Intelligence" (PRs), Endurance (weekly consistency), Stamina (long-term). Ranks go E → S. Weekly quests, plus **return missions** after a break. No decay mentioned [[19]][r19] | Rated 4.5 from 32 ratings. Motivating, but early versions had too few exercises and no hold timers, and workouts repeated [[19]][r19] |
| **Level-UP: Fitness** *(vendor)* | Anime-style RPG | Workouts raise stats. Dungeons, gear, and "reincarnation" [[20]][r20] | Rated 4.0 from 177 ratings. Users who forgot to log lost progress or had to watch ads, and they asked for a reminder before the day ends [[20]][r20] |
| **Fitness RPG** *(vendor)* | Steps → hero game | Steps become energy for heroes. Loot boxes and passes [[21]][r21] | Rated 4.7 from ~2.1K ratings. Complaints: too many currencies, frequent notifications, and steps above 10K earn nothing [[21]][r21] |

### A2. Evidence

| Study | What | Finding |
|---|---|---|
| Mazeas et al. 2022, meta-analysis [[3]][r3] | 16 RCTs, 2,407 people | Effect on physical activity g = 0.42 during, g = 0.15 at ~14-week follow-up (only 5–6 trials). Overall evidence rated low. Social features were in 15 of 18 studies. The authors say a one-shot intervention isn't enough |
| STEP UP RCT 2019 [[4]][r4] [[5]][r5] | 602 adults: a 24-week game, then 12 weeks without it | Each week players got 70 points up front and lost 10 for each missed day (loss aversion), across 5 levels. All three social game arms beat control during the game. **Only the competition arm stayed above control afterwards** [[5]][r5] |
| Pokémon GO cohort [[6]][r6] | 1,182 young adults | The novelty effect was gone in ~6 weeks |
| Overjustification effect [[22]][r22] | Summary of Deci, Koestner & Ryan 1999 | **Expected, tangible, task-contingent** rewards are most likely to reduce intrinsic motivation. Feedback that tells you how competent you are is fine. Cameron & Pierce argue the effect is small |
| Huang, Chen & Zhou 2024 [[23]][r23] | Survey of 514 users of Chinese fitness apps | Rewards and social recognition *raised* intrinsic motivation ("crowding in"). One-time survey only, and the authors note rewards can backfire when they feel controlling |

### A3. What backfires

- **All-or-nothing streaks.** Apple's old rule wiped a streak after one missed day [[11]][r11], and Bend users call the same rule discouraging [[67]][r67]. A habit is different from acting out of fear of losing the streak [[10]][r10].
- **Punishment spirals.** In Habitica, dying costs a level, all your gold, and gear [[17]][r17]. In Level-UP, forgetting to log costs progress [[20]][r20].
- **Novelty wears off** within weeks [[6]][r6]. Story and new content are what kept Zombies, Run! users for years [[8]][r8].
- **Reward crowding-out** happens with controlling, expected, tangible rewards [[22]][r22].
- **Complexity and nagging:** too many currencies and too many notifications [[21]][r21], and a steep learning curve [[16]][r16].

### A4. Food as a reward ("cheat-meal tokens")

**Evidence.** I found no study that tests food rewards inside a fitness app. The concerns below come from nearby research.

| Source | Finding |
|---|---|
| Ganson et al. 2022 [[24]][r24] | 2,717 Canadian adolescents and young adults. In the past year, 60.9% of men, 53.7% of women and 52.5% of TGNC participants had cheat meals. Cheat meals were linked to eating-disorder behaviour and psychopathology, including binge eating |
| Murray et al. 2018 [[25]][r25] | 248 young adults, and 89.1% reported cheat meals. In men, more frequent cheat meals went with more eating-disorder symptoms and binge episodes. The authors call this preliminary |
| Exercise-snacking licensing scale, 2018 [[26]][r26] | Believing "exercise earns me a snack" was stronger after tiring exercise. It went with *less* autonomous exercise motivation. Only beliefs were measured, not actual eating |
| Cleveland Clinic RD, 2024 [[27]][r27] | "Cheat" language breeds guilt and all-or-nothing thinking. The advice is to stop labelling foods good or bad |
| MyFitnessPal blog *(vendor)* [[28]][r28] | Using exercise to earn food can turn exercise into punishment, and calorie estimates are imprecise anyway |
| Flinders review, 2025 [[29]][r29] | 38 studies. Users of diet and fitness apps showed more disordered-eating symptoms. Risky features included pressure and guilt when goals are missed |

**How a careful designer would frame it** *(analysis)*:
- Use **no food by default**. A token earned for the weekly quest is expected, tangible and earned for a task, which is the riskiest reward type [[22]][r22].
- **Never use the word "cheat"** [[27]][r27], and never present a reward as making up for calories [[28]][r28].
- Make the default reward about **flexibility**: a rest token that protects the streak, as Duolingo's freezes do [[9]][r9].
- Let the user write their own optional real-world reward ("new grips", "movie night"). The app never suggests food.

### Lessons for IronLog

- Keep the game **opt-in**, as planned. Many users don't want it [[16]][r16].
- Use **weekly streaks** that count rest days, and let back-dated logs fix a missed week [[12]][r12] [[14]][r14] [[15]][r15].
- Include **pause** modes: exams, sick, injured, travel [[11]][r11] [[15]][r15].
- **Refresh content** on a rhythm: monthly boss fights, seasonal quests [[6]][r6] [[8]][r8].
- **Social features are optional but backed by evidence.** One duo quest for the two users fits [[3]][r3] [[5]][r5].
- Give **informational feedback** ("3 sets to Back L5") rather than controlling messages [[22]][r22].
- Use **one currency** and no ads [[21]][r21].

---

## B. Plan-generating coach apps

| App | Feature | How it works | What users say |
|---|---|---|---|
| **Fitbod** *(vendor)* | Generated workouts | Inputs are goal, experience, equipment, split and duration, and duration sets the number of exercises. Each muscle has a recovery %, fresher muscles are preferred, and full recovery takes up to 7 days. Weights drop after a training break [[32]][r32] | Some users say they "never" fully trusted the weights. Others like the adaptation and logging. Price complaints [[35]][r35] |
| Fitbod *(vendor)* | Injuries/Limitations | Pick an area (back, shoulder, knee, wrist…). The app asks questions, then excludes risky exercises and recommends alternatives until you tap "Mark as not injured". It says it isn't medical advice [[33]][r33] | Focus exercises still override the exclusions [[33]][r33] |
| Fitbod (CEO interview) | Explanations | Users "expect understanding", and trust is "a huge issue". "Workout Insights" explaining picks was still upcoming [[34]][r34] | — |
| **JuggernautAI** *(vendor)* | Readiness | Before each session you rate sleep, mood, energy and soreness, and that day's volume or weight changes. Top-set RPE adjusts the back-off sets live. Weekly check-ins tune the next week [[36]][r36] | Not found |
| **RP Hypertrophy** *(vendor)* | After-session feedback | Answers about pump, soreness ("healed on time?") and workload add or remove sets. Weight rises a few % a week, or by +1 rep if the next weight jump is too big. Manual override is allowed [[37]][r37] [[38]][r38] | (See round 1: price) |
| **Freeletics** *(vendor)* | "Adapt session" | For one day: no equipment, ~2×2 m space, shorter, no running, **no jumps (quiet)**, exclude sore areas, or change difficulty [[39]][r39] | Forum: the coach keeps assigning sessions that are too hard, despite feedback [[40]][r40] |
| **Future** | Human coach in an app | A weekly plan arrives by Sunday. **Vacation dates** go in your profile. Coaches adapt to injuries, you rate each workout, and you can send form videos [[41]][r41] | $199/month [[41]][r41] |
| **Caliber** | Coach plans | Questionnaire covers equipment and injuries. **A few exercises repeat for weeks**, aiming for +reps or +weight weekly, and your last numbers show during the workout [[42]][r42] | Free tier with 500+ exercise demos. Coached plans are $19/month and up, with pricing hidden until a call [[42]][r42] |
| **Peloton Strength+** *(vendor)* | Generator | Builds from goal, muscle focus, time, experience and equipment. You can swap or reorder anything [[43]][r43] | No mention of automatic weight adjustment [[43]][r43] |
| **Dr. Muscle** *(vendor)* | Auto-progression | Updates after every set or workout using RIR, with **automatic deloads** [[44]][r44] | Rated 4.4 from 366 ratings. The page shows only testimonials [[44]][r44] |

### Lessons for IronLog

- IronLog's inputs (time, equipment, soreness) **match the market** [[32]][r32] [[39]][r39]. Add Freeletics-style toggles: **quiet (no jumps)**, **small space**, **no running** [[39]][r39].
- **Explain picks and changes** ("You said your joints ached → swapped jumps for step-ups"). This targets the trust gap [[34]][r34] [[35]][r35] and the "ignored my feedback" complaint [[40]][r40].
- Add an **RP-style soreness check at the next session** for muscles trained last time [[38]][r38].
- **Plan for breaks:** cut loads after time off [[32]][r32], and let users enter away dates [[41]][r41].
- An **injury profile** should persist until the user clears it, and should carry a clear "not medical advice" note [[33]][r33].
- **Keep exercise picks stable** so double progression can work [[42]][r42]. Round 1 found "exercises change often" was a Fitbod complaint.
- Always allow a **manual override** [[38]][r38] [[43]][r43].

---

## C. Progress photos

| App | Feature | How it works | What users say |
|---|---|---|---|
| **Progress** (iOS) *(vendor)* | Daily selfie | Ghost of yesterday's photo for alignment, time-lapse export. No camera-roll access, and photos stay on the phone [[45]][r45] | Rated 3.0 from 25 ratings, with few written reviews [[45]][r45] |
| **My Body Tracker: PhotoJourney** *(vendor)* | Full photo journal | Ghost mode, grid and body templates, countdown. Side-by-side, swipe and zoom compare. Time-lapse/GIF export. Reminders hourly, daily or weekly. Separate "journeys". Stored on the device plus your own iCloud, with camera roll optional [[46]][r46] | Rated 4.4 from 117 ratings. Clean and simple. **One user lost every photo.** Another complained about the price ($8/week or $35/year) [[46]][r46] |
| **Progress Pics Photo Tracker** *(vendor)* | Private series | Before/after views. Series for body, hair or skin. Measurements logged with each check-in. **Optional Face ID lock**. Kept on the device, separate from the camera roll, no account [[47]][r47] | No ratings yet [[47]][r47] |
| **MacroFactor** *(vendor)* | Photos + metrics | Front, side and back photo each day. Before/after side by side. The gallery shows the body metrics from the same day [[48]][r48] | Tips: monthly; mornings before eating; same place, light and flash use; same distance and crop; same clothing [[49]][r49] |

### Privacy patterns

| Pattern | How | Source |
|---|---|---|
| Keep photos out of the gallery | Android internal app storage: other apps can't read it, it's encrypted on Android 10+, and it's deleted on uninstall | [[50]][r50] |
| Hide from screenshots and the app switcher | `FLAG_SECURE` blocks screenshots, screen recording and the recents preview. Android 13's `setRecentsScreenshotEnabled(false)` hides only the preview | [[51]][r51] |
| App lock | Optional Face ID lock inside the app [[47]][r47]. iOS 18 can require Face ID for apps and hide them [[54]][r54]. Fitbit users have asked for a lock to keep weight data private [[53]][r53] | — |
| Local or own-cloud only | No developer servers [[45]][r45] [[46]][r46] | — |
| **Web app on iPhone** | Browser storage is "best effort" and can be evicted (least recently used first). `navigator.storage.persist()` is granted by heuristics, such as being opened as a Home Screen web app | [[52]][r52] |
| Encryption | **None of the 3 photo apps I read mentions it** | [[45]][r45] [[46]][r46] [[47]][r47] |

**What users fear:** losing their photos [[46]][r46], the price [[46]][r46], and body-image harm. Body-image concerns are linked to diet and fitness apps in general [[29]][r29].

### Lessons for IronLog

- **Capture screen:** ghost overlay at ~30–40% opacity, front/side/back slots, and a countdown [[45]][r45] [[46]][r46] [[48]][r48].
- **Compare screen:** side-by-side plus a swipe slider, with that week's weight and waist shown underneath [[46]][r46] [[48]][r48]. A time-lapse can play **inside the app only**, since photos are never exported.
- Remind **monthly, not daily** [[49]][r49], and ask gently because of body-image risk [[29]][r29].
- **Storage:** Android internal storage [[50]][r50] plus `FLAG_SECURE` on the photo screens [[51]][r51].
- **Encrypt before sync.** IronLog would be different from the apps I read here [[45]][r45] [[46]][r46] [[47]][r47]. Plan for **key backup**, because losing the key means losing the photos, the exact fear users report [[46]][r46].
- **iPhone web build:** ask for persistent storage and push users to install to the Home Screen [[52]][r52]. Treat the server copy as the real backup.

---

## D. Voice logging of workouts

| App / study | Feature | How it works | What users say |
|---|---|---|---|
| **Repvoice** (Android) *(vendor)* | Voice sets | Hold to talk or tap once. Example phrases: "three by eight at eighty", "bench 100 for 5", "same again", "pull ups 8 7 6", holds in seconds. If no exercise is named, it **asks which one**. If unsure between **18 and 80, it asks** instead of guessing. Speech is recognised **on the phone only**, and if that isn't available you log by tap or type [[55]][r55] | No ratings shown. Pro costs $23.99/year, and the free tier includes 10 voice logs [[55]][r55] |
| **VoiceFitLog** *(vendor)* | "AI voice" | Doesn't say where audio is processed [[56]][r56] | No ratings [[56]][r56] |
| **Liftly** *(vendor)* | Voice + suggestions | "3 sets of 10 bench press at 185" [[57]][r57] | Launched 2025, no reviews visible [[57]][r57] |
| **Vora blog** *(vendor)* | Design claims | Small gym vocabulary, context ("add ten" → +10), and reading back what it heard to confirm. Its "95%+ accuracy" claim has no source (**unverified**) [[58]][r58] | It admits gyms are a hard acoustic environment and that 135 vs 185 mix-ups are a real worry [[58]][r58] |
| **WHOOP community** | Feature request | A user wants to speak sets mid-session. The band has no mic, so the phone would be needed [[59]][r59] | No votes visible [[59]][r59] |
| **TandemTrack** (CHI 2020) | Voice + app study | Alexa skill plus app, 22 people, 4 weeks [[60]][r60] | Of the 11 with both, 5 preferred voice, 4 preferred the app, and 2 never used voice. Some found talking awkward or worried about being overheard. **No difference in adherence** [[60]][r60] |

### On-device speech options

| Option | Platform | Offline? | Size | Notes |
|---|---|---|---|---|
| `SpeechRecognizer` (default) | Android | Often not | — | The docs say it "is likely to stream audio to remote servers" and isn't meant for continuous listening. Must run on the main thread, and you must call `destroy()` [[61]][r61] |
| `createOnDeviceSpeechRecognizer` | Android 12+ (API 31) | Yes | System model | Check `isOnDeviceRecognitionAvailable` first. `checkRecognitionSupport`/`triggerModelDownload` need API 33, and the download listener needs API 34 [[61]][r61] |
| Web Speech API | Safari iOS 14.5+ (partial), Chrome (partial), Firefox off by default | Not stated | — | Listed as an unofficial spec [[62]][r62] |
| **whisper.cpp** (MIT) | Any (C/C++) | Yes | tiny 75 MiB (q5_1: 31), base 142 MiB (q5_1: 57), small 466 MiB (q5_1: 181) [[63]][r63] | The WASM build needs SIMD and runs models up to small. tiny/base run about 2–3× faster than real time on a modern CPU. Audio is capped at 120 s [[64]][r64]. Phone speed figures come from a blog only (**unverified**) [[65]][r65] |
| **Vosk** (Apache-2.0) | Android, desktop | Yes | ~50 MB | "Reconfigurable vocabulary", which can be limited to gym words [[66]][r66] |

### Parsing gym phrases *(analysis)*

- Write the parser as a **pure C++ function**: `text + context(last set, current exercise) → SetCommand | Question`. It then works on typed text, keyboard dictation, and any future speech engine.
- Patterns to cover: `W for R` / `R at W` / `S by R at W` / `same again` / `drop to W` / `plus five` / `hold 45`. Number words ("eighty") become ints.
- **Ask when unsure**, as Repvoice does [[55]][r55]:
  - teen/-ty pairs (13/30 … 19/90)
  - a weight far from the last set's weight
  - a missing exercise
- Show what was parsed with an **undo**, rather than a confirm dialog on every set. *(analysis)*

### Lessons for IronLog

- Demand is **unproven**. Many new apps exist, but few have ratings [[55]][r55] [[56]][r56] [[57]][r57], and preference was split in the only study I found [[60]][r60].
- v1: ship a **quick-entry box with the parser**. Keyboard dictation is free, and the parser is testable C++.
- Later (AI track): use the Android on-device recognizer [[61]][r61]. Alternatively, use Vosk with a restricted vocabulary [[66]][r66] or whisper.cpp tiny [[63]][r63], all on-device.
- Qt 6 has text-to-speech but no speech-to-text module *(general knowledge)*. Plan on JNI or a C library.

---

## E. Ideas from the owner's own plan

### E1. Guided stretching and mobility players

| App | Feature | How it works | What users say |
|---|---|---|---|
| **Bend** *(vendor)* | Routine player | A timer for each hold. A beep plus an on-screen prompt to switch sides or move on. Illustrations, a demo, and cautions per stretch. Custom routines, reminders, streaks [[67]][r67] | Rated 4.8 from ~175K ratings. Liked: clear demos, the beeps. Disliked: skipping starts the next stretch **without a countdown**, transitions are too short, no easier versions, and the **all-or-nothing streak** is discouraging [[67]][r67] |
| **GOWOD** *(vendor)* | Test → plan | A free mobility test sets a baseline and personalised daily routines follow. Timers per pose. 50+ sports [[68]][r68] | Rated 4.8 from ~1.3K ratings (UK store). Unclear whether to **hold or repeat**, no scaled versions, **no "can't get into this pose" option**, and one test result was disputed by a physio [[68]][r68] |
| **pliability** *(vendor)* | Assessment + sessions | A 3-minute on-device range-of-motion test. Choose a short, medium or long session (~12–20 min daily). Voice cues. Works offline [[69]][r69] | Rated 4.8 from ~10K ratings. Calm voice praised. Older reviews mention inconsistent countdowns. $19.99/month [[69]][r69] |

### E2. Fitness benchmark tests

| Source | How it works |
|---|---|
| Peloton Strength Benchmarks *(vendor)* | Tests measure hold time, reps in a set time, or the heaviest weight for set reps. **Retest after ~6 weeks** with the same class [[70]][r70] |
| Nike *(vendor)* | Examples: max push-ups, a timed mile. A trainer suggests retesting **every 1–3 months**, and keeping the test identical each time [[71]][r71] |
| GOWOD / pliability | Mobility tests are used to track progress month to month [[68]][r68] [[69]][r69] |

### E3. Exercise how-to media you can legally ship

| Source | Licence (as stated) | Caveat |
|---|---|---|
| **free-exercise-db** (yuhonas) | **Unlicense** (public domain). 800+ exercises with images [[72]][r72], 873 by one count [[81]][r81] | Derived from exercises.json [[72]][r72]. Asked about image rights, the maintainer said licensing info "still needs to be added" to the README and pointed to an earlier issue (#2, **not read**) [[73]][r73]. **Image provenance is unclear.** |
| **exercises.json** (wrkout) | Unlicense [[74]][r74] | No data or image sources listed [[74]][r74] |
| **wger** | Code AGPL-3.0+. Exercise data: Creative Commons, **set per entry** [[75]][r75]. The first data set is also CC-BY-SA 3.0 [[76]][r76] | The maintainer wants "CC-BY-SA or similar" images and says **some community uploads may not be free** [[77]][r77]. Keep each image's author and licence |
| **exercemus/exercises** | Code MIT. **Licence set per exercise**, with wger author and licence fields kept [[78]][r78] | You must show author, licence and link for each exercise [[78]][r78] |
| **Everkinetic** illustrations | CC BY-SA 4.0, **per a third-party credits file** [[79]][r79] | **Unverified at the source.** Check the original repository |
| **hasaneyldrm/exercises-dataset** | Data MIT, but the **GIFs are © Gym visual**, used by permission only [[80]][r80] | **Not free to reuse.** A warning example |

### E4. App lock

- Users ask for a lock to keep weight data private [[53]][r53]. Photo apps offer Face ID locks [[47]][r47].
- iOS 18 can lock and hide apps with Face ID [[54]][r54]. **Whether that works for Home Screen web apps is unverified.**
- *(analysis)* Lock the **Photos section** with an in-app PIN or biometric. For the whole app, point users to the OS lock.

### E5. Reminders that don't nag

| Source | Finding |
|---|---|
| Android 13+ [[82]][r82] | Notifications are **off by default** on new installs. Don't ask at startup; ask in context after a user action. Check `areNotificationsEnabled()` before posting |
| JOOL micro-randomised trial, 2018 [[83]][r83] | 1,255 users. A tailored nudge raised the chance of logging within 24 h by only **~3.9%**, and it worked best around midday. Inactive users were nudged less often (every 2–3 days, down to every 15 days) |
| iOS web push [[84]][r84] | Needs iOS 16.4+, a **Home Screen install**, and a permission prompt triggered by a user tap. Messages go through Apple's push service |
| Users | Too many notifications annoy users [[21]][r21]. Yet users also *missed* a reminder before the day ends [[20]][r20]. Strava's streak banners can't be turned off [[12]][r12] |

### Lessons for IronLog

- The **mobility player** needs: a countdown after skip, adjustable transitions, a side-switch beep, a hold-vs-reps label, easier versions, and a "can't do this yet" button [[67]][r67] [[68]][r68].
- **Boss fights** every 4–6 weeks [[70]][r70] [[71]][r71]: use reps-in-time, holds and timed cardio. **Never test a 1RM.** Give injury-aware variants *(analysis)*.
- **Media:** start with free-exercise-db *text*. Bundle only images whose licence is confirmed. Store `author/licence/url` for every asset and add a credits screen [[73]][r73] [[77]][r77] [[78]][r78].
- **Reminders:** ask for permission in context [[82]][r82]. Send at most one a day, only when a planned session hasn't been logged, with quiet hours and backing off for inactive users [[83]][r83]. The iPhone web build needs a server for push [[84]][r84].

---

## Critique of IronLog's current game rules

| Rule | Verdict | Change / add | Why | Source |
|---|---|---|---|---|
| 8 skills | **Keep** | — | Clear, maps to the body map. Ascend uses 5 broader stats [[19]][r19] | [[19]][r19] |
| XP per set (+10 primary, +5 secondary) | **Keep + cap** | Count working sets only. Cap XP per skill per week at a high-volume ceiling. Cap cardio minutes per day | Without a cap the game rewards junk volume *(analysis)*. Half credit for secondary muscles matches round 1's half-set counting (see `feature-research.md` [s42]) | [[22]][r22] |
| +25 per PR | **Keep, tune** | Count rep PRs and e1RM PRs, not just heavier singles | Avoids rewarding max-out attempts while working around an injury *(analysis)*. Benchmarks use reps and time [[70]][r70] | [[70]][r70] |
| Level cost 50×L | **Keep** | Show "sets to next level", not raw XP | Total XP to reach level L is 25·L·(L−1). L10 = 2,250 XP ≈ 225 primary sets *(analysis)*. Feedback about competence motivates; controlling messages don't | [[22]][r22] |
| Decay −1 %/day after 10 days | **Change** | Start after **14 days**, at **−0.5 %/day**. Warn first ("Back starts fading in 3 days") and grey out the body map before XP drops | Strength barely moves in 3 weeks off [[30]][r30]. Gradual decay is better than a hard reset [[10]][r10], and punishment spirals drive people away [[17]][r17] [[20]][r20] | [[30]][r30] |
| Floor 50 % of peak | **Keep** | — | Stops a death spiral, unlike Habitica's death penalty [[17]][r17] | [[17]][r17] |
| Muscle memory ×2 XP below peak | **Keep** | Label it "Comeback bonus" | After a 3-week break, gains came back as fast as the first time, and the groups ended level [[31]][r31]. Matches Ascend's return missions [[19]][r19] | [[31]][r31] |
| Exam mode (≤ 21 days) pauses decay | **Keep + generalise** | **Pause** with a reason: exams / sick / injured / travel, each with a cap. Injury pause asks weekly to confirm | Apple allows a 90-day pause and keeps the streak [[11]][r11]. Gentler Streak has sick/injured modes [[15]][r15] | [[11]][r11] |
| Weekly quest = planned sessions | **Keep + tweak** | The target follows the current mode (the exam-week minimum becomes the quest). Extra sessions earn a bonus | Weekly streaks are standard and forgiving [[12]][r12] [[14]][r14] | [[12]][r12] |
| Cheat-meal token (bank max 2) | **Change** | Make it a **Rest token** that protects the streak, bank max 2 (same as Duolingo). Add an optional reward the user writes. No food suggestions, no "cheat" wording | See A4 [[24]][r24] [[25]][r25] [[27]][r27]. Freezes raised daily users with no extra days off [[9]][r9] | [[9]][r9] |
| Streak + heat map | **Change streak** | Count streaks **weekly**, and count rest days. Back-dated logs restore a week. Celebrate milestones instead of shaming a break | [[12]][r12] [[14]][r14] [[15]][r15] [[10]][r10] | [[10]][r10] |
| Body map by level | **Keep + add** | Highlight the **most-neglected skill** (the nudge decay is meant to give) | *(analysis)* | — |
| Unlocks through safety gates | **Keep + add** | Add a "can't do this yet" option, an easier version for each gate, and a note that it isn't medical advice | GOWOD complaint [[68]][r68], Fitbod injury flow [[33]][r33] | [[33]][r33] |
| Opt-in checkbox | **Keep** | Game-off still shows plain progress. Toggling never deletes anything, because XP is derived | Many users don't want a game layer [[16]][r16] | [[16]][r16] |
| XP derived, never stored | **Keep + add** | Version the rules and show "rules v2" when numbers shift | Re-deriving XP rewrites history *(analysis)*. It also fixes forgot-to-log losses for free [[20]][r20] | [[20]][r20] |
| **Add:** boss fights | **Add** | A benchmark every 4–6 weeks becomes a "boss" | Fights novelty fade [[6]][r6], and the retest timing has backing [[70]][r70] [[71]][r71] | [[70]][r70] |
| **Add:** duo quest | **Add (optional)** | A shared weekly co-op quest for the two users, with private stats | Social features drove most of the effects [[3]][r3]. Teaming up may suit family members [[5]][r5] | [[3]][r3] |
| **Add:** one currency, no ads | **Add (constraint)** | Only XP and tokens | Too many currencies is a complaint [[21]][r21] | [[21]][r21] |

---

## Ranked recommendations

Layer: **core** = pure C++ rules · **services** = sync/persistence/platform · **ui** = Qt Quick screens.

| # | Feature | Area | Demand (evidence) | Fit for IronLog | Effort | Layer | What it would teach |
|---|---|---|---|---|---|---|---|
| 1 | **Rest token replaces cheat-meal token** (+ optional reward the user writes) | A | High: health concern [[24]][r24] [[25]][r25]; freezes work [[9]][r9] | High: same "bank max 2" mechanic | S | core + ui | Turning ethics into rules; keeping tunable values in config |
| 2 | **Gentler decay + Pause modes** (exam/sick/injured/travel) | A | High [[11]][r11] [[15]][r15] [[30]][r30] | High: game rules are already derived | S | core | Pure functions over a time series; date maths; table-driven tests |
| 3 | **Weekly streak, rest days count, back-dating, kind copy** | A | High [[12]][r12] [[14]][r14] [[10]][r10] | High | S | core + ui | ISO weeks and time zones with `std::chrono` / `QDate` |
| 4 | **Explain every pick and every change** ("because you said…") | B | High [[34]][r34] [[35]][r35] [[40]][r40] | Very high: the rule engine already writes reasons | S | core + ui | Rules that return reason objects; passing structs to QML |
| 5 | **Gym-phrase parser for quick entry** (typed or dictated) | D | Med: many new apps [[55]][r55], split preference [[60]][r60] | High: no speech engine needed yet | S–M | core | Tokenising and hand-written parsers; ambiguity handling; unit tests |
| 6 | **Photo capture with ghost overlay, 3 poses, compare slider** | C | Med–High [[45]][r45] [[46]][r46] [[48]][r48] | High (already a Must) | M | ui + services | Qt Multimedia camera; QML layering and opacity; image scaling |
| 7 | **Photo privacy stack**: internal storage, `FLAG_SECURE`, section lock, encrypt-before-sync, key backup, `persist()` on web | C/E | Med–High: data-loss fear [[46]][r46], lock requests [[53]][r53] | High: a differentiator | M–L | services | Authenticated encryption and key handling; JNI; storage on each platform |
| 8 | **Mobility player** (holds, side-switch beep, countdown after skip, easier versions) | E | High: Bend's 175K ratings [[67]][r67] | High: the Mobility skill and timed holds already exist | M | ui + core | QML state machines; `Timer`; sound effects; accessibility |
| 9 | **Licensed exercise media pack** with an author/licence record per asset and a credits screen | E | High: demos are expected; unclear instructions are a top complaint [[68]][r68] | High | M | services + ui | Licence compliance; import scripts; asset pipelines |
| 10 | **Boss fights**: benchmark every 4–6 weeks | A/E | Med [[70]][r70] [[71]][r71] | High: fits both game and plan | M | core + ui | Scheduling; modelling test results; charts |
| 11 | "Train now" toggles: **quiet / small space / no running** | B | Med [[39]][r39] | High: home training | S | core | Constraint filtering over an exercise catalogue |
| 12 | **Next-session soreness check** (RP-style) feeding volume | B | Med [[38]][r38] [[36]][r36] | High | S | core | Feedback loops; rule tables |
| 13 | **Return-from-break rule + comeback quest** | A/B | Med [[32]][r32] [[19]][r19] | High | S | core | Rules that query history |
| 14 | **XP anti-farming caps** | A | Design need *(analysis)* | High | S | core | Game balance; simulation tests |
| 15 | **Smart reminders** (≤ 1/day, only if a planned session is unlogged, quiet hours) | E | Med [[83]][r83] [[20]][r20] [[21]][r21] | Med: each platform differs | M | services | Android notification permission; Web Push; Windows toasts |
| 16 | **Optional duo co-op quest** | A | Med [[3]][r3] [[5]][r5] | Med: needs multi-user sync | M | services + core | Multi-user data; merging sync conflicts |
| 17 | **On-device voice set logging** (Android recognizer / Vosk / whisper.cpp) | D | Low–Med [[59]][r59] [[60]][r60] | Med: AI track | L | services | Integrating a C/C++ library; audio capture; packaging models |

---

## Sources

**A. Gamification**

1. [Fitocracy (Wikipedia)](https://en.wikipedia.org/wiki/Fitocracy)
2. [Fitocracy? (MetaTalk, 2018)](https://metatalk.metafilter.com/24789/Fitocracy)
3. [Mazeas et al. 2022, Evaluating the Effectiveness of Gamification on Physical Activity: Systematic Review and Meta-analysis of RCTs (JMIR)](https://www.jmir.org/2022/1/e26779/)
4. [Patel et al. 2019, STEP UP trial abstract (PubMed)](https://pubmed.ncbi.nlm.nih.gov/31498375/)
5. [Key Takeaways Regarding Gamification to Increase Physical Activity: STEP UP (ACC)](https://www.acc.org/latest-in-cardiology/articles/2019/11/26/08/23/key-takeaways-regarding-gamification-to-increase-physical-activity)
6. [Pokémon GO only slightly improves physical activity among adults (Karolinska Institutet news, on Howe et al., BMJ 2016)](https://news.ki.se/pokemon-go-only-slightly-improves-physical-activity-among-adults)
7. [Pokémon GO research news (The BMJ)](https://www.bmj.com/content/355/bmj.i6684)
8. [Zombies, Run! users' engagement with physical activity: qualitative study (UCL author manuscript)](https://discovery-pp.ucl.ac.uk/10139156/3/Potts_ZR_users_interviews_paper_Games%20for%20Health_Last%20version_NF.pdf) *(author manuscript)*
9. [How the Duolingo streak builds habit (Duolingo blog)](https://blog.duolingo.com/how-duolingo-streak-builds-habit) *(vendor)*
10. [Designing a Streak System: The UX and Psychology of Streaks (Smashing Magazine, 2026)](https://smashingmagazine.com/2026/02/designing-streak-system-ux-psychology/) *(blog)*
11. [Close your rings… but in watchOS 11 it's okay if you don't (9to5Mac)](https://9to5mac.com/2024/07/16/close-your-ringsbut-in-watchos-11-its-okay-if-you-dont/)
12. [Streaks on Strava (Strava support)](https://support.strava.com/hc/en-us/articles/36553427481997-Streaks-on-Strava) *(vendor)*
13. [Strava Local Legends feature (DC Rainmaker)](https://dcrainmaker.com/2020/06/strava-legends-feature.html)
14. [What is a weekly streak? (Hevy help)](https://help.hevyapp.com/hc/en-us/articles/34467766576279) *(vendor)*
15. [Gentler Streak Workout Tracker (App Store)](https://apps.apple.com/us/app/gentler-streak-workout-tracker/id1576857102) *(vendor)*
16. [Habitica review (TechRadar)](https://www.techradar.com/reviews/habitica)
17. [Obstacles (Habitica Wiki)](https://habitica.fandom.com/wiki/Obstacles)
18. [Habitica (Wikipedia)](https://en.wikipedia.org/wiki/Habitica)
19. [Ascend: Fitness RPG Gym Log (App Store)](https://apps.apple.com/app/id6755652663) *(vendor)*
20. [Level-UP: Fitness (App Store)](https://apps.apple.com/us/app/-/id6499099763) *(vendor)*
21. [Fitness RPG: Hero health game (App Store)](https://apps.apple.com/us/app/fitness-rpg-hero-health-game/id1252580641) *(vendor)*
22. [Overjustification effect (Wikipedia)](https://en.wikipedia.org/wiki/Overjustification_effect)
23. [Huang, Chen & Zhou 2024, Motivation crowding effects on continued use of gamified fitness apps (Frontiers in Psychology)](https://www.frontiersin.org/journals/psychology/articles/10.3389/fpsyg.2023.1286463/pdf)
24. [Ganson et al. 2022, Characterizing cheat meals among Canadian adolescents and young adults (J Eating Disorders, via DOAJ)](https://doaj.org/article/e31353039b2b48dcb95d89561afc68c4)
25. [Murray et al. 2018, Cheat meals: a benign or ominous variant of binge eating behavior? (Appetite, via Western Sydney Univ.)](https://researchers.westernsydney.edu.au/en/publications/cheat-meals-a-benign-or-ominous-variant-of-binge-eating-behavior/)
26. [Preliminary Validation of the Exercise-Snacking Licensing Scale (Nutrients, 2018)](https://mdpi.com/2072-6643/10/12/1866)
27. [Why you should ditch the term "cheat day" (Cleveland Clinic Newsroom, 2024)](https://newsroom.clevelandclinic.org/2024/01/05/why-you-should-ditch-the-term-cheat-day)
28. [Why you shouldn't use exercise to earn your food (MyFitnessPal blog)](https://blog.myfitnesspal.com/why-you-shouldnt-use-exercise-to-earn-your-food/) *(vendor)*
29. [Fitness apps fuelling disordered eating (Flinders University news, on Anderberg et al., Body Image 2025)](https://news.flinders.edu.au/blog/2025/02/22/fitness-apps-fuelling-disordered-eating/)
30. [Räntilä et al. 2021, High responders to hypertrophic strength training also tend to lose more during detraining (JSCR, Univ. of Jyväskylä copy)](https://jyx.jyu.fi/bitstreams/c643820b-d619-4d7b-a678-92a1baf8a6ee/download)
31. [Ogasawara et al. 2010, Interruptions to resistance training do not produce strength losses (ACSM abstract, via Coaching Science Abstracts)](https://coachsci.sdsu.edu/csa/vol171/ogasawar.htm) *(conference abstract)*

**B. Coach apps**

32. [How Fitbod creates your workout (Fitbod help)](https://help.fitbod.me/hc/en-us/articles/360004429814-How-Fitbod-Creates-Your-Workout) *(vendor)*
33. [Injuries / Limitations (Fitbod help)](https://help.fitbod.me/hc/en-us/articles/37629269518103) *(vendor)*
34. [People want to know why: Fitbod's founder on the AI fitness boom (TechRadar)](https://techradar.com/health-fitness/fitness-apps/its-no-longer-enough-for-an-app-to-tell-you-what-to-do-people-want-to-know-why-fitness-app-fitbods-founder-on-the-reason-behind-the-ai-fitness-boom)
35. [Fitbod reviews (Trustpilot, page 2)](https://www.trustpilot.com/review/fitbod.me?page=2)
36. [How JuggernautAI is individualized to you (JTS help)](https://help.jtsstrength.com/en/articles/3-how-juggernautai-is-individualized-to-you) *(vendor)*
37. [How does the app determine when to add weight, reps and sets? (RP help)](https://help.rpstrength.com/hc/en-us/articles/32600173777815-How-does-the-app-determine-when-to-add-weight-reps-and-sets) *(vendor)*
38. [RP Hypertrophy feedback article (RP help, Zendesk)](https://hypertrophy.zendesk.com/hc/en-us/articles/14605661323671) *(vendor)*
39. [Adapt Today / Quick Adapt (Freeletics blog)](https://freeletics.com/en/blog/posts/quick-adapt) *(vendor)*
40. [Coach keeps suggesting too much, too hard (Freeletics forum)](https://forum.freeletics.com/t/coach-keeps-suggesting-too-much-too-hard-beat-your-pbs/10635)
41. [Future app review (Garage Gym Reviews)](https://www.garagegymreviews.com/future-app-review)
42. [Caliber app review (Garage Gym Reviews)](https://www.garagegymreviews.com/caliber-app-review)
43. [Peloton Strength+ app (Peloton blog)](https://www.onepeloton.com/en-CA/blog/peloton-strength-plus-app) *(vendor)*
44. [Dr. Muscle AI Personal Trainer (App Store)](https://apps.apple.com/us/app/id1073943857) *(vendor)*

**C. Progress photos and privacy**

45. [Progress: See your progress (App Store)](https://apps.apple.com/us/app/-/id1046634084) *(vendor)*
46. [My Body Tracker: PhotoJourney (App Store)](https://apps.apple.com/us/app/-/id6499454966) *(vendor)*
47. [Progress Pics Photo Tracker (App Store)](https://apps.apple.com/app/id6776659549) *(vendor)*
48. [MacroFactor expands progress tracking with photos and body metrics](https://macrofactor.com/body-metrics/) *(vendor)*
49. [Tips for taking good progress photos (MacroFactor help)](https://help.macrofactorapp.com/en/articles/117-tips-for-taking-good-progress-photos) *(vendor)*
50. [Storage use cases and best practices (Android Developers)](https://developer.android.com/training/data-storage/use-cases)
51. [Multitasking intrusion and preventing screenshots in Android apps (ProAndroidDev)](https://proandroiddev.com/multitasking-intrusion-and-preventing-screenshots-in-android-app-15bd8757c24d) *(blog)*
52. [Updates to Storage Policy (WebKit blog)](https://webkit.org/?p=14403)
53. [Lock for app (Fitbit Community feedback)](https://community.fitbit.com/t5/Product-Feedback/Lock-for-app/m-p/2316813)
54. [iOS 18 lets you lock and hide your apps (MacRumors)](https://macrumors.com/2024/06/10/ios-18-hide-lock-apps)

**D. Voice**

55. [Repvoice: Voice Workout Log (Google Play)](https://play.google.com/store/apps/details?id=app.repvoice) *(vendor)*
56. [VoiceFitLog (App Store)](https://apps.apple.com/us/app/id6454792467) *(vendor)*
57. [Liftly: voice-powered workout logging (Product Hunt)](https://www.producthunt.com/products/liftly-voice-powered-workout-logging/reviews) *(vendor)*
58. [Voice workout logging, hands-free (Vora blog)](https://askvora.com/blog/voice-workout-logging-hands-free) *(vendor)*
59. [Feature request: capture the workout by voice (WHOOP Community)](https://www.community.whoop.com/t/feature-request-capture-the-workout-by-voice-during-the-session/15590)
60. [TandemTrack: complementing a mobile app with a smart speaker (HCIL blog on CHI 2020 paper)](https://medium.com/hcil-at-umd/tandemtrack-shaping-consistent-exercise-experience-by-complementing-a-mobile-app-with-a-smart-1bb0de5054cd)
61. [SpeechRecognizer reference (Android Developers)](https://developer.android.com/reference/android/speech/SpeechRecognizer)
62. [Speech Recognition API (Can I use)](https://caniuse.com/speech-recognition)
63. [OpenAI's Whisper models in ggml format for whisper.cpp (Hugging Face)](https://huggingface.co/ggerganov/whisper.cpp)
64. [whisper.wasm README (jsDelivr mirror of whisper.cpp example)](https://cdn.jsdelivr.net/npm/nodejs-whisper@0.3.0/cpp/whisper.cpp/examples/whisper.wasm/README.md)
65. [Whisper.cpp and on-device speech (personal blog)](https://jvoltci.github.io/mosaic/edge-ai/multimodal/whisper-cpp) *(blog; figures unverified)*
66. [vosk 0.3.21 (PyPI)](https://pypi.org/project/vosk/0.3.21)

**E. Owner's-plan ideas**

67. [Bend: Stretching & Flexibility (App Store)](https://apps.apple.com/us/app/-/id1513988468) *(vendor)*
68. [GOWOD (App Store UK)](https://apps.apple.com/gb/app/gowod/id1227834875) *(vendor)*
69. [pliability: Stretch + Mobility (App Store)](https://apps.apple.com/app/id1175346453) *(vendor)*
70. [Strength Benchmark fitness test (Peloton blog)](https://www.onepeloton.com/blog/strength-benchmark-fitness-test) *(vendor)*
71. [What are benchmark workouts? (Nike)](https://www.nike.com/a/what-are-benchmark-workouts) *(vendor)*
72. [free-exercise-db README](https://raw.githubusercontent.com/yuhonas/free-exercise-db/main/README.md)
73. [free-exercise-db issue #12: image licensing question](https://github.com/yuhonas/free-exercise-db/issues/12)
74. [wrkout/exercises.json (GitHub)](https://github.com/wrkout/exercises.json)
75. [wger README (jsDelivr mirror)](https://cdn.jsdelivr.net/gh/wger-project/wger@master/README.md)
76. [wger documentation](https://wger.readthedocs.io/)
77. [wger issue #2196: more exercise images](https://github.com/wger-project/wger/issues/2196)
78. [exercemus/exercises (GitHub)](https://github.com/exercemus/exercises)
79. [GymMane CREDITS.md (lists Everkinetic as CC BY-SA 4.0)](https://raw.githubusercontent.com/InlitX/GymMane/master/CREDITS.md)
80. [exercises-dataset NOTICE.md (Gym visual media terms)](https://raw.githubusercontent.com/hasaneyldrm/exercises-dataset/main/NOTICE.md)
81. [Free Exercise DB (API Evangelist)](https://providers.apievangelist.com/providers/free-exercise-db/)
82. [Notification runtime permission (Android Developers)](https://developer.android.com/develop/ui/views/notifications/notification-permission)
83. [Micro-randomized trial of push notifications in a workplace well-being app (JMIR mHealth and uHealth, 2018)](https://mhealth.jmir.org/2018/11/e10123/)
84. [Web Push for Web Apps on iOS and iPadOS (WebKit blog)](https://webkit.org/?p=13878)

[r1]: https://en.wikipedia.org/wiki/Fitocracy
[r2]: https://metatalk.metafilter.com/24789/Fitocracy
[r3]: https://www.jmir.org/2022/1/e26779/
[r4]: https://pubmed.ncbi.nlm.nih.gov/31498375/
[r5]: https://www.acc.org/latest-in-cardiology/articles/2019/11/26/08/23/key-takeaways-regarding-gamification-to-increase-physical-activity
[r6]: https://news.ki.se/pokemon-go-only-slightly-improves-physical-activity-among-adults
[r7]: https://www.bmj.com/content/355/bmj.i6684
[r8]: https://discovery-pp.ucl.ac.uk/10139156/3/Potts_ZR_users_interviews_paper_Games%20for%20Health_Last%20version_NF.pdf
[r9]: https://blog.duolingo.com/how-duolingo-streak-builds-habit
[r10]: https://smashingmagazine.com/2026/02/designing-streak-system-ux-psychology/
[r11]: https://9to5mac.com/2024/07/16/close-your-ringsbut-in-watchos-11-its-okay-if-you-dont/
[r12]: https://support.strava.com/hc/en-us/articles/36553427481997-Streaks-on-Strava
[r13]: https://dcrainmaker.com/2020/06/strava-legends-feature.html
[r14]: https://help.hevyapp.com/hc/en-us/articles/34467766576279
[r15]: https://apps.apple.com/us/app/gentler-streak-workout-tracker/id1576857102
[r16]: https://www.techradar.com/reviews/habitica
[r17]: https://habitica.fandom.com/wiki/Obstacles
[r18]: https://en.wikipedia.org/wiki/Habitica
[r19]: https://apps.apple.com/app/id6755652663
[r20]: https://apps.apple.com/us/app/-/id6499099763
[r21]: https://apps.apple.com/us/app/fitness-rpg-hero-health-game/id1252580641
[r22]: https://en.wikipedia.org/wiki/Overjustification_effect
[r23]: https://www.frontiersin.org/journals/psychology/articles/10.3389/fpsyg.2023.1286463/pdf
[r24]: https://doaj.org/article/e31353039b2b48dcb95d89561afc68c4
[r25]: https://researchers.westernsydney.edu.au/en/publications/cheat-meals-a-benign-or-ominous-variant-of-binge-eating-behavior/
[r26]: https://mdpi.com/2072-6643/10/12/1866
[r27]: https://newsroom.clevelandclinic.org/2024/01/05/why-you-should-ditch-the-term-cheat-day
[r28]: https://blog.myfitnesspal.com/why-you-shouldnt-use-exercise-to-earn-your-food/
[r29]: https://news.flinders.edu.au/blog/2025/02/22/fitness-apps-fuelling-disordered-eating/
[r30]: https://jyx.jyu.fi/bitstreams/c643820b-d619-4d7b-a678-92a1baf8a6ee/download
[r31]: https://coachsci.sdsu.edu/csa/vol171/ogasawar.htm
[r32]: https://help.fitbod.me/hc/en-us/articles/360004429814-How-Fitbod-Creates-Your-Workout
[r33]: https://help.fitbod.me/hc/en-us/articles/37629269518103
[r34]: https://techradar.com/health-fitness/fitness-apps/its-no-longer-enough-for-an-app-to-tell-you-what-to-do-people-want-to-know-why-fitness-app-fitbods-founder-on-the-reason-behind-the-ai-fitness-boom
[r35]: https://www.trustpilot.com/review/fitbod.me?page=2
[r36]: https://help.jtsstrength.com/en/articles/3-how-juggernautai-is-individualized-to-you
[r37]: https://help.rpstrength.com/hc/en-us/articles/32600173777815-How-does-the-app-determine-when-to-add-weight-reps-and-sets
[r38]: https://hypertrophy.zendesk.com/hc/en-us/articles/14605661323671
[r39]: https://freeletics.com/en/blog/posts/quick-adapt
[r40]: https://forum.freeletics.com/t/coach-keeps-suggesting-too-much-too-hard-beat-your-pbs/10635
[r41]: https://www.garagegymreviews.com/future-app-review
[r42]: https://www.garagegymreviews.com/caliber-app-review
[r43]: https://www.onepeloton.com/en-CA/blog/peloton-strength-plus-app
[r44]: https://apps.apple.com/us/app/id1073943857
[r45]: https://apps.apple.com/us/app/-/id1046634084
[r46]: https://apps.apple.com/us/app/-/id6499454966
[r47]: https://apps.apple.com/app/id6776659549
[r48]: https://macrofactor.com/body-metrics/
[r49]: https://help.macrofactorapp.com/en/articles/117-tips-for-taking-good-progress-photos
[r50]: https://developer.android.com/training/data-storage/use-cases
[r51]: https://proandroiddev.com/multitasking-intrusion-and-preventing-screenshots-in-android-app-15bd8757c24d
[r52]: https://webkit.org/?p=14403
[r53]: https://community.fitbit.com/t5/Product-Feedback/Lock-for-app/m-p/2316813
[r54]: https://macrumors.com/2024/06/10/ios-18-hide-lock-apps
[r55]: https://play.google.com/store/apps/details?id=app.repvoice
[r56]: https://apps.apple.com/us/app/id6454792467
[r57]: https://www.producthunt.com/products/liftly-voice-powered-workout-logging/reviews
[r58]: https://askvora.com/blog/voice-workout-logging-hands-free
[r59]: https://www.community.whoop.com/t/feature-request-capture-the-workout-by-voice-during-the-session/15590
[r60]: https://medium.com/hcil-at-umd/tandemtrack-shaping-consistent-exercise-experience-by-complementing-a-mobile-app-with-a-smart-1bb0de5054cd
[r61]: https://developer.android.com/reference/android/speech/SpeechRecognizer
[r62]: https://caniuse.com/speech-recognition
[r63]: https://huggingface.co/ggerganov/whisper.cpp
[r64]: https://cdn.jsdelivr.net/npm/nodejs-whisper@0.3.0/cpp/whisper.cpp/examples/whisper.wasm/README.md
[r65]: https://jvoltci.github.io/mosaic/edge-ai/multimodal/whisper-cpp
[r66]: https://pypi.org/project/vosk/0.3.21
[r67]: https://apps.apple.com/us/app/-/id1513988468
[r68]: https://apps.apple.com/gb/app/gowod/id1227834875
[r69]: https://apps.apple.com/app/id1175346453
[r70]: https://www.onepeloton.com/blog/strength-benchmark-fitness-test
[r71]: https://www.nike.com/a/what-are-benchmark-workouts
[r72]: https://raw.githubusercontent.com/yuhonas/free-exercise-db/main/README.md
[r73]: https://github.com/yuhonas/free-exercise-db/issues/12
[r74]: https://github.com/wrkout/exercises.json
[r75]: https://cdn.jsdelivr.net/gh/wger-project/wger@master/README.md
[r76]: https://wger.readthedocs.io/
[r77]: https://github.com/wger-project/wger/issues/2196
[r78]: https://github.com/exercemus/exercises
[r79]: https://raw.githubusercontent.com/InlitX/GymMane/master/CREDITS.md
[r80]: https://raw.githubusercontent.com/hasaneyldrm/exercises-dataset/main/NOTICE.md
[r81]: https://providers.apievangelist.com/providers/free-exercise-db/
[r82]: https://developer.android.com/develop/ui/views/notifications/notification-permission
[r83]: https://mhealth.jmir.org/2018/11/e10123/
[r84]: https://webkit.org/?p=13878
