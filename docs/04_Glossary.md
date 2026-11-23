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
| **Why? sheet** | Bottom sheet that explains any number or plan choice (formula, rule, inputs). Reused on every screen. | UI | — | — |
| **Mini bar** | Strip above the tab bar showing a running session; tap to return to it. | UI | — | — |
| **Lighter weeks** | The game-off name for a Pause. | UI | — | pause |
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
| **Session review** | The end-of-session "How was it?" answers (overall, energy, joints, note). | `SessionReview` | — | check-in |
| **Pre-picked answer** | A review answer the app suggests from session data; one tap confirms it. | flag | joints never pre-picked as pain | autofill |
| **Progress photo** | Front, side or back photo taken at a check-in; encrypted on the device before sync. | `ProgressPhoto` | — | — |
| **Envelope encryption** | One random key encrypts the data; that key is itself locked ("enveloped") by other keys. Changing a password re-locks only the small key. | design | — | key wrapping |
| **Photo key** | Random key that encrypts all of a user's photos and previews. | 256-bit | never stored unlocked | data key |
| **Lock key** | Key derived from the password that locks/unlocks envelope A. Never leaves the device. | 256-bit | — | wrapping key, KEK |
| **Login key** | Key derived from the password and sent at login instead of the password. The server stores only its hash. | 256-bit | — | auth key |
| **Recovery code** | Random code shown once when photo sync is turned on; opens envelope B if the password is forgotten. | text, grouped | keep it in a password manager or on paper | backup code |
| **Envelope A / B** | The photo key locked with the lock key (A) or the recovery code (B); both stored on the server. | blob | — | — |
| **Timed set** | A set measured in seconds instead of reps (plank, wall sit, balance hold). | `TimedSet` | 1–600 s | hold |
| **Streak** | Consecutive weeks with the weekly quest completed; rest days count; paused weeks and rest-token weeks keep it. | derived | — | — |
| **Body map** | Picture of the body where each muscle group is coloured by its skill level. | UI | — | — |
| **Deload** | A planned lighter week after a stall, to recover and resume progress. | planner hint | — | — |
| **Revision** | Server counter for a user's log; bumps on every successful push. | integer | ≥ 0 | — |
| **Tombstone** | A record marked `deleted` instead of removed, so deletes sync. | flag | — | — |
| **Merge** | Combining two copies of a log by record id (union; newer edit wins). | `sync::merge` | — | — |
| **Invite code** | Secret needed to register on the server. | text | — | — |
| **Key pose** *(ADR-024, rejected — prototype only)* | One position of an exercise (e.g. top and bottom of a squat) stored as hip, spine angle and hand/foot targets. The picture animates between key poses. | catalog data | 2–3 per exercise | keyframe |
| **Inverse kinematics (IK)** *(prototype only)* | Working out a middle joint (knee, elbow) from where a limb starts and where its end must be. | `pose::solveLimb` | target clamped to limb reach | IK |
| **How-to picture** | Image of the IronLog character doing an exercise: start pose → end pose, side view. One per exercise. | `ironlog-<id>-how.jpg` | checked by eye for form errors | how picture |
| **Muscle diagram** | Front + back body diagram for an exercise: main muscles red, helper muscles orange, the rest grey. Always shown with the muscle names as text. | `ironlog-<id>-muscles.jpg` | colours from one grey base diagram | muscle map |
| **Device token** | Long-lived credential that lets a remembered device get a session without a password. Stored hashed. | text | — | — |

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 7 §7.8–7.9.
- Epley formula and set/rep vocabulary: general strength-training knowledge.
