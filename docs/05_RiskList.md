# 05 — Risk List

> **What this is**: What could go wrong, ranked, with mitigations. UP is
> risk-driven: early iterations attack the riskiest items first. (Larman,
> Ch. 4 §4.3; Ch. 8 §8.3.) Re-rank every iteration.

---

## Risks

| ID | Description | Type | Likelihood | Impact | Mitigation | Status |
|----|-------------|------|------------|--------|------------|--------|
| R-01 | **Data loss** from a crash mid-save or a corrupt file being overwritten. | Technical | M | H | `QSaveFile` atomic writes; unreadable file renamed aside; test pins it. | Mitigated |
| R-02 | **Wrong stats** (PRs, weekly counts) erode trust. Week boundaries and "first session" are easy to get wrong. | Technical | M | H | Rules in pure functions with edge-case tests (Sunday vs Monday, ties, first-ever). | Mitigated |
| R-03 | **One muscle per exercise** under-counts compound lifts (bench also trains triceps). | Business | H | M | Catalog v2 adds secondary muscles; they count as half-sets (F2, game XP rule). | Planned fix |
| R-04 | **Exercise renamed** breaks links, because sets refer to exercises by name. | Technical | M | M | Schema v2 gives exercises stable ids (F1, F2). | Planned fix |
| R-05 | **Qt toolchain mismatch** on Windows (wrong compiler, missing DLLs). | Resource | M | M | Build in Qt Creator with the Qt kit; `windeployqt` for terminal runs. See TROUBLESHOOTING. | Mitigated |
| R-06 | **e1RM is an estimate.** Epley gets less accurate above ~10 reps, so PRs on high-rep sets are rough. | Business | H | L | Labelled "Est. 1RM" with a tooltip; Glossary defines it. | Accepted |
| R-07 | **Scope creep**: v2 vision is large (coach, game, server, 3 platforms) for one student. | Schedule | H | H | Milestones v0.2–v0.6, backlog cap ~25, "Later" list, risk-driven order. | Watching |
| R-08 | **Bodyweight moves** show 0 kg volume and 0 e1RM. | Business | H | L | Documented; future: optional body-weight entry. | Accepted |
| R-09 | **Windows dark mode** renders parts of the UI dark and parts light (TickTimer hit this with a stylesheet + palette mix). | Technical | M | L | No stylesheet; the chart and models paint from the palette or translucent fills. Not yet checked on a dark-mode Windows PC. | Watching |
| R-10 | **Renaming the app** moves the data folder (`setApplicationName` decides it), so users appear to lose data. | Technical | L | H | Don't rename casually. If renamed: copy the old file on first launch (never move). | Open |
| R-11 | **Generated plans cause injury** (e.g. knee overload), since the app now tells people what to lift. | Business | M | H | Rules from written decisions; constraint stages and gates; pain check; property test "never exceeds knee stage"; "not medical advice" note. | Open |
| R-12 | **Gamification backfires**: decay feels like punishment during exams; rewards encourage overtraining. | Business | M | M | 10-day grace, exam mode, muscle memory, 50 % floor; quest = planned sessions, not "more". | Open |
| R-13 | **QML + Android + WebAssembly** are new to this codebase; could stall v0.3. | Technical | M | H | Spike first (I4) before committing (ADR-012 is "proposed"). | Open |
| R-14 | **Sync merge loses or duplicates sets.** | Technical | M | H | Record ids + tombstones; pure `sync::merge` with exhaustive tests; nightly server backups. | Open |
| R-15 | **Health data exposure**: public repo, shared VPS. | Business | L | H | Personal plans never committed (ADR-019); HTTPS; server on loopback; invite-only; per-user data. | Open |
| R-16 | **"Lean" can't be measured** without food tracking; users may feel no progress. | Business | M | M | Weekly body weight + waist trend (F18); explain in the Vision. | Accepted |
| R-17 | **QHttpServer** (not designed for internet exposure) misbehaves. | Technical | L | M | Loopback only, Caddy in front (Qt's documented limit); fall back to TickTimer's parser if needed. | Open |

**Scale**: H = High, M = Medium, L = Low.

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 4 §4.3 (Table 4.1); Ch. 8 §8.3.
