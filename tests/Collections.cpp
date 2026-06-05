// Unit tests for LIST_OF / DICTIONARY_OF — the anachronism aisle. No DOS-era
// BASIC had these, which is exactly why it's fun that this one does.
#include "TestKit.h"

FUNCTION(main() AS INTEGER)
  // ----- LIST: construction and counting ------------------------------------
  DIM(numbers AS LIST_OF(INTEGER))
  check(LIST_COUNT(numbers) == 0, "fresh list is empty");

  LIST_ADD(numbers, 10)
  LIST_ADD(numbers, 20)
  LIST_ADD(numbers, 30)
  check(LIST_COUNT(numbers) == 3, "LIST_ADD grows the list");
  check(LIST_ITEM(numbers, 1) == 10, "LIST_ITEM is 1-based (first)");
  check(LIST_ITEM(numbers, 3) == 30, "LIST_ITEM is 1-based (last)");

  // ----- LIST: insert / remove ------------------------------------------------
  LIST_INSERT(numbers, 1, 5)                  // 5, 10, 20, 30
  check(LIST_ITEM(numbers, 1) == 5, "LIST_INSERT at the front");
  LIST_INSERT(numbers, 3, 15)                 // 5, 10, 15, 20, 30
  check(LIST_ITEM(numbers, 3) == 15, "LIST_INSERT in the middle");
  LIST_REMOVE(numbers, 1)                     // 10, 15, 20, 30
  check(LIST_ITEM(numbers, 1) == 10 AND LIST_COUNT(numbers) == 4, "LIST_REMOVE");

  // ----- LIST: search -----------------------------------------------------------
  check(LIST_CONTAINS(numbers, 15), "LIST_CONTAINS hit");
  check(NOT LIST_CONTAINS(numbers, 999), "LIST_CONTAINS miss");
  check(LIST_INDEX_OF(numbers, 20) == 3, "LIST_INDEX_OF is 1-based");
  check(LIST_INDEX_OF(numbers, 999) == 0, "LIST_INDEX_OF miss is 0");

  // ----- LIST: sort / reverse ------------------------------------------------------
  LIST_REVERSE(numbers)                       // 30, 20, 15, 10
  check(LIST_ITEM(numbers, 1) == 30, "LIST_REVERSE");
  LIST_SORT(numbers)                          // 10, 15, 20, 30
  check(LIST_ITEM(numbers, 1) == 10 AND LIST_ITEM(numbers, 4) == 30, "LIST_SORT");

  // ----- FOR_EACH ... IN ... NEXT -----------------------------------------------------
  LET(total = 0)
  FOR_EACH(number IN numbers)
    SET(total = total + number)
  NEXT
  check(total == 75, "FOR_EACH sums the list");

  LET(visited = 0)
  DIM(empty AS LIST_OF(INTEGER))
  FOR_EACH(number IN empty)
    SET(visited = visited + 1)
  NEXT
  check(visited == 0, "FOR_EACH over an empty list");

  // ----- LIST of STRING ---------------------------------------------------------------
  DIM(words AS LIST_OF(STRING))
  LIST_ADD(words, "PRINT")
  LIST_ADD(words, "GOTO")
  LIST_ADD(words, "PEEK")
  DIM(sentence AS STRING)
  FOR_EACH(word IN words)
    SET(sentence = sentence + LEFT$(word, 1))
  NEXT
  check(sentence == "PGP", "LIST_OF(STRING) + FOR_EACH + LEFT$");

  LIST_CLEAR(words)
  check(LIST_COUNT(words) == 0, "LIST_CLEAR");

  // ----- DICTIONARY: basics --------------------------------------------------------------
  DIM(ages AS DICTIONARY_OF(STRING, INTEGER))
  check(DICT_COUNT(ages) == 0, "fresh dictionary is empty");

  DICT_SET(ages, "BASIC", 61)
  DICT_SET(ages, "C++", 41)
  check(DICT_COUNT(ages) == 2, "DICT_SET adds entries");
  check(DICT_ITEM(ages, "BASIC") == 61, "DICT_ITEM reads back");

  DICT_SET(ages, "BASIC", 62)
  check(DICT_ITEM(ages, "BASIC") == 62 AND DICT_COUNT(ages) == 2, "DICT_SET overwrites in place");

  check(DICT_HAS(ages, "C++"), "DICT_HAS hit");
  check(NOT DICT_HAS(ages, "COBOL"), "DICT_HAS miss");

  DICT_REMOVE(ages, "C++")
  check(NOT DICT_HAS(ages, "C++") AND DICT_COUNT(ages) == 1, "DICT_REMOVE");

  // ----- DICTIONARY: keys / values --------------------------------------------------------
  DICT_SET(ages, "ASM", 77)
  LET(keys = DICT_KEYS(ages))                 // std::map keeps keys sorted
  check(LIST_COUNT(keys) == 2, "DICT_KEYS count");
  check(LIST_ITEM(keys, 1) == "ASM" AND LIST_ITEM(keys, 2) == "BASIC", "DICT_KEYS are sorted");

  LET(yearSum = 0)
  FOR_EACH(years IN DICT_VALUES(ages))
    SET(yearSum = yearSum + years)
  NEXT
  check(yearSum == 139, "DICT_VALUES + FOR_EACH");

  DICT_CLEAR(ages)
  check(DICT_COUNT(ages) == 0, "DICT_CLEAR");

  // ----- composition: a dictionary of lists — macro grinder stress test -------------------
  DIM(groups AS DICTIONARY_OF(STRING, LIST_OF(INTEGER)))
  LIST_ADD(DICT_ITEM(groups, "primes"), 2)
  LIST_ADD(DICT_ITEM(groups, "primes"), 3)
  LIST_ADD(DICT_ITEM(groups, "primes"), 5)
  check(LIST_COUNT(DICT_ITEM(groups, "primes")) == 3, "DICTIONARY_OF(STRING, LIST_OF(INTEGER))");
  check(LIST_ITEM(DICT_ITEM(groups, "primes"), 3) == 5, "nested container access");

  END_OF_TESTS
END_FUNCTION
