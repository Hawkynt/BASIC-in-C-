// Shared test scaffolding for the BASIC.h suites — written in the dialect
// itself, naturally. Include this, call check(...) per assertion, finish
// main() with END_OF_TESTS: it prints the summary and sets the exit code.
#pragma once
#include "../BASIC.h"

LET(failures = 0)

SUB(check(BOOLEAN condition, STRING name))
  IF(condition) THEN
    PRINT("PASS: ", name)
  ELSE
    PRINT("FAIL: ", name)
    SET(failures = failures + 1)
  ENDIF
END_SUB

#define END_OF_TESTS         \
  IF(failures == 0) THEN     \
    PRINT("ALL TESTS PASSED") \
  ELSE                       \
    PRINT(failures, " TEST(S) FAILED") \
    RETURN(1);               \
  ENDIF                      \
  END
