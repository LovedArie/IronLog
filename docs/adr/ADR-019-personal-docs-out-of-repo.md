# ADR-019: Personal training documents stay out of the public repo

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

The repo is public; the owner's fitness plan and decision report contain health details.

## Decision

Never commit personal plans, reports, photos or secrets. Planner rules are written generically (e.g. "constraint stage").

## Consequences

- Docs describe users and constraints in general terms.
- `*.local.*` is git-ignored for personal notes.

## Alternatives considered

- A private repo.

## Sources

- docs/08_DevelopmentCase.md (privacy rule)
