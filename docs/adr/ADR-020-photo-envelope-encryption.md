# ADR-020: Progress photos: envelope encryption

- **Status:** Accepted
- **Date:** 2026-11-23
- **Deciders:** owner + Claude (pairing)

## Context

Body photos are the most private data in the app. They must sync across Android, web and Windows, the server admin must not be able to read another user's photos, and a forgotten password should not lose everything.

## Decision

A random photo key encrypts photos and previews (libsodium secretstream). It is locked twice: envelope A with a lock key derived from the password (Argon2id → KDF; never leaves the device) and envelope B with a recovery code shown once when photo sync is turned on. The server never sees the password: a derived login key is sent and hashed; `/prelogin` returns salt and settings (fake salt for unknown users).

## Consequences

- Server stores only hashes, envelopes and ciphertext.
- Password change re-locks envelope A only.
- Recovery code restores access; losing both password and code loses server copies (Risk R-20).
- Devices upload encrypted previews because the server can't make thumbnails.

## Alternatives considered

- Key derived directly from the password (no recovery, re-encrypt everything on password change).
- Server-side encryption (admin could read photos).
- Photos never synced (lost with the phone).

## Sources

- arc42 §8.14; diagrams/photo_keys.puml
- docs/research/feature-research-2.md (photo privacy)
