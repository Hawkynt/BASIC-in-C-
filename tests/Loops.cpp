// Unit tests for the full DO/LOOP safari plus the PowerBASIC flow statements
// (EXIT, ITERATE, INCR/DECR, SWAP) and the MOD operator.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  // ----- DO WHILE ... LOOP (pre-test) ----------------------------------------
  LET(a = 0)
  DO_WHILE(a < 3)
    INCR(a)
  LOOP
  check(a == 3, "DO_WHILE/LOOP runs while true");

  LET(b = 0)
  DO_WHILE(b > 0)
    INCR(b)
  LOOP
  check(b == 0, "DO_WHILE/LOOP skips when false upfront");

  // ----- DO UNTIL ... LOOP (pre-test) -------------------------------------------
  LET(c = 0)
  DO_UNTIL(c >= 4)
    INCR(c)
  LOOP
  check(c == 4, "DO_UNTIL/LOOP runs until true");

  LET(d = 9)
  DO_UNTIL(d == 9)
    INCR(d)
  LOOP
  check(d == 9, "DO_UNTIL/LOOP skips when true upfront");

  // ----- DO ... LOOP WHILE (post-test) ---------------------------------------------
  LET(e = 0)
  DO
    INCR(e)
  LOOP_WHILE(e < 3)
  check(e == 3, "DO/LOOP_WHILE repeats while true");

  LET(f = 99)
  DO
    INCR(f)
  LOOP_WHILE(f < 0)
  check(f == 100, "DO/LOOP_WHILE always runs at least once");

  // ----- DO ... LOOP UNTIL (post-test) -----------------------------------------------
  LET(g = 0)
  DO
    INCR(g)
  LOOP_UNTIL(g >= 5)
  check(g == 5, "DO/LOOP_UNTIL repeats until true");

  LET(h = 7)
  DO
    INCR(h)
  LOOP_UNTIL(h > 0)
  check(h == 8, "DO/LOOP_UNTIL always runs at least once");

  // ----- WHILE ... WEND -----------------------------------------------------------------
  LET(countdown = 5)
  LET(ticks = 0)
  WHILE(countdown > 0)
    DECR(countdown)
    INCR(ticks)
  WEND
  check(ticks == 5, "WHILE/WEND");

  // ----- DO ... LOOP_FOREVER + EXIT_DO ------------------------------------------------------
  LET(spins = 0)
  DO
    INCR(spins)
    IF(spins == 7) THEN
      EXIT_DO
    ENDIF
  LOOP_FOREVER
  check(spins == 7, "LOOP_FOREVER bails out via EXIT_DO");

  // ----- ITERATE_DO ----------------------------------------------------------------------------
  LET(i = 0)
  LET(odds = 0)
  DO
    INCR(i)
    IF((i MOD 2) == 0) THEN
      ITERATE_DO
    ENDIF
    INCR(odds)
  LOOP_UNTIL(i >= 10)
  check(odds == 5, "ITERATE_DO skips to the loop condition");

  // ----- EXIT_FOR / ITERATE_FOR -------------------------------------------------------------------
  LET(visited = 0)
  FOR(k, 1, 100)
    IF(k == 4) THEN
      EXIT_FOR
    ENDIF
    INCR(visited)
  NEXT
  check(visited == 3, "EXIT_FOR");

  LET(kept = 0)
  FOR(k, 1, 9)
    IF((k MOD 3) != 0) THEN
      ITERATE_FOR
    ENDIF
    INCR(kept)
  NEXT
  check(kept == 3, "ITERATE_FOR keeps only multiples of three");

  // ----- EXIT_WHILE -----------------------------------------------------------------------------------
  LET(guard = 0)
  WHILE(true)
    INCR(guard)
    IF(guard == 2) THEN
      EXIT_WHILE
    ENDIF
  WEND
  check(guard == 2, "EXIT_WHILE");

  // ----- INCR / DECR with amounts ------------------------------------------------------------------------
  LET(value = 10)
  INCR(value)
  check(value == 11, "INCR by one");
  INCR(value, 5)
  check(value == 16, "INCR by amount");
  DECR(value)
  check(value == 15, "DECR by one");
  DECR(value, 10)
  check(value == 5, "DECR by amount");

  // ----- SWAP ------------------------------------------------------------------------------------------------
  LET(left = 1)
  LET(right = 2)
  SWAP(left, right)
  check(left == 2 AND right == 1, "SWAP integers");

  DIM(first AS STRING)
  DIM(second AS STRING)
  SET(first = "PEEK")
  SET(second = "POKE")
  SWAP(first, second)
  check(first == "POKE" AND second == "PEEK", "SWAP strings");

  // ----- MOD -------------------------------------------------------------------------------------------------------
  check(10 MOD 3 == 1, "MOD");
  check(9 MOD 3 == 0, "MOD exact division");
  check((7 MOD 2) + (8 MOD 3) == 3, "MOD composes in expressions");

  END_OF_TESTS
END_FUNCTION
