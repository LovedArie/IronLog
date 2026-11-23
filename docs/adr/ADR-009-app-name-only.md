# ADR-009: Set only the application name (no organization name)

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

`QStandardPaths::AppDataLocation` is `<organization>/<application>`; TickTimer ended up with a doubled `TickTimer/TickTimer/` folder.

## Decision

`main.cpp` sets only the application name, so data lives in `AppData/Roaming/IronLog/`.

## Consequences

- No doubled folder.
- The app name *is* the data folder: renaming the app needs a copy-the-old-file migration (Risk R-10).

## Alternatives considered

- Organization = application = "IronLog".

## Sources

- TickTimer TROUBLESHOOTING: "App data lands in a DOUBLED folder"
