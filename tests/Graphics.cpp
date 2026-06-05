// Unit tests for the terminal VGA: framebuffer primitives, palette, the ANSI
// renderer — and the memory-mapped segment &HA000, because of course.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  WINDOW_FIT(FALSE)                            // deterministic frames, whatever console CI gives us

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
  GRAPHICS(160, 100)
  check(SCREEN_WIDTH() == 160 AND SCREEN_HEIGHT() == 100, "GRAPHICS sets a custom resolution");
  PSET(0, 0, 4)
  LET(frame = __VGA_RENDER())
  check(TALLY(frame, "\n") == 50, "one text row per two pixel rows");
  check(INSTR(frame, "\xE2\x96\x80") > 0, "frame uses the upper half block");
  check(INSTR(frame, "\x1b[38;2;") > 0 AND INSTR(frame, "\x1b[48;2;") > 0, "frame uses 24-bit ANSI colours");
  check(LEN(frame) > 160 * 50 * 3, "frame covers every cell");

  // ----- auto-fit: the frame scales down to the real terminal ---------------------------------------------------
  PSET(0, 0, 15)                               // a white beacon in the top-left pixel
  LET(scaled = __VGA_RENDER_INTO(80, 200))     // width-bound: 160x100 -> 80 cols, 25 rows
  check(TALLY(scaled, "\n") == 25, "render scales down uniformly to fit the width");
  check(INSTR(scaled, "\x1b[38;2;255;255;255m") > 0, "scaled frame still samples the framebuffer");
  LET(tiny = __VGA_RENDER_INTO(16, 5))         // height and width agree at scale 0.1
  check(TALLY(tiny, "\n") == 5, "render scales down to tiny terminals");
  LET(unbounded = __VGA_RENDER_INTO(0, 0))
  check(TALLY(unbounded, "\n") == 50, "no bounds, no scaling");
  LET(roomy = __VGA_RENDER_INTO(500, 500))
  check(TALLY(roomy, "\n") == 50, "bigger terminals never upscale");

  // ----- the mode zoo --------------------------------------------------------------------------------------------
  SCREEN(1)
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 200 AND SCREEN_COLORS() == 4, "SCREEN 1 is CGA 320x200x4");
  PSET(5, 0, 7)
  check(POINT(5, 0) == 3, "CGA masks colours to 0..3");
  check(__vga_palette[1][0] == 21 AND __vga_palette[1][1] == 63 AND __vga_palette[1][2] == 63, "CGA colour 1 is light cyan");
  DEF_SEG(0xB800)
  POKE(0, 3)
  check(POINT(0, 0) == 3, "CGA video lives at B800");
  DEF_SEG(0xA000)
  POKE(0, 2)
  check(POINT(0, 0) == 3, "A000 is plain RAM while CGA is on");

  SCREEN(2)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 200 AND SCREEN_COLORS() == 2, "SCREEN 2 is CGA 640x200x2");
  check(__vga_palette[1][0] == 63 AND __vga_palette[1][1] == 63 AND __vga_palette[1][2] == 63, "mode 2 is white on black");

  SCREEN(3)
  check(SCREEN_WIDTH() == 720 AND SCREEN_HEIGHT() == 348 AND SCREEN_COLORS() == 2, "SCREEN 3 is Hercules 720x348");
  check(__vga_palette[1][0] == 0 AND __vga_palette[1][1] == 63 AND __vga_palette[1][2] == 0, "green phosphor, as nature intended");
  DEF_SEG(0xB000)
  POKE(0, 1)
  check(POINT(0, 0) == 1, "Hercules video lives at B000");

  SCREEN(7)
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 200 AND SCREEN_COLORS() == 16, "SCREEN 7 is EGA 320x200x16");
  SCREEN(8)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 200 AND SCREEN_COLORS() == 16, "SCREEN 8 is EGA 640x200x16");
  SCREEN(9)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 350 AND SCREEN_COLORS() == 16, "SCREEN 9 is EGA 640x350x16");
  SCREEN(12)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 480 AND SCREEN_COLORS() == 16, "SCREEN 12 is VGA 640x480x16");
  PSET(0, 0, 200)
  check(POINT(0, 0) == 8, "16-colour modes mask to 0..15");

  // ----- real-mode address arithmetic: physical = segment * 16 + offset --------------------------------------------
  SCREEN(13)
  PSET(16, 0, 7)
  DEF_SEG(0xA001)
  check(PEEK(0) == 7, "A001:0000 is A000:0010");
  DEF_SEG(0x9FFF)
  check(PEEK(0x20) == 7, "9FFF:0020 is A000:0010 too");
  POKE(0x10, 5)                                // 9FFF:0010 = physical 0xA0000 = pixel (0,0)
  check(POINT(0, 0) == 5, "9FFF:0010 pokes the first pixel");
  DEF_SEG(0xFFFF)
  POKE(0x10, 99)                               // physical 0x100000 wraps to 0x00000
  DEF_SEG(0)
  check(PEEK(0) == 99, "FFFF:0010 wraps to 0000:0000 (the A20 line is off in this house)");

  // ----- OUT 3C8/3C9: DAC programming like a demo coder ------------------------------------------------------------
  SCREEN(13)
  OUT(0x3C8, 100)
  OUT(0x3C9, 10)
  OUT(0x3C9, 20)
  OUT(0x3C9, 30)
  check(__vga_palette[100][0] == 10 AND __vga_palette[100][1] == 20 AND __vga_palette[100][2] == 30, "OUT 3C8/3C9 programs a DAC entry");
  OUT(0x3C9, 1)
  OUT(0x3C9, 2)
  OUT(0x3C9, 3)
  check(__vga_palette[101][0] == 1, "DAC write index auto-increments");
  OUT(0x3C8, 102)
  OUT(0x3C9, 0xFF)
  check(__vga_palette[102][0] == 63, "DAC values are six bits, the rest falls off");

  OUT(0x3C7, 100)
  check(INP(0x3C9) == 10 AND INP(0x3C9) == 20 AND INP(0x3C9) == 30, "INP 3C7/3C9 reads a DAC entry back");
  check(INP(0x3C9) == 1, "DAC read index auto-increments");

  // 3DA carries real (fake) timing: a 60Hz frame with 262 scanlines. Bit 3
  // pulses during vertical retrace, bit 0 during any blanking.
  LET(sawRetrace = FALSE)
  LET(sawPicture = FALSE)
  LET(patience = TIMER() + 0.5f)
  DO
    IF((INP(0x3DA) AND 8) != 0) THEN
      SET(sawRetrace = TRUE)
    ELSE
      SET(sawPicture = TRUE)
    ENDIF
  LOOP_UNTIL((sawRetrace AND sawPicture) OR TIMER() > patience)
  check(sawRetrace AND sawPicture, "the vertical retrace pulses at 60Hz");

  LET(sawBlank = FALSE)
  LET(sawActive = FALSE)
  LET(morePatience = TIMER() + 0.5f)
  DO
    IF((INP(0x3DA) AND 1) != 0) THEN
      SET(sawBlank = TRUE)
    ELSE
      SET(sawActive = TRUE)
    ENDIF
  LOOP_UNTIL((sawBlank AND sawActive) OR TIMER() > morePatience)
  check(sawBlank AND sawActive, "the horizontal blank flickers at scanline speed");

  WAIT(0x3DA, 8)                               // must terminate within a frame
  check(TRUE, "WAIT for vertical retrace returns");

  OUT(0x123, 77)
  check(INP(0x123) == 77, "unknown ports are a junk drawer");

  // ----- mode X/Y/Z: unchained planar madness -----------------------------------------------------------------------
  MODE_X()
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 240 AND SCREEN_COLORS() == 256, "mode X is 320x240x256");
  DEF_SEG(0xA000)
  OUT(0x3C4, 2)
  OUT(0x3C5, 0xF)                              // all planes
  POKE(0, 9)
  check(POINT(0, 0) == 9 AND POINT(1, 0) == 9 AND POINT(2, 0) == 9 AND POINT(3, 0) == 9, "unchained POKE writes four pixels");
  OUT(0x3C5, 2)                                // plane 1 only
  POKE(80, 5)                                  // row 1 (stride is width/4 = 80)
  check(POINT(1, 1) == 5 AND POINT(0, 1) == 0 AND POINT(2, 1) == 0, "map mask selects the plane");
  OUT(0x3CE, 4)
  OUT(0x3CF, 1)                                // read map select: plane 1
  check(PEEK(80) == 5, "read map select reads the chosen plane");
  OUT(0x3CF, 0)
  check(PEEK(80) == 0, "and only the chosen plane");

  MODE_Y()
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 200, "mode Y is 320x200 unchained");
  MODE_Z()
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 400, "mode Z is 320x400 unchained");

  // ----- fake VESA + bank switching -----------------------------------------------------------------------------------
  SCREEN(0x101)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 480 AND SCREEN_COLORS() == 256, "VESA 101h is 640x480x256");
  DEF_SEG(0xA000)
  VESA_BANK(0)
  POKE(0, 3)
  check(POINT(0, 0) == 3, "bank 0 starts at the top");
  VESA_BANK(1)
  POKE(0, 7)                                   // physical pixel 65536 = (256, 102)
  check(POINT(256, 102) == 7, "bank 1 shows the next 64KB slice");
  check(POINT(0, 0) == 3, "bank switching leaves other banks alone");
  OUT(0x3C4, 0x0E)
  OUT(0x3C5, 3)                                // Trident bank register: 3 XOR 2 = bank 1
  POKE(1, 8)
  check(POINT(257, 102) == 8, "the Trident XOR-2 bank quirk is faithfully reproduced");
  check(INP(0x3C5) == 3, "and reads back through the same fog");

  SCREEN(0x103)
  check(SCREEN_WIDTH() == 800 AND SCREEN_HEIGHT() == 600, "VESA 103h is 800x600");
  SCREEN(0x105)
  check(SCREEN_WIDTH() == 1024 AND SCREEN_HEIGHT() == 768, "VESA 105h is 1024x768");

  // ----- truecolor: the 15/16/24bpp headache, faithfully reproduced -------------------------------------------------
  SCREEN(0x10E)
  check(SCREEN_WIDTH() == 320 AND SCREEN_HEIGHT() == 200 AND SCREEN_COLORS() == 65536, "VESA 10Eh is 320x200 in 65536 colours");
  PSET(0, 0, RGB16(255, 0, 0))
  check(POINT(0, 0) == 0xF800, "16-bit red is F800");
  check(RGB16(255, 255, 255) == 0xFFFF, "16-bit white fills every bit");
  DEF_SEG(0xA000)
  check(PEEK(1) == 0xF8 AND PEEK(0) == 0x00, "pixels are little-endian bytes - the real headache");
  POKE(0, 0x1F)                                // low byte: maximum blue
  check(POINT(0, 0) == 0xF81F, "byte pokes compose with the high byte");
  VESA_BANK(1)
  POKE(0, 0xAA)                                // byte 65536 = low byte of pixel 32768 = (128, 102)
  check(POINT(128, 102) == 0xAA, "banked byte pokes hit the right truecolor pixel");
  VESA_BANK(0)

  LET(frame16 = __VGA_RENDER())
  check(TALLY(frame16, "\n") == 100, "truecolor frames render");

  SCREEN(0x10D)
  check(SCREEN_COLORS() == 32768, "VESA 10Dh is 15-bit");
  PSET(0, 0, RGB15(255, 255, 255))
  check(POINT(0, 0) == 0x7FFF, "15-bit white is 7FFF");

  SCREEN(0x112)
  check(SCREEN_WIDTH() == 640 AND SCREEN_HEIGHT() == 480 AND SCREEN_COLORS() == 16777216, "VESA 112h is 640x480 in 16.7M colours");
  PSET(0, 0, RGB24(10, 20, 30))
  check(POINT(0, 0) == 0x0A141E, "24-bit pixels read back whole");
  DEF_SEG(0xA000)
  check(PEEK(0) == 30 AND PEEK(1) == 20 AND PEEK(2) == 10, "24-bit little-endian byte order");

  DIM(trueSprite AS SPRITE)
  GET_SPRITE(0, 0, 0, 0, trueSprite)
  PUT_SPRITE(5, 5, trueSprite, PSET)
  check(POINT(5, 5) == 0x0A141E, "sprites carry truecolor");

  // ----- CGA palette select ---------------------------------------------------------------------------------------------
  SCREEN(1)
  CGA_PALETTE(0)
  check(__vga_palette[1][1] == 42 AND __vga_palette[2][0] == 42 AND __vga_palette[3][1] == 21, "palette 0 is green/red/brown");
  CGA_PALETTE(1)
  check(__vga_palette[1][1] == 63 AND __vga_palette[1][2] == 63, "palette 1 is back to cyan");

  // ----- GET / PUT sprites ----------------------------------------------------------------------------------------------
  SCREEN(13)
  PSET(0, 0, 1)
  PSET(1, 0, 2)
  PSET(0, 1, 3)
  PSET(1, 1, 4)
  DIM(ship AS SPRITE)
  GET_SPRITE(0, 0, 1, 1, ship)
  check(SPRITE_WIDTH(ship) == 2 AND SPRITE_HEIGHT(ship) == 2, "GET captures the rectangle");

  PUT_SPRITE(10, 10, ship, PSET)
  check(POINT(10, 10) == 1 AND POINT(11, 11) == 4, "PUT PSET stamps the sprite");

  PUT_SPRITE(20, 20, ship, XOR)
  check(POINT(20, 20) == 1, "PUT XOR on empty background draws");
  PUT_SPRITE(20, 20, ship, XOR)
  check(POINT(20, 20) == 0 AND POINT(21, 21) == 0, "PUT XOR twice erases - the oldest trick in the book");

  LINE_BF(30, 30, 31, 31, 255)
  PUT_SPRITE(30, 30, ship, AND)
  check(POINT(30, 30) == 1 AND POINT(31, 31) == 4, "PUT AND masks");
  PUT_SPRITE(40, 40, ship, OR)
  check(POINT(40, 40) == 1, "PUT OR merges");
  PUT_SPRITE(50, 50, ship, PRESET)
  check(POINT(50, 50) == 254, "PUT PRESET inverts");

  GET(60, 60, 61, 61, ship)                    // the true believers' aliases
  PUT(60, 60, ship, PSET)
  check(SPRITE_WIDTH(ship) == 2, "GET/PUT aliases work too");

  // ----- PAINT with borders and patterns ---------------------------------------------------------------------------------
  LINE_B(100, 100, 120, 120, 5)
  PSET(105, 105, 9)                            // a multicoloured interior
  PAINT_BORDER(110, 110, 6, 5)
  check(POINT(110, 110) == 6 AND POINT(105, 105) == 6, "PAINT_BORDER floods everything inside the border");
  check(POINT(100, 100) == 5 AND POINT(99, 110) == 0, "PAINT_BORDER respects the border");

  LINE_B(200, 100, 220, 120, 5)
  LET(tile = CHR$(1) + CHR$(2))                // two rows: colour 1, colour 2
  PAINT_PATTERN(210, 110, tile, 1, 5)
  LET(rowColorA = POINT(210, 110))
  LET(rowColorB = POINT(210, 111))
  check(rowColorA != rowColorB AND rowColorA + rowColorB == 3, "PAINT_PATTERN tiles by row");
  check(POINT(205, 110) == POINT(215, 110), "pattern is consistent across a row");

  // ----- video pages: the hidden backbuffer (PCOPY like it's QB 4.5) ----------------------------------------------
  SCREEN(13)
  PAGES(2)
  ACTIVE_PAGE(1)
  PSET(5, 5, 9)
  check(POINT(5, 5) == 9, "drawing lands on the active page");
  ACTIVE_PAGE(0)
  check(POINT(5, 5) == 0, "the visible page stays untouched");
  LET(visibleFrame = __VGA_RENDER())
  VISUAL_PAGE(1)
  LET(hiddenFrame = __VGA_RENDER())
  check(visibleFrame != hiddenFrame, "FLIP shows the visual page");
  VISUAL_PAGE(0)
  PCOPY(1, 0)
  check(POINT(5, 5) == 9, "PCOPY blits the hidden page across");
  ACTIVE_PAGE(1)
  CLS()
  check(POINT(5, 5) == 0, "CLS clears only the active page");
  ACTIVE_PAGE(0)
  check(POINT(5, 5) == 9, "the other page survives CLS");
  ACTIVE_PAGE(7)
  check(POINT(5, 5) == 9, "out-of-range page selects are ignored");

  // ----- back to text mode ---------------------------------------------------------------------------------------
  SCREEN(0)
  check(SCREEN_WIDTH() == 0, "SCREEN 0 returns to text mode");
  check(POINT(0, 0) == -1, "no framebuffer in text mode");

  END_OF_TESTS
END_FUNCTION
