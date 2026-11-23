# ADR-007: Atomic saves; never overwrite an unreadable file

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Months of training history must survive crashes and bad files.

## Decision

Write with `QSaveFile` (temp file + rename). Be strict about the file (bad JSON, newer schema → refuse, move it aside as `*.unreadable-<time>`) and tolerant about items (skip and count bad entries).

## Consequences

- A crash mid-save leaves the old file intact.
- The app never turns "can't read" into "deleted".
- One bad set doesn't cost a year of history.

## Alternatives considered

- Plain `QFile` writes; starting fresh over an unreadable file.

## Sources

- Qt docs: QSaveFile
- IronLog TROUBLESHOOTING / test_services
