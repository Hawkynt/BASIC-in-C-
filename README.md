# 🧮 BASIC.h

[![License](https://img.shields.io/github/license/Hawkynt/BASIC-in-C-)](https://github.com/Hawkynt/BASIC-in-C-/blob/main/LICENSE)
[![Language](https://img.shields.io/github/languages/top/Hawkynt/BASIC-in-C-?color=8957D5)](https://github.com/Hawkynt/BASIC-in-C-)

[![CI](https://github.com/Hawkynt/BASIC-in-C-/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/Hawkynt/BASIC-in-C-/actions/workflows/ci.yml)
![Last Commit](https://img.shields.io/github/last-commit/Hawkynt/BASIC-in-C-?branch=main)
![Activity](https://img.shields.io/github/commit-activity/m/Hawkynt/BASIC-in-C-)

[![Stars](https://img.shields.io/github/stars/Hawkynt/BASIC-in-C-?color=FFD700)](https://github.com/Hawkynt/BASIC-in-C-/stargazers)
[![Forks](https://img.shields.io/github/forks/Hawkynt/BASIC-in-C-?color=008080)](https://github.com/Hawkynt/BASIC-in-C-/network/members)
[![Issues](https://img.shields.io/github/issues/Hawkynt/BASIC-in-C-)](https://github.com/Hawkynt/BASIC-in-C-/issues)
![Code Size](https://img.shields.io/github/languages/code-size/Hawkynt/BASIC-in-C-?color=4CAF50)
![Repo Size](https://img.shields.io/github/repo-size/Hawkynt/BASIC-in-C-?color=FF9800)

[![Release](https://img.shields.io/github/v/release/Hawkynt/BASIC-in-C-?sort=semver)](https://github.com/Hawkynt/BASIC-in-C-/releases/latest)
[![Nightly](https://img.shields.io/github/v/release/Hawkynt/BASIC-in-C-?include_prereleases=true&sort=date&label=nightly&color=FF9800)](https://github.com/Hawkynt/BASIC-in-C-/releases)
[![Downloads](https://img.shields.io/github/downloads/Hawkynt/BASIC-in-C-/total)](https://github.com/Hawkynt/BASIC-in-C-/releases)

> The header your C++ professor warned you about.

---

## 🎯 What is this?

`BASIC.h` is a C++ header that lets you write code like it’s 1985 — **but it compiles today**.

It gives you a **PowerBASIC-style façade over modern C++**, with macros like:

```vb
FUNCTION(main() AS INTEGER)
  DIM(x AS INTEGER)
  SET(x, 42)
  IF(x > 0) THEN
    PRINT("We're back in business!")
  ENDIF
  END
END_FUNCTION
```

Yes. That compiles.
No. I’m not sorry.

---

## 🧪 Why?

Because BASIC was:

* Readable 🧑‍🏫
* Fast to write 💨
* Dangerous 🔥
* Full of gotos and global state 😈
* Didn't have those ugly brackets and semicolons 🗑
* Was my first programming language 🎉

And so is this.

Except this time, it's **C++** wearing BASIC's skin.

---

## 🔨 What’s Inside?

### 🧱 Structured Syntax

| BASIC            | C++ with BASIC.h                |
| ---------------- | ------------------------------- |
| `DIM x AS INT`   | `DIM(x AS INTEGER)`             |
| `SET x = 1`      | `SET(x, 1)`                     |
| `IF x > 0 THEN`  | `IF(x > 0) THEN`                |
| `ELSE`           | `ELSE`                          |
| `ENDIF`          | `ENDIF`                         |
| `FOR i = 1 TO 5` | `FOR(i, 1, 5)`                  |
| `NEXT`           | `NEXT`                          |
| `FOR EACH x IN xs` | `FOR_EACH(x IN xs)` (yes, a real `IN`) |
| `DO WHILE c ... LOOP` | `DO_WHILE(c) ... LOOP`     |
| `DO UNTIL c ... LOOP` | `DO_UNTIL(c) ... LOOP`     |
| `DO ... LOOP WHILE c` | `DO ... LOOP_WHILE(c)`     |
| `DO ... LOOP UNTIL c` | `DO ... LOOP_UNTIL(c)`     |
| `EXIT DO / FOR`  | `EXIT_DO` / `EXIT_FOR` / `EXIT_WHILE` |
| `ITERATE DO / FOR` | `ITERATE_DO` / `ITERATE_FOR`  |
| `INCR x / DECR x` | `INCR(x)`, `INCR(x, 5)`, `DECR(x)` |
| `SWAP a, b`      | `SWAP(a, b)`                    |
| `x MOD y`        | `x MOD y` (a real `MOD` operator) |
| `FUNCTION`       | `FUNCTION(name(...) AS TYPE)`   |
| `RETURN`         | `RETURN(value)`                 |
| `END`            | `END` (yes, no semicolon)       |

### 🖥️ Console I/O

| Command        | Behavior                                 |
| -------------- | ---------------------------------------- |
| `PRINT(...)`   | Sends to `std::cout << ...`              |
| `INPUT(x)`     | Reads into `x` from `cin`                |
| `CLS()`        | Clears screen (WinAPI, no system())      |
| `LOCATE(y,x)`  | Moves cursor to line/column              |
| `COLOR(fg,bg)` | Sets color via `SetConsoleTextAttribute` |

### 🧠 Types

```cpp
INTEGER → int  
LONG    → long  
SINGLE  → float  
DOUBLE  → double  
STRING  → std::string  
BYTE    → unsigned char  
BOOLEAN → bool  
ASCIIZ  → char (use DIM_ARRAY(buf, 32 AS ASCIIZ) for PowerBASIC's zero-terminated buffers)
```

Because `auto` is too fancy for 80s kids.

### 🧵 Strings Like It's 1989

The `$` is **part of the identifier** — MSVC and GCC both allow it, so `LEFT$`
really is called `LEFT$`. Even your variables can be called `name$`. Positions
are 1-based, out-of-range arguments clamp instead of crashing.

| Function | Does |
| -------- | ---- |
| `LEFT$(s,n)` / `RIGHT$(s,n)` / `MID$(s,start[,len])` | The holy slicing trinity |
| `INSTR([start,] hay, needle)` | 1-based find, `0` = not found |
| `UCASE$(s)` / `LCASE$(s)` | SHOUTING and whispering |
| `LTRIM$(s)` / `RTRIM$(s)` / `TRIM$(s)` | Space removal services |
| `SPACE$(n)` / `STRING$(n, ch)` / `REPEAT$(n, s)` | Padding factories |
| `CHR$(code)` / `ASC(s)` | Character ↔ code |
| `STR$(x)` / `VAL(s)` | Number ↔ string (`STR$` keeps the classic sign slot: `" 42"`) |
| `HEX$(x)` / `OCT$(x)` / `BIN$(x)` | Bases for nerds |
| `TALLY(hay, needle)` | Count occurrences |

### 📦 Collections (the anachronism aisle)

No DOS-era BASIC had these. This one does, because we're already in too deep:

```cpp
DIM(scores AS LIST_OF(INTEGER))
LIST_ADD(scores, 42)
PRINT(LIST_ITEM(scores, 1))           // 1-based, of course

DIM(ages AS DICTIONARY_OF(STRING, INTEGER))
DICT_SET(ages, "BASIC", 61)
IF(DICT_HAS(ages, "BASIC")) THEN
  PRINT(DICT_ITEM(ages, "BASIC"))
ENDIF

FOR_EACH(score IN scores)
  PRINT(score)
NEXT
```

Lists: `LIST_ADD/INSERT/REMOVE/ITEM/COUNT/CLEAR/SORT/REVERSE/CONTAINS/INDEX_OF` (1-based).
Dictionaries: `DICT_SET/ITEM/HAS/REMOVE/COUNT/CLEAR/KEYS/VALUES`.
They even nest: `DICTIONARY_OF(STRING, LIST_OF(INTEGER))` survives the macro grinder.

### 🖥️ SCREEN 13 In Your Terminal

A real 256-colour VGA framebuffer with the classic DAC palette, rendered as
ANSI 24-bit half-blocks — every character cell is two stacked pixels. Works in
conhost, Windows Terminal, and over ssh. No SDL. No dependencies. No remorse.

```cpp
SCREEN(13)                       // 320x200x256, the one true mode
PSET(160, 100, 4)                // a red pixel
LINE(0, 0, 319, 199, 9)          // Bresenham, like everyone before us
CIRCLE(160, 100, 50, 14)         // midpoint, eight octants at a time
PAINT(160, 100, 6)               // bucket fill
PALETTE(4, 63, 0, 0)             // real 6-bit DAC values
FLIP()                           // blit the frame to the terminal
```

The whole mode zoo is on board, each parking its video window at the
historically correct segment:

| Mode | Adapter | Resolution | Colours | Video segment |
|------|---------|-----------|---------|---------------|
| `SCREEN(1)` | CGA | 320×200 | 4 (cyan/magenta/white, palette 1) | `&HB800` |
| `SCREEN(2)` | CGA | 640×200 | 2 | `&HB800` |
| `SCREEN(3)` | Hercules | 720×348 | 2 (green phosphor) | `&HB000` |
| `SCREEN(7)` | EGA | 320×200 | 16 | `&HA000` |
| `SCREEN(8)` | EGA | 640×200 | 16 | `&HA000` |
| `SCREEN(9)` | EGA | 640×350 | 16 | `&HA000` |
| `SCREEN(12)` | VGA | 640×480 | 16 | `&HA000` |
| `SCREEN(13)` | VGA | 320×200 | 256 | `&HA000` |

`GRAPHICS(w, h)` is yours to abuse, `SCREEN(0)` returns to text mode.
`POINT(x, y)` reads pixels, `CLS()` wipes the framebuffer, colours mask to the
mode's depth like the attribute registers of old.

### ☢️ PEEK / POKE / DEF SEG

One flat megabyte of perfectly fake RAM with **honest real-mode address
arithmetic**: physical = segment × 16 + offset, wrapping at 1MB because the
A20 line is off in this house. `A001:0000`, `A000:0010` and `9FFF:0020` are
all the same byte — just like grandpa's 8086. The active video mode maps its
framebuffer at its real segment (`&HA000` VGA/EGA, `&HB800` CGA, `&HB000`
Hercules — spelled `0xA000` etc., C++ has feelings too), one linear byte per
pixel:

```cpp
SCREEN(13)
DEF_SEG(0xA000)
POKE(100 * 320 + 160, 12)        // yes, this draws a pixel. we're sorry. (no we're not)
PRINT(PEEK(100 * 320 + 160))     // 12
DEF_SEG(0xA001)
PRINT(PEEK(100 * 320 + 144))     // still 12 - same physical byte
```

### 🔌 OUT / INP / WAIT — the register file, lovingly faked

The VGA's most beloved ports answer like the real thing:

```cpp
OUT(0x3C8, 32)                   // DAC write index...
OUT(0x3C9, 63) OUT(0x3C9, 0) OUT(0x3C9, 0)   // ...three writes, auto-increment: palette animation works
OUT(0x3C7, 32)                   // DAC read index
PRINT(INP(0x3C9))                // 63 - read it right back
WAIT(0x3DA, 8)                   // wait for vertical retrace (the world's most cooperative retrace:
                                 // the bit toggles every read, so your demo never hangs)
```

Every other port is a junk drawer: `INP` returns whatever `OUT` last shouted at it.

### 🌀 Mode X / Y / Z — unchained melody

The demo-scene tweaks, planar semantics included. One address = four pixels;
the sequencer map mask (`OUT 3C4/3C5`, index 2) picks which planes `POKE`
writes, the read map select (`OUT 3CE/3CF`, index 4) picks which plane `PEEK`
reads:

```cpp
MODE_X()                         // 320x240x256, square pixels, Carmack approved
DEF_SEG(0xA000)
OUT(0x3C4, 2) OUT(0x3C5, 0xF)    // all four planes
POKE(0, 9)                       // writes pixels (0,0) through (3,0) in one byte. unchained!
```

`MODE_Y()` is 320×200, `MODE_Z()` 320×400. `PSET`/`POINT` stay linear because they never lied to you.

### 💸 Fake VESA — SVGA you couldn't afford in 1992

`SCREEN(0x100)` 640×400, `SCREEN(0x101)` 640×480, `SCREEN(0x103)` 800×600,
`SCREEN(0x105)` 1024×768, `SCREEN(0x107)` 1280×1024 — all 256 colours.
Anything bigger than 64KB is **bank-switched through the A000 window**:

```cpp
SCREEN(0x101)
VESA_BANK(1)                     // the window now shows the second 64KB slice
POKE(0, 7)                       // pixel 65536 = (256, 102)

OUT(0x3C4, 0x0E)                 // or do it the Trident way:
OUT(0x3C5, 3)                    // bank = 3 XOR 2 = 1. the famous quirk, faithfully reproduced,
                                 // because somebody out there still has nightmares
```

### 👾 GET / PUT sprites

```cpp
DIM(ship AS SPRITE)
GET(0, 0, 7, 7, ship)            // grab an 8x8 block
PUT(100, 50, ship, XOR)          // draw it (XOR is the default, ask any sprite)
PUT(100, 50, ship, XOR)          // ...and erase it. the oldest trick in the book
```

Verbs: `PSET`, `PRESET` (inverts), `AND`, `OR`, `XOR` — smuggled past the
preprocessor through the stringizer, which is the only reason `AND` and `OR`
survive. `GET_SPRITE`/`PUT_SPRITE` aliases exist for the cautious.

### 🎨 PAINT, extended family

```cpp
PAINT_BORDER(110, 110, 6, 5)               // flood with 6 until colour-5 walls (QB's PAINT ,,border)
PAINT_PATTERN(110, 110, CHR$(1) + CHR$(2), 1, 5)   // tiled fill: build patterns with CHR$ like it's 1991
```

Each character of the tile string is one pixel colour, `tileWidth` pixels per
row. `CGA_PALETTE(0)` flips SCREEN 1 to the muddy green/red/brown set and
back with `CGA_PALETTE(1)`.

### 🎵 SOUND & PLAY

A real Music Macro Language parser, straight out of QBasic — `Beep()` reroutes
to the sound card on modern Windows, no PC speaker required:

```cpp
PLAY("T180 O4 L8 E E P8 E P8 C E P8 G")   // you know exactly which game this is
SOUND(440, 18.2)                          // one second of concert pitch
```

Notes `A`–`G` with `#`/`+`/`-`, octaves `O0`–`O6`/`>`/`<`, lengths `L`/dots,
tempo `T`, rests `P`/`R`, note numbers `N1`–`N84` (`N37` = middle C),
articulation `MN`/`ML`/`MS`.

### 📼 DATA / READ / RESTORE

```cpp
DATA(10, 20, 30)
DATA("WALL", "FLOOR", 3.5)

DIM(x AS INTEGER)
READ(x)                          // 10 — types convert themselves
RESTORE                          // rewind the tape
```

### 📁 File Channels

```cpp
OPEN("scores.txt" FOR_OUTPUT AS 1)       // the same AS as everywhere else
PRINT_FILE(1, "HIGHSCORE ", 9000)
CLOSE_FILE(1)

OPEN("scores.txt" FOR_INPUT AS 1)
DIM(line AS STRING)
LINE_INPUT_FILE(1, line)
CLOSE_FILE(1)
```

Plus `INPUT_FILE` (comma-aware), `EOF_FILE(n)`, `FOR_APPEND`, `KILL`,
`FILE_EXISTS`. Channel numbers are yours to manage, exactly like 1989.

---

## 🧪 Math & Conversion

```cpp
CINT(x)   → (int)(x)  
CSNG(x)   → (float)(x)  
CDBL(x)   → (double)(x)  
CSTR(x)   → std::to_string(x)  
LEN(x)    → length of string (supports std::string & char*)  
RND()     → float between 0.0 and 1.0  
RANDOMIZE_TIMER() → seed with time  
TIMER()   → current time in seconds
```

---

## ☢️ What’s the Catch?

* 🧨 You’re committing crimes against both C++ and BASIC simultaneously
* 🧠 Nested `FOR` loops? We made that work with line-number macros
* 🕳️ You will enter a state of macro-induced madness
* 🧼 Your linter will cry
* 👁️‍🗨️ Your teammates will **never trust you again**

---

## ⚙️ How It Works (Madness Inside™)

This entire monstrosity operates on a volatile cocktail of macro abuse, template trickery, and type-correct chaos. We simulate structured BASIC syntax using C++ macros that expand into scoped blocks, control flow, and even variable declarations — complete with type inference and BASIC-style AS TYPE syntax. We use UNIQUE identifiers based on _ _ LINE _ _ to prevent loop collisions (so you can nest a FOR inside a FOR_STEP inside a FUNCTION and somehow sleep at night). Type conversions like CINT, CSTR, and LEN are powered by a symphony of static_casts and overload-resolved templates that pretend C++ is duck-typed — until the compiler reminds you that it’s not. Even the CLS() function rewrites the console buffer byte-by-byte via Windows API, just so we don’t call system("cls") like a peasant. It's a macro metaverse. A DSL fever dream. A header file so cursed and beautiful it probably qualifies as modern art.

---

## 💡 Why You Might Actually Use It

* Teaching programming in a friendly syntax (but secretly C++)
* When there's just no BASIC interpreter around your toolbox but a GCC at hand
* Porting old BASIC algorithms to C++ with minimal effort
* Writing **console games** like Snake, Nibbles, etc.
* Because you can. And shouldn't. But still can.

---

## 🐍 Example: Snake in BASIC.h

Just look inside this repository's **ConsoleSnake** solution to see how weird this can be.

Yes, I made this compile.
Yes, it moves a snake across the console.
No, it’s not okay. 
Yes, it’s beautiful.

---

## 🔧 Building

It's a single header — `#include "BASIC.h"` and go. The ConsoleSnake demo builds with:

```sh
# MSVC (Visual Studio 2022)
msbuild ConsoleSnake/ConsoleSnake.sln /p:Configuration=Release /p:Platform=x64

# mingw-w64 (also cross-compiles from Linux)
x86_64-w64-mingw32-g++ -std=c++17 -static -o ConsoleSnake.exe ConsoleSnake/ConsoleSnake.cpp
```

CI builds both on every push, compiles and runs every unit-test suite in
`tests/` (smoke, strings, collections, loops — all written in the dialect
itself), publishes a `nightly-yyyyMMdd` prerelease on every green main build,
and a manual dispatch of the Release workflow cuts a dated `vyyyyMMdd`
release — binaries for both architectures plus the header itself.

---

## 🧪 Final Word

**BASIC.h** is a shrine to weirdness, a playground for nostalgia, and a tool for creating retro magic inside modern compilers.

You're not just writing C++.

You're writing BASIC.
In C++.
And getting away with it.

😉
