# ADR-016: QHttpServer behind Caddy on loopback

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

TickTimer's hand-written HTTP parser needed hardening and caused bugs. Qt's HTTP Server module exists, but its docs say not to expose it to the internet.

## Decision

`ironlog-server` uses QHttpServer bound to 127.0.0.1:8081; Caddy is its only client and handles TLS and public traffic. **Proposed** until it is proven in F15.

## Consequences

- Less hand-written HTTP code.
- Satisfies Qt's "trusted environment" requirement.
- Fallback: TickTimer's parser (Risk R-17).

## Alternatives considered

- Copy TickTimer's hand-written server.

## Sources

- Qt HTTP Server — Limitations and Security: https://doc.qt.io/qt-6/qthttpserver-index.html
