# ADR-011: Phone-first: Android, web (WASM) and Windows

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Training happens at the gym and at home; the partner will use a phone; TickTimer already proved Android APK + Qt WebAssembly + Windows from one C++ codebase.

## Decision

IronLog targets Android (APK), a Qt WebAssembly web app (for iPhone, served by Caddy at `/app`) and Windows — no app stores.

## Consequences

- Touch-first UI (ADR-012).
- No native iOS app; iPhone users get the web app, whose storage is a cache (Risk R-21).
- Same distribution channels as TickTimer.

## Alternatives considered

- Desktop-only (v0.1).
- Native Android/iOS apps — two more codebases.

## Sources

- TickTimer docs: ANDROID.md, WEB.md, ROLLOUT.md
