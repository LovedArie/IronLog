# Architecture Decision Records (ADRs)

One file per architecture decision: the context, what was decided, what it
costs, and what else was considered. `docs/arc42.md` §9 holds the summary
table; the **reasoning lives here**.

> **Why separate files?** A decision is a moment in time. Keeping each one
> in its own file means its history (`git log docs/adr/ADR-015-*.md`) is the
> history of that decision alone, and arc42 stays readable.

## Rules

1. **Never rewrite an accepted ADR's decision.** If it changes, write a new
   ADR that *supersedes* it, and set the old one's status to
   `Superseded by ADR-0NN`. Typos and links can be fixed freely.
2. **Numbers are permanent** and never reused (same rule as the backlog ids).
3. **Proposed → Accepted** when the evidence arrives (e.g. ADR-012 after the
   QML spike, I4). Write *why* in the Consequences section.
4. New ADR: copy [template.md](template.md) to `ADR-0NN-short-slug.md`, add
   a row below and in arc42 §9, in the same commit as the change it records.

## Status values

`Proposed` · `Accepted` · `Rejected` · `Deprecated` · `Superseded by ADR-0NN`

## Index

| # | Decision | Status | Date |
|---|---|---|---|
| [001](ADR-001-three-layers.md) | Three layers as three CMake targets | Accepted | 2026-11-23 |
| [002](ADR-002-own-date-type.md) | Own `Date` type in core instead of `QDate` | Accepted | 2026-11-23 |
| [003](ADR-003-setentry-invariant.md) | `SetEntry` invariant via private constructor + `std::optional` factory | Accepted | 2026-11-23 |
| [004](ADR-004-stats-free-functions.md) | Analysis as free functions (`stats::`) | Accepted | 2026-11-23 |
| [005](ADR-005-jsonstore-pure-fabrication.md) | Persistence as a Pure Fabrication (`JsonStore`) | Accepted | 2026-11-23 |
| [006](ADR-006-single-controller.md) | All writes through one `TrainingController` + signals | Accepted | 2026-11-23 |
| [007](ADR-007-atomic-save.md) | Atomic saves; never overwrite an unreadable file | Accepted | 2026-11-23 |
| [008](ADR-008-hand-painted-chart.md) | Hand-painted chart (no QtCharts) | Accepted | 2026-11-23 |
| [009](ADR-009-app-name-only.md) | Set only the application name (no organization name) | Accepted | 2026-11-23 |
| [010](ADR-010-schema-v2-one-migration.md) | Schema v2 in one migration | Accepted | 2026-11-23 |
| [011](ADR-011-phone-first.md) | Phone-first: Android, web (WASM) and Windows | Accepted | 2026-11-23 |
| [012](ADR-012-qml-ui.md) | Qt Quick (QML) for the new UI | Proposed | 2026-11-23 |
| [013](ADR-013-rule-based-planner.md) | Rule-based planner; AI later on top | Accepted | 2026-11-23 |
| [014](ADR-014-derived-game-state.md) | Game state derived from the log | Accepted | 2026-11-23 |
| [015](ADR-015-record-level-merge.md) | Record-level merge on top of TickTimer's revisioned protocol | Accepted | 2026-11-23 |
| [016](ADR-016-qhttpserver-behind-caddy.md) | QHttpServer behind Caddy on loopback | Proposed | 2026-11-23 |
| [017](ADR-017-systemd-no-docker.md) | systemd, no Docker, on the existing VPS | Accepted | 2026-11-23 |
| [018](ADR-018-risk-driven-order.md) | Risk-driven milestone order | Accepted | 2026-11-23 |
| [019](ADR-019-personal-docs-out-of-repo.md) | Personal training documents stay out of the public repo | Accepted | 2026-11-23 |
| [020](ADR-020-photo-envelope-encryption.md) | Progress photos: envelope encryption | Accepted | 2026-11-23 |
| [021](ADR-021-optional-game.md) | Game layer is optional per user | Accepted | 2026-11-23 |
| [022](ADR-022-game-rules-v2.md) | Game rules v2 (evidence-based) | Accepted | 2026-11-23 |
