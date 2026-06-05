# Changelog

## v20260605 (2026-06-05)

### Added
- Visual Studio solution + project for the demo (Demos/SecondUnreality.sln), mirroring the ConsoleSnake setup incl. /Zc:preprocessor and C++17; CI builds it on the windows leg (96d3364)
- repo meta per house standard: AGENTS.md, .editorconfig (abbc88f)
- ConsoleSnake glow-up, still pure text mode: bordered arena, colour-coded everything, live score line, blinking golden power-ups (+5 points +2 segments), magenta enemies spawning every fifth fruit and wandering drunkenly, speed scaling with score, ESC to surrender * README documents TO/STEP, the box filter, the eleven-part demo and the new snake (1494704)
- demo part eight: scrolltext marquees in all four directions, sine-wobbled and hue-cycled, rendered from a 5x5 font ROM stored in DATA statements like a proper type-in listing * demo source speaks the new FOR..TO/STEP dialect throughout (a12f9e6)
- the FLIP downscaler is now a box filter: every cell averages all pixels it covers, so lone stars dim gracefully instead of flickering out of existence (9a83e63)
- FOR loops with real TO and STEP keywords: FOR(i, 1 TO 5), FOR(i, 10 TO 1 STEP -3) - commas in trench coats plus an argument-count dispatcher; the comma spellings and FOR_STEP stay valid * MSVC switches to the conforming preprocessor (/Zc:preprocessor) in the vcxproj and every cl invocation (016e4c2)
- FLIP auto-fits the terminal: SCREEN asks for a resize via the xterm escape, the renderer measures the real window every frame and downscales uniformly (nearest neighbour, aspect kept) when the frame is too big; WINDOW_FIT(FALSE) restores raw 1:1 + rows end with ESC[K so a downscaled frame leaves no junk to its right (1957076)
- POSIX backend: BASIC.h now builds and runs on Linux and macOS - termios INKEY with ANSI-arrow-to-DOS-scancode translation, ANSI CLS/LOCATE/COLOR, chrono-based TIMER/SLEEP/retrace, PLAY mimes in perfect tempo (no portable beeper) + ci runs the test suites natively on windows, ubuntu AND macos; mingw cross-compile keeps gcc eyes on the Win32 path + _build produces eight targets: win-x86/x64/arm64 (MSVC cross), linux-x86/x64/arm64 (multilib + aarch64, static), macos-x64/arm64 (clang -arch); nightly/release merge all target artifacts * TIMER now returns seconds since program start (steady clock) instead of CPU clock (44c6a41)
- video pages, QB 4.5 style: PAGES(n) allocates VRAM pages back to back, ACTIVE_PAGE picks the drawing target, VISUAL_PAGE what FLIP shows, PCOPY blits pages - the hidden backbuffer for feedback effects + demo grows to ten parts: fire, starfield, DATA-driven wireframe cube, bouncing GET/PUT sprites, particle fireworks with gravity and trails, and a Timeless-style dot tunnel composed over a zoom-feedback backbuffer with orbiting glow sprites; any key skips, ESC quits * CLS clears only the active page; unchained planar PEEK/POKE offsets span all pages (mode Y's raison d'etre) (1935bc4)
- honest retrace timing on INP(3DA): fake 60Hz frame with 262 scanlines, bit 3 pulses on vertical retrace, bit 0 on any blanking - the wait-not-retrace-then-retrace two-step works verbatim + truecolor VESA modes 10Dh-118h: 15/16/24bpp little-endian multi-byte pixels through the banked A000 window, RGB15/RGB16/RGB24 composers, format-aware renderer, truecolor-capable sprites + math corner: SIN/COS/TAN/ATN/SQR/SGN/PI + Demos/SecondUnreality.cpp: copper bars via pure DAC animation, three-sine plasma, shadebobs, 16-bit rotozoom - all vsynced on 3DA; compiled (not run) by CI and shipped with releases (3f65c85)
- OUT/INP/WAIT port emulation: DAC programming via 3C8/3C9 with auto-increment, readback via 3C7/3C9, 3DA retrace bit that toggles every read, junk-drawer semantics for unknown ports + mode X/Y/Z unchained planar modes: map mask (3C4/3C5 idx 2) selects POKE planes, read map select (3CE/3CF idx 4) selects the PEEK plane, one address = four pixels + fake VESA modes 100h/101h/103h/105h/107h with bank-switched 64KB A000 window via VESA_BANK, including the Trident XOR-2 bank quirk on 3C4/3C5 idx 0Eh + GET/PUT sprites with PSET/PRESET/AND/OR/XOR verbs (smuggled past the preprocessor via stringizer) + PAINT_BORDER (QB border-bounded fill) and PAINT_PATTERN (CHR$-built tile fills) + CGA_PALETTE(0/1) palette select for SCREEN 1 (950e6aa)
- the full video mode zoo: SCREEN 1/2 (CGA at B800), SCREEN 3 (Hercules at B000, green phosphor), SCREEN 7/8/9 (EGA), SCREEN 12 (VGA 640x480), each with the correct resolution/colour depth and per-mode palette; colours mask to the mode depth * PEEK/POKE use honest real-mode address arithmetic over a flat megabyte: physical = segment*16 + offset, wrapping at 1MB (A20 off), so A001:0000 == A000:0010 == 9FFF:0020; the active mode maps its framebuffer at its historical segment * SCREEN(7) is now authentic EGA 320x200x16 instead of the made-up 160x100 (use GRAPHICS(w,h) for custom resolutions) (b3a1752)
- unit test suites: Graphics (framebuffer, palette, ANSI renderer, A000 PEEK/POKE), Music (MML parser, silent on CI), Data, Files * pipeline README documents the new suites (0896c06)
- terminal VGA: SCREEN(13)/SCREEN(7)/GRAPHICS framebuffer with the classic 256-colour DAC palette, PSET/POINT/LINE/LINE_B/LINE_BF/CIRCLE/PAINT/PALETTE, FLIP renders ANSI 24-bit half-blocks (two pixels per character cell) + PEEK/POKE/DEF_SEG: 64KB of fake RAM, segment 0xA000 memory-mapped onto the live framebuffer like real mode 13h + SOUND and PLAY with a full Music Macro Language parser (notes/accidentals/octaves/lengths/dots/tempo/rests/N-numbers/articulation) + DATA/READ/RESTORE inline data statements with typed READ and DATA_REMAINING + file channels: OPEN..FOR_INPUT/OUTPUT/APPEND..AS n, PRINT_FILE, LINE_INPUT_FILE, INPUT_FILE (comma-aware), EOF_FILE, CLOSE_FILE, KILL, FILE_EXISTS (b3b9bdd)
- unit test suites per area: Strings (boundary+exceptional cases), Collections (incl. nested containers), Loops (all DO/LOOP combinations, EXIT/ITERATE/INCR/DECR/SWAP/MOD) + shared TestKit.h with check() and END_OF_TESTS verdict * Smoke suite refactored onto TestKit (14cd331)
- classic string function zoo with real $-suffixed identifiers: LEFT$/RIGHT$/MID$/INSTR/UCASE$/LCASE$/LTRIM$/RTRIM$/TRIM$/SPACE$/STRING$/REPEAT$/CHR$/ASC/STR$/VAL/HEX$/OCT$/BIN$/TALLY (1-based, clamping) + collections: LIST_OF/DICTIONARY_OF types with LIST_*/DICT_* operations and FOR_EACH(x IN xs), composable/nestable + PowerBASIC flow statements: EXIT_DO/EXIT_FOR/EXIT_WHILE/EXIT_SUB, ITERATE_DO/ITERATE_FOR, INCR/DECR (with optional amount), SWAP, MOD operator, ASCIIZ type * DIM/DIM_ARRAY type slot is now variadic so templated types containing commas survive the macro grinder (09b2e7e)

### Changed
- ConsoleSnake renders the arena as half-block cells via the framebuffer engine - one character is two stacked square pixels, so the snake is finally square instead of 1:2-stretched, auto-fits the terminal, and moves uniformly on both axes + input queue is drained per tick with last-direction-wins, so buffered key repeats no longer lag the steering * score moved from a text line to a bar of arcade pips along the top (90c450c)
- mirror current changelog/prune scripts from the shared template (notes-only mode, bot-commit filtering, prune retry + newest-kept guard) * nightly: same-day re-runs replace release AND tag at the validated SHA * release: the vyyyyMMdd tag is placed ON the changelog commit so bookkeeping never pollutes the next notes (a5d5aaa)
- README: repo-name title, substantial one-liner blockquote, mandatory support and license sections (d17476f)
- ci compiles and runs every suite in tests/ on MSVC, cross-compiles them all with mingw-w64 (b353a2a)

### Fixed
- windows INKEY returned extended keys as TWO results (the 0/224 prefix, then the scan code) so every arrow press registered twice - the pair is now swallowed atomically, one press = one code (769c963)
- native CI builds wrote -o ConsoleSnake which collides with the ConsoleSnake/ directory - output 'snake' instead (3b57ee1)

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
