# ADR-005: Persistence as a Pure Fabrication (`JsonStore`)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Expert would let the domain save itself, coupling it to a file format and Qt's JSON classes.

## Decision

A made-up class, `JsonStore`, owns saving and loading. The domain knows nothing about files.

## Consequences

- Domain stays pure and cohesive.
- Storage can be swapped (SQLite, server) without touching the domain.
- The same conversion can feed the disk and the network (TickTimer lesson).

## Alternatives considered

- `TrainingLog::saveTo(path)`.

## Sources

- Larman, Ch. 25 §25.2 (Pure Fabrication — saving a Sale in a database)
