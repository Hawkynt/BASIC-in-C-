# Changelog

## v20260605 (2026-06-05)

### Added
- CI/CD/release workflow quartett (ci, nightly, release, _build) from the hawkynt-standard template + helper scripts: version.pl, update-changelog.mjs, prune-nightlies.mjs + CHANGELOG.md seeded for automatic maintenance (3e2a229)
- self-checking smoke suite covering the BASIC.h macro surface (declarations, conditionals, the loop zoo, SELECT, GOTO, FUNCTION/SUB, conversions) (dd77260)
- initial commit (551e606)

### Changed
- README: add CI badge and ship row (release/nightly/downloads), document building locally and the release pipeline (bf0eba5)
- rename to README.md and standardize the badge block to the house style (fa93bc4)
- readme (dd6f9e3)
- docs (a359fc8)
- variable declaration feels more BASIC now (c1942ea)

### Fixed
- packaging: the solution platform is named x86 (not Win32) and the bin/ output dir was only configured for Debug|x64 - apply OutDir/IntDir to all configurations (819e032)
- C++17 language standard was only set on Debug|x64 - fold expressions in BASIC.h failed to compile every other configuration # silence C4005: undef windows.h's CONST before redefining it function-like (8032acd)
- add missing standard library includes - the header relied on MSVC pulling in <cstring>/<cstdlib>/<ctime>/<string> transitively which breaks under mingw-w64 (8f229db)
- bite yourself * perf-opt (ea22d0c)

All notable changes are recorded here. This file is maintained automatically by
`.github/workflows/scripts/update-changelog.mjs`, which bucketises commits by
their prefix (`+` added, `*` changed, `-` removed, `#` fixed).

## [Unreleased]

- Initial repository setup.
