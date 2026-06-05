# Agent guide — BASIC-in-C-

Working agreement for **all** coding agents and human contributors working in
this repository. These rules are not optional. The full house spec lives in
the `Hawkynt/project-template` repo (`STANDARD.md`); this file is the
per-repo distillation.

## What this is

A single-header **C++ library** (`BASIC.h`) that embeds classic line-numbered
BASIC into C++ via preprocessor/template machinery, plus `Demos/` (the
eleven-part text-mode demo), `ConsoleSnake/`, and `tests/`. The humor in the
README is part of the product — keep the tone.

## Commits

- **Group changes semantically/logically** — one dialect feature / demo part
  / concern per commit.
- **Every subject line starts with a prefix**: `+` added · `-` removed ·
  `*` changed · `#` bug fixed · `!` critical todo.
- Never start a subject with "fix"/"bugfix"/"changed"/"modified".
- **No AI traces anywhere**: no `Co-Authored-By` AI lines, no "Generated
  with" footers, no agent mentions in messages, comments, or authorship.

## The loop (always, in this order)

1. **Before committing**: build and run the tests the same way `ci.yml`
   does (compile `tests/` and the demos on a real compiler); every new
   dialect feature gets demo coverage or a test. Update the README's
   keyword table when the dialect grows; `CHANGELOG.md` is generated —
   never edit it by hand.
2. **Commit** (rules above) and **push**.
3. **Wait for CI**; on `main` a green CI triggers the nightly (prerelease +
   GFS prune). Fix and loop until everything is green.

Stable releases are **manual** (`gh workflow run release.yml`) — never cut
one unless explicitly asked.

## Code conventions

- Everything ships in **one header** — no new translation units; demos and
  tests are the only .cpp files.
- The BASIC dialect is the API: keywords stay upper-case, line numbers stay
  mandatory, and any deviation from classic BASIC semantics is documented in
  the "What's the Catch?" section.
- Pure text mode only — no platform-specific graphics; ANSI escapes are the
  ceiling.

## README & repo conventions

- Standard frame: title → badges → one-line `>` blockquote; fixed emoji
  mapping for the standard sections (`## ❤️ Support`, `## 📜 License`);
  the repo-specific sections keep their consistent topical emojis.
- License is LGPL-3.0-or-later; the `## ❤️ Support` section and
  `.github/FUNDING.yml` stay intact.
