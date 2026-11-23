# ADR-015: Record-level merge on top of TickTimer's revisioned protocol

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

TickTimer syncs a whole document with a revision number and asks the user on conflict. A training log is mostly *appended*: two devices adding different sets is not a real conflict.

## Decision

Keep TickTimer's wire protocol (`GET/PUT` with `baseRevision`, 409 on conflict; server stores an opaque blob). On 409 the client runs `sync::merge(local, remote)`: union by record id, newer `updatedAt` wins, tombstones beat older edits.

## Consequences

- Offline logging on two devices just works.
- Merge is a pure function with exhaustive tests.
- Needs ids and tombstones (ADR-010).

## Alternatives considered

- Full-document "human chooses" (TickTimer).
- CRDTs — more machinery than needed.

## Sources

- TickTimer design-addendum-sync.md
