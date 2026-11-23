# 04 — Glossary

> **What this is**: Agreed meanings for this project's terms. Doubles as a
> data dictionary (formats, validation rules). (Larman, Ch. 7 §7.8–7.9.)

---

## Terms

| Term | Definition / Information | Format | Validation Rules | Aliases |
|------|--------------------------|--------|------------------|---------|
| **Set** | One continuous effort of an exercise: a weight lifted for a number of reps. | `SetEntry` | 0 ≤ kg ≤ 1000; 1 ≤ reps ≤ 100 | working set |
| **Rep** | One full repetition of the movement. | integer | ≥ 1 | repetition |
| **Session** | All sets done on one calendar day (v0.1). From schema v2, activities are timed records and a day can hold several. | `Workout` | max one per day (v0.1 only) | workout, training day |
| **Exercise** | A named movement, tied to one primary muscle group. | `Exercise` | name non-empty, unique (case-insensitive) | lift, movement |
| **How-to picture** | Image of the IronLog character doing an exercise: start pose → end pose, side view. One per exercise. | `ironlog-<id>-how.jpg` | checked by eye for form errors | how picture |
| **Muscle diagram** | Front + back body diagram for an exercise: main muscles dark blue, helper muscles light blue, the rest grey. Not the game's body map. Always shown with the muscle names as text. | `ironlog-<id>-muscles.jpg` | colours from one grey base diagram | muscle map |
| **Primary muscle group** | The muscle an exercise counts toward for weekly volume. | `MuscleGroup` enum | one of 10 values | target muscle |
| **Volume (set)** | Weight × reps for one set. | kg, double | — | tonnage |
| **Volume (session)** | Sum of set volumes in a session. | kg, double | — | total load |
| **Weekly sets** | Number of sets per muscle group, Monday–Sunday. | integer | — | weekly volume |
| **Volume target** | The weekly-sets range treated as "on target". | `{low, high}` | default 10–20 | — |
| **1RM** | One-rep max: the heaviest weight you can lift once. | kg | — | max |
| **e1RM** | *Estimated* 1RM, from a multi-rep set (Epley formula). | kg, double | — | estimated max |
| **Epley formula** | e1RM = weight × (1 + reps / 30). For 1 rep, e1RM = weight. | — | — | — |
| **PR** | Personal record: a session's top set for an exercise whose e1RM beats every earlier session. First-ever session is not a PR. | flag | strictly greater | personal best, PB |
| **Progressive overload** | Adding weight or reps over time so the body keeps adapting. | — | — | — |
| **Push / Pull / Legs** | A common split: pressing muscles, pulling muscles, lower body on separate days. | — | — | PPL |
| **Week** | Monday 00:00 to Sunday 23:59 (ISO 8601). | `Date::mondayOfWeek()` | — | training week |
| **Log** | The whole training history. | `TrainingLog` | sorted by date, unique dates | history |
| **Schema version** | Number in the JSON file telling which layout it uses. | integer | 1 ≤ v ≤ current | — |
| **RIR** | Reps in reserve: how many more reps you could have done. 0 = failure. | integer | 0–5 | — |
| **RPE** | Rate of perceived exertion, 1–10. RPE ≈ 10 − RIR. | number | 1–10 | — |
| **Set type** | Working, warm-up, drop or failure set. Only working sets count for PRs and weekly sets. | enum | — | — |
| **Zone 2** | Easy, steady cardio: breathing harder but able to talk in full sentences. | — | — | conversational pace |
| **Double progression** | Raise reps within a range first; when every set hits the top, raise the weight. | — | — | — |
| **Goal program** | A data file describing weekly sessions, skill weights and per-muscle targets for a goal (e.g. Badminton singles). | JSON | — | program |
| **Profile** | A lifter's goal program, units, equipment per place, constraints and availability. | record | — | — |
| **Equipment set** | The equipment available at a place (home, gym, limited). | list | — | kit |
| **Situation** | How today is: Normal, Cramming, or Sore / tired. Changes the plan. | enum | — | — |
| **Train now** | The flow that asks time + situation + place/equipment, then builds a session plan. | UC-11 | — | — |
| **Session plan** | The planner's output: warm-up, main block, stretch, each with a reason. | `SessionPlan` | — | plan |
| **Guided session** | Following a session plan step by step while logging. | UC-12 | — | — |
| **Constraint stage** | How far a lifter may progress on a constrained area (e.g. knee stage 1–3). Caps exercise choice. | integer | — | — |
| **Progression gate** | A condition to unlock a harder exercise (e.g. 3 pain-free weeks of step-ups → reverse lunge). | rule | — | unlock |
| **Pain check** *(replaced by pain monitoring, ADR-026)* | Reporting sharp pain, pinching or next-day swelling on an exercise; drops it one stage for 2 weeks. If it happens again or doesn't settle, the app suggests stopping that exercise and seeing a health professional. General guidance, not medical advice. | UC-18 | — | — |
| **Body limitation** | A joint that needs care (knee, shoulder, lower back, ankle, hip, wrist, neck), with a side and a stage. Self-reported, never a diagnosis. | `Limitation` | 7 joints | injury, constraint |
| **Stage ladder** | Per joint: what each stage allows and what unlocks next. A data file. | data | stage 1 … 4 | — |
| **Joint stress** | How hard an exercise loads a joint: low, medium or high. | per exercise, per joint | — | knee stress |
| **Pain monitoring** | Rating pain 0–10 during an exercise and the next morning; decides hold, keep or drop a stage. | `PainReport` | 0–10 | pain scale |
| **Red flag** | A sign to stop and see a professional: swelling, heat, locking, giving way, can't take weight. | list | — | — |
| **Flare-up** | Pain reported any time, outside a session, for a joint (0–10, side, red flags). The app registers it and adjusts that joint's stage. | `PainReport` (kind flare-up) | 0–10 | — |
| **Pain log** | All pain reports for a joint over time: session, flare-up, next morning, weekly rating. | derived list | — | pain history |
| **Pain pin** | A number on the body map at a joint with recent pain (last 7 days, ≥ 3/10); "!" for a red flag. | UI | — | — |
| **Gate** | Criteria plus a minimum time that unlock an exercise, stage or phase. | data | — | unlock rule |
| **Trigger** | A condition the app watches; when it's met, an offer appears ("ready for…"). Never automatic. | data | — | — |
| **Goal test** | A measurement every 4 weeks that shows progress toward a goal (reach, balance, time before gassed). | `GoalTest` | — | re-test, benchmark |
| **Return ramp** | Lower volume after a long break, rising each week. | rule P9 | — | — |
| **Prehab block** | A 10–12 min set of exercises that protects a joint or prepares for a sport, with a weekly target. | data | — | knee care, stability |
| **Sport profile** | Data for a sport: body areas it loads, its warm-up, its weak spots. | data | — | — |
| **Morning stretch** | The daily guided routine: 12 static stretches, ~12 min, hold timer per side. | `StretchRoutine` | holds 30–60 s | morning routine |
| **Add-on stretch** | One of 12 extra stretches done after a session or in the evening, picked for areas not stretched yet this week. | stretch id | — | cool-down stretch |
| **Reach test** | Weekly check after the towel stretch: how far apart the hands are behind the back, per side. | `MobilityEntry.reach` | cm per side, 0 = touching | towel test |
| **Why? sheet** | Bottom sheet that explains any number or plan choice (formula, rule, inputs). Reused on every screen. | UI | — | — |
| **Mini bar** | Strip above the tab bar showing a running session; tap to return to it. | UI | — | — |
| **Lighter weeks** | The label of the Pause row on *You* when the game is off. Same thing as Pause. | UI | — | pause |
| **Game mode** | Per-user checkbox. Off: game hidden, decay paused, plans and safety rules unchanged. | `GameModeChange` | — | gamification |
| **Skill** | One of 8 character stats: Arms, Shoulders, Chest, Back, Core, Legs, Stamina, Mobility. | enum | — | — |
| **XP** | Experience points earned per set, cardio minute or stretch minute. | integer | ≥ 0 | — |
| **Level** | A skill's rank, from its XP (level L → L+1 costs 50 × L XP). | integer | ≥ 1 | — |
| **Total level** | Sum of the 8 skill levels. | integer | 8+ | — |
| **Decay** | Losing XP in a skill after 14 days without training it (−0.5 %/day), warned 3 days ahead. | rule | never below floor | fading |
| **Floor** | Decay never drops a skill below 50 % of its peak XP. | rule | — | — |
| **Comeback bonus** | XP counts double while a skill is below its peak. | rule | — | muscle memory |
| **Pause** | A period that stops decay and changes the weekly quest, with a reason: exams (≤ 21 days), sick (≤ 14), injured (≤ 42), travel (≤ 21). | `PausePeriod` | capped per reason | exam mode (v1 name) |
| **Weekly quest** | The week's planned sessions; completing it earns a rest token and shows your reward. | derived | — | quest |
| **Rest token** | Earned per completed quest (bank max 2); keeps a missed week in the streak. Replaced the v1 cheat-meal token. | integer | 0–2 | streak freeze |
| **Your reward** | Optional text the user writes as their own reward for completing a quest. The app never suggests food. | `UserReward` | free text | — |
| **Derived state** | Values recomputed from the log (XP, levels, quests, earned rest tokens, streak), never stored. | — | — | — |
| **Body check-in** | Weekly body weight + waist entry (+ progress photos); the "lean" trend. | `BodyCheckIn` | — | weigh-in |
| **Session review** | The end-of-session "How was it?" answers (overall, energy, joints, note). | `SessionReview` | — | "How was it?" (not "check-in": that word is for the body check-in) |
| **Pre-picked answer** | A review answer the app suggests from session data; one tap confirms it. | flag | joints never pre-picked as pain | autofill |
| **Progress photo** | Front, side or back photo taken at a check-in; encrypted on the device before sync. | `ProgressPhoto` | — | — |
| **Envelope encryption** | One random key encrypts the data; that key is itself locked ("enveloped") by other keys. Changing a password re-locks only the small key. | design | — | key wrapping |
| **Photo key** | Random key that encrypts all of a user's photos and previews. | 256-bit | never stored unlocked | data key |
| **Lock key** | Key derived from the password that locks/unlocks envelope A. Never leaves the device. | 256-bit | — | wrapping key, KEK |
| **Login key** | Key derived from the password and sent at login instead of the password. The server stores only its hash. | 256-bit | — | auth key |
| **Device token** | Long-lived credential that lets a remembered device get a session without a password. Stored hashed. | text | — | — |
| **Recovery code** | Random code shown once when photo sync is turned on; opens envelope B if the password is forgotten. | text, grouped | keep it in a password manager or on paper | backup code |
| **Envelope A / B** | The photo key locked with the lock key (A) or the recovery code (B); both stored on the server. | blob | — | — |
| **Timed set** | A set measured in seconds instead of reps (plank, wall sit, balance hold). | `TimedSet` | 1–600 s | hold |
| **Streak** | Consecutive weeks with the weekly quest completed; rest days count; paused weeks and rest-token weeks keep it. | derived | — | — |
| **Body map** | Game picture of the body: each skill's area is shaded by its level (one purple hue, darker = higher; stripes = fading). Not the exercise muscle diagram. | UI | — | — |
| **Skill detail** | What a body part got: level, sets per muscle vs target, strength trend, endurance, stretching, limits, pain. Opened by tapping the body map. | derived | — | body part detail |
| **Deload** | A planned easy recovery week after a stall, then progress resumes. Not a Pause. | planner hint | — | — |
| **Revision** | Server counter for a user's log; bumps on every successful push. | integer | ≥ 0 | — |
| **Tombstone** | A record marked `deleted` instead of removed, so deletes sync. | flag | — | — |
| **Merge** | Combining two copies of a log by record id (union; newer edit wins). | `sync::merge` | — | — |
| **Invite code** | Secret needed to register on the server. | text | — | — |
| **Key pose** *(ADR-024, rejected — removed from the prototype 2026-11-23)* | One position of an exercise (e.g. top and bottom of a squat) stored as hip, spine angle and hand/foot targets. The picture animates between key poses. | was `MEDIA` in the prototype (git history) | 2–3 per exercise | keyframe |
| **Inverse kinematics (IK)** *(ADR-024, rejected — removed 2026-11-23)* | Working out a middle joint (knee, elbow) from where a limb starts and where its end must be. | was `ik()` in the prototype (git history) | target clamped to limb reach | IK |

---

## Tech words

Short meanings for words the other docs use without explaining.

| Word | Meaning |
|---|---|
| 1RM / e1RM | Max weight for one rep / estimated 1RM, from a set (Epley) |
| RIR | Reps in reserve: reps you had left |
| XP | Experience points (game score) |
| MMO | Online role-playing game (IronLog borrows the levelling, not the multiplayer) |
| QML | Qt's language for app screens |
| View model | C++ object holding a screen's data |
| WASM / WebAssembly | Runs C++ in a browser (the **web app**) |
| APK | Android app install file |
| CI / CD | Automatic build + test / automatic release |
| Release button | The *Run workflow* button on GitHub's Release workflow page |
| Smoke test | Quick "does it start?" check |
| LTS | Long-term support version |
| Spike | Small throwaway test build |
| VPS | Rented server online (Hetzner) |
| Caddy | Web server; adds HTTPS |
| systemd | Linux service manager |
| Loopback / 127.0.0.1 | Reachable only inside the server |
| 409 | Server reply meaning "conflict" |
| TickTimer | The owner's earlier Qt timer app, with its own sync server on the same VPS |
| UP | Unified Process (Larman's method) |
| Inception | First planning phase in UP |
| arc42 | 12-part architecture doc template |
| ADR | Architecture Decision Record: one decision, one file |
| GRASP | Larman's rules for which class does what |
| Pure Fabrication | Helper class that isn't a domain idea |
| Property test | Test over many random inputs |
| Argon2id | Slow password-hashing method |
| KDF | Makes sub-keys from one key |
| Salt | Random extra input per user |
| Zone 2 | Easy cardio; you can still talk |
| Static stretch | Hold one position 15–60 s |
| PAR-Q+ | Free health check before exercise |
| Talk test | Can talk, can't sing = easy cardio |
| Dynamic warm-up | Moving stretches, getting faster |
| Compound / isolation | Multi-joint / one-joint exercise |
| RAMP | Warm-up: raise, activate, mobilise, potentiate |
| Hypertrophy | Muscle growth |
| Must | Top priority (MoSCoW) |
| Bottom sheet | Panel that slides up from the bottom |
| Left rail | Side tab bar on wide screens |
| Headless | Runs without a screen |
| CORS | Browser rule for cross-site requests |
| Provenance | Where a file came from |

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 7 §7.8–7.9.
- Epley formula and set/rep vocabulary: general strength-training knowledge.
