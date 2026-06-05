// ============================================================================
// SECOND UNREALITY — a loving macro-crime tribute to the demos we grew up on:
// Second Reality, the Kukoo2 Pleasure Access BBS intro, and every copper bar
// that ever crawled across a CRT while the modem negotiated.
//
// Everything runs on BASIC.h's fake VGA: palette animation goes through
// OUT 3C8/3C9 like a demo coder would, frames sync on the (fake but
// punctual) vertical retrace at INP(3DA), and the rotozoom flexes the
// truecolor VESA headache. Press any key to advance the universe.
//
//   Build:  cl /EHsc /std:c++17 /O2 Demos/SecondUnreality.cpp
//      or:  x86_64-w64-mingw32-g++ -std=c++17 -O2 -static Demos/SecondUnreality.cpp
//
// Greetings fly out to everyone who ever owned a Gravis Ultrasound.
// ============================================================================
#include "../BASIC.h"

SUB(waitFrame())
  // the classic two-step: leave the current retrace, then catch the next one
  WHILE((INP(0x3DA) AND 8) != 0)
  WEND
  WAIT(0x3DA, 8)
END_SUB

// --------------------------------------------------------------- copper bars
// The screen is painted ONCE; after that only the DAC moves. Pure 3C8/3C9.
SUB(copperBars())
  SCREEN(13)
  FOR(y, 0, 199)
    LINE(0, y, 319, y, 32 + y)                  // one palette entry per scanline
  NEXT
  FOR(frame, 1, 200)
    OUT(0x3C8, 32)
    FOR(row, 0, 199)
      DIM(r AS DOUBLE)
      DIM(g AS DOUBLE)
      DIM(b AS DOUBLE)
      SET(r = 0)
      SET(g = 0)
      SET(b = 0)
      FOR(bar, 0, 2)
        LET(center = 100.0 + 85.0 * SIN(frame * 0.06 + bar * 2.1))
        LET(distance = row - center)
        LET(glow = 63.0 - distance * distance / 4.0)
        IF(glow > 0) THEN
          IF(bar == 0) THEN
            SET(r = glow)
          ENDIF
          IF(bar == 1) THEN
            SET(g = glow)
          ENDIF
          IF(bar == 2) THEN
            SET(b = glow)
          ENDIF
        ENDIF
      NEXT
      OUT(0x3C9, CINT(r))
      OUT(0x3C9, CINT(g))
      OUT(0x3C9, CINT(b))
    NEXT
    FLIP()
    waitFrame();
    IF(INKEY() != 0) THEN
      EXIT_SUB
    ENDIF
  NEXT
END_SUB

// -------------------------------------------------------------------- plasma
// Three sines walk into a framebuffer. The palette is a 256-entry rainbow.
SUB(plasma())
  SCREEN(13)
  OUT(0x3C8, 0)
  FOR(i, 0, 255)
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0)))
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0 + 2.1)))
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0 + 4.2)))
  NEXT
  FOR(frame, 1, 200)
    LET(t = frame * 0.1)
    FOR(y, 0, 199)
      FOR(x, 0, 319)
        LET(v = SIN(x * 0.06 + t) + SIN(y * 0.09 - t) + SIN((x + y) * 0.05 + t * 0.7))
        PSET(x, y, CINT(127.5 + 42.0 * v))
      NEXT
    NEXT
    FLIP()
    waitFrame();
    IF(INKEY() != 0) THEN
      EXIT_SUB
    ENDIF
  NEXT
END_SUB

// ----------------------------------------------------------------- shadebobs
// Additive blobs on Lissajous orbits, burning into an ember palette.
SUB(shadeBobs())
  SCREEN(13)
  OUT(0x3C8, 0)
  FOR(i, 0, 255)
    OUT(0x3C9, i / 4)                           // red climbs first
    OUT(0x3C9, i * i / 1020)                    // green follows quadratically
    OUT(0x3C9, i / 8)                           // blue stays shy
  NEXT
  LET(t = 0.0)
  FOR(frame, 1, 300)
    FOR(bob, 0, 2)
      LET(cx = CINT(160.0 + 110.0 * SIN(t * 1.1 + bob * 2.1)))
      LET(cy = CINT(100.0 + 70.0 * SIN(t * 1.7 + bob * 1.3)))
      FOR(dy, -7, 7)
        FOR(dx, -7, 7)
          IF(dx * dx + dy * dy <= 49) THEN
            LET(shade = POINT(cx + dx, cy + dy))
            IF(shade >= 0 AND shade < 252) THEN
              PSET(cx + dx, cy + dy, shade + 3)
            ENDIF
          ENDIF
        NEXT
      NEXT
    NEXT
    SET(t = t + 0.04)
    FLIP()
    waitFrame();
    IF(INKEY() != 0) THEN
      EXIT_SUB
    ENDIF
  NEXT
END_SUB

// ------------------------------------------------------------------ rotozoom
// The obligatory rotozoomer - in 65536 colours, because the VESA headache
// has to pay rent. The & 31 is a real bitmask; AND would be a lie here.
SUB(rotozoom())
  SCREEN(0x10E)                                 // 320x200, 16bpp
  FOR(frame, 1, 200)
    LET(angle = frame * 0.04)
    LET(zoom = 0.8 + 0.6 * SIN(frame * 0.03))
    LET(ca = COS(angle) * zoom)
    LET(sa = SIN(angle) * zoom)
    FOR(y, 0, 199)
      FOR(x, 0, 319)
        LET(u = CINT((x - 160) * ca - (y - 100) * sa) & 31)
        LET(v = CINT((x - 160) * sa + (y - 100) * ca) & 31)
        IF((u < 16) == (v < 16)) THEN
          PSET(x, y, RGB16(255 - u * 4, 32 + v * 4, 96))
        ELSE
          PSET(x, y, RGB16(16, 24 + u * 4, 128 + v * 3))
        ENDIF
      NEXT
    NEXT
    FLIP()
    waitFrame();
    IF(INKEY() != 0) THEN
      EXIT_SUB
    ENDIF
  NEXT
END_SUB

FUNCTION(main() AS INTEGER)
  DO
    copperBars();
    IF(INKEY() != 0) THEN
      BREAK;
    ENDIF
    plasma();
    IF(INKEY() != 0) THEN
      BREAK;
    ENDIF
    shadeBobs();
    IF(INKEY() != 0) THEN
      BREAK;
    ENDIF
    rotozoom();
    IF(INKEY() != 0) THEN
      BREAK;
    ENDIF
  LOOP_FOREVER
  SCREEN(0)
  CLS()
  PRINT("SECOND UNREALITY - it's all BASIC now.")
  PRINT("Greetings to everyone who ever owned a Gravis Ultrasound.")
  END
END_FUNCTION
