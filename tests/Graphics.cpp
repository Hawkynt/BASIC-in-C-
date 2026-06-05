// Unit tests for the terminal VGA: framebuffer primitives, palette, the ANSI
// renderer — and the memory-mapped segment &HA000, because of course.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  // ----- SCREEN 13 ------------------------------------------------------------
  SCREEN(13)
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 200, "SCREEN 13 is 320x200");
  check(POINT(0, 0) == 0, "fresh framebuffer is colour 0");

  // ----- PSET / POINT -----------------------------------------------------------
  PSET(10, 20, 4)
  check(POINT(10, 20) == 4, "PSET/POINT roundtrip");
  PSET(-1, 0, 4)
  PSET(320, 0, 4)
  PSET(0, 200, 4)
  check(POINT(-1, 0) == -1 AND POINT(320, 0) == -1 AND POINT(0, 200) == -1, "out of bounds POINT is -1");

  // ----- LINE -----------------------------------------------------------------------
  LINE(0, 5, 9, 5, 7)
  LET(allSet = TRUE)
  FOR(x, 0, 9)
    IF(POINT(x, 5) != 7) THEN
      SET(allSet = FALSE)
    ENDIF
  NEXT
  check(allSet, "horizontal LINE sets every pixel");

  LINE(30, 30, 37, 37, 9)
  check(POINT(30, 30) == 9 AND POINT(37, 37) == 9 AND POINT(33, 33) == 9, "diagonal LINE");

  // ----- LINE_B / LINE_BF ----------------------------------------------------------------
  LINE_B(50, 50, 60, 58, 3)
  check(POINT(50, 50) == 3 AND POINT(60, 58) == 3 AND POINT(55, 50) == 3, "LINE_B outline");
  check(POINT(55, 54) == 0, "LINE_B leaves the interior alone");
  LINE_BF(70, 50, 80, 58, 8)
  check(POINT(75, 54) == 8 AND POINT(70, 50) == 8, "LINE_BF fills");

  // ----- CIRCLE --------------------------------------------------------------------------------
  CIRCLE(100, 100, 10, 14)
  check(POINT(110, 100) == 14 AND POINT(90, 100) == 14, "CIRCLE east/west points");
  check(POINT(100, 110) == 14 AND POINT(100, 90) == 14, "CIRCLE north/south points");
  check(POINT(100, 100) == 0, "CIRCLE leaves the centre alone");

  // ----- PAINT ------------------------------------------------------------------------------------
  LINE_B(200, 100, 220, 120, 5)
  PAINT(210, 110, 6)
  check(POINT(210, 110) == 6 AND POINT(201, 101) == 6, "PAINT fills the interior");
  check(POINT(200, 100) == 5, "PAINT respects the border");
  check(POINT(199, 110) == 0, "PAINT stays inside");

  // ----- PEEK / POKE / DEF SEG: segment &HA000 is the framebuffer -----------------------------------
  DEF_SEG(0xA000)
  POKE(320 * 40 + 7, 12)
  check(POINT(7, 40) == 12, "POKE into A000 draws a pixel (we are so sorry)");
  PSET(8, 40, 13)
  check(PEEK(320 * 40 + 8) == 13, "PEEK from A000 reads a pixel");
  check(PEEK(320 * 200 + 5) == 0, "PEEK beyond the framebuffer is 0");
  POKE(320 * 200 + 5, 9)                       // must not crash or wrap
  DEF_SEG(0x1234)
  POKE(100, 42)
  check(PEEK(100) == 42, "other segments are plain fake RAM");
  check(POINT(100, 0) == 0, "fake RAM does not leak into video");
  DEF_SEG(0xA000)
  check(PEEK(100) == 0, "segment switch really switches");

  // ----- CLS in graphics mode --------------------------------------------------------------------------
  CLS()
  check(POINT(10, 20) == 0 AND POINT(75, 54) == 0, "CLS wipes the framebuffer");

  // ----- PALETTE -----------------------------------------------------------------------------------------
  check(__vga_palette[0][0] == 0 AND __vga_palette[0][1] == 0 AND __vga_palette[0][2] == 0, "colour 0 is black");
  check(__vga_palette[15][0] == 63 AND __vga_palette[15][1] == 63 AND __vga_palette[15][2] == 63, "colour 15 is white");
  check(__vga_palette[16][0] == 0 AND __vga_palette[31][0] == 63, "gray ramp endpoints");
  check(__vga_palette[32][0] == 0 AND __vga_palette[32][1] == 0 AND __vga_palette[32][2] == 63, "colour 32 is vivid blue");
  PALETTE(5, 1, 2, 3)
  check(__vga_palette[5][0] == 1 AND __vga_palette[5][1] == 2 AND __vga_palette[5][2] == 3, "PALETTE sets DAC values");
  PALETTE(6, 99, -5, 63)
  check(__vga_palette[6][0] == 63 AND __vga_palette[6][1] == 0 AND __vga_palette[6][2] == 63, "PALETTE clamps to 0..63");

  // ----- the ANSI renderer ------------------------------------------------------------------------------------
  SCREEN(7)
  check(SCREEN_WIDTH() == 160 AND SCREEN_HEIGHT() == 100, "SCREEN 7 is 160x100");
  PSET(0, 0, 4)
  LET(frame = __VGA_RENDER())
  check(TALLY(frame, "\n") == 50, "one text row per two pixel rows");
  check(INSTR(frame, "\xE2\x96\x80") > 0, "frame uses the upper half block");
  check(INSTR(frame, "\x1b[38;2;") > 0 AND INSTR(frame, "\x1b[48;2;") > 0, "frame uses 24-bit ANSI colours");
  check(LEN(frame) > 160 * 50 * 3, "frame covers every cell");

  // ----- back to text mode ---------------------------------------------------------------------------------------
  SCREEN(0)
  check(SCREEN_WIDTH() == 0, "SCREEN 0 returns to text mode");
  check(POINT(0, 0) == -1, "no framebuffer in text mode");

  END_OF_TESTS
END_FUNCTION
