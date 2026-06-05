// Unit tests for the file channels: OPEN ... FOR ... AS n, PRINT #n,
// LINE INPUT #n, INPUT #n, EOF(n), CLOSE #n, KILL. Real files, real 1989.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  CONST(TESTFILE = "BasicFilesTest.tmp")
  CONST(COPYFILE = "BasicFilesTest2.tmp")
  KILL(TESTFILE)                              // clean slate even if a previous run crashed
  KILL(COPYFILE)
  check(NOT FILE_EXISTS(TESTFILE), "no leftover test file");

  // ----- write -----------------------------------------------------------------
  OPEN(TESTFILE FOR_OUTPUT AS 1)
  PRINT_FILE(1, "HELLO FILE")
  PRINT_FILE(1, 1, ", ", 2)
  PRINT_FILE(1, 42)
  CLOSE_FILE(1)
  check(FILE_EXISTS(TESTFILE), "OPEN FOR OUTPUT creates the file");

  // ----- read back ----------------------------------------------------------------
  OPEN(TESTFILE FOR_INPUT AS 1)
  DIM(line AS STRING)
  LINE_INPUT_FILE(1, line)
  check(line == "HELLO FILE", "LINE INPUT reads a full line");

  DIM(first AS INTEGER)
  DIM(second AS INTEGER)
  INPUT_FILE(1, first)
  INPUT_FILE(1, second)
  check(first == 1 AND second == 2, "INPUT reads comma-separated values");

  check(NOT EOF_FILE(1), "not EOF before the last value");
  DIM(answer AS INTEGER)
  INPUT_FILE(1, answer)
  check(answer == 42, "INPUT reads a number");
  check(EOF_FILE(1), "EOF after the last value");
  CLOSE_FILE(1)

  // ----- append --------------------------------------------------------------------
  OPEN(TESTFILE FOR_APPEND AS 2)
  PRINT_FILE(2, "POSTSCRIPT")
  CLOSE_FILE(2)

  OPEN(TESTFILE FOR_INPUT AS 3)
  LET(lines = 0)
  DO_UNTIL(EOF_FILE(3))
    LINE_INPUT_FILE(3, line)
    INCR(lines)
  LOOP
  CLOSE_FILE(3)
  check(lines == 4, "APPEND adds to the end");

  // ----- two channels at once ----------------------------------------------------------
  OPEN(TESTFILE FOR_INPUT AS 1)
  OPEN(COPYFILE FOR_OUTPUT AS 2)
  LINE_INPUT_FILE(1, line)
  PRINT_FILE(2, LCASE$(line))
  CLOSE_FILE(1)
  CLOSE_FILE(2)
  OPEN(COPYFILE FOR_INPUT AS 1)
  LINE_INPUT_FILE(1, line)
  CLOSE_FILE(1)
  check(line == "hello file", "two channels in parallel + LCASE$ on the way through");

  // ----- KILL ----------------------------------------------------------------------------
  KILL(TESTFILE)
  KILL(COPYFILE)
  check(NOT FILE_EXISTS(TESTFILE) AND NOT FILE_EXISTS(COPYFILE), "KILL removes files");

  END_OF_TESTS
END_FUNCTION
