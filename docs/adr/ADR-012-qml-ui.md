# ADR-012: Qt Quick (QML) for the new UI

- **Status:** Proposed
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Phone-first touch UI with animations (level-ups, body map). TickTimer made Qt Widgets work on phones, but only after a long responsive-layout effort.

## Decision

Build new screens in Qt Quick (QML) over C++ view models; keep the Widgets UI until QML reaches parity, then retire it (H1). **Proposed** until the I4 spike runs QML on the owner's phone and in a browser.

## Consequences

- `core/` and `services/` stay unchanged (payoff of ADR-001).
- New skills: QML, view models, Qt Quick Controls.
- Two UIs exist for a while.

## Alternatives considered

- Responsive Qt Widgets (TickTimer's path).

## Sources

- TickTimer design-addendum-responsive.md
- docs/research/ui-research.md (when available)
