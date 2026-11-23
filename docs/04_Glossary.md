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
| **Pain check** | Reporting sharp pain, pinching or next-day swelling on an exercise; drops it one stage for 2 weeks. | UC-18 | — | — |
| **Game mode** | Per-user checkbox. Off: game hidden, decay paused, plans and safety rules unchanged. | `GameModeChange` | — | gamification |
| **Skill** | One of 8 character stats: Arms, Shoulders, Chest, Back, Core, Legs, Stamina, Mobility. | enum | — | — |
| **XP** | Experience points earned per set, cardio minute or stretch minute. | integer | ≥ 0 | — |
| **Level** | A skill's rank, from its XP (level L → L+1 costs 50 × L XP). | integer | ≥ 1 | — |
| **Total level** | Sum of the 8 skill levels. | integer | 8+ | — |
| **Decay** | Losing XP in a skill after 10 days without training it (−1 %/day). | rule | never below floor | — |
| **Muscle-memory floor** | Decay never drops a skill below 50 % of its peak XP. | rule | — | — |
| **Muscle memory** | XP counts double while a skill is below its peak. | rule | — | comeback bonus |
| **Exam mode** | A period (≤ 21 days) that pauses decay and lowers the weekly quest. | `ExamPeriod` | ≤ 21 days | — |
| **Weekly quest** | The week's planned sessions; completing it earns a token. | derived | — | quest |
| **Cheat-meal token** | Reward for a completed weekly quest. Bank at most 2. | integer | 0–2 | token |
| **Derived state** | Values recomputed from the log (XP, levels, quests, earned tokens), never stored. | — | — | — |
| **Body check-in** | Weekly body weight + waist entry (+ progress photos); the "lean" trend. | `BodyCheckIn` | — | weigh-in |
| **Session review** | The end-of-session "How was it?" answers (overall, energy, joints, note). | `SessionReview` | — | check-in |
| **Pre-picked answer** | A review answer the app suggests from session data; one tap confirms it. | flag | joints never pre-picked as pain | autofill |
| **Progress photo** | Front, side or back photo taken at a check-in; encrypted on the device before sync. | `ProgressPhoto` | — | — |
| **Timed set** | A set measured in seconds instead of reps (plank, wall sit, balance hold). | `TimedSet` | 1–600 s | hold |
| **Streak** | Consecutive weeks with the weekly quest completed; exam weeks don't break it. | derived | — | — |
| **Body map** | Picture of the body where each muscle group is coloured by its skill level. | UI | — | — |
| **Deload** | A planned lighter week after a stall, to recover and resume progress. | planner hint | — | — |
| **Revision** | Server counter for a user's log; bumps on every successful push. | integer | ≥ 0 | — |
| **Tombstone** | A record marked `deleted` instead of removed, so deletes sync. | flag | — | — |
| **Merge** | Combining two copies of a log by record id (union; newer edit wins). | `sync::merge` | — | — |
| **Invite code** | Secret needed to register on the server. | text | — | — |
| **Device token** | Long-lived credential that lets a remembered device get a session without a password. Stored hashed. | text | — | — |

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 7 §7.8–7.9.
- Epley formula and set/rep vocabulary: general strength-training knowledge.
