# Doc review — 2026-11-23

> **What this is**: the result of three review agents run on the whole doc
> set, and what happened to each finding. **Read it** before running a new
> review: what's ✅ here doesn't need checking again.

| | |
|---|---|
| Reviewed commit | `5939378` (docs after ADR-025) |
| Fixed in | `5e66649` (colours, README) and "Docs review fixes (3 agents)" — the commit that added this file |
| Raw reports | [coherence](2026-11-23/coherence.md) · [readability](2026-11-23/readability.md) · [content](2026-11-23/content.md) |
| Rules learned | [DOC-RULES.md](DOC-RULES.md) |

**Status key:** ✅ fixed · 🟡 partly fixed · ⬜ open · ➖ won't fix (reason given).
**IDs:** `C` = coherence, `R` = readability, `V` = content validation. The
number is the row number in that raw report. Line numbers in the raw
reports point to commit `5939378`.

---

## Totals

| Review | Question it answered | Findings | ✅ | 🟡 | ⬜ | ➖ |
|---|---|---|---|---|---|---|
| Coherence | Do the docs agree with each other, and do they flow? | 30 | 28 | 1 | 1 | 0 |
| Readability | Can the owner read them quickly? | 35 + terms | 27 | 4 | 4 | 0 |
| Content | Are the claims true (code, builds, outside facts)? | 31 | 31 | 0 | 0 | 0 |

Many findings were the same problem seen by two or three agents. They are
listed under each agent anyway, so each raw report can be traced.

---

## Coherence (C)

| ID | Sev. | Where | Finding | Status |
|---|---|---|---|---|
| C1 | High | 02_UseCaseModel | Leftover v0.1 table with stale statuses | ✅ deleted |
| C2 | High | README, 07_PhasePlan | v0.6 listed body check-ins (F18 is v0.4); F22 missing | ✅ both rows match BACKLOG |
| C3 | High | arc42 §11 | "No CI yet" | ✅ deleted |
| C4 | High | 08_DevelopmentCase | CI/CD boxes unchecked | ✅ CI checked; CD partly (I3 ✅, I6 open) |
| C5 | High | BACKLOG F1 | Missing record kinds: session review, game-mode change | ✅ added |
| C6 | High | prototype README, index.html | Rejected code-drawn pictures listed as "Real" | ✅ moved to "Fake"; comment says rejected |
| C7 | Med | arc42 §8 | Sections out of order (8.15, 8.14, 8.13) | ✅ reordered |
| C8 | Med | arc42 §8.6 | Heading said "v1" for the v2 rules | ✅ |
| C9 | Med | arc42 §7.3, RELEASING | No status column; APK/web release had no backlog item | ✅ status column; new item **I7** (v0.3) |
| C10 | Med | README, BACKLOG, 06 | v0.2 still described as "CI/CD" | ✅ |
| C11 | Med | README layout | `assets/` missing, `prototypes/` "(empty)" | ✅ |
| C12 | Med | README docs table, arc42 Sources | No links to prototypes/ and research | ✅ |
| C13 | Med | 06 DoD | ADR-012 pointed to arc42 | ✅ links `adr/` |
| C14 | Med | BACKLOG rules | "Reasoning goes in arc42 §9" | ✅ points to `adr/` |
| C15 | Med | 06 Iteration 2 | Still "(next)"; unplanned work not recorded | ✅ "(in progress)" + "Also done" |
| C16 | Med | exercises.json vs MuscleGroup vs skills | Three muscle vocabularies, no mapping | ✅ mapping in arc42 §8.16 |
| C17 | Med | ADR-012 | Source "(when available)" | ✅ linked |
| C18 | Med | CREDITS, R-23 | Claimed per-exercise prompts were saved | ✅ reworded: only template prompts saved |
| C19 | Med | 02_UseCaseModel | No UCs for photos, how-to pictures, session review | ✅ UC-26, UC-27, UC-28 |
| C20 | Low | Vision, RiskList, arc42 §10, 06 | Lists out of order | ✅ sorted |
| C21 | Low | Glossary | Related terms split; fake function names | ✅ moved; formats fixed |
| C22 | Low | BACKLOG | F23 under "progress" | ✅ under "coach" |
| C23 | Low | BACKLOG cap | Cap ~25, 27 open | ✅ cap ~30 (R-07 updated) |
| C24 | Low | arc42 §7.1, §8.16; 03 | No photo store path; file sizes; wrong § | ✅ |
| C25 | Low | ADR-024 | "Rejected" + "superseded" mixed | ✅ "replaced by ADR-025" |
| C26 | Low | README, DEVELOPING, arc42 | CI trigger wording | ✅ "push to main / PR" |
| C27 | Low | 03 Supplementary Spec | No picture provenance or "never colour alone" rule | ✅ both added |
| C28 | Low | 08_DevelopmentCase | Missing rows (guides, prototype, assets, question bank) | ✅ |
| C29 | Low | many | ADR and § references in plain text, not links | 🟡 assets README, arc42 Sources, ADR-012 linked; others open |
| C30 | Low | README opener | "desktop log" vs "phone-first coach"; no v2 pointer | ✅ |

---

## Readability (R)

| ID | Where | Finding | Status |
|---|---|---|---|
| R1 | 02_UseCaseModel | Broken duplicate table (= C1) | ✅ |
| R2 | README | 73-word sentence at the top | ✅ bullets |
| R3 | Vision | Coach and Game cells are ~100 words each | ⬜ split into sub-tables |
| R4 | Glossary | "Lighter weeks" vs "deload" clash | ✅ Deload = "easy recovery week, not a Pause"; Lighter weeks = UI label of Pause |
| R5 | Vision, BACKLOG, Glossary | "Check-in" meant two things | ✅ "session review" for "How was it?" |
| R6 | arc42, Vision, RiskList, BACKLOG, RELEASING | "TickTimer" never explained | ✅ defined at first use + Glossary |
| R7 | Glossary | One 74-row table, no groups | 🟡 tech words split out; term groups open |
| R8 | Glossary | Tech words missing | ✅ "Tech words" table (40 terms) |
| R9–R11 | BACKLOG F1, F8, F13, F14, F15, F21 | 30–62-word run-on items | ✅ sub-bullets |
| R12 | ADR-022 | Decision is a 55-word chain | ✅ rule table |
| R13 | ADR-020 | Crypto words unexplained | 🟡 "Words" line added; long sentence remains |
| R14 | arc42, Vision | Numbering out of order (= C7, C20) | ✅ |
| R15 | RiskList | Says "ranked" but isn't; key at the bottom | ✅ key on top, sorted by ID, opener fixed |
| R16 | README | 1RM / e1RM unexplained | ✅ |
| R17 | README roadmap | Five new terms at once | ✅ |
| R18 | Vision, README, arc42 | "Plays like an MMO" unexplained | 🟡 in Glossary; Vision sentence open |
| R19 | README, arc42 | Stale facts (= C2, C3, C11) | ✅ |
| R20–R24 | arc42 §8.14, §8.6, §5, §8.5 | Dense crypto, server list, italics, GRASP and planner jargon | ✅ intro line, bullets, keys |
| R25 | Vision, UCM, Glossary, RiskList | Openers cite books first; no "who reads it, when" | ⬜ |
| R26 | 02_UseCaseModel | ⭐, "fully dressed", "6a", "inception" unexplained | ✅ key under the table |
| R27 | UC-11 | 29-word success guarantee | ✅ bullets |
| R28 | README | No "start here" | ✅ |
| R29–R30 | BACKLOG | "Skipped" and "Now" as run-ons | ✅ tables |
| R31 | prototype README | Rejected design in "Real" (= C6) | ✅ |
| R32 | prototype README | Dense code notes | ⬜ rewrite when the prototype switches to the ADR-025 pictures |
| R33 | RELEASING | Chained cells; "Run workflow" vs "Release button" | 🟡 both names defined in Glossary; cells open |
| R34 | DEVELOPING | 32-word merge-conflict sentence | ✅ 3 steps |
| R35 | ADR-024, CREDITS | Two long list-sentences | ⬜ |
| Terms | many | 16 concepts with 2+ names | 🟡 fixed: check-in, Pause, muscle diagram (Glossary, docs), body map vs muscle diagram, TickTimer. "wizard" → "questions" also fixed. Open: "session" (login) → "login session"; ADR-024 keeps "muscle map" (rejected record, left as written) |

---

## Content validation (V)

Each row says what the evidence was. "Repo" = checked against the files or
a build in the repo at `5939378`.

| ID | Sev. | Where | Finding | Evidence | Status |
|---|---|---|---|---|---|
| V1 | High | README build | Windows command had no `-G "MinGW Makefiles"` | Repo: TROUBLESHOOTING, `ci.yml` | ✅ |
| V2 | High | arc42 §11 | "No CI yet" (= C3) | Repo: `ci.yml` | ✅ |
| V3 | High | RiskList R-23, ADR-025 | Sora "shut down in March 2026" | [OpenAI Help: Sora discontinuation](https://help.openai.com/en/articles/20001152-what-to-know-about-the-sora-discontinuation) — app and web closed 2026-04-26, API ends 2026-09-24; [AP via OPB, 2026-03-24](https://www.opb.org/article/2026/03/24/openai-pulls-the-plug-on-sora-the-viral-ai-video-app-that-sparked-deepfake-concerns) | ✅ dates fixed, sources linked |
| V4 | High | R-22, CREDITS | "Knees past toes" called wrong form | Fry, Smith & Schilling 2003, *JSCR* 17(4):629–633 ([PubMed 14636100](https://pubmed.ncbi.nlm.nih.gov/14636100/)): restricting knee travel lowered knee torque (150 → 117 N·m) but raised hip torque (28 → 303 N·m) | ✅ R-22 example replaced; CREDITS says vertical shins are a knee-stage choice, not a rule |
| V5 | High | README layout | (= C11) | Repo | ✅ |
| V6 | High | README v0.6 | (= C2) | Repo: BACKLOG | ✅ |
| V7 | High | 02_UseCaseModel | (= C1) | Repo | ✅ |
| V8 | High | ADR-004 | Invariant "one-record-per-id" | Repo: `include/core/Workout.h:37–38` | ✅ "sorted by date, at most one per date" |
| V9 | Med | arc42 §7.3 | CI/CD rows described the future as now (= C9) | Repo: `ci.yml`, `release.yml` | ✅ |
| V10 | Med | README core build | `build-core/` not in `.gitignore` → `git add -A` would commit binaries | Repo: simulated run | ✅ now `build/core` (covered by `build/`) |
| V11 | Med | arc42 §8.6 | (= C8) | Repo | ✅ |
| V12 | Med | 03 Licensing | "Qt under LGPL (dynamic linking)" not true for all planned parts | [Qt HTTP Server licence](https://doc.qt.io/qt-6/qthttpserver-index.html) (GPLv3 or commercial); [Qt for WebAssembly](https://doc.qt.io/qt-6.11/wasm.html) (static linking by default) | ✅ |
| V13 | Med | Glossary IK | `pose::solveLimb` doesn't exist | Repo: prototype has `ik()` | ✅ |
| V14 | Med | arc42 §8.2 | `schemaVersion` 2 not marked planned | Repo: `JsonStore.h:29` writes 1 | ✅ |
| V15 | Med | Glossary, arc42 P7 | Pain check had no "see a professional" step | Repo: safety framing only in §8.9 | ✅ added to both |
| V16 | Med | arc42 §8.5 vs §8.6 | Exam minimum: "1–2 cardio" vs "1 cardio" | Repo | ✅ 1 cardio in both |
| V17 | Low | arc42, ADR-020, ADR-023 | Recovery code shown at two different moments | Repo | ✅ "at the first photo, which turns on photo sync" |
| V18 | Low | README, DEVELOPING | CI trigger wording (= C26) | Repo: `ci.yml:13–17` | ✅ |
| V19 | Low | RELEASING | "smoke-tests the zip" — it tests the folder | Repo: `release.yml:236–248` | ✅ |
| V20 | Low | 06 | Stale "I3 moved up" note; I3 row order | Repo | ✅ |
| V21 | Low | arc42 §8.16 | Sizes "~80–130 KB" | Repo: 65.8–138.2 KB | ✅ "~65–140 KB" |
| V22 | Low | 03, README | macOS claimed; binary path | Repo: no macOS CI; `MACOSX_BUNDLE ON` | ✅ "should build, not tested"; `.app` path |
| V23 | Low | ADR-012 | (= C17) | Repo | ✅ |
| V24 | Low | BACKLOG | (= C14) | Repo | ✅ |
| V25 | Low | BACKLOG cap | (= C23) | Repo | ✅ |
| V26 | Low | UC-11, F7 | "P1–P5"; the planner has P1–P7 | Repo: ADR-013, arc42 §8.5 | ✅ |
| V27 | Low | DEVELOPING | Android/iPhone "v0.2+" | Repo: RELEASING, `release.yml` say v0.3 | ✅ |
| V28 | Low | ADR-024 | (= C25) | Repo: `adr/README.md` | ✅ |
| V29 | Low | arc42, RiskList, Vision | Ordering (= C20) | Repo | ✅ |
| V30 | Low | CREDITS | OpenAI terms clause has limits ("if any") | [OpenAI Terms of Use](https://openai.com/policies/terms-of-use/), effective 2026-01-01 | ✅ |
| V31 | Low | README | 10–20 sets read as a prescription | arc42 §8.13 framing | ✅ |

### Outside facts the content agent confirmed

No need to check these again unless the source changes.

| Claim in the docs | Source |
|---|---|
| Qt 6.11 for Android: JDK 21, NDK 27.2.12479018 | https://doc.qt.io/qt-6.11/android.html |
| Qt 6.8 for Android used JDK 17 | https://doc.qt.io/qt-6.8/android.html |
| Qt 6.11 on Windows uses MinGW 13.1 | https://doc.qt.io/qt-6.11/supported-platforms.html |
| Qt targets one Emscripten version per minor release | https://doc.qt.io/qt-6.11/wasm.html |
| Qt HTTP Server: local or trusted network only, not the internet | https://doc.qt.io/qt-6/qthttpserver-index.html |
| GitHub's windows-2025 runner includes Inno Setup | https://newreleases.io/project/github/actions/runner-images/release/win25%2F20251014.59 |
| Epley 1RM = w × (1 + r/30) | https://www.topendsports.com/testing/calculators/1repmax.htm |
| 10–20 weekly sets per muscle (Baz-Valle et al. 2022, 12–20) | https://pmc.ncbi.nlm.nih.gov/articles/PMC8884877 |
| Schoenfeld, Ogborn & Krieger 2017: more sets, more growth; ≥ 9 sets p = 0.076; data to ~10 sets | https://doi.org/10.1080/02640414.2016.1210197 (checked by Claude after the review; the agent couldn't reach it) |
| Höffler & Leutner 2007: animations beat static pictures for motor skills, d = 1.06 | https://www.leibniz-ipn.de/en/research/publications/instructional-animation-versus-static-pictures-a-meta-analysis |
| "Nearly 50 %" of AI-drawn humans distorted (4 models) | https://arxiv.org/abs/2503.00811 |
| Hinnant's date algorithms are public domain | https://howardhinnant.github.io/date_algorithms.html |

The agent also built the code: core-only and full (Qt 6.4.2) builds, 0
warnings, `test_core` 18 cases / 799 assertions, `test_services` 11 cases.

### Not checked (by anyone yet)

| Claim | Why not | Who can check |
|---|---|---|
| CI is green; v0.1.0 Release is published | The agent didn't query GitHub | Owner: the repo's **Actions** and **Releases** pages |
| aqtinstall 3.3.0 caused the CI folder problem (TROUBLESHOOTING) | Needs the old CI log | Owner: old Actions run |
| EyesOff uses libsodium | Owner's other repo | Owner |
| ChatGPT offers no video option on this account | Account-specific | Owner |
| JPEG quality 0.88; which chat made which diagram (CREDITS) | Process claims | — (only dimensions were checked) |
| < 100 ms per action, start-up < 1 s (03, UC-01) | No benchmark run | A future timing test |

---

## Open items

Small enough to do the next time the file is touched; not worth a backlog id.

- R3 Vision module cells → sub-tables · R25 same opener in every doc ("What
  this is / Read it when") · R32 prototype code notes as a table · R35
  ADR-024 and CREDITS long sentences · C29 link the first ADR/§ mention in
  each doc · term: "session" (login) → "login session" in arc42 §8.8 and
  the Glossary.
