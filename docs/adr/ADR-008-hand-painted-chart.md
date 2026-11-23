# ADR-008: Hand-painted chart (no QtCharts)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

QtCharts is an optional add-on module being replaced by QtGraphs — one more install step that can break a fresh build.

## Decision

Draw the e1RM chart with `QPainter` in a custom `paintEvent()`; X axis by real date.

## Consequences

- No extra module; teaches how widgets draw.
- No zoom or tooltips for free.
- Revisit for the QML UI (Qt Quick Shapes / QtGraphs).

## Alternatives considered

- QtCharts; QtGraphs.

## Sources

- Qt docs: QPainter
