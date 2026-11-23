# ADR-004: Analysis as free functions (`stats::`)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Information Expert suggests `TrainingLog` should compute PRs, histories and weekly sets — but that would make it a low-cohesion god class.

## Decision

Put analysis in free functions in namespace `stats::` that read `TrainingLog` through its public interface.

## Consequences

- `TrainingLog` keeps one job: its sorted, one-record-per-id invariant.
- Stats are tested on their own.
- New reports don't touch the log class.

## Alternatives considered

- Methods on `TrainingLog`.

## Sources

- Larman, Ch. 17 (Information Expert and its contraindications)
