# ADR-003: `SetEntry` invariant via private constructor + `std::optional` factory

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

A set with negative weight or zero reps must never exist. Bad input from a form is *expected*, not exceptional.

## Decision

`SetEntry` has a private constructor; the only way in is `static std::optional<SetEntry> make(...)`, which checks the rules first.

## Consequences

- An invalid `SetEntry` cannot be created.
- Callers must handle the empty `optional` — the type forces the check.
- No exceptions for ordinary bad input.

## Alternatives considered

- Throwing constructor — exceptions for expected input.
- Public struct + `isValid()` — invalid objects can exist and be forgotten (TickTimer's empty-id refusals were easy to miss).

## Sources

- Stroustrup, *The C++ Programming Language*, §16.2.4 (class vs struct), §17.6.3.2 (establish invariants in constructors)
