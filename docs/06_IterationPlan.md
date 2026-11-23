# 06 — Iteration Plan

> **What this is**: The current and next iteration only — not the whole
> project. Rank work by **risk**, **coverage**, and **criticality**.
> (Larman, Ch. 2; Ch. 4 §4.3; Ch. 8 §8.3.) The full queue lives in
> [BACKLOG.md](BACKLOG.md).

---

## Iteration 1 — v0.1.0, 2026-11-23 ✅ done

**Goal**: A working training log with PRs, weekly volume, and progress
chart, on a tested three-layer architecture.

**Shipped**: UC-01 to UC-07; pure C++ core + Catch2 tests; JSON persistence
with atomic save and corrupt-file handling; PlantUML diagrams; headless
screenshot mode.

**Definition of done (met)**: `test_core` (18 cases) and `test_services`
(11 cases) pass; zero warnings with `-Wall -Wextra -Wpedantic`; docs match code.

**Risks attacked**: R-01 data loss, R-02 wrong stats, R-05 toolchain.

**Then**: planning session (same day) → Vision v2, `arc42.md`, `BACKLOG.md`,
feature research.

---

## Iteration 2 — v0.2 Foundation (next)

**Goal**: Protect the code with CI, settle the data format once, and prove
QML runs on the owner's Android phone and in a browser — before any
v0.3 feature is built on top.

**Backlog items, in order**: I1 ✅ → I2 ✅ → I3 → I4 → F1 → F2 → F3 → F4

*I3 moved up: the release workflow was written with CI, so it's tested next.*

| Id | Item | Size |
|---|---|---|
| I1 | GitHub repo + v0.1.0 tag 👤 — ✅ 2026-11-23, `LovedArie/IronLog` | S |
| I2 | CI: Linux core-only, Linux + Qt 6.4, Windows MinGW + Qt 6.8 LTS — ✅ 2026-11-23, green | M |
| I4 | QML spike → Android APK + WASM page on the owner's phone 👤 | M |
| F1 | Schema v2 + migration | L |
| F2 | Catalog v2 + owner's home-kit exercises | M |
| F3 | Edit a set + undo/redo | M |
| F4 | Rotating backups + CSV export | S |
| I3 | CD: Windows zip on tag → GitHub Release | M |

**Definition of done**:
- CI is green on all three jobs for the final commit.
- A v1 file migrates to v2 with no lost sets (test with the sample log).
- The spike's result is written into arc42 ADR-012 (accept or reject QML).
- Tag `v0.2.0` produces a GitHub Release with the Windows zip automatically.
- Docs, diagrams and `question-bank.md` updated.

**Risks attacked**: R-13 (QML/Android/WASM), R-04 (stable ids), R-03
(secondary muscles), R-07 (scope — by finishing a small milestone).

**Estimated duration**: 2–3 weeks at student pace (low precision, Larman Ch. 4 §4.1).

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 2, Ch. 4 §4.1, §4.3, Ch. 8 §8.3.
