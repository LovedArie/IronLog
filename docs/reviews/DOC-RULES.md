# Doc rules

> **What this is**: the checklist the 2026-11-23 review taught us. **Use
> it** every time a doc changes. If every box holds, a full three-agent
> review isn't needed. Each rule names the findings that taught it
> ([log](2026-11-23-doc-review.md)).

---

## 1. One fact, one home

| Rule | Why (findings) |
|---|---|
| Milestones and items live **only** in `BACKLOG.md`. Other docs link to it, not copy it. | One wrong copy spread to two docs (C2, V6) |
| Decisions live in `docs/adr/ADR-0NN-*.md`. arc42 §9 is only the index. | Stale "arc42 ADR-012" links (C13, C14, V24) |
| A number used in two docs (game rules, sizes, counts) gets checked in all of them. | Exam minimum 1 vs 1–2 cardio (V16); recovery-code moment (V17) |
| When something ships, update **every** "not yet" line about it: arc42 §11, 06, 08, README. | "No CI yet" after CI shipped (C3, C4, V2) |
| Delete leftovers after moving a table or section. | Orphaned use-case table (C1, R1, V7) |

## 2. Say what's real

| Rule | Why (findings) |
|---|---|
| Planned things say **planned** and name their backlog id. Done things get ✅. | §7.3 CD, `schemaVersion` 2, macOS (C9, V9, V14, V22) |
| Name only code that exists. Check with `grep` before writing a class or function name. | `pose::solveLimb` (C21, V13); ADR-004 invariant (V8) |
| Commands in docs must be ones that were run. Copy them from CI or TROUBLESHOOTING. | Windows build without `-G "MinGW Makefiles"` (V1); `build-core/` not ignored (V10) |
| "Saved" or "recorded" claims must point to the file that holds it. | CREDITS said per-exercise prompts were saved (C18) |
| An ADR is either *Rejected* or *Superseded by*, never both. | ADR-024 (C25, V28) |

## 3. Outside facts need a source

| Rule | Why (findings) |
|---|---|
| Every outside fact (dates, versions, licences, studies) gets a link next to it, or in the doc's Sources. | Sora date (V3); Qt licences (V12) |
| Prefer the first-hand source: the vendor's help page, the paper, the docs page. A news story is second. | Sora: AP gave the announcement, OpenAI's page gave the real dates (V3) |
| Training advice is checked against research, not gym habits. Frame it as general guidance, not medical advice, and say when to see a professional. | "Knees past toes" (V4); pain check (V15); 10–20 sets (V31) |
| Quote limits of a source when they matter ("p = 0.076", "if any"). | Schoenfeld 2017; OpenAI terms (V30) |
| Never put the owner's health details in the repo (ADR-019). Say "users on a knee stage", not the owner's condition. | Review reports were scrubbed before saving |

## 4. Easy to read

| Rule | Why (findings) |
|---|---|
| Sentences ≤ 25 words. A list hidden in a sentence becomes bullets or a table. | BACKLOG run-ons, ADR-022 (R2, R9–R12) |
| Define a word at first use in each doc (≤ 5 words), and add it to the Glossary's Tech words. | TickTimer, GRASP, Argon2id, RAMP (R6, R8, R20–R24) |
| One concept, one name. Check the Glossary before naming something. | check-in, Lighter weeks vs deload, muscle map (R4, R5, Terms) |
| Every doc opens with **What this is** and **Read it when**. | Openers started with book citations (R25) |
| Numbered things stay in order: sections, IDs, versions. | §8 order, Q9/Q10, risks, Vision history (C7, C20, R14) |
| A table cell holds one idea. More than ~20 words → its own section or sub-table. | Vision Coach/Game cells (R3) |
| Colour is never the only signal: one hue light → dark, plus text. | Red/orange muscle diagrams failed the UI research check |

---

## When the rules aren't enough

Run the review again (see [README](README.md)) when:

- a milestone ships (v0.2, v0.3, …),
- more than ~5 docs changed since the last review, or
- an outside fact may have changed (a new Qt version, new OpenAI terms).
