// Smoke suite for the BASIC.h macro surface — written in the dialect itself,
// because what better way to test crimes than committing more of them.
//
// Self-checking: every check prints PASS/FAIL, the process exits non-zero if
// anything failed. No console-buffer tricks (LOCATE/COLOR) and no INPUT/INKEY
// so it runs headless on CI with redirected stdout.
#include "TestKit.h"

FUNCTION(add(INTEGER a, INTEGER b) AS INTEGER)
  RETURN(a + b);
END_FUNCTION

FUNCTION(main() AS INTEGER)
  CLS()                                       // guarded no-op when stdout is a pipe

  // ----- DIM / SET / LET / CONST ------------------------------------------
  DIM(x AS INTEGER)
  SET(x = 42)
  check(x == 42, "DIM/SET");

  LET(y = 8)
  check(y == 8, "LET");

  CONST(ANSWER = 42)
  check(ANSWER == 42, "CONST");

  DIM_ARRAY(arr, 3 AS INTEGER)
  SET(arr[0] = 1)
  SET(arr[1] = 2)
  SET(arr[2] = 4)
  check(arr[0] + arr[1] + arr[2] == 7, "DIM_ARRAY");

  // ----- IF / ELSEIF / ELSE -----------------------------------------------
  DIM(grade AS STRING)
  LET(score = 85)
  IF(score >= 90) THEN
    SET(grade = "A")
  ELSEIF(score >= 80)
    SET(grade = "B")
  ELSE
    SET(grade = "C")
  ENDIF
  check(grade == "B", "IF/ELSEIF/ELSE");

  // ----- AND / OR / NOT -----------------------------------------------------
  LET(a = 5)
  check(a > 0 AND a < 10, "AND");
  check(a < 0 OR a == 5, "OR");
  check(NOT(a == 6), "NOT");

  // ----- SELECT / CASE ------------------------------------------------------
  LET(selector = 2)
  LET(category = 0)
  SELECT(selector)
    CASE(1)
      SET(category = 10)
    END_CASE
    CASE(2)
      SET(category = 20)
    END_CASE
    CASE_ELSE
      SET(category = 99)
    END_CASE
  END_SELECT
  check(category == 20, "SELECT/CASE/CASE_ELSE");

  // ----- the loop zoo (basics; the full safari lives in Loops.cpp) ----------
  LET(sum = 0)
  FOR(i, 1, 5)
    SET(sum = sum + i)
  NEXT
  check(sum == 15, "FOR/NEXT");

  LET(stepSum = 0)
  FOR_STEP(i, 10, 1, -3)                      // 10, 7, 4, 1
    SET(stepSum = stepSum + i)
  NEXT
  check(stepSum == 22, "FOR_STEP with negative step");

  LET(cells = 0)
  FOR(row, 1, 3)
    FOR(col, 1, 4)
      SET(cells = cells + 1)
    NEXT
  NEXT
  check(cells == 12, "nested FOR");

  LET(evens = 0)
  FOR(i, 1, 10)
    IF(i == 7) THEN
      BREAK;
    ENDIF
    IF((i % 2) == 1) THEN
      CONTINUE;
    ENDIF
    SET(evens = evens + 1)
  NEXT
  check(evens == 3, "BREAK/CONTINUE");

  // ----- GOTO / LABEL -------------------------------------------------------
  LET(reached = 0)
  GOTO(skip_here);
  SET(reached = 1)
  LABEL(skip_here)
  check(reached == 0, "GOTO/LABEL");

  // ----- FUNCTION / SUB -----------------------------------------------------
  check(add(2, 3) == 5, "FUNCTION with parameters");

  // ----- core functions & conversions --------------------------------------
  DIM(s AS STRING)
  SET(s = "HELLO")
  check(LEN(s) == 5, "LEN(STRING)");
  check(LEN("ABC") == 3, "LEN(char*)");

  check(CINT(3.7) == 3, "CINT");
  check(CLNG(40) + CINT(2.9) == 42L, "CLNG");
  check(CSNG(1) / CSNG(2) == 0.5f, "CSNG");
  check(CDBL(1) / CDBL(4) == 0.25, "CDBL");
  check(CSTR(42) == STRING("42"), "CSTR");

  RANDOMIZE_TIMER()
  LET(r = RND())
  check(r >= 0.0f AND r <= 1.0f, "RND in [0,1]");

  check(TIMER() >= 0.0f, "TIMER");
  SLEEP(1)

  END_OF_TESTS
END_FUNCTION
