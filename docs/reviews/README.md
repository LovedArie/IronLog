# Doc reviews

> **What this is**: past reviews of the doc set, and how to run the next
> one. **Read it when** you plan a review, or wonder if a doc was already
> checked.

| File | What's in it |
|---|---|
| [DOC-RULES.md](DOC-RULES.md) | The checklist. Use it on every doc change |
| [2026-11-23-doc-review.md](2026-11-23-doc-review.md) | Every finding, its evidence and its status |
| [2026-11-23/](2026-11-23/) | The three agent reports, kept as written |

---

## How a review works

Three agents run at the same time. Each one reads the docs and writes a
report. None of them edits the repo. Then Claude applies the fixes and
fills in the status log.

| Agent | Question | Checks against |
|---|---|---|
| Coherence | Do the docs agree and flow? | The other docs |
| Readability | Can the owner read it fast? | Sentence length, jargon, one name per concept |
| Content | Is each claim true? | The code, a real build, outside sources (with links) |

## Running the next one cheaply

1. Run the [DOC-RULES](DOC-RULES.md) checklist first. Fix what it finds.
2. Give the agents only the docs that changed since the last reviewed
   commit: `git diff --stat <last reviewed commit> -- docs README.md assets`.
3. Tell them to skip what's ✅ in the last log, and the outside facts
   already confirmed there, unless the source changed.
4. Ask for the same report format: a findings table with
   `# · Severity · File:line · Problem · Evidence · Fix`, then "Verified
   OK", then "Not checked". Every outside fact needs a URL.
5. Save the reports in `docs/reviews/<date>/`, write `<date>-doc-review.md`
   with a status for every finding, and add any new rule to DOC-RULES.

**Privacy:** remove anything personal about the owner (health, contact
details) from the reports before committing them (ADR-019).
