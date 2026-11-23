# tests/

Unit and integration tests, using **Catch2 v2** (single header:
`include/catch.hpp`). CTest runs them.

```bash
ctest --test-dir build --output-on-failure
```

## Files

| File | Builds into | Tests | Needs Qt? |
|---|---|---|---|
| `test_main.cpp` | `catch_main` (OBJECT library) | — Catch2's `main()`, compiled once and shared | No |
| `test_core.cpp` | `test_core` | Date math, SetEntry invariant, e1RM, ExerciseLibrary, TrainingLog ordering, PRs, last set, weekly sets, sample data | No |
| `test_services.cpp` | `test_services` | JSON round-trip, broken/newer files, skipped items, controller actions + signals, corrupt-file backup | Qt Core only |

## Conventions

- One `TEST_CASE` per rule, named `"Area: the rule in plain words"`.
- Use `REQUIRE` when later lines depend on it (stops the test), `CHECK`
  otherwise (keeps going and reports every failure).
- Never read the real clock in a test — pass dates in.
- File tests use `QTemporaryDir`, which deletes itself.

## Why Catch2 and not QtTest?

The core has no Qt, so its tests shouldn't either. Using one framework for
both test files keeps things simple. (Earlier projects used QtTest for
GUI-level tests; that's still the right tool if we ever test widgets.)
