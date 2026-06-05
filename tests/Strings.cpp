// Unit tests for the classic string functions. Equivalence classes, boundary
// values and exceptional cases included — 1985 never had it this thorough.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  // ----- LEFT$ ---------------------------------------------------------------
  check(LEFT$("HELLO", 2) == "HE", "LEFT$ middle of string");
  check(LEFT$("HELLO", 5) == "HELLO", "LEFT$ exact length");
  check(LEFT$("HELLO", 99) == "HELLO", "LEFT$ count beyond length clamps");
  check(LEFT$("HELLO", 0) == "", "LEFT$ zero count");
  check(LEFT$("HELLO", -3) == "", "LEFT$ negative count");
  check(LEFT$("", 3) == "", "LEFT$ empty string");

  // ----- RIGHT$ --------------------------------------------------------------
  check(RIGHT$("HELLO", 3) == "LLO", "RIGHT$ middle of string");
  check(RIGHT$("HELLO", 5) == "HELLO", "RIGHT$ exact length");
  check(RIGHT$("HELLO", 99) == "HELLO", "RIGHT$ count beyond length clamps");
  check(RIGHT$("HELLO", 0) == "", "RIGHT$ zero count");
  check(RIGHT$("", 2) == "", "RIGHT$ empty string");

  // ----- MID$ ----------------------------------------------------------------
  check(MID$("HELLO", 2) == "ELLO", "MID$ from position to end");
  check(MID$("HELLO", 1) == "HELLO", "MID$ from start");
  check(MID$("HELLO", 6) == "", "MID$ start beyond length");
  check(MID$("HELLO", 0) == "HELLO", "MID$ start below 1 clamps");
  check(MID$("HELLO", 2, 3) == "ELL", "MID$ with count");
  check(MID$("HELLO", 4, 99) == "LO", "MID$ count beyond end clamps");
  check(MID$("HELLO", 2, 0) == "", "MID$ zero count");

  // ----- INSTR ---------------------------------------------------------------
  check(INSTR("BANANA", "AN") == 2, "INSTR finds first match (1-based)");
  check(INSTR("BANANA", "XYZ") == 0, "INSTR absent needle is 0");
  check(INSTR(3, "BANANA", "AN") == 4, "INSTR with start position");
  check(INSTR(99, "BANANA", "AN") == 0, "INSTR start beyond length");
  check(INSTR("BANANA", "") == 1, "INSTR empty needle returns start");

  // ----- UCASE$ / LCASE$ -------------------------------------------------------
  check(UCASE$("MiXeD 123!") == "MIXED 123!", "UCASE$ leaves digits and symbols");
  check(LCASE$("MiXeD 123!") == "mixed 123!", "LCASE$ leaves digits and symbols");
  check(UCASE$("") == "", "UCASE$ empty string");

  // ----- LTRIM$ / RTRIM$ / TRIM$ -----------------------------------------------
  check(LTRIM$("   X ") == "X ", "LTRIM$ strips left spaces only");
  check(RTRIM$(" X   ") == " X", "RTRIM$ strips right spaces only");
  check(TRIM$("   X   ") == "X", "TRIM$ strips both sides");
  check(TRIM$("      ") == "", "TRIM$ all-space string");
  check(TRIM$("X") == "X", "TRIM$ nothing to strip");

  // ----- SPACE$ / STRING$ / REPEAT$ ---------------------------------------------
  check(SPACE$(3) == "   ", "SPACE$");
  check(SPACE$(0) == "", "SPACE$ zero");
  check(SPACE$(-5) == "", "SPACE$ negative");
  check(STRING$(4, 42) == "****", "STRING$ with character code");
  check(STRING$(3, "Abc") == "AAA", "STRING$ repeats first character of string");
  check(STRING$(3, "") == "", "STRING$ with empty source");
  check(REPEAT$(3, "AB") == "ABABAB", "REPEAT$");
  check(REPEAT$(0, "AB") == "", "REPEAT$ zero count");

  // ----- CHR$ / ASC ------------------------------------------------------------
  check(CHR$(65) == "A", "CHR$");
  check(ASC("A") == 65, "ASC");
  check(ASC(CHR$(126)) == 126, "CHR$/ASC roundtrip");
  check(ASC("") == 0, "ASC of empty string");

  // ----- STR$ / VAL ------------------------------------------------------------
  check(STR$(42) == " 42", "STR$ keeps the classic sign slot");
  check(STR$(-7) == "-7", "STR$ negative");
  check(STR$(3.5) == " 3.5", "STR$ double without trailing zeros");
  check(VAL("42") == 42.0, "VAL integer");
  check(VAL(" 3.14cm") == 3.14, "VAL parses leading number, ignores tail");
  check(VAL("XYZ") == 0.0, "VAL non-numeric is 0");
  check(VAL("-2.5") == -2.5, "VAL negative");

  // ----- HEX$ / OCT$ / BIN$ -----------------------------------------------------
  check(HEX$(255) == "FF", "HEX$ uppercase");
  check(HEX$(0) == "0", "HEX$ zero");
  check(OCT$(8) == "10", "OCT$");
  check(BIN$(5) == "101", "BIN$");
  check(BIN$(0) == "0", "BIN$ zero");
  check(BIN$(255) == "11111111", "BIN$ eight bits");

  // ----- TALLY -------------------------------------------------------------------
  check(TALLY("BANANA", "AN") == 2, "TALLY counts non-overlapping matches");
  check(TALLY("BANANA", "Q") == 0, "TALLY absent needle");
  check(TALLY("BANANA", "") == 0, "TALLY empty needle");

  // ----- $ is legal in variable names too — peak BASIC ---------------------------
  DIM(greeting$ AS STRING)
  SET(greeting$ = LEFT$("HELP", 3) + LCASE$("LO"))
  check(greeting$ == "HELlo", "$-suffixed variable names");

  // ----- ASCIIZ buffers (PowerBASIC's zero-terminated strings) -------------------
  DIM_ARRAY(buffer, 8 AS ASCIIZ)
  SET(buffer[0] = 'O')
  SET(buffer[1] = 'K')
  SET(buffer[2] = 0)
  check(LEN(buffer) == 2, "ASCIIZ buffer length stops at the zero");

  // ----- composition ---------------------------------------------------------------
  check(UCASE$(MID$(TRIM$("  make it loud  "), 9, 2)) == "LO", "functions compose");

  END_OF_TESTS
END_FUNCTION
