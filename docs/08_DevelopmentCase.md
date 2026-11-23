# 08 — Development Case

> **What this is**: Which UP artifacts and practices *this* project uses.
> Almost every UP artifact is optional — pick what adds value. (Larman,
> Ch. 2 §2.12.)

---

## Artifacts in scope for this project

`s` = start, `r` = refine.

| Discipline | Artifact | Inception | Elaboration | Construction | Transition |
|------------|----------|-----------|-------------|--------------|------------|
| Business modeling | Domain Model | | s (in class diagram) | | |
| Requirements | Use-Case Model | s | r | | |
| Requirements | Vision | s | r | | |
| Requirements | Supplementary Specification | s | r | | |
| Requirements | Feature and UI research (`research/`) | s | r | | |
| Requirements | Glossary | s | r | r | |
| Design | Design Model (`diagrams/`, `arc42.md` §5–§6) | | s | r | |
| Design | SW Architecture Document (`arc42.md`) | | s | r | |
| Design | Architecture Decision Records (`adr/`, one file per decision) | s | r | r | r |
| Design | Data Model (`arc42.md` §8.1–§8.2) | | s | r | |
| Project management | Risk List | s | r | r | r |
| Project management | Iteration Plan | s | r | r | r |
| Project management | Backlog (`BACKLOG.md`, TickTimer rules) | s | r | r | r |
| Environment | Development Case (this file) | s | r | | |
| Environment | How-to guides (`DEVELOPING`, `RELEASING`, `TROUBLESHOOTING`) | | s | r | r |
| Design | UI prototype (`prototypes/ui-prototype/`) | | s | | |
| Implementation | Exercise pictures + `CREDITS.md` (`assets/exercises/`) | | s | r | |
| Learning | Question bank (`question-bank.md`) | s | r | r | r |
| Quality | Doc reviews + doc rules (`reviews/`) | | s | r | r |

**Changed 2026-11-23**: the v0.1 `09_DesignNotes.md` is replaced by
**`arc42.md`**, one architecture document in a standard 12-section template
(archived copy in `archive/`). Why arc42: the project now has a server,
three client platforms and CI/CD — enough structure that a known template
(context, deployment, concepts, decision log) beats a free-form notes file.
The Phase Plan's feature list moved to **`BACKLOG.md`**.

---

## Practices for this project

- [x] Iterative development
- [x] Unit tests for every domain rule (written alongside the code)
- [x] Warnings on (`-Wall -Wextra -Wpedantic`), zero tolerated
- [x] Docs and diagrams updated in the same change as the code
- [x] Troubleshooting log for every real build/run problem
- [x] Continuous integration — GitHub Actions `ci.yml`, 3 jobs (I2 ✅)
- [ ] Continuous delivery — Windows zip on tag ✅ (I3); APK, web bundle and VPS deploy with `/version` check still to do (I6)
- [ ] Code review (solo project)

---

## Coding standards

- **C++ standard**: C++17.
- **Naming**: `PascalCase` types; `camelCase` functions and variables;
  `m_` prefix for data members; `kName` for constants.
- **Header guards**: `#pragma once`.
- **Namespaces**: everything in `ironlog`; analysis in `ironlog::stats`.
- **Braces**: same line (K&R style).
- **Comments**: explain *why*, not *what*. Cite the book section when a
  pattern comes from Larman or Stroustrup.
- **Errors**: expected bad input → `std::optional` or an enum result.
  Exceptions only for truly exceptional situations.
- **Layering**: `core/` never includes Qt. `services/` never includes Widgets or QML.
- **Privacy**: no personal health documents, secrets or keys in the repo (ADR-019).

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 2 §2.12.
