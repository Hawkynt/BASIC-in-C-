// Unit tests for DATA / READ / RESTORE — inline data statements with a global
// cursor, exactly like the level data blocks of yore.
#include "TestKit.h"

DATA(10, 20, 30)
DATA("WALL", "FLOOR")
DATA(3.5, -7)

FUNCTION(main() AS INTEGER)
  check(DATA_REMAINING() == 7, "all DATA registered in source order");

  DIM(a AS INTEGER)
  DIM(b AS INTEGER)
  DIM(c AS INTEGER)
  READ(a)
  READ(b)
  READ(c)
  check(a == 10 AND b == 20 AND c == 30, "READ integers in DATA order");

  DIM(tile AS STRING)
  READ(tile)
  check(tile == "WALL", "READ a string");
  READ(tile)
  check(tile == "FLOOR", "READ the next string");

  DIM(d AS DOUBLE)
  READ(d)
  check(d == 3.5, "READ a double");

  DIM(negative AS INTEGER)
  READ(negative)
  check(negative == -7, "READ a negative number");

  check(DATA_REMAINING() == 0, "data exhausted");
  READ(negative)
  check(negative == 0, "reading past the end yields zero (mercy, not Error 4)");
  READ(tile)
  check(tile == "", "string past the end is empty");

  RESTORE
  check(DATA_REMAINING() == 7, "RESTORE rewinds");
  READ(negative)
  check(negative == 10, "READ after RESTORE starts over");

  END_OF_TESTS
END_FUNCTION
