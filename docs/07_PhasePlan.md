# 07 — Phase Plan & Software Development Plan

> **What this is**: A **low-precision** roadmap plus tools and resources.
> Inception estimates are not reliable — they show order of magnitude only.
> (Larman, Ch. 4 §4.1, §4.3.) The item-level queue moved to
> [BACKLOG.md](BACKLOG.md) on 2026-11-23.

---

## Rough phase estimates

| Phase | Covers | Estimated duration | Notes |
|-------|--------|--------------------|-------|
| Inception | v0.1 + planning session | done | Vision v2, arc42, backlog, research |
| Elaboration | v0.2 Foundation, v0.3 Coach | 5–8 weeks | Riskiest parts: QML on phones, planner rules, data v2 |
| Construction | v0.4 Game, v0.5 Online, v0.6 Progress+ | 6–10 weeks | Server reuses TickTimer's proven design |
| Transition | v1.0 | 1–2 weeks | Partner onboarding, release routine, QA checklist |

Student pace: exam periods will stretch these. That's expected — the app
itself has an exam mode for the same reason.

---

## Milestones

| Version | Theme | Exit criterion |
|---|---|---|
| ✅ v0.1.0 | Desktop logbook | Tests green, docs complete |
| v0.2 | Foundation | CI green; data v2; QML decision made |
| v0.3 | Coach | Owner trains a full week from *Train now* plans |
| v0.4 | Game | Character sheet, decay, exam mode, quest tokens in daily use |
| v0.5 | Online | Owner + partner synced through the VPS |
| v0.6 | Progress+ | Body check-ins, PR types, calculators; Widgets UI retired |
| v1.0 | Release | One semester of real use without data loss |

Items per milestone: [BACKLOG.md — Roadmap](BACKLOG.md#roadmap).

---

## Resources

**People**: one developer (learning C++/Qt), pairing with Claude. Owner and
partner are also the first users.

**Tools**:
- Build: CMake 3.20+, Qt 6.11 (MinGW kit on Windows; Android kit; WebAssembly kit)
- IDE: Qt Creator
- Version control + CI/CD: git, GitHub, GitHub Actions
- Diagrams: PlantUML (`plantuml -tpng diagrams/*.puml -o rendered`)
- Tests: Catch2 v2 single header, run via CTest

**Environment**:
- Clients: Windows PC, Android phone, any browser (iPhone)
- Server: existing Hetzner CX23 VPS (Ubuntu) with Caddy + systemd, shared
  with TickTimer

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 4 §4.1, §4.3.
