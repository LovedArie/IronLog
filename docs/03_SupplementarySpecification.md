# 03 — Supplementary Specification

> **What this is**: Requirements that aren't use cases — mostly quality
> rules (FURPS+). Some of these shape the architecture, so they're decided
> early. (Larman, Ch. 5 §5.5, Ch. 7.)

---

## Functionality (cross-cutting)

- **Validation in one place.** Every set is checked by `SetEntry::make()`:
  0–1000 kg, 1–100 reps, finite numbers, non-empty exercise.
- **Exercise names are case-insensitive** and trimmed ("bench press" =
  "Bench Press").
- **Safety (v2)**: generated plans never exceed a lifter's constraint stages
  or skip a progression gate; every plan screen says "general guidance, not
  medical advice; stop on sharp pain". (arc42 §8.9, Risk R-11)
- **Security (v2)**: HTTPS between devices and Caddy; server on loopback
  only; invite-only registration; the password never leaves the device (a
  derived login key is sent and hashed again); progress photos use envelope
  encryption with a recovery code; each user reads and writes only their
  own data. (arc42 §8.8, §8.14)
- **Game fairness (v2)**: every XP, level and token can be explained from
  logged records; no decay during a pause; no food rewards, no "cheat" wording. (arc42 §8.6)

## Usability

- Logging a repeat set: **≤ 3 clicks** (weight/reps pre-fill from last time).
- Keyboard: Ctrl+Enter adds a set; Delete removes the selected set.
- Colours must read on **light and dark** themes (use palette colours or
  translucent fills).
- **Never colour alone:** every colour cue also has text (muscle names on
  muscle diagrams, numbers on the body map). Shades of one hue, not red vs
  green or red vs orange (design system v1, `research/ui-research.md`).
- **Exercise pictures** have a recorded source: ChatGPT images, prompts and
  date in `assets/exercises/CREDITS.md` (ADR-025). (arc42 §8.16)

## Reliability

- **No data loss on crash:** saves are atomic (`QSaveFile`: temp file + rename).
- **Never overwrite unreadable data:** a corrupt or newer-version file is
  renamed `*.unreadable-<timestamp>` before a fresh log starts.
- **Tolerant item reading:** one bad set in the file is skipped and counted;
  the rest still loads.
- **Sync never loses a set (v2):** merges by record id; deletes travel as
  tombstones; the server keeps nightly backups. (arc42 §8.3, §7.1)
- **Offline first (v2):** every feature except sync works with no network.

## Performance

- *Train now*: plan shown **< 1 s** after the last answer, offline, on a phone.
- Any user action → views updated in **< 100 ms** for 5 years of data
  (~1,500 sessions, ~40,000 sets). Full model resets are fine at this size.
- Startup < 1 s.

## Supportability

- Domain core builds and tests **without Qt** (`-DIRONLOG_BUILD_APP=OFF`).
- File format has a `schemaVersion` so future versions can migrate.
- Weekly target (10–20) is a parameter (`stats::VolumeTarget`), not hard-coded logic.

## "+" — Additional

- **Implementation:** C++17, Qt 6.4+ (developed against 6.11), CMake 3.20+.
- **Platforms:** v0.1 Windows (MinGW), Linux; macOS should build but is not tested. **v2: phone-first** —
  Android (APK), web via Qt WebAssembly (for iPhone), Windows desktop.
- **Server:** existing Hetzner VPS, Caddy + systemd, no Docker (arc42 §7).
- **Licensing:** Catch2 is Boost-licensed; Qt under LGPLv3 with dynamic linking on desktop and
  Android. The WebAssembly build links Qt statically by default, so LGPL
  relinking duties apply. `ironlog-server` uses Qt HTTP Server, which is
  GPLv3 or commercial only (no LGPL) — fine for a public GPL-compatible repo,
  re-check before any closed release.

---

## Sources

- Larman, *Applying UML and Patterns* (3rd ed.), Ch. 4 §4.3, Ch. 5 §5.5, Ch. 7.
