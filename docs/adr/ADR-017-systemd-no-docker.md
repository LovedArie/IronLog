# ADR-017: systemd, no Docker, on the existing VPS

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

The Hetzner VPS already runs Caddy and TickTimer under systemd with hardening and backup timers.

## Decision

Same pattern: `ironlog.<domain>` → Caddy → 127.0.0.1:8081; own systemd unit and user; own data folder and nightly backup timer.

## Consequences

- One way of running services on the box.
- Copy-and-adapt TickTimer's `deploy/` files.
- No container isolation beyond systemd's sandboxing.

## Alternatives considered

- Docker / Docker Compose.

## Sources

- TickTimer ROLLOUT.md §2, §2i; deploy/ticktimer.service.example
