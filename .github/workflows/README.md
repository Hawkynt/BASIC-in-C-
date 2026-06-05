# CI/CD Pipeline — BASIC-in-C-

> Everything in this folder is the automated pipeline for this repository.
> Workflows live here, their helper scripts live in `scripts/`.

## What this does

Three workflows, one shared build block, three helper scripts:

| File                            | Trigger                             | Purpose                                     |
|---------------------------------|-------------------------------------|---------------------------------------------|
| `ci.yml`                        | push + PR + `workflow_call`         | Build + macro smoke suite (MSVC + mingw)    |
| `release.yml`                   | **manual dispatch**                 | Package + publish, then tag `vyyyyMMdd`     |
| `nightly.yml`                   | successful CI run on `main`/`master`| Publish `nightly-yyyyMMdd` prerelease       |
| `_build.yml`                    | `workflow_call` (internal)          | Shared publish/packaging building block     |
| `scripts/version.pl`            | invoked by the workflows            | Stamp package manifests (no-op here — none) |
| `scripts/update-changelog.mjs`  | invoked by the workflows            | Bucketise commits into CHANGELOG.md         |
| `scripts/prune-nightlies.mjs`   | invoked by the workflows            | 3-gen (GFS) retention of nightlies          |

## How it works

```
                push / PR
                    │
                    ▼
            ┌───────────────┐
            │    ci.yml     │──► MSVC build + smoke run on windows
            └───┬───────┬───┘    mingw-w64 cross build on ubuntu
                │       │
   dispatch ────┤       │  on success on main/master
                ▼       ▼
        ┌──────────┐  ┌─────────────┐
        │ release  │  │  nightly    │
        │  .yml    │  │   .yml      │
        └────┬─────┘  └─────┬───────┘
             │              │
             ▼              ▼
        (both call _build.yml)
             │              │
             │   ConsoleSnake-win-x64.exe + ConsoleSnake-win-x86.exe
             │   + BASIC.h (the header IS the product)
             ▼              ▼
  publish + tag vyyyyMMdd  nightly-yyyyMMdd (prerelease)
                                │
                                ▼
                       scripts/prune-nightlies.mjs
                       (GFS: 7 daily + 4 weekly + 3 monthly)
```

## Test tiers

`BASIC.h` is a Win32 header (windows.h + conio.h), so both matrix legs build
**for** Windows:

| Leg                | What runs                              | Required? |
|--------------------|----------------------------------------|-----------|
| `windows-latest`   | MSVC build of ConsoleSnake + compile **and run** `tests/Smoke.cpp` | ✓ |
| `ubuntu-latest`    | mingw-w64 cross build of ConsoleSnake + compile `tests/Smoke.cpp`  | ✓ |

The smoke suite exercises the macro surface (DIM/SET/LET, IF/SELECT, the loop
zoo, FUNCTION/SUB, conversions) and self-checks its results; the cross leg is
compile-only because the host can't execute Win32 binaries.

## What it's for

- Every PR is built on MSVC **and** mingw-w64 before it can merge — two
  preprocessors and two standard libraries keep the macro madness honest.
- Every merge to `main`/`master` produces a **tested** nightly prerelease.
- A **manual dispatch** cuts a stable release from artifacts built by `_build.yml`, then tags the dated `vyyyyMMdd` Release at that commit.
- Old nightlies are auto-pruned on a **Grandfather-Father-Son** schedule.

## Why it's built this way

- **No cron triggers.** Event-driven only — CI fires on PRs, nightlies fire when CI passes on main, stable releases fire on manual dispatch.
- **Files drive versions, never tags.** This repo ships no package manifests, so there is no version to stamp; the repo-level Release/tag is the date marker `vyyyyMMdd`.
- **Release calls CI via `workflow_call`.** Calling ci.yml explicitly keeps tests and releases in lockstep with zero copy-paste.
- **Nightly builds from the `workflow_run` payload's SHA**, not branch tip — so a nightly is always a build of code CI actually validated.
- **`_build.yml` is the single packaging block**, shared by release and nightly so they never diverge. It runs on windows-latest because the product is Win32.
- **3-generation (GFS) retention**, not "keep last N". GFS guarantees at least one build per week for a month and one per month for a quarter.

## Scripts

### `version.pl`

The one versioner, identical in every repo of the family. It scans for package
manifests (csproj/package.json/composer.json/*.pm) and stamps each with its own
version + commit count. This repo has none, so it leaves everything untouched —
it is kept so the pipeline stays byte-comparable across repos.

### `update-changelog.mjs`

Prepends a new section to `CHANGELOG.md`. Commit-subject convention: `+` Added, `*` Changed, `#` Fixed, `-` Removed, `!` TODO, anything else → Other.

### `prune-nightlies.mjs`

GFS retention with `DAILY_KEEP=7`, `WEEKLY_KEEP=4`, `MONTHLY_KEEP=3`. Dry-run with `--dry-run`.

## Who maintains this

This is an instance of the shared template for the Hawkynt repo family
(`hawkynt-standard`). When changing the shared parts, prototype in the template
then mirror the change here.

## Release artifacts

| Artifact                                             | Produced by          |
|------------------------------------------------------|----------------------|
| `ConsoleSnake-win-x64.exe` / `ConsoleSnake-win-x86.exe` | release + nightly |
| `BASIC.h`                                            | release + nightly    |
