// ============================================================================
// SECOND UNREALITY — a loving macro-crime tribute to the demos we grew up on:
// Second Reality, the Kukoo2 Pleasure Access BBS intro, and every copper bar
// that ever crawled across a CRT while the modem negotiated.
//
// Eleven parts: copper bars, plasma, fire, starfield, a wireframe cube,
// bouncing sprites, fireworks, scrolltext marquees in all four directions
// (font ROM in DATA, like a proper type-in listing), a Timeless-style
// tunnel over a hidden feedback backbuffer, shadebobs and a truecolor
// rotozoom.
// Everything runs on BASIC.h's fake VGA: palette animation goes through
// OUT 3C8/3C9 like a demo coder would, frames sync on the (fake but
// punctual) vertical retrace at INP(3DA), the cube reads its edge list
// from DATA statements, the tunnel composes on a hidden page via
// PAGES/PCOPY, and the rotozoom flexes the VESA truecolor headache.
// Any key skips to the next part; ESC leaves the party.
//
//   Build:  cl /EHsc /std:c++17 /O2 Demos/SecondUnreality.cpp
//      or:  x86_64-w64-mingw32-g++ -std=c++17 -O2 -static Demos/SecondUnreality.cpp
//
// Greetings fly out to everyone who ever owned a Gravis Ultrasound.
// ============================================================================
#include "../BASIC.h"

DATA(0, 1, 1, 3, 3, 2, 2, 0)                    // cube edges: bottom face,
DATA(4, 5, 5, 7, 7, 6, 6, 4)                    // top face,
DATA(0, 4, 1, 5, 2, 6, 3, 7)                    // and the pillars

// The font ROM: 29 glyphs (A-Z, space, !, .), five rows of five each,
// lovingly typed in like a 1987 listing. Read by the marquee part.
DATA(".###.", "#...#", "#####", "#...#", "#...#")  // A
DATA("####.", "#...#", "####.", "#...#", "####.")  // B
DATA(".####", "#....", "#....", "#....", ".####")  // C
DATA("####.", "#...#", "#...#", "#...#", "####.")  // D
DATA("#####", "#....", "####.", "#....", "#####")  // E
DATA("#####", "#....", "####.", "#....", "#....")  // F
DATA(".####", "#....", "#.###", "#...#", ".###.")  // G
DATA("#...#", "#...#", "#####", "#...#", "#...#")  // H
DATA("#####", "..#..", "..#..", "..#..", "#####")  // I
DATA("..###", "...#.", "...#.", "#..#.", ".##..")  // J
DATA("#...#", "#..#.", "###..", "#..#.", "#...#")  // K
DATA("#....", "#....", "#....", "#....", "#####")  // L
DATA("#...#", "##.##", "#.#.#", "#...#", "#...#")  // M
DATA("#...#", "##..#", "#.#.#", "#..##", "#...#")  // N
DATA(".###.", "#...#", "#...#", "#...#", ".###.")  // O
DATA("####.", "#...#", "####.", "#....", "#....")  // P
DATA(".###.", "#...#", "#.#.#", "#..#.", ".##.#")  // Q
DATA("####.", "#...#", "####.", "#..#.", "#...#")  // R
DATA(".####", "#....", ".###.", "....#", "####.")  // S
DATA("#####", "..#..", "..#..", "..#..", "..#..")  // T
DATA("#...#", "#...#", "#...#", "#...#", ".###.")  // U
DATA("#...#", "#...#", "#...#", ".#.#.", "..#..")  // V
DATA("#...#", "#...#", "#.#.#", "##.##", "#...#")  // W
DATA("#...#", ".#.#.", "..#..", ".#.#.", "#...#")  // X
DATA("#...#", ".#.#.", "..#..", "..#..", "..#..")  // Y
DATA("#####", "...#.", "..#..", ".#...", "#####")  // Z
DATA(".....", ".....", ".....", ".....", ".....")  // space
DATA("..#..", "..#..", "..#..", ".....", "..#..")  // !
DATA(".....", ".....", ".....", ".....", "..#..")  // .

DIM_ARRAY(g_fontRow, 145 AS STRING)             // 29 glyphs x 5 rows, filled by loadFont
LET(g_fontLoaded = FALSE)

SUB(loadFont())
  IF(g_fontLoaded) THEN
    EXIT_SUB
  ENDIF
  RESTORE
  DIM(dummy AS INTEGER)
  FOR(skip, 1 TO 24)                            // hop over the cube edges
    READ(dummy)
  NEXT
  FOR(i, 0 TO 144)
    READ(g_fontRow[i])
  NEXT
  SET(g_fontLoaded = TRUE)
END_SUB

FUNCTION(glyphOf(INTEGER code) AS INTEGER)
  IF(code >= 'A' AND code <= 'Z') THEN
    RETURN(code - 'A');
  ENDIF
  IF(code == '!') THEN
    RETURN(27);
  ENDIF
  IF(code == '.') THEN
    RETURN(28);
  ENDIF
  RETURN(26);
END_FUNCTION

SUB(drawGlyph(INTEGER glyph, INTEGER x, INTEGER y, INTEGER colour, INTEGER pixelSize))
  FOR(row, 0 TO 4)
    LET(bits = g_fontRow[glyph * 5 + row])
    FOR(column, 0 TO 4)
      IF(MID$(bits, column + 1, 1) == "#") THEN
        LINE_BF(x + column * pixelSize, y + row * pixelSize, x + column * pixelSize + pixelSize - 1, y + row * pixelSize + pixelSize - 1, colour)
      ENDIF
    NEXT
  NEXT
END_SUB

SUB(waitFrame())
  // the classic two-step: leave the current retrace, then catch the next one
  WHILE((INP(0x3DA) AND 8) != 0)
  WEND
  WAIT(0x3DA, 8)
END_SUB

FUNCTION(clamp63(INTEGER v) AS INTEGER)
  IF(v < 0) THEN
    RETURN(0);
  ENDIF
  IF(v > 63) THEN
    RETURN(63);
  ENDIF
  RETURN(v);
END_FUNCTION

// --------------------------------------------------------------- copper bars
// The screen is painted ONCE; after that only the DAC moves. Pure 3C8/3C9.
FUNCTION(copperBars() AS INTEGER)
  SCREEN(13)
  FOR(y, 0 TO 199)
    LINE(0, y, 319, y, 32 + y)                  // one palette entry per scanline
  NEXT
  FOR(frame, 1 TO 200)
    OUT(0x3C8, 32)
    FOR(row, 0 TO 199)
      DIM(r AS DOUBLE)
      DIM(g AS DOUBLE)
      DIM(b AS DOUBLE)
      SET(r = 0)
      SET(g = 0)
      SET(b = 0)
      FOR(bar, 0 TO 2)
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
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// -------------------------------------------------------------------- plasma
// Three sines walk into a framebuffer. The palette is a 256-entry rainbow.
FUNCTION(plasma() AS INTEGER)
  SCREEN(13)
  OUT(0x3C8, 0)
  FOR(i, 0 TO 255)
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0)))
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0 + 2.1)))
    OUT(0x3C9, CINT(31.5 + 31.5 * SIN(i * PI / 32.0 + 4.2)))
  NEXT
  FOR(frame, 1 TO 200)
    LET(t = frame * 0.1)
    FOR(y, 0 TO 199)
      FOR(x, 0 TO 319)
        LET(v = SIN(x * 0.06 + t) + SIN(y * 0.09 - t) + SIN((x + y) * 0.05 + t * 0.7))
        PSET(x, y, CINT(127.5 + 42.0 * v))
      NEXT
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ---------------------------------------------------------------------- fire
// THE fire. Stoke the bottom row with noise, let the heat rise and cool.
FUNCTION(fire() AS INTEGER)
  SCREEN(13)
  OUT(0x3C8, 0)                                 // black -> red -> yellow -> white
  FOR(i, 0 TO 255)
    OUT(0x3C9, clamp63(i))
    OUT(0x3C9, clamp63(i - 85))
    OUT(0x3C9, clamp63(i - 170))
  NEXT
  FOR(frame, 1 TO 220)
    FOR(x, 0 TO 319)                              // stoke the coals
      IF(RND() > 0.5f) THEN
        PSET(x, 199, 255)
      ELSE
        PSET(x, 199, 0)
      ENDIF
    NEXT
    FOR(y, 60 TO 198)                             // heat rises and cools
      FOR(x, 1 TO 318)
        LET(deeper = POINT(x, y + 2))
        IF(deeper < 0) THEN
          SET(deeper = 0)
        ENDIF
        LET(heat = CINT((POINT(x, y + 1) + POINT(x - 1, y + 1) + POINT(x + 1, y + 1) + deeper) * 0.24f))
        PSET(x, y, heat)
      NEXT
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ----------------------------------------------------------------- starfield
// Two hundred and twenty particles of pure 1993 flying at your face.
FUNCTION(starfield() AS INTEGER)
  SCREEN(13)                                    // default palette: 16..31 is the gray ramp
  CONST(STARS = 220)
  DIM_ARRAY(sx, STARS AS SINGLE)
  DIM_ARRAY(sy, STARS AS SINGLE)
  DIM_ARRAY(sz, STARS AS SINGLE)
  FOR(i, 0 TO STARS - 1)
    SET(sx[i] = (RND() - 0.5f) * 320.0f)
    SET(sy[i] = (RND() - 0.5f) * 200.0f)
    SET(sz[i] = 1.0f + RND() * 255.0f)
  NEXT
  FOR(frame, 1 TO 260)
    CLS()
    FOR(i, 0 TO STARS - 1)
      SET(sz[i] = sz[i] - 2.5f)
      LET(visible = FALSE)
      IF(sz[i] >= 1.0f) THEN
        LET(screenX = CINT(160.0f + sx[i] * 140.0f / sz[i]))
        LET(screenY = CINT(100.0f + sy[i] * 140.0f / sz[i]))
        IF(screenX >= 0 AND screenX < 320 AND screenY >= 0 AND screenY < 200) THEN
          LET(shade = 31 - CINT(sz[i] / 18.0f))  // closer = brighter
          PSET(screenX, screenY, shade)
          IF(sz[i] < 60.0f) THEN                 // near stars get girth
            PSET(screenX + 1, screenY, shade)
            PSET(screenX, screenY + 1, shade)
            PSET(screenX + 1, screenY + 1, shade)
          ENDIF
          SET(visible = TRUE)
        ENDIF
      ENDIF
      IF(NOT visible) THEN                       // back to the void with you
        SET(sx[i] = (RND() - 0.5f) * 320.0f)
        SET(sy[i] = (RND() - 0.5f) * 200.0f)
        SET(sz[i] = 200.0f + RND() * 55.0f)
      ENDIF
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ------------------------------------------------------------------- cube 3d
// The obligatory rotating wireframe cube. Its edge list lives in DATA
// statements at the top of the file, read back like level data of old.
FUNCTION(cube3d() AS INTEGER)
  SCREEN(13)
  DIM_ARRAY(edgeA, 12 AS INTEGER)
  DIM_ARRAY(edgeB, 12 AS INTEGER)
  RESTORE
  FOR(e, 0 TO 11)
    READ(edgeA[e])
    READ(edgeB[e])
  NEXT
  DIM_ARRAY(vertX, 8 AS INTEGER)
  DIM_ARRAY(vertY, 8 AS INTEGER)
  DIM_ARRAY(vertZ, 8 AS INTEGER)
  FOR(v, 0 TO 7)                                  // corners from the bit fairy
    SET(vertX[v] = ((v & 1) * 2 - 1) * 40)
    SET(vertY[v] = (((v >> 1) & 1) * 2 - 1) * 40)
    SET(vertZ[v] = (((v >> 2) & 1) * 2 - 1) * 40)
  NEXT
  FOR(frame, 1 TO 240)
    LET(a = frame * 0.035)
    LET(b = frame * 0.021)
    DIM_ARRAY(px, 8 AS INTEGER)
    DIM_ARRAY(py, 8 AS INTEGER)
    FOR(v, 0 TO 7)
      LET(x1 = vertX[v] * COS(a) - vertZ[v] * SIN(a))
      LET(z1 = vertX[v] * SIN(a) + vertZ[v] * COS(a))
      LET(y2 = vertY[v] * COS(b) - z1 * SIN(b))
      LET(z2 = vertY[v] * SIN(b) + z1 * COS(b))
      SET(px[v] = 160 + CINT(x1 * 180.0 / (z2 + 160.0)))
      SET(py[v] = 100 + CINT(y2 * 180.0 / (z2 + 160.0)))
    NEXT
    CLS()
    FOR(e, 0 TO 11)
      LINE(px[edgeA[e]], py[edgeA[e]], px[edgeB[e]], py[edgeB[e]], 32 + ((frame * 2 + e * 16) MOD 192))
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ------------------------------------------------------------ sprite bounce
// A radially shaded ball, GET once, PUT five times a frame. Rectangular
// sprite clipping included free of charge, exactly like 1991.
FUNCTION(spriteBounce() AS INTEGER)
  SCREEN(13)
  FOR(dy, -15 TO 15)                              // paint the master ball
    FOR(dx, -15 TO 15)
      LET(dist = CINT(SQR(CDBL(dx * dx + dy * dy))))
      IF(dist <= 15) THEN
        PSET(16 + dx, 16 + dy, 47 - dist)       // radial shade through the hue band
      ENDIF
    NEXT
  NEXT
  DIM(ball AS SPRITE)
  GET_SPRITE(1, 1, 31, 31, ball)
  CONST(BALLS = 5)
  DIM_ARRAY(bx, BALLS AS SINGLE)
  DIM_ARRAY(by, BALLS AS SINGLE)
  DIM_ARRAY(bvx, BALLS AS SINGLE)
  DIM_ARRAY(bvy, BALLS AS SINGLE)
  FOR(i, 0 TO BALLS - 1)
    SET(bx[i] = RND() * 280.0f)
    SET(by[i] = RND() * 160.0f)
    SET(bvx[i] = 1.0f + RND() * 2.5f)
    SET(bvy[i] = 1.0f + RND() * 2.5f)
  NEXT
  FOR(frame, 1 TO 260)
    CLS()
    FOR(i, 0 TO BALLS - 1)
      SET(bx[i] = bx[i] + bvx[i])
      SET(by[i] = by[i] + bvy[i])
      IF(bx[i] < 0 OR bx[i] > 288) THEN
        SET(bvx[i] = -bvx[i])
        SET(bx[i] = bx[i] + bvx[i])
      ENDIF
      IF(by[i] < 0 OR by[i] > 168) THEN
        SET(bvy[i] = -bvy[i])
        SET(by[i] = by[i] + bvy[i])
      ENDIF
      PUT_SPRITE(CINT(bx[i]), CINT(by[i]), ball, PSET)
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ----------------------------------------------------------------- fireworks
// Particles, gravity, and trails burned in by never quite clearing the
// screen. Explosions on a schedule, like a proper finale.
FUNCTION(fireworks() AS INTEGER)
  SCREEN(13)
  OUT(0x3C8, 0)                                 // ember palette
  FOR(i, 0 TO 255)
    OUT(0x3C9, i / 4)
    OUT(0x3C9, i * i / 1020)
    OUT(0x3C9, i / 8)
  NEXT
  CONST(MAXP = 360)
  DIM_ARRAY(fx, MAXP AS SINGLE)
  DIM_ARRAY(fy, MAXP AS SINGLE)
  DIM_ARRAY(fvx, MAXP AS SINGLE)
  DIM_ARRAY(fvy, MAXP AS SINGLE)
  DIM_ARRAY(flife, MAXP AS SINGLE)
  FOR(i, 0 TO MAXP - 1)
    SET(flife[i] = 0)
  NEXT
  LET(nextBurst = 1)
  FOR(frame, 1 TO 320)
    FOR(y, 0 TO 199)                              // fade everything: instant trails
      FOR(x, 0 TO 319)
        LET(heat = POINT(x, y))
        IF(heat > 2) THEN
          PSET(x, y, heat - 2)
        ELSEIF(heat > 0)
          PSET(x, y, 0)
        ENDIF
      NEXT
    NEXT
    IF(frame >= nextBurst) THEN                 // light the next one
      LET(ex = 40.0f + RND() * 240.0f)
      LET(ey = 30.0f + RND() * 100.0f)
      LET(spawned = 0)
      FOR(i, 0 TO MAXP - 1)
        IF(flife[i] <= 0 AND spawned < 90) THEN
          LET(angle = RND() * 2.0f * CSNG(PI))
          LET(speed = 0.5f + RND() * 2.5f)
          SET(fx[i] = ex)
          SET(fy[i] = ey)
          SET(fvx[i] = CSNG(COS(angle)) * speed)
          SET(fvy[i] = CSNG(SIN(angle)) * speed - 0.5f)
          SET(flife[i] = 40.0f + RND() * 50.0f)
          SET(spawned = spawned + 1)
        ENDIF
      NEXT
      SET(nextBurst = frame + 45)
    ENDIF
    FOR(i, 0 TO MAXP - 1)                         // physics, such as it is
      IF(flife[i] > 0) THEN
        SET(fx[i] = fx[i] + fvx[i])
        SET(fy[i] = fy[i] + fvy[i])
        SET(fvy[i] = fvy[i] + 0.04f)            // gravity never sleeps
        SET(flife[i] = flife[i] - 1)
        PSET(CINT(fx[i]), CINT(fy[i]), 160 + CINT(flife[i]))
      ENDIF
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ------------------------------------------------------------------ marquees
// Scrolltext in all four directions, because a demo without a greeting
// marquee is legally just a screensaver. Sine-wobbled, hue-cycled, rendered
// from the DATA font ROM at chunky 2x2 pixels.
FUNCTION(marquees() AS INTEGER)
  SCREEN(13)
  loadFont();
  LET(message = STRING("GREETINGS TO EVERYONE WHO EVER OWNED A GRAVIS ULTRASOUND! IT IS ALL BASIC NOW..."))
  LET(letters = LEN(message))
  CONST(CELL = 14)                              // 5x5 glyph at 2x2 pixels + spacing
  CONST(PHASE_FRAMES = 130)
  FOR(frame, 1 TO PHASE_FRAMES * 4)
    CLS()
    LET(phase = (frame - 1) / PHASE_FRAMES)     // 0: left, 1: right, 2: up, 3: down
    LET(advance = ((frame - 1) MOD PHASE_FRAMES) * 4)
    FOR(idx, 0 TO letters - 1)
      LET(glyph = glyphOf(ASC(MID$(message, idx + 1, 1))))
      LET(colour = 32 + ((frame * 2 + idx * 6) MOD 192))
      IF(phase == 0) THEN                       // right to left, the classic
        LET(x = 320 - advance + idx * CELL)
        drawGlyph(glyph, x, 90 + CINT(22.0 * SIN(x * 0.045 + frame * 0.12)), colour, 2);
      ELSEIF(phase == 1)                        // left to right, for the rebels
        LET(x = advance - letters * CELL + idx * CELL)
        drawGlyph(glyph, x, 90 + CINT(22.0 * SIN(x * 0.045 - frame * 0.12)), colour, 2);
      ELSEIF(phase == 2)                        // bottom to top, credits style
        LET(y = 200 - advance + idx * CELL)
        drawGlyph(glyph, 154 + CINT(40.0 * SIN(y * 0.03 + frame * 0.08)), y, colour, 2);
      ELSE                                      // top to bottom, gravity wins
        LET(y = advance - letters * CELL + idx * CELL)
        drawGlyph(glyph, 154 + CINT(40.0 * SIN(y * 0.03 - frame * 0.08)), y, colour, 2);
      ENDIF
    NEXT
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ----------------------------------------------------------- timeless tunnel
// For the Timeless heads: a dot tunnel layered over a zoom-feedback
// backbuffer. Page 1 is the hidden accumulator - every frame it gets pulled
// toward the viewer and faded, fresh rings are drawn on top, glow sprites
// orbit over everything, and the result is PCOPYed back for next time.
FUNCTION(timelessTunnel() AS INTEGER)
  SCREEN(13)
  OUT(0x3C8, 0)                                 // ember palette
  FOR(i, 0 TO 255)
    OUT(0x3C9, i / 4)
    OUT(0x3C9, i * i / 1020)
    OUT(0x3C9, i / 8)
  NEXT
  PAGES(2)
  ACTIVE_PAGE(0)
  FOR(dy, -7 TO 7)                                // paint a little glow orb
    FOR(dx, -7 TO 7)
      LET(dist = dx * dx + dy * dy)
      IF(dist <= 49) THEN
        PSET(8 + dx, 8 + dy, 255 - dist * 3)
      ENDIF
    NEXT
  NEXT
  DIM(orb AS SPRITE)
  GET_SPRITE(1, 1, 15, 15, orb)
  CLS()
  FOR(frame, 1 TO 280)
    FOR(y, 0 TO 199)                              // feedback: zoom the hidden page in, fading
      FOR(x, 0 TO 319)
        ACTIVE_PAGE(1)
        LET(glow = POINT(160 + CINT((x - 160) * 0.93f), 100 + CINT((y - 100) * 0.93f)))
        ACTIVE_PAGE(0)
        IF(glow > 4) THEN
          PSET(x, y, glow - 4)
        ELSE
          PSET(x, y, 0)
        ENDIF
      NEXT
    NEXT
    FOR(ring, 0 TO 7)                             // the tunnel: dot rings flying outward
      LET(depth = 286 - ((frame * 6 + ring * 32) MOD 256))
      LET(radius = 9000.0f / depth)
      FOR(dot, 0 TO 15)
        LET(angle = dot * CSNG(PI) / 8.0f + frame * 0.02f + depth * 0.01f)
        PSET(160 + CINT(radius * CSNG(COS(angle))), 100 + CINT(radius * CSNG(SIN(angle)) * 0.7f), 255)
      NEXT
    NEXT
    // a pair of orbiting glow sprites on top
    PUT_SPRITE(152 + CINT(90.0f * CSNG(COS(frame * 0.07f))), 92 + CINT(55.0f * CSNG(SIN(frame * 0.05f))), orb, OR)
    PUT_SPRITE(152 + CINT(90.0f * CSNG(COS(frame * 0.06f + 3.0f))), 92 + CINT(55.0f * CSNG(SIN(frame * 0.04f + 1.5f))), orb, OR)
    PCOPY(0, 1)                                 // remember tonight for tomorrow
    FLIP()
    waitFrame();
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ----------------------------------------------------------------- shadebobs
// Additive blobs on Lissajous orbits, burning into an ember palette.
FUNCTION(shadeBobs() AS INTEGER)
  SCREEN(13)
  OUT(0x3C8, 0)
  FOR(i, 0 TO 255)
    OUT(0x3C9, i / 4)                           // red climbs first
    OUT(0x3C9, i * i / 1020)                    // green follows quadratically
    OUT(0x3C9, i / 8)                           // blue stays shy
  NEXT
  LET(t = 0.0)
  FOR(frame, 1 TO 300)
    FOR(bob, 0 TO 2)
      LET(cx = CINT(160.0 + 110.0 * SIN(t * 1.1 + bob * 2.1)))
      LET(cy = CINT(100.0 + 70.0 * SIN(t * 1.7 + bob * 1.3)))
      FOR(dy, -7 TO 7)
        FOR(dx, -7 TO 7)
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
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

// ------------------------------------------------------------------ rotozoom
// The obligatory rotozoomer - in 65536 colours, because the VESA headache
// has to pay rent. The & 31 is a real bitmask; AND would be a lie here.
FUNCTION(rotozoom() AS INTEGER)
  SCREEN(0x10E)                                 // 320x200, 16bpp
  FOR(frame, 1 TO 200)
    LET(angle = frame * 0.04)
    LET(zoom = 0.8 + 0.6 * SIN(frame * 0.03))
    LET(ca = COS(angle) * zoom)
    LET(sa = SIN(angle) * zoom)
    FOR(y, 0 TO 199)
      FOR(x, 0 TO 319)
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
    LET(key = INKEY())
    IF(key != 0) THEN
      RETURN(key);
    ENDIF
  NEXT
  RETURN(0);
END_FUNCTION

FUNCTION(main() AS INTEGER)
  RANDOMIZE_TIMER()
  DIM(key AS INTEGER)
  SET(key = 0)
  DO
    SET(key = copperBars())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = plasma())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = fire())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = starfield())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = cube3d())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = spriteBounce())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = fireworks())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = marquees())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = timelessTunnel())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = shadeBobs())
    IF(key == 27) THEN
      BREAK;
    ENDIF
    SET(key = rotozoom())
    IF(key == 27) THEN
      BREAK;
    ENDIF
  LOOP_FOREVER
  SCREEN(0)
  CLS()
  PRINT("SECOND UNREALITY - it's all BASIC now.")
  PRINT("Greetings to everyone who ever owned a Gravis Ultrasound.")
  END
END_FUNCTION
