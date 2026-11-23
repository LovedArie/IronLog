# IronLog: UI/UX Research

> **Date:** 2026-11-23 · **Scope:** screen layout of top fitness, habit and game-like apps; gym usability; game UI in serious tools; key flows; accessibility and a design system; Qt Quick (QML) feasibility on Android, WebAssembly (iPhone) and Windows.
> **Builds on:** [`feature-research.md`](feature-research.md) (loggers, logging speed) and [`feature-research-2.md`](feature-research-2.md) (game rules, coach apps, progress photos, mobility players). This report does not repeat them; it links back where it relies on them.
> **Read this first:**
> - *(vendor)* = written by the company that sells the product. ScreensDesign pages are third-party screen libraries built from captions, not hands-on tests.
> - *(analysis)* = my own reasoning, no source behind it. *(general knowledge)* = not checked in this round. *(unverified)* = I could not confirm it.
> - I could not open the apps myself. App layouts come from vendor pages, help centres, reviews and screen libraries.
> - Citations look like [[12]][u12]. Every one links to its source at the end.
> - Wireframes are 40 characters wide (a phone in portrait). `[x]` = selected chip, `( )` = unselected chip, `[*Word*]` = pre-picked/selected, `>` = opens a page, `v` = expands.

---

## Summary

- **Best apps keep 3–5 tabs and put "start training" one tap from home.** Strong starts a workout and logs a set in about two taps [[6]][u6]; Hevy starts from its Workout tab [[4]][u4]. Apple says a tab bar is for *navigation, not actions*, must stay visible, and should never hide or disable a tab [[28]][u28]. → IronLog: **4 tabs** and one big **Train now** button on *Today*.
- **Workout screens have converged on one pattern.** A set table with last time's numbers pre-filled, a tick per set that **auto-starts the rest timer**, ±15 s adjust, swipe-to-delete, set type behind the set number, rare actions behind "⋯" [[1]][u1] [[2]][u2] [[8]][u8]. IronLog's guided session should copy it and add one big bottom **Log set** button.
- **Gym use = one hand, a glance, sweaty fingers.** 49 % of people use one hand, and touches are fastest and most accurate in the centre, not the corners [[33]][u33]. Apple's workout guidance: large high-contrast text, nothing unrelated on screen, clear start/stop, a summary at the end [[27]][u27]. Targets: 44 pt (Apple) / 48 dp (Google) [[29]][u29] [[41]][u41].
- **Clutter is beaten with progressive disclosure, max 2 levels** [[36]][u36]. Fitbod puts actions in a "⋯" menu at each level [[8]][u8]; Gentler Streak cut 4 tabs to 3 and moved key info onto one home page that reorders through the day [[12]][u12].
- **Game UI must be quiet and skippable.** Users ask to skip Zwift's streak screen [[24]][u24] and to hide Strava's weekly snapshot because it "makes me feel bad" [[14]][u14]. Todoist lets users switch Karma off and protect streaks on vacation [[23]][u23]. → Celebrations only at session end, short, tap-to-skip; turning the game off changes the *You* tab's content, never removes the tab.
- **Wizards: show the steps, label Back with the step name, pre-fill last answers, and give repeat users a faster path** [[38]][u38]. → *Train now* gets a **"Same as last time"** one-tap start.
- **Onboarding: ask only what the app needs to work, keep it short and skippable, show progress, say why** [[37]][u37]. Hevy's flow has ~11 stages including a paywall [[3]][u3]; MacroFactor Workouts has 38 steps [[10]][u10]. → IronLog: 5 short steps.
- **A small design system with validated colours.** Status colours built from colour-blind-safe Okabe-Ito hues [[44]][u44] pass WCAG contrast (≥ 3:1 for marks [[43]][u43]) and a colour-vision-deficiency check in light *and* dark, and always come with an icon + word [[29]][u29].
- **Qt Quick fits every pattern with stock parts** (Material style, Material 3 look since Qt 6.5 [[50]][u50]; `SafeArea` since 6.9 [[51]][u51]; `SwipeDelegate`, `Shape`). **Platform risks** to test in the I4 spike: an `ApplicationWindow` edge-to-edge regression on Android [[53]][u53], the soft keyboard [[55]][u55], keeping the screen awake (JNI / Wake Lock) [[54]][u54] [[45]][u45], **no vibration on iPhone web** [[48]][u48], camera on WebAssembly is a technology preview [[57]][u57], only 3 built-in fonts on WebAssembly [[57]][u57].

---

## App patterns

| App | Pattern | How it works | Why it works / user complaints |
|---|---|---|---|
| **Hevy** *(vendor)* | Active-workout set table | Header shows duration, sets and volume, *Finish* top-right. Each set shows a **Previous** value; weight/reps pre-filled from last time. Tick a set → rest timer starts. Tap the set number for warm-up/drop/failure. Swipe left → red Delete. "⋯" per exercise for superset, reorder, replace [[1]][u1] | Last-time values remove typing. Rest timer sits as a bottom overlay [[3]][u3]. PR alerts fire on the tick [[1]][u1] |
| Hevy *(vendor)* | Rest timer | Auto-starts on set complete; per-exercise 5 s–5 min; **±15 s** after a set; can be turned off for circuits [[2]][u2] | Rest-timer alerts while in other apps were a complaint in round 1 ([`feature-research.md`](feature-research.md)) |
| Hevy | Navigation + onboarding | Home = social feed; **Workout** tab = Quick Start, routine cards, Explore. Onboarding: carousel → sign-up → permission primers → units → Health → paywall → survey → tutorial [[3]][u3] [[4]][u4] | Social feed on the home tab divides users (round 1). Long onboarding before first use [[3]][u3] |
| Hevy *(vendor)* | Body chart | Profile → Statistics: a muscle diagram under a 7-day graph; trained parts in blue; "Muscle distribution (Body)" = sets per muscle per week [[5]][u5] | Tucked two taps deep, not on home — a good model for IronLog's game-off mode |
| **Strong** | Minimal logger | Routines, history, exercises, settings; **~2 taps to start, ~2 taps per set**; tap a set to repeat last time's numbers; auto rest timer [[6]][u6]. Live Activity + Dynamic Island for rest timers [[7]][u7] | Rated 4.9 (~109K) [[7]][u7]; praised as "lightning-fast" and uncluttered [[6]][u6]. Complaints: exercise database flexibility, bar weights, price [[7]][u7] |
| **Fitbod** *(vendor)* | Workout tab rebuild | Big timer at top; "+" top-right adds exercises; "X" top-left discards; quick edits (duration, muscles) above the list; **"⋯" menus at workout, group and exercise level**; long-press to reorder [[8]][u8] | Rebuilt "from the ground up" for usability; users asked to see duration at a glance [[8]][u8] |
| **MacroFactor Workouts** *(vendor)* | Equipment per gym | One profile per place (home, commercial, travel) listing equipment, bars, plates, machines; pick one per workout; plate calculator uses your plates [[9]][u9] | Same idea as IronLog's "equipment per place". Onboarding is 38 steps with a hard paywall [[10]][u10] |
| **Apple Workout (watchOS 26)** | Corner buttons | One full-screen page per workout type with **four corner buttons** (metrics, goals, Workout Buddy, media); swipe or crown to change type [[21]][u21] | Reviewer: scrolling is too sensitive; Workout Buddy feels forced [[21]][u21] |
| Apple Fitness (iOS 26) | Lock-screen session | Workouts started on iPhone show a Live Activity with duration and pause/stop on the Lock Screen [[22]][u22] | A session you can see and control without opening the app |
| **Gentler Streak** | One calm home page | Redesign cut **4 tabs → 3**. The main page sits under the Activity Path; in the morning "For You" (sleep, wellbeing) is on top, later the day's workouts move up. Users choose which cards appear [[12]][u12] | CEO: people weren't finding features in the old layout [[12]][u12]. Design aims to guide, not push; explains numbers in simple charts [[11]][u11] |
| **Duolingo** *(vendor)* | Path home screen | The skill tree became **one linear path**: tap the next circle. A floating button returns you to your spot. Quests moved to their own tab [[15]][u15] | Learners weren't sure they were using the app "the right way" [[15]][u15]. → IronLog's *Train now* plays the same role: the app picks the next step |
| Duolingo *(vendor)* | Tab consistency | Tabs added over time looked unrelated; redesign set tiered header sizes, fewer type styles, whitespace instead of boxes [[16]][u16] | "Higher engagement across tabs" (no figures) [[16]][u16]. Lesson: one type scale from day one |
| Duolingo *(vendor)* | Milestone animation | Streak milestones (week, month, 100 days…) get a special animation; later a shareable card [[17]][u17] | Early metrics suggested more kept their streaks (no figures) [[17]][u17] |
| **Finch** | Pet + daily goals | You pick your own goals and tick them off; each good day reveals more of the pet [[18]][u18] | Critique: Quests and Settings show too many options → decision fatigue; suggests progressive disclosure and **one quest at a time** [[18]][u18] |
| **Habitica** | RPG header | Avatar + party in the header; inventory, shops, guilds as dropdowns; skills in a bar at the bottom of Tasks [[19]][u19] | Reorganised after user tests. Round 2: steep learning curve, punishing rules ([`feature-research-2.md`](feature-research-2.md)) |
| **Bend** *(vendor)* | Routine player | Minimal player with clear visual timers; edit/remove/reorder exercises in a routine [[20]][u20] | Rating prompt and sign-up push come too early [[20]][u20]. Round 2: no countdown after skip, transitions too short |
| **Strava** | "You" tab | 2021 layout: **You** (your stats, history, profile), Home (feed), Maps, Groups, Record [[13]][u13] | Aim: make features easier to find [[13]][u13]. Weekly snapshot on the feed can't be hidden; users find it stressful [[14]][u14] |
| **Todoist** *(vendor)* | Game off switch | Karma (points, streaks) can be switched off in Settings → Productivity; **vacation mode** keeps Karma and streaks intact [[23]][u23] | A serious tool where the game layer is optional — same idea as ADR-021 |
| **Zwift** | Streak screen | A streak screen appears after each ride [[24]][u24] | Users ask to skip or disable it: extra waiting and clicks, unwanted gamification [[24]][u24] |
| **EZ Progress** *(vendor)* | Photo ghost | Last photo fades over the live camera to match angle, distance and light; new photo shown beside its anchor; before/after slider [[25]][u25] | Only 1 rating — the pattern, not the app, is the evidence (see round 2 for more photo apps) |

**Other evidence**

- Reddit summaries say the best loggers need **~2–3 taps per set**; complaints are waiting through screens/animations, timers that don't notify, bloat, and heavy gamification [[26]][u26] *(vendor, threads not linked — treat as a direction)*.
- A review study of smart health apps found manual data entry burden is one of the three main complaints, next to accuracy and tailoring [[35]][u35] (120 reviews, 6 apps — small).

---

## Proposed navigation

**Tab bar (4):** **Today · Log · Progress · You**

- 4 tabs fits Android's 3–5 rule [[39]][u39] and Apple's "fewer is easier" [[28]][u28].
- *Train now* is an **action**, so it is a big button on *Today*, not a tab [[28]][u28].
- The guided session is **full-screen** (covers the tab bar, allowed for modal views [[28]][u28]). It can collapse into a **mini bar above the tabs**, like Apple's MiniPlayer accessory [[28]][u28].
- *You* never disappears: with the game off it shows plain progress (Apple: don't hide tabs [[28]][u28]; Strava's *You* tab is the model [[13]][u13]).
- Settings is a gear on *You*: it's rarely used, so the top corner is fine *(analysis)*.
- Wide screens (Windows, tablets): the same 4 items become a **navigation rail** on the left [[39]][u39].

```text
IronLog
├─ Today                      (home)
│   ├─ Weekly quest card      game off: "2 of 3 planned sessions"
│   ├─ One warning card       fading skill · deload hint · check-in due
│   ├─ Done today             all activities logged today
│   ├─ [TRAIN NOW]            → Train now wizard (full screen)
│   │    ├─ 1 Time  → 2 How is today → 3 Where / Nearby / Sore parts
│   │    └─ Plan (reasons, swap, XP preview) → [START]
│   │          └─ Guided session (full screen, can minimise)
│   │               ├─ Strength block · hold timer · stretch player
│   │               └─ Finish → How was it? → reward reveal (game on)
│   └─ [Same as last time]    → straight to the plan
├─ Log
│   ├─ Week strip → day list → session detail (edit / undo / delete)
│   └─ [+ Add to <day>]       strength · cardio · badminton · stretch · body check-in
├─ Progress
│   ├─ Strength   PRs · e1RM chart · sets per muscle · deload hint
│   │    └─ Exercise detail   e1RM history · rep-max table · PR types
│   ├─ Body       weekly check-in · weight trend · waist trend
│   └─ Photos     (lock) capture · gallery · compare
└─ You
    ├─ Game ON    total level · body map · quest + streak + tokens + reward
    │             · skills (top 3, "show all 8") · Pause · unlocks · how XP works
    ├─ Game OFF   program · this week · training calendar · new exercises · lighter weeks
    └─ [cog] Settings
        ├─ Profile        goal program · training days · injuries & limits
        ├─ Places         home · gym · limited → equipment page each
        ├─ Training       units · default rest · keep screen on · sounds · vibration
        ├─ Game           play it like a game · rules version
        ├─ Look           theme · reduce motion
        ├─ Privacy & data account & sync · photo lock & key backup · backups & CSV
        └─ About          not medical advice · credits · developer (sample data)
```

---

## Feature → screen map

**Tier:** **A** = always visible on its screen · **1** = one tap away · **T** = tucked away (2+ taps, "⋯", or Settings). ⚠ = flag.

### Coach

| Feature (id) | Home: screen → control / gesture | Tier |
|---|---|---|
| *Train now* (F8, UC-11) | Today → big **TRAIN NOW** button (bottom, thumb zone) | A |
| "Same as last time" *(new)* | Today → button under Train now; skips the 3 questions | A |
| Time · situation · place questions (F8) | Wizard steps 1–3: big chips / cards, tap = next | 1 |
| *Sore* → which body parts (F8) | Wizard step 3 (sore): multi-select chips | 1 |
| *Cramming* → nearby equipment (UC-11 5a) | Wizard step 3 (cramming): filter chips pre-ticked from last time | 1 |
| Quiet / small-space options (research 2 #11) | Wizard step 3 (normal) → "Options" checkboxes | T |
| Plan with one-line "why" (F7) | Plan screen → grey line under each exercise | A |
| XP preview (R1) | Plan header (game on only) | A |
| Swap an exercise (UC-11 8b) | Plan → `[swap]` per row; session → exercise "⋯" | 1 |
| "Not medical advice; stop on sharp pain" (§8.9) | Plan footer (always); session "⋯"; Settings → About | A |
| Plan kept for the day (UC-11 9a) | Today → Train now button becomes **Resume today's plan** | A |
| Goal program (F6, UC-09) | Onboarding step 1; You → Program row (game off); Settings → Profile | T |
| Training days / availability (F5) | Onboarding step 4; Settings → Profile | T |
| Guided session (F9, UC-12) | Full screen from *Start*; ˅ collapses it to a mini bar over the tabs | A |
| Log set / repeat last set (F9, Q1) | Session → big bottom **LOG SET** button, pre-filled: **1 tap** | A |
| ± steppers (F9) | Session → active set card, 48 dp `[-]` `[+]` | A |
| Type an exact number | Session → tap the number → numeric keypad | 1 |
| RIR · set type · note (F1) | Active set card → "RIR · type · note" row (collapsed) | T |
| Rest timer (F9) | Session → bar above LOG button; starts on LOG; ±15 s, Skip | A |
| Hold timer for timed sets (F9, F1) | Session → ring timer page for plank / wall sit / balance | A |
| Stretch / mobility player (F11, research 2 #8) | Session stretch block; Log → + Add → Stretch | 1 |
| Edit a set + undo (F3, UC-08) | Tap a done set → bottom sheet; **Undo** in a snackbar and in "⋯" | 1 |
| Delete a set (UC-02) | Swipe left **or** bottom sheet → Delete (gesture always has a button [[29]][u29]) | 1 |
| Plate + warm-up calculator (F20) | Session → exercise "⋯" → *Plates & warm-up* | T |
| "How was it?" (F10) | After *Finish*, full screen, chips pre-picked | A |
| Voice note (F10) | "How was it?" → note field (keyboard mic) | 1 |
| Pain check (P7, UC-18) | Joints → **Pain** chip → sheet "which exercise?" | 1 |
| Unlocks (P6) | Plan "NEW" tag; reveal after session; You → Unlocks / "New exercises" | 1 ⚠ |
| Stall → deload hint (F22) | Progress → Strength card; Today warning card when it affects the next plan | 1 |
| "Every number explains itself" (Vision) | Tap any number (XP, level, PR, load) → **Why?** sheet | 1 ⚠ |

### Log

| Feature (id) | Home: screen → control / gesture | Tier |
|---|---|---|
| History / review a session (UC-03) | Log → day → session detail | 1 |
| Back-dated logging (F14 streak restore) | Log → week strip → pick day → **+ Add to <day>** | 1 |
| Cardio: treadmill time/speed/incline (F11) | Log → + Add → Cardio (steppers) | 1 |
| Badminton (F11) | Log → + Add → Badminton (minutes + intensity chips) | 1 |
| Several activities a day (Vision) | Log day list; Today "Done today" | A |
| Custom exercise (UC-06) | Exercise picker → "+ New exercise" | T |
| Body check-in (F18, UC-19) | Progress → Body → *Log weight + waist*; Today card on check-in day; Log → + Add | 1 |

### Progress

| Feature (id) | Home: screen → control / gesture | Tier |
|---|---|---|
| PRs + more PR types (F19) | Progress → Strength → *Recent PRs*; exercise detail | A |
| Rep-max table (F19) | Progress → exercise detail | T |
| e1RM chart (UC-05) | Progress → Strength → chart card + exercise picker | A |
| Weekly sets per muscle (UC-04) | Progress → Strength → *Sets this week* (top 3, "show all") | A |
| Weight + waist trend (F18) | Progress → Body: **two small charts**, never one chart with two y-axes *(dataviz rule)* | 1 |
| Progress photos: capture (F21) | Progress → Photos → *Take photos*; also offered after a body check-in | 1 |
| Photos: compare (F21) | Progress → Photos → *Compare* (slider / side by side) | 1 |
| Photo lock + key backup (ADR-020) | **Prompt on first photo**; Settings → Privacy | T ⚠ |

### Game (ADR-021: everything here hides when the game is off)

| Feature (id) | Home: screen → control / gesture | Tier |
|---|---|---|
| Character sheet + body map (F12, UC-15) | You tab (top) | A |
| Total level (F12) | You header | A |
| 8 skills + "sets to next level" (F12) | You → Skills (top 3 + "show all 8") | A / 1 |
| XP per set (§8.6) | Session: small "+10" next to the logged row — no pop-up | A |
| Level-up / quest-complete moment (F14) | **One** reveal screen after "How was it?" Save | A (end) |
| Decay warning (F13) | Today warning card (one at a time) + striped muscle on the body map | A |
| Most-neglected skill (research 2) | Body map stripe + Today card | A |
| Comeback bonus ×2 (F13) | You → skill row badge "x2" | 1 |
| Pause with a reason (F13, UC-16) | You → **[Pause]** → sheet: exams · sick · injured · travel | 1 ⚠ |
| Weekly quest (F14) | Today card + You | A |
| Rest tokens (F14) | Quest card `<> <>` | A |
| Your reward text (UC-17) | Quest card → edit | 1 |
| Weekly streak (F14) | Quest card "streak 6 wk" | A |
| Training heat map (F14) | You → calendar (game off: plain calendar, no streak number) | 1 |
| "Game rules updated" (§8.6) | One-time banner on You; Settings → Game | T |
| Game mode on/off (UC-23) | Onboarding step 5; Settings → Game | T |

### Online, data, platform

| Feature (id) | Home: screen → control / gesture | Tier |
|---|---|---|
| Register / log in with invite (F15, UC-20) | First launch: *Sign in* or *Use offline*; Settings → Account | T |
| Sync status (F16) | Today header dot; Settings → Account | A |
| Sync conflict choice (R3 step 5) | Modal sheet only when needed | ⚠ |
| Partner flow (F17) | Invite link → register → onboarding | — |
| Invite a new lifter (UC-22) | Server admin task — no app screen yet | ⚠ |
| Backups (F4) | Automatic; Settings → Data shows the last one | T |
| CSV export (F4, UC-10) | Settings → Data → Export | T |
| Units kg / lb (§8.7) | Onboarding step 2; Settings → Training | T |
| Equipment per place (F5) | Onboarding step 2; Settings → Places; wizard "Change today's equipment" | T |
| Injuries & limits (F5) | Onboarding step 3; Settings → Profile | T |
| Sample data (UC-07) | Settings → About → Developer (desktop) | T |
| Credits / licences (research 2 E3) | Settings → About | T |
| Desktop shortcuts (Ctrl+Enter) | Desktop only; shown in tooltips | — |
| *Later:* reminders · boss fights · voice logging · party view | Settings → Reminders · quest card slot · session "⋯" · You → Party | — |

### ⚠ Features without a good home yet

1. **Pause when the game is off.** Pause also sets the exam-week minimum for the planner (§8.6, P5), so it must exist with the game off. Proposal: *You* → "Lighter weeks (exams, sick…)" in both modes.
2. **Cramming vs Pause: exams.** One is for today, one for weeks. Users may mix them up. Proposal: after 3 *Cramming* answers in one week, Today offers "Exam weeks? Start a pause" *(analysis)*.
3. **Unlocks with the game off.** Gates stay active (ADR-021), so they need a plain home: "NEW" tag on the plan + a "New exercises" card on *You*.
4. **"Every number can explain where it came from"** is a Vision success criterion with no UI. Proposal: one reusable **Why?** bottom sheet on every number.
5. **This week's planned sessions.** No screen shows what's planned. Proposal: the quest card lists them ("Str A ✓ · Str B ✓ · Zone 2 ○").
6. **Photo key backup** — losing the key loses every photo (round 2). Must be a prompt on first capture, not only a settings row.
7. **Sync conflict choice** and **invite a lifter** — rare, but each needs a designed sheet (or a server CLI for invites).

### Screens at risk of crowding

| Screen | Risk | Fix |
|---|---|---|
| **You (game on)** | 12+ items: level, body map, quest, streak, tokens, reward, 8 skills, pause, unlocks, comeback, rules | Body map + quest card first; skills show top 3; unlocks / rules / how-XP-works in one row at the bottom |
| **Guided session** | Weight, reps, RIR, type, note, rest, XP, why, last time | Only weight + reps + LOG visible; RIR/type/note collapsed; XP is a tiny "+10" |
| **Today** | Quest, decay warning, deload hint, check-in due, photo due, sync, done-today | **Card budget: max 3 cards** — quest, **one** warning (highest priority wins), done today. The plan lives in the big button (*Resume today's plan*), the session in the mini bar. Gentler Streak reorders its home by time of day [[12]][u12] |

---

## Wireframes

### Today (game on)

```text
+--------------------------------------+
| Today - Mon 23 Nov        (o) Synced |
|--------------------------------------|
| WEEKLY QUEST             2 of 3 done |
| [############-------] 1 session left |
| Tokens <> <>   Reward: movie night > |
|--------------------------------------|
| (!) Back starts fading in 3 days.    |
|     Train now will add a back move.  |
|--------------------------------------|
| DONE TODAY                           |
|   Morning stretch   10 min  +20 Mob  |
|--------------------------------------|
|                                      |
|   +------------------------------+   |
|   |        >  TRAIN NOW          |   |
|   +------------------------------+   |
|   [ Same as last time: 30' home  ]   |
|--------------------------------------|
|  [Today]    Log    Progress    You   |
+--------------------------------------+
```

- One primary button. Quest card on top, **one** warning card at most.
- Game off: the quest card reads "2 of 3 planned sessions", no tokens/reward, no "+20 Mob".
- While a session runs, a mini bar sits above the tabs:

```text
+--------------------------------------+
|   ...Today content...                |
|--------------------------------------|
|  > DB row - set 3 of 5 - rest 1:12 ^ |
|--------------------------------------|
|  [Today]    Log    Progress    You   |
+--------------------------------------+
```

### Train now wizard

Each step is one question. Tapping an answer moves on (no *Next* button). Back is labelled with the step it returns to [[38]][u38]. Answers stay when you go back; last time's answer is outlined [[38]][u38].

**Step 1 — time**

```text
+--------------------------------------+
| <  Train now               Step 1/3  |
|    (*)----( )----( )                 |
|                                      |
| How much time do you have?           |
|                                      |
|  +--------+ +--------+ +--------+    |
|  | 15 min | | 20 min | | 30 min |    |
|  +--------+ +--------+ +--------+    |
|  +--------+ +--------+ +--------+    |
|  | 45 min | | 60 min | | 90 min |    |
|  +--------+ +--------+ +--------+    |
|                                      |
|  30 min is outlined: last time.      |
|  Tap a time to go on.                |
|--------------------------------------|
|  [ < Cancel ]                        |
+--------------------------------------+
```

**Step 2 — situation**

```text
+--------------------------------------+
| <  Train now               Step 2/3  |
|    (*)----(*)----( )                 |
|  30 min                      [edit]  |
| How is today?                        |
|  +--------------------------------+  |
|  | Normal day                     |  |
|  | Full plan for your goal        |  |
|  +--------------------------------+  |
|  +--------------------------------+  |
|  | Cramming                       |  |
|  | Short, compound-only, no PRs   |  |
|  +--------------------------------+  |
|  +--------------------------------+  |
|  | Sore / tired                   |  |
|  | Mobility + easy cardio         |  |
|  +--------------------------------+  |
|--------------------------------------|
|  [ < Time ]                          |
+--------------------------------------+
```

**Step 3 — depends on step 2.** Normal → where:

```text
+--------------------------------------+
| <  Train now               Step 3/3  |
|    (*)----(*)----(*)                 |
|  30 min - Normal             [edit]  |
| Where are you training?              |
|  +--------------+ +--------------+   |
|  |     HOME     | |     GYM      |   |
|  | DB 5-25 lb,  | | full gym     |   |
|  | bands, tread | |              |   |
|  +--------------+ +--------------+   |
| Options (optional)                   |
|  [ ] Quiet - no jumps                |
|  [ ] Small space                     |
|  Change today's equipment >          |
|--------------------------------------|
|  [ < How is today ]                  |
+--------------------------------------+
```

Cramming → what's nearby (filter chips, multi-select [[40]][u40]):

```text
+--------------------------------------+
| <  Train now               Step 3/3  |
|  30 min - Cramming           [edit]  |
| What's nearby?  (ticked = last time) |
|  [x Dumbbells] [x Mat] [ Bands ]     |
|  [ Chair ] [ Treadmill ] [ None ]    |
|                                      |
|   +------------------------------+   |
|   |       BUILD MY PLAN  >       |   |
|   +------------------------------+   |
|--------------------------------------|
|  [ < How is today ]                  |
+--------------------------------------+
```

Sore / tired → which parts:

```text
+--------------------------------------+
| <  Train now               Step 3/3  |
|  30 min - Sore / tired       [edit]  |
| What feels sore?  (pick any)         |
|  [ Legs ] [ Back ] [ Shoulders ]     |
|  [ Arms ] [ Core ] [ All over ]      |
|                                      |
|   +------------------------------+   |
|   |       BUILD MY PLAN  >       |   |
|   +------------------------------+   |
|--------------------------------------|
|  [ < How is today ]                  |
+--------------------------------------+
```

**Result — the plan**

```text
+--------------------------------------+
| <  Today's plan   30 min      [...]  |
| Home - Normal - about +140 XP        |
|--------------------------------------|
| WARM-UP                       6 min  |
|   Easy treadmill 3' + RAMP drills    |
| MAIN                         20 min  |
|  1 Box squat  3 x 8  20 lb  [swap]   |
|    why: joint-friendly, compound 1st |
|  2 DB row     3 x 10 25 lb  [swap]   |
|    why: Back is fading - priority    |
|  3 Rev. lunge 2 x 8  body   [swap]   |
|    NEW - unlocked: 3 clean weeks     |
| STRETCH                       4 min  |
|--------------------------------------|
| (i) General guidance, not medical    |
|     advice. Stop on sharp pain.      |
|   +------------------------------+   |
|   |        START SESSION         |   |
|   +------------------------------+   |
+--------------------------------------+
```

### Guided session — set logging + rest timer

```text
+--------------------------------------+
| v  24:10      7 of 14 sets [Finish]  |
|--------------------------------------|
| 2 of 5   DB ROW                [...] |
| why: Back is fading - priority       |
| Last time: 25 lb x 10, 10, 9         |
|--------------------------------------|
|  SET  TARGET     DONE                |
|   1   25 x 10    25 x 10   [v] +10   |
|   2   25 x 10    25 x 10   [v] +10   |
|   3 +----------------------------+   |
|     |  [ - ]   25 lb    [ + ]    |   |
|     |  [ - ]  10 reps   [ + ]    |   |
|     |  RIR - type - note      v  |   |
|     +----------------------------+   |
|   4   25 x 10                        |
|   + add set                          |
|--------------------------------------|
|  REST 1:12      [-15s] [+15s] [Skip] |
|  [==============--------]            |
|--------------------------------------|
|   +------------------------------+   |
|   |   LOG SET 3:  25 lb x 10     |   |
|   +------------------------------+   |
|   [< Prev exercise]  [Next ex. >]    |
+--------------------------------------+
```

- **Repeat set = 1 tap** on LOG (values pre-filled from plan / last set). Changing by one step = 2 taps.
- Rest timer starts on LOG, like Hevy [[2]][u2]. LOG stays usable during rest ("log early").
- Delete: swipe a done row left, or tap it → sheet → Delete.
- Top-left ˅ collapses to the mini bar. *Finish* asks to confirm.

### Guided session — timed hold

```text
+--------------------------------------+
| v  31:40      9 of 14 sets [Finish]  |
|--------------------------------------|
| 4 of 5   PLANK                 [...] |
| Hold 3 x 30 s - last: 30, 30, 25 s   |
|--------------------------------------|
|                                      |
|             .-''''''-.               |
|           /            \             |
|          |     0:18     |            |
|          |   of 0:30    |            |
|           \            /             |
|             '-......-'               |
|             hold 2 of 3              |
|                                      |
|   3-2-1 before start; beep + buzz    |
|   at 0:00; 'switch sides' cue        |
|--------------------------------------|
|  [ Pause ]            [-5 s] [+5 s]  |
|   +------------------------------+   |
|   |      STOP & LOG  0:18        |   |
|   +------------------------------+   |
+--------------------------------------+
```

- Countdown before start and a clear side-switch cue fix Bend's top complaints (round 2).

### "How was it?"

```text
+--------------------------------------+
| How was it?                          |
| 30 min - 12 sets - 1 PR - +150 XP    |
|--------------------------------------|
| OVERALL                              |
|  ( Rough ) ( OK ) ( Good ) [*Great*] |
|    suggested: you set a PR           |
| ENERGY                               |
|  [*Low*]    ( Normal )    ( High )   |
|    suggested: you picked Cramming    |
| JOINTS                               |
|  [*Fine*]   ( Stiff )     ( Pain )   |
| NOTE (optional)                      |
|  +--------------------------------+  |
|  | Type, or tap the keyboard mic  |  |
|  +--------------------------------+  |
|--------------------------------------|
|   +------------------------------+   |
|   |            SAVE              |   |
|   +------------------------------+   |
+--------------------------------------+
```

- Pre-picked chips are filled and carry a one-line reason (arc42 §8.15). **Joints is never pre-picked as Pain.**
- One tap on SAVE confirms everything. *Pain* opens:

```text
+--------------------------------------+
| Which exercise hurt?                 |
|  ( Box squat ) ( Reverse lunge )     |
|  ( DB row )    ( Not sure )          |
|                                      |
| That exercise drops one stage for    |
| 2 weeks. Sharp pain or swelling the  |
| next day? Talk to a professional.    |
|   +------------------------------+   |
|   |            SAVE              |   |
|   +------------------------------+   |
+--------------------------------------+
```

- Then, game on only, **one** reveal screen (skippable, ≤ 1.5 s of motion):

```text
+--------------------------------------+
|                                      |
|           QUEST COMPLETE             |
|                                      |
|      +1 rest token     <> <>         |
|      Your reward: movie night        |
|                                      |
|      Chest   L5 -> L6                |
|      [##########]                    |
|                                      |
|   +------------------------------+   |
|   |            NICE              |   |
|   +------------------------------+   |
|        or tap anywhere to close      |
+--------------------------------------+
```

### You — game on (character sheet)

```text
+--------------------------------------+
| You                  [Pause] [cog]   |
| Total level 23                 [?]   |
|--------------------------------------|
|      FRONT            BACK           |
|       (O)              (O)           |
|      /|#|\            /|=|\          |
|     / |#| \          / |/| \         |
|       / \              / \           |
|      /   \            /   \          |
|  darker = higher level               |
|  striped grey = fading soon          |
|--------------------------------------|
| WEEKLY QUEST   2 of 3   streak 6 wk  |
|  Str A [v]  Str B [v]  Zone 2 [ ]    |
|  Tokens <> <>  Reward: movie night>  |
|--------------------------------------|
| SKILLS                   sets to go  |
|  Chest   L6  [#####---]   4          |
|  Back    L4  [##------]   fading     |
|  Legs    L5  [###-----]   7   x2     |
|  Show all 8 skills               v   |
|--------------------------------------|
|  Unlocks (2)  -  How XP works    >   |
+--------------------------------------+
```

### You — game off (same tab, plain progress)

```text
+--------------------------------------+
| You                           [cog]  |
| Program: Lean & toned (upper)     >  |
|--------------------------------------|
| THIS WEEK            2 of 3 planned  |
|  Str A [v]  Str B [v]  Zone 2 [ ]    |
|  [############--------]              |
|--------------------------------------|
| TRAINING CALENDAR (12 weeks)         |
|    M T W T F S S                     |
|    # . # . # . .                     |
|    # . # # . . .                     |
|    . # . # . # .                     |
|--------------------------------------|
| NEW EXERCISES AVAILABLE              |
|  Reverse lunge - 3 clean weeks    >  |
|--------------------------------------|
|  Lighter weeks (exams, sick...)   >  |
|--------------------------------------|
|  [Today]    Log    Progress   [You]  |
+--------------------------------------+
```

### Log

```text
+--------------------------------------+
| Log                          [cal]   |
|  < Mon Tue Wed Thu Fri Sat [Sun] >   |
|--------------------------------------|
| SUNDAY 22 NOV                        |
|  Strength - 42 min - 14 sets      >  |
|  Badminton - 60 min - hard        >  |
|--------------------------------------|
| SATURDAY 21 NOV                      |
|  Stretch routine - 12 min         >  |
|--------------------------------------|
|   +------------------------------+   |
|   |   + ADD TO SUNDAY            |   |
|   +------------------------------+   |
|   Strength | Cardio | Badminton |    |
|   Stretch  | Body check-in           |
|--------------------------------------|
|  Today    [Log]    Progress    You   |
+--------------------------------------+
```

### Progress — Strength

```text
+--------------------------------------+
| Progress                             |
|  [Strength]  ( Body )  ( Photos )    |
|--------------------------------------|
| RECENT PRS                           |
|  Bench press  e1RM 123.5  23 Nov >   |
|  DB row       12 reps     20 Nov >   |
|--------------------------------------|
| e1RM   [ Bench press         v ]     |
|  |                          *        |
|  |                 .-----.-'         |
|  |        .-.-----'                  |
|  |---.---'                           |
|  +--------------------------------   |
|   Oct 5                     Nov 23   |
|--------------------------------------|
| SETS THIS WEEK   (target 10-20)      |
|  Chest  14 [=======    ] (v) ok      |
|  Quads   4 [==         ] (!) low     |
|  Back   22 [===========] (^) high    |
|  Show all muscles                v   |
|--------------------------------------|
| (i) Squat stalled 3 sessions.        |
|     Plan a lighter week?    [Why?]   |
+--------------------------------------+
```

- Status = icon + word + colour, never colour alone [[29]][u29].

### Progress — Body

```text
+--------------------------------------+
| Progress                             |
|  ( Strength )  [Body]  ( Photos )    |
|--------------------------------------|
| WEEKLY CHECK-IN         due Sunday   |
|   [ Log weight + waist ]             |
|--------------------------------------|
| WEIGHT - 4-week trend    -x.x kg     |
|   small line chart, own axis         |
|--------------------------------------|
| WAIST - 4-week trend     -x.x cm     |
|   small line chart, own axis         |
|--------------------------------------|
| PHOTOS     last: 4 weeks ago      >  |
+--------------------------------------+
```

### Progress photos — capture with ghost overlay

```text
+--------------------------------------+
| x  Front - 1 of 3       [5 s timer]  |
| +---------------------------------+  |
| |          live camera            |  |
| |   . . . ghost of week 1 . . .   |  |
| |   :         ( )           :     |  |
| |   :        /| |\          :     |  |
| |   :         | |           :     |  |
| |   :        /   \          :     |  |
| |   . . .  pose outline . . .     |  |
| +---------------------------------+  |
|  Ghost  [-----o-----------]  35%     |
|  [*Front*]    ( Side )    ( Back )   |
|                                      |
|              +--------+              |
|              |  (O)   |              |
|              +--------+              |
+--------------------------------------+
```

- Ghost = last photo of the same pose at ~35 %, adjustable; pose outline; 5 s timer (round 2).
- Photo screens block screenshots on Android (`FLAG_SECURE`, round 2).

### Progress photos — compare slider

```text
+--------------------------------------+
| <  Compare                [lock]     |
|  Week 1  v             Now  v        |
| +----------------+----------------+  |
| |                |                |  |
| |    week 1     <|>     now       |  |
| |                |                |  |
| +----------------+----------------+  |
|  drag <|> to wipe between photos     |
|  [*Slider*]   ( Side by side )       |
|  [*Front*]    ( Side )    ( Back )   |
|--------------------------------------|
|              Week 1        Now       |
|  Weight      xx.x kg       xx.x kg   |
|  Waist       xx cm         xx cm     |
+--------------------------------------+
```

### Settings

```text
+--------------------------------------+
| <  Settings                          |
| PROFILE                              |
|  Goal program   Badminton singles >  |
|  Training days        3 a week    >  |
|  Injuries & limits                >  |
| PLACES & EQUIPMENT                   |
|  Home     DB 5-25 lb, bands...    >  |
|  Gym      full gym                >  |
|  Limited  (cramming checklist)    >  |
| TRAINING                             |
|  Units                 [ kg | lb ]   |
|  Default rest          90 s       >  |
|  Keep screen on in session   [on]    |
|  Sounds                      [on]    |
|  Vibration                   [on]    |
| GAME                                 |
|  Play it like a game         [on]    |
|  Game rules v2 - how it works     >  |
| LOOK                                 |
|  Theme     [ System | Light | Dark ] |
|  Reduce motion              [off]    |
| PRIVACY & DATA                       |
|  Account & sync   (o) synced      >  |
|  Photo lock & key backup          >  |
|  Backups & CSV export             >  |
| ABOUT                                |
|  Not medical advice - credits     >  |
+--------------------------------------+
```

### Settings — equipment for one place

```text
+--------------------------------------+
| <  Home equipment                    |
| WEIGHTS                              |
|  Dumbbells   5 10 15 20 25 lb    >   |
|  Plates      none               >    |
| OTHER (tap to toggle)                |
|  [x Bands] [x Mat] [x Treadmill]     |
|  [ Bench ] [ Pull-up bar ] [ Box ]   |
|  [ Kettlebell ] [ + Add item ]       |
| SPACE                                |
|  [ ] Small space   [ ] Quiet only    |
|--------------------------------------|
|  Used by Train now when you pick     |
|  Home. Changes apply next plan.      |
+--------------------------------------+
```

---

## Flow notes

**Onboarding (F5) — 5 short steps, then straight to Today.** NN/g: only ask what the app needs, keep it brief, show progress, say why, allow skip [[37]][u37].

| # | Step | Input | Skippable? |
|---|---|---|---|
| 0 | Sign in / use offline | invite + account, or local only | — |
| 1 | Goal program | 4 big cards | No (plans need it) |
| 2 | Units + where you train | kg/lb toggle; Home / Gym / Both → quick equipment chips | Equipment: yes (defaults) |
| 3 | Injuries & limits | "None" big button, or body-area chips → conservative stages | Yes ("None") |
| 4 | Training days | weekday chips | Yes (default 3) |
| 5 | Play it like a game? | two cards with a small preview of each *You* tab | No (one tap) |

- Ask for **camera** and **notification** permission in context (first photo, first reminder), not at onboarding (round 2).
- Theme is **not** an onboarding question: NN/g says visual customisation belongs in settings [[37]][u37].

**Train now** — max 3 taps to a plan, 1 tap with *Same as last time* (G1: ≤ 4 answers). NN/g: wizards suit infrequent tasks; repeat users need a faster path and pre-filled defaults [[38]][u38].

**"How was it?"** — chips, not sliders or text: one tap confirms (arc42 §8.15). Filter/choice chips are the Material pattern for "pick from a small set" [[40]][u40].

**Progress photos** — Body check-in → "Take photos?" → Front → Side → Back → compare with week 1. Monthly, gentle prompt (round 2).

**Equipment per place** — one page per place, like MacroFactor's gym profiles [[9]][u9]. Weights listed as the real dumbbells/plates so loads always exist (§8.7).

---

## Gym usability rules

Each rule is testable in the QA checklist (arc42 §8.11).

1. **A repeat set takes 1 tap** (LOG, pre-filled). A ±1 step change takes 2 taps. *(Q1 allows ≤ 3.)*
2. **No keyboard needed to log a set** that matches the plan or last set ± a few steps.
3. **Every in-session primary action sits in the bottom 40 % of the screen** (thumb reach; centre is most accurate [[33]][u33]; core actions buried top-right are a known one-hand problem [[34]][u34]). Check by overlaying a grid on screenshots.
4. **Tap targets ≥ 48 × 48 dp**, gaps ≥ 8 dp [[41]][u41]; in-session primary buttons ≥ 56 dp tall, full width. (Meets Apple 44 pt [[29]][u29] and WCAG 2.5.5's 44 px [[42]][u42].)
5. **Key numbers readable from a bench** (~1 m): weight/reps ≥ 22 sp bold, timers ≥ 48 sp [[27]][u27]. Test: phone on a bench, read it standing.
6. **One primary button per screen**, same place every time (the Finch critique praises a fixed button position [[18]][u18]).
7. **Every gesture has a button alternative** (swipe-delete ↔ sheet Delete) [[29]][u29].
8. **No long-press-only or double-tap-only actions**, no precise drags for logging (sweaty fingers) *(analysis)*.
9. **The screen stays on during a session** and is released at Finish / minimise.
10. **Rest timer survives app switch and screen-off**: time comes from a stored start timestamp, not a ticking counter *(analysis)*. End = sound + vibration (where available) + visual.
11. **Nothing pops up mid-set.** PRs show as a small inline badge; celebrations wait for the session end *(analysis, from [[24]][u24] [[26]][u26])*.
12. **Every destructive action can be undone** (QUndoStack, F3); Finish and Discard ask to confirm.
13. **Status is never colour alone:** icon + word + colour [[29]][u29].
14. **Contrast:** text ≥ 4.5 : 1, bars/icons/borders ≥ 3 : 1, in light **and** dark [[29]][u29] [[43]][u43]. Check with the token table below.
15. **Works fully offline** during a session (QG2).
16. **Same-as-last-time to first set ≤ 3 taps** from opening the app (Today → Same as last time → Start).
17. **Sounds and vibration are separate switches**; the app is fully usable with both off [[32]][u32].

---

## Design system v1

**Why a system now:** Duolingo had to redesign its tabs because headers, type and spacing drifted as features were added [[16]][u16]. One `Theme.qml` singleton from the first screen avoids that *(analysis)*.

### Type scale

Sizes in sp/pt (Qt uses device-independent pixels). Loosely based on the Material 3 scale *(general knowledge — check m3.material.io)*; Apple's default body is 17 pt, minimum 11 pt [[29]][u29].

| Token | Size / line | Weight | Use |
|---|---|---|---|
| `timer` | 56 / 64 | Bold, tabular digits | Rest and hold timers |
| `display` | 36 / 44 | Bold | Level-up number, big stats |
| `headline` | 24 / 32 | SemiBold | Screen titles, exercise name in session |
| `number` | 22 / 28 | Bold, tabular digits | Weight, reps, sets in rows |
| `title` | 18 / 24 | SemiBold | Card titles, section heads |
| `body` | 16 / 24 | Regular | Everything else |
| `label` | 14 / 20 | Medium | Chips, buttons, tab labels |
| `caption` | 12 / 16 | Regular | "why" lines, chart axes (never smaller) |

- Avoid thin weights (Apple: thin fonts need bigger sizes [[29]][u29]).
- Ship one font file with the app: WebAssembly has only 3 built-in fonts [[57]][u57]. Pick an open licence font *(general knowledge)*.
- Support at least 200 % text size where the OS offers it [[29]][u29]; test layouts at 1.3× and 2×.

### Spacing and size

| Token | dp | Use |
|---|---|---|
| `xs` | 4 | icon ↔ label |
| `s` | 8 | min gap between targets [[41]][u41] |
| `m` | 12 | inside chips/cards |
| `l` | 16 | screen side margin, card padding |
| `xl` | 24 | between sections |
| `xxl` | 32 | above the primary button |
| `tap` | 48 | min target (Material) [[41]][u41] |
| `tapPrimary` | 56 | in-session buttons, wizard cards |
| `radius` | 12 / full | cards / chips and buttons (Material 3 rounding [[50]][u50]) |

### Colour roles

Values checked with WCAG contrast maths and the dataviz skill's validator (lightness band, colour-blind separation). **Text always uses text roles; status colours are for icons, bars and chip fills** *(dataviz rule)*. Hues come from the Okabe-Ito colour-blind-safe set [[44]][u44].

| Role | Light | ratio on surface | Dark | ratio on surface |
|---|---|---|---|---|
| `background` | `#F7F8FA` | — | `#111316` | — |
| `surface` (cards) | `#FFFFFF` | — | `#1A1C1F` | — |
| `surfaceVariant` (chips, bars' track) | `#EDEFF3` | — | `#2A2D32` | — |
| `onSurface` (main text) | `#1B1D21` | 16.9 | `#E4E5E9` | 13.6 |
| `onSurfaceVariant` (secondary text) | `#4A4E57` | 8.3 | `#BFC3CC` | 9.7 |
| `outline` (borders) | `#767A84` | 4.3 | `#8B8F99` | 5.3 |
| `primary` (buttons, selection) | `#2F5BD0` | 6.0 | `#9DB6FF` | 8.6 |
| `onPrimary` (text on primary) | `#FFFFFF` | 6.0 on primary | `#0B1E57` | 7.9 on primary |
| `seriesPrimary` (chart line) | `#2F5BD0` | 6.0 | `#5B82E8` | 4.7 |
| `statusBelow` ▼ "low" | `#9A6B00` | 4.7 | `#B97F00` | 5.0 |
| `statusOn` ✓ "on target" | `#00875F` | 4.5 | `#1A9E73` | 5.0 |
| `statusAbove` ▲ "high" | `#0072B2` | 5.2 | `#3A8FD3` | 4.9 |
| `danger` (pain, errors) | `#C4470A` | 4.9 | `#DD5A35` | 4.5 |
| `xp` (levels, XP bars — game only) | `#A0457F` | 5.8 | `#B868A2` | 4.5 |
| `fading` (decay, + stripes) | `#6B6F78` | 5.0 | `#9A9EA7` | 6.4 |

Validator results:

- **Below / On / Above** together (all pairs): light and dark **pass** every check (worst colour-blind ΔE 8.3 light, 9.5 dark; ≥ 8 is the target).
- **danger + xp** and **primary + xp**: pass in both modes.
- **Below (amber) vs danger (vermillion): fail** — too close for colour-blind users. Rule: they never appear on the same screen; pain uses its own icon. **xp vs statusOn** also fails, so XP bars never sit next to volume status bars.
- Body map shading = **one hue light → dark** by level (sequential rule), plus stripes for fading (texture works for colour-blind users) *(dataviz rule)*.
- Theme default = **System** (Apple: people expect apps to follow the system setting [[30]][u30]); Light/Dark overrides only because Qt's system detection must be tested on WebAssembly *(unverified)*.

### Motion

| Rule | Value |
|---|---|
| Press feedback | 100 ms |
| Value changes (bars, numbers) | 200 ms `Behavior { NumberAnimation }` |
| Screen push / sheet | 250 ms |
| Celebration | ≤ 1.5 s, tap anywhere to close, never auto-closes content you must read (Apple: avoid time-boxed UI [[29]][u29]) |
| Never | Motion during a set; flashing; looping animations |
| **Reduce motion on** | Slides/zooms → fades; no particles or bounce; numbers jump, no count-up; level-up = static card [[29]][u29] |

- Qt 6.12 adds `QAccessibilityHints::motionPreference` (Android reads "Remove animations") [[68]][u68]; 6.11 has only `contrastPreference` [[67]][u67]. → For Qt 6.8–6.11 use the in-app **Reduce motion** switch.

### Haptics and sound

- Short, consistent, optional, always paired with a visual [[32]][u32].
- Map: set logged = light tick · rest over = strong pulse + beep · PR = success pattern *(analysis)*.

### Component list

| Component | Used on |
|---|---|
| `AppShell` (window, safe areas, tab bar / rail) | everywhere |
| `PrimaryButton` (56 dp, full width) | Today, plan, session, review |
| `ChoiceChip` / `FilterChip` | wizard, review, equipment |
| `ChoiceCard` (big tappable card with subtitle) | wizard steps 2–3, onboarding |
| `StepDots` + labelled Back | wizard, onboarding |
| `Stepper` (`[-] value [+]`, tap value = keypad) | session, cardio, check-in |
| `SetRow` (swipe + tap → sheet) | session, Log |
| `RestBar` (time, ±15 s, Skip, progress) | session |
| `RingTimer` | holds, stretch player |
| `ProgressBar` / `XpBar` | quest, skills, sets per muscle |
| `StatusBadge` (icon + word + colour) | sets per muscle, warnings |
| `Card` + `WarningCard` (dismissable) | Today, Progress |
| `BodyMap` (front/back, regions shaded by level) | You |
| `CalendarHeatmap` | You |
| `LineChart` (one series, one axis) | Progress |
| `BottomSheet` (Why?, edit set, pain, pause) | many |
| `Snackbar` with Undo | session, Log |
| `MiniSessionBar` | above tabs during a session |
| `CameraView` + `GhostOverlay` + `CompareSlider` | Photos |
| `SettingsRow` (label, value, chevron / switch / segmented) | Settings |
| `EmptyState` (why it's empty + one action) | every list (Apple: explain empty sections [[28]][u28]) |
| `RevealOverlay` | after "How was it?" |

---

## QML implementation notes

### Component per pattern

| Pattern | Qt Quick parts | Notes |
|---|---|---|
| App shell + tabs | `Window` (or `ApplicationWindow`), `TabBar` + `TabButton` in a footer, `StackLayout` for tab pages [[62]][u62] | One `StackView` **per tab** keeps each tab's place, as Apple expects [[28]][u28]. Avoid `SwipeView` for tabs: sideways swipes would fight swipe-to-delete rows *(analysis)* |
| Safe areas / notch | `SafeArea.margins` (Qt 6.9+) as padding [[51]][u51] | `ApplicationWindow` keeps `contentItem` inside safe areas, but **header/footer need manual handling** [[52]][u52] |
| Wide layout | Same model, `ListView` rail on the left when width > ~600 dp | Android: rail on large screens [[39]][u39]. Material **Dense** variant for desktop [[49]][u49] |
| Style | Qt Quick Controls **Material** style; `Material.theme: Material.System`; `accent`, `primary`, `roundedScale` [[49]][u49] | Material 3 look since Qt 6.5; dynamic colour not mentioned [[50]][u50]. Put the colour roles above in a `Theme.qml` singleton and bind `Material.*` to it |
| Wizard / drill-down | `StackView.push/pop` [[63]][u63] | Answers live in a C++ `TrainNowViewModel`, so Back keeps them |
| Bottom sheets | `Drawer { edge: Qt.BottomEdge }` or `Popup` | Close on Android back |
| Set rows | `ListView` + `SwipeDelegate` (`swipe.right` = Delete) [[69]][u69] | Use `ListView.delayRemove` for a smooth collapse [[69]][u69]; C++ `QAbstractListModel` as in v0.1 |
| Steppers / numbers | `RoundButton` pair + `TextField { inputMethodHints: Qt.ImhFormattedNumbersOnly }` [[71]][u71] | Apple: pair a stepper with a field when big jumps are likely [[31]][u31] |
| Chips | `Button { checkable: true }` in a `ButtonGroup` (single) or plain checkable buttons (multi), styled as chips | No chip control in Qt Quick Controls *(general knowledge)* |
| Progress / XP bars | `ProgressBar` or a `Rectangle` with `Behavior on width` | Duration 0 when reduce motion is on |
| Ring timer, body map | `Shape` + `ShapePath` + `PathArc`; one `Shape` with many paths [[70]][u70] | `preferredRendererType: Shape.CurveRenderer` (6.6+) for smooth arcs [[70]][u70]. Body map art: own or clearly licensed (round 2) |
| Charts | `Shape` + `PathPolyline` *(general knowledge)*, or keep the hand-drawn approach of ADR-008 *(analysis)* | One series, one axis per chart |
| Celebrations | Plain QML: `ScaleAnimator` + `OpacityAnimator` | **Qt Lottie is GPLv3 / commercial** [[60]][u60]; `VectorImage` Lottie is a tech preview in 6.10 [[61]][u61]. Check the repo licence before using either |
| Camera + ghost | `CaptureSession { camera: Camera {}; imageCapture: ImageCapture {}; videoOutput: … }`, an `Image` with `opacity: 0.35` stacked on `VideoOutput`, `captureToFile` [[64]][u64] [[65]][u65] | Pick the front/back camera via `MediaDevices` [[65]][u65]. Compare slider = two `Image`s, the top one clipped by a draggable `Item` |

### Platform pitfalls and fixes

| Platform | Pitfall | Fix |
|---|---|---|
| Android | Edge-to-edge broke for `ApplicationWindow` after Qt 6.9.2; plain `Window` worked [[53]][u53] | Test both in I4; fall back to `Window` + own footer |
| Android | Soft keyboard: in 6.10.1, tapping a field while the keyboard is open resized the window back to full height [[55]][u55]; covered fields in older Qt [[56]][u56] | Avoid typing in the session (steppers); put forms in a `Flickable` and scroll the focused field above the keyboard [[56]][u56] |
| Android | Keep screen on | JNI: add `FLAG_KEEP_SCREEN_ON` **on the Android main thread** via `QAndroidApplication::runOnAndroidMainThread`; clear it at Finish [[54]][u54] |
| Android | Haptics | No Qt 6 haptics API *(general knowledge)*; one developer could not get `performHapticFeedback` or `VibratorManager` working [[66]][u66] → spike it, treat as optional |
| Android | Back button | Qt 6.12 adds predictive back and backgrounds the app on back [[68]][u68]. Order: close sheet → pop stack → go to Today → leave |
| WASM (iPhone) | Keep screen on | `navigator.wakeLock.request('screen')` via Emscripten; HTTPS only; **re-request on `visibilitychange`** [[45]][u45]. Baseline in all browsers since 2024 [[46]][u46]; works in Home Screen web apps from iOS 18.4 [[47]][u47] |
| WASM (iPhone) | No vibration | Safari on iOS has no Vibration API [[48]][u48] → sound + visual only |
| WASM | Fonts | Only 3 embedded fonts; system fonts unavailable → bundle the font via resources / `FontLoader` [[57]][u57] |
| WASM | Camera | Qt Multimedia on WASM is a **technology preview** [[57]][u57]; camera needs HTTPS + permission [[58]][u58] → keep an "upload a photo" fallback *(analysis)* |
| WASM | Storage | Default file system is in memory and lost on reload [[57]][u57] → persistent storage + server copy (round 2) |
| WASM | Threads | Threaded Qt Quick render loop not supported; don't block the main thread (timers stall) [[57]][u57] → keep animations light |
| WASM | Accessibility | Off by default; only basic controls [[57]][u57] |
| WASM | HiDPI | Some controls (e.g. checkboxes) can show graphics artefacts [[57]][u57] → test custom chips instead |
| WASM | Virtual keyboard | Opened via a hidden HTML input; mobile support arrived after 6.3 [[59]][u59] → test numeric entry on the iPhone |
| All | Reduced motion | `motionPreference` only from 6.12 [[68]][u68] → in-app switch for 6.8–6.11 |

---

## Ranked recommendations

| # | Change | Why | Effort | Backlog |
|---|---|---|---|---|
| 1 | **4-tab layout: Today · Log · Progress · You**; *Train now* as the big button on Today; session full-screen with a mini bar | Every feature gets one home; 3–5 tabs [[39]][u39]; tabs are for navigation [[28]][u28] | M | F8 F9 F12 H1 |
| 2 | **1-tap repeat set**: big bottom LOG button, pre-filled, with ± steppers; no keyboard | Tap count is the #1 logger complaint (round 1); ~2 taps in Strong [[6]][u6] | M | F9 F3 |
| 3 | **"Same as last time"** quick start | Wizards need a fast path for repeat users + pre-filled defaults [[38]][u38]; G1 | S | F8 |
| 4 | **Rest timer from a stored timestamp + keep screen on + sound/visual alert** | Timers that reset or don't notify are disliked [[26]][u26]; wake lock needs per-platform code [[54]][u54] [[45]][u45] | M | F9 I4 |
| 5 | **`Theme.qml` tokens** (type, spacing, validated colour roles, motion) before the first screen | Avoids Duolingo-style drift [[16]][u16]; contrast + colour-blind safety built in [[43]][u43] | S | I4 F8 |
| 6 | **You tab swaps content with game mode**, never hides; Pause and unlocks get plain versions | Apple: don't hide tabs [[28]][u28]; ADR-021; Todoist's opt-out model [[23]][u23] | M | F12 F13 F5 |
| 7 | **Celebrations only after "How was it?"**: one screen, ≤ 1.5 s, tap to close, static with reduce motion | Users ask to skip streak screens [[24]][u24]; Apple motion guidance [[29]][u29] | S | F14 |
| 8 | **Today card budget (max 3, fixed priority)** | Gentler Streak's single, reordering home [[12]][u12]; Finch's decision fatigue [[18]][u18] | S | F14 F18 F22 |
| 9 | **Reusable "Why?" sheet** on every number | Vision success criterion; trust gap in coach apps (round 2) | S | F7 F12 |
| 10 | **Expand the I4 spike checklist**: safe areas, keyboard, wake lock, fonts, camera, haptics on both phone targets | Each is a known pitfall [[53]][u53] [[55]][u55] [[57]][u57] [[48]][u48] | S | I4 |
| 11 | **Photo capture with ghost slider + compare slider**, upload fallback on web, key-backup prompt on first photo | Pattern from photo apps [[25]][u25]; WASM camera is a preview [[57]][u57] | L | F21 I4 |
| 12 | **Two charts for weight and waist** (no dual axis) | Dataviz rule; each trend readable on its own | S | F18 |
| 13 | **Desktop: navigation rail + Material Dense** | Android rail guidance [[39]][u39]; Dense for desktop [[49]][u49] | S | H1 |
| 14 | **Avoid GPL-only modules for celebrations** (Qt Lottie) unless the licence fits | Qt Lottie is GPLv3/commercial [[60]][u60] | S | ADR-012 |

---

## Sources

**Apps**

1. [Hevy — Track workouts](https://www.hevyapp.com/features/track-workouts/) *(vendor)*
2. [Hevy — Workout rest timer](https://www.hevyapp.com/features/workout-rest-timer/) *(vendor)*
3. [ScreensDesign — Hevy screens](https://screensdesign.com/apps/hevy-workout-tracker-gym-log/?vs=212387)
4. [Hevy Help — How to log a workout](https://help.hevyapp.com/hc/en-us/articles/35361530647959-How-to-Log-a-Workout-in-the-Hevy-App-Step-by-Step-Guide) *(vendor)*
5. [Hevy — Muscle group workout chart](https://www.hevyapp.com/features/muscle-group-workout-chart/) *(vendor)*
6. [Lifting Apps — Strong review](https://lifting-apps.pages.dev/reviews/strong) *(independence unknown)*
7. [Strong — App Store page](https://apps.apple.com/us/app/strong-workout-tracker-gym-log/id464254577) *(vendor)*
8. [Fitbod — A better Workout tab](https://fitbod.me/blog/a-better-workout-tab/) *(vendor)*
9. [MacroFactor Workouts](https://macrofactor.com/workouts/) *(vendor)*
10. [ScreensDesign — MacroFactor Workouts](https://screensdesign.com/showcase/macrofactor-workouts-tracker)
11. [Apple Developer — Behind the Design: Gentler Streak](https://developer.apple.com/news/?id=3m0ht22s)
12. [BGR — Gentler Streak redesign](https://bgr.com/tech/gentler-streak-gets-a-major-redesign-focused-on-your-wellbeing/)
13. [BikeRadar — Strava's new layout and You tab](https://www.bikeradar.com/news/strava-beta-test-layout/)
14. [road.cc — Strava's weekly snapshot "makes me feel bad"](https://road.cc/content/tech-news/stravas-weekly-snapshot-makes-me-feel-bad-307315)
15. [Duolingo — New home screen design](https://blog.duolingo.com/new-duolingo-home-screen-design) *(vendor)*
16. [Duolingo — Core tabs redesign](https://blog.duolingo.com/core-tabs-redesign) *(vendor)*
17. [Duolingo — Streak milestone animation](https://blog.duolingo.com/streak-milestone-design-animation) *(vendor)*
18. [Pratt IxD — Design critique: Finch](https://ixd.prattsi.org/2026/02/design-critique-finch-self-care-pet-ios-app/)
19. [Habitica Wiki — Redesign fact sheet](https://habitica.fandom.com/wiki/Habitica_Redesign_Fact_Sheet)
20. [ScreensDesign — Bend](https://screensdesign.com/showcase/stretching-flexibility-bend)
21. [MacStories — watchOS 26 review](https://www.macstories.net/stories/watchos-26-the-macstories-review/3/)
22. [9to5Mac — iOS 26 Fitness app upgrades](https://9to5mac.com/2025/12/30/ios-26s-fitness-app-has-three-upgrades-ready-for-new-years-goals/)
23. [Todoist — Introduction to Karma](https://todoist.com/help/articles/introduction-to-karma-OgWkWy) *(vendor)*
24. [Zwift Forums — Disable streak screen](https://forums.zwift.com/t/disable-streak-screen/620084)
25. [EZ Progress — App Store page](https://apps.apple.com/us/app/-/id6748616451) *(vendor)*
26. [Setgraph — Best workout tracker app, Reddit](https://setgraph.app/ai-blog/best-workout-tracker-app-reddit) *(vendor)*

**Usability, accessibility, design**

27. [Apple HIG — Workouts](https://developer.apple.com/design/human-interface-guidelines/workouts)
28. [Apple HIG — Tab bars](https://developer.apple.com/design/human-interface-guidelines/tab-bars)
29. [Apple HIG — Accessibility](https://developer.apple.com/design/human-interface-guidelines/accessibility)
30. [Apple HIG — Dark Mode](https://developer.apple.com/design/human-interface-guidelines/dark-mode)
31. [Apple HIG — Steppers](https://developer.apple.com/design/human-interface-guidelines/steppers)
32. [Apple HIG — Playing haptics](https://developer.apple.com/design/human-interface-guidelines/playing-haptics)
33. [UXmatters — Hoober: How do users really hold mobile devices?](https://uxmatters.com/mt/archives/2013/02/how-do-users-really-hold-mobile-devices.php)
34. [Smashing Magazine — Designing mobile apps for one-hand usage](https://smashingmagazine.com/2020/02/design-mobile-apps-one-hand-usage/)
35. [Mohajeri & Cheng 2022 — "Inconsistent Performance": concerns of smart mHealth app users (arXiv)](https://arxiv.org/abs/2208.10705)
36. [NN/g — Progressive disclosure](https://www.nngroup.com/articles/progressive-disclosure/)
37. [NN/g — Mobile app onboarding](https://www.nngroup.com/articles/mobile-app-onboarding/)
38. [NN/g — Wizards](https://www.nngroup.com/articles/wizards/)
39. [Android Developers — Layouts and navigation patterns](https://developer.android.com/design/ui/mobile/guides/layout-and-content/layout-and-nav-patterns)
40. [Android Developers — Chip](https://developer.android.com/develop/ui/compose/components/chip)
41. [Google Accessibility — Touch target size](https://support.google.com/accessibility/android/answer/7101858?hl=en)
42. [W3C — Understanding SC 2.5.8 Target Size (Minimum)](https://www.w3.org/WAI/WCAG22/Understanding/target-size-minimum)
43. [W3C — Understanding SC 1.4.11 Non-text Contrast](https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast)
44. [ConceptViz — Okabe-Ito palette hex codes](https://conceptviz.app/blog/okabe-ito-palette-hex-codes-complete-reference)

**Web platform**

45. [Chrome for Developers — Screen Wake Lock API](https://developer.chrome.com/docs/capabilities/web-apis/wake-lock)
46. [web.dev — Screen Wake Lock supported in all browsers](https://web.dev/blog/screen-wake-lock-supported-in-all-browsers)
47. [WebKit Bugzilla 254545 — Wake Lock in Home Screen web apps](https://bugs.webkit.org/show_bug.cgi?id=254545)
48. [Can I use — Vibration API](https://caniuse.com/vibration)

**Qt**

49. [Qt docs — Material style](https://doc.qt.io/qt-6/qtquickcontrols-material.html)
50. [Qt blog — Material 3 changes in Qt Quick Controls](https://qt.io/blog/material-3-changes-in-qt-quick-controls)
51. [Qt docs — SafeArea QML type](https://doc.qt.io/qt-6/qml-qtquick-safearea.html)
52. [Qt blog — Expanded client areas and safe areas in Qt 6.9](https://www.qt.io/blog/expanded-client-areas-and-safe-areas-in-qt-6.9)
53. [Qt Interest list — Android edge-to-edge broken in Qt 6.9.2](https://lists.qt-project.org/pipermail/interest/2025-August/040269.html)
54. [Qt Forum — Disable screensaver on Qt 6.9 Android app](https://forum.qt.io/topic/162740/how-to-disable-screensaver-on-qt6.9.1-android-app/16)
55. [Qt Interest list — QTBUG-139787 (Android on-screen keyboard)](https://lists.qt-project.org/pipermail/interest/2025-December/040398.html)
56. [Qt Forum — Android keyboard covers the input field](https://forum.qt.io/topic/160010/android-keyboard-covers-the-inputfield)
57. [Qt docs — Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html)
58. [Qt docs — Qt Multimedia on WebAssembly](https://doc.qt.io/qt-6/qtmultimedia-wasm.html)
59. [Qt blog — Qt for WebAssembly on mobile devices](https://www.qt.io/blog/qt-for-webassembly-on-mobile-devices)
60. [Qt docs — Qt Lottie Animation](https://doc.qt.io/qt-6/qtlottieanimation-index.html)
61. [Qt blog — Animated vector graphics in Qt 6.10](https://www.qt.io/blog/animated-vector-graphics-in-qt-6.10)
62. [Qt docs — Navigation controls](https://doc.qt.io/qt-6/qtquickcontrols-navigation.html)
63. [Qt 6 Book — Common patterns (StackView, SwipeView)](https://www.qt.io/product/qt6/qml-book/ch06-controls-common-patterns)
64. [Qt docs — QML Camera example](https://doc.qt.io/qt-6/qtmultimedia-declarative-camera-example.html)
65. [Qt docs — Camera overview](https://doc.qt.io/qt-6/cameraoverview.html)
66. [Qt Forum — Haptics on Android](https://forum.qt.io/topic/154335/haptics-on-android)
67. [Qt docs — QAccessibilityHints (6.11)](https://doc.qt.io/qt-6.11/qaccessibilityhints.html)
68. [Qt docs — What's new in Qt 6.12](https://doc.qt.io/qt-6/whatsnew612.html)
69. [Qt docs — SwipeDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-swipedelegate.html)
70. [Qt docs — Shape QML type](https://doc.qt.io/qt-6/qml-qtquick-shapes-shape.html)
71. [Qt docs — TextInput (inputMethodHints)](https://doc.qt.io/qt-6/qml-qtquick-textinput.html)

[u1]: https://www.hevyapp.com/features/track-workouts/
[u2]: https://www.hevyapp.com/features/workout-rest-timer/
[u3]: https://screensdesign.com/apps/hevy-workout-tracker-gym-log/?vs=212387
[u4]: https://help.hevyapp.com/hc/en-us/articles/35361530647959-How-to-Log-a-Workout-in-the-Hevy-App-Step-by-Step-Guide
[u5]: https://www.hevyapp.com/features/muscle-group-workout-chart/
[u6]: https://lifting-apps.pages.dev/reviews/strong
[u7]: https://apps.apple.com/us/app/strong-workout-tracker-gym-log/id464254577
[u8]: https://fitbod.me/blog/a-better-workout-tab/
[u9]: https://macrofactor.com/workouts/
[u10]: https://screensdesign.com/showcase/macrofactor-workouts-tracker
[u11]: https://developer.apple.com/news/?id=3m0ht22s
[u12]: https://bgr.com/tech/gentler-streak-gets-a-major-redesign-focused-on-your-wellbeing/
[u13]: https://www.bikeradar.com/news/strava-beta-test-layout/
[u14]: https://road.cc/content/tech-news/stravas-weekly-snapshot-makes-me-feel-bad-307315
[u15]: https://blog.duolingo.com/new-duolingo-home-screen-design
[u16]: https://blog.duolingo.com/core-tabs-redesign
[u17]: https://blog.duolingo.com/streak-milestone-design-animation
[u18]: https://ixd.prattsi.org/2026/02/design-critique-finch-self-care-pet-ios-app/
[u19]: https://habitica.fandom.com/wiki/Habitica_Redesign_Fact_Sheet
[u20]: https://screensdesign.com/showcase/stretching-flexibility-bend
[u21]: https://www.macstories.net/stories/watchos-26-the-macstories-review/3/
[u22]: https://9to5mac.com/2025/12/30/ios-26s-fitness-app-has-three-upgrades-ready-for-new-years-goals/
[u23]: https://todoist.com/help/articles/introduction-to-karma-OgWkWy
[u24]: https://forums.zwift.com/t/disable-streak-screen/620084
[u25]: https://apps.apple.com/us/app/-/id6748616451
[u26]: https://setgraph.app/ai-blog/best-workout-tracker-app-reddit
[u27]: https://developer.apple.com/design/human-interface-guidelines/workouts
[u28]: https://developer.apple.com/design/human-interface-guidelines/tab-bars
[u29]: https://developer.apple.com/design/human-interface-guidelines/accessibility
[u30]: https://developer.apple.com/design/human-interface-guidelines/dark-mode
[u31]: https://developer.apple.com/design/human-interface-guidelines/steppers
[u32]: https://developer.apple.com/design/human-interface-guidelines/playing-haptics
[u33]: https://uxmatters.com/mt/archives/2013/02/how-do-users-really-hold-mobile-devices.php
[u34]: https://smashingmagazine.com/2020/02/design-mobile-apps-one-hand-usage/
[u35]: https://arxiv.org/abs/2208.10705
[u36]: https://www.nngroup.com/articles/progressive-disclosure/
[u37]: https://www.nngroup.com/articles/mobile-app-onboarding/
[u38]: https://www.nngroup.com/articles/wizards/
[u39]: https://developer.android.com/design/ui/mobile/guides/layout-and-content/layout-and-nav-patterns
[u40]: https://developer.android.com/develop/ui/compose/components/chip
[u41]: https://support.google.com/accessibility/android/answer/7101858?hl=en
[u42]: https://www.w3.org/WAI/WCAG22/Understanding/target-size-minimum
[u43]: https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast
[u44]: https://conceptviz.app/blog/okabe-ito-palette-hex-codes-complete-reference
[u45]: https://developer.chrome.com/docs/capabilities/web-apis/wake-lock
[u46]: https://web.dev/blog/screen-wake-lock-supported-in-all-browsers
[u47]: https://bugs.webkit.org/show_bug.cgi?id=254545
[u48]: https://caniuse.com/vibration
[u49]: https://doc.qt.io/qt-6/qtquickcontrols-material.html
[u50]: https://qt.io/blog/material-3-changes-in-qt-quick-controls
[u51]: https://doc.qt.io/qt-6/qml-qtquick-safearea.html
[u52]: https://www.qt.io/blog/expanded-client-areas-and-safe-areas-in-qt-6.9
[u53]: https://lists.qt-project.org/pipermail/interest/2025-August/040269.html
[u54]: https://forum.qt.io/topic/162740/how-to-disable-screensaver-on-qt6.9.1-android-app/16
[u55]: https://lists.qt-project.org/pipermail/interest/2025-December/040398.html
[u56]: https://forum.qt.io/topic/160010/android-keyboard-covers-the-inputfield
[u57]: https://doc.qt.io/qt-6/wasm.html
[u58]: https://doc.qt.io/qt-6/qtmultimedia-wasm.html
[u59]: https://www.qt.io/blog/qt-for-webassembly-on-mobile-devices
[u60]: https://doc.qt.io/qt-6/qtlottieanimation-index.html
[u61]: https://www.qt.io/blog/animated-vector-graphics-in-qt-6.10
[u62]: https://doc.qt.io/qt-6/qtquickcontrols-navigation.html
[u63]: https://www.qt.io/product/qt6/qml-book/ch06-controls-common-patterns
[u64]: https://doc.qt.io/qt-6/qtmultimedia-declarative-camera-example.html
[u65]: https://doc.qt.io/qt-6/cameraoverview.html
[u66]: https://forum.qt.io/topic/154335/haptics-on-android
[u67]: https://doc.qt.io/qt-6.11/qaccessibilityhints.html
[u68]: https://doc.qt.io/qt-6/whatsnew612.html
[u69]: https://doc.qt.io/qt-6/qml-qtquick-controls-swipedelegate.html
[u70]: https://doc.qt.io/qt-6/qml-qtquick-shapes-shape.html
[u71]: https://doc.qt.io/qt-6/qml-qtquick-textinput.html
