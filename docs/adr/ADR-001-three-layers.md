# ADR-001: Three layers as three CMake targets

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

One student developer. Domain rules must be testable without a UI, and the UI is expected to change (Widgets now, QML later).

## Decision

Split the code into `core/` (pure C++17), `services/` (Qt Core) and `ui/` (Qt Widgets, later QML). Each layer is its own CMake target and may only use the layers below it.

## Consequences

- `core/` builds and tests on a machine with no Qt (CI job "core only").
- The UI can be replaced (ADR-012) without touching rules or storage.
- Read-only *layer bridging* is allowed: UI models call `stats::` directly; every write still goes through the controller (ADR-006).
- Cost: more targets and include folders than a single executable.

## Alternatives considered

- One executable with everything inside it — fastest to start, hardest to test and reuse.
- Qt-only MVC — ties the rules to Qt.

## Sources

- Larman, *Applying UML and Patterns*, Ch. 13 (Layers)
- Bass et al., *Software Architecture in Practice*, Ch. 8 §8.4 (one-way allowed-to-use)
