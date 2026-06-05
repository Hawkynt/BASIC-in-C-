// Unit tests for the Music Macro Language parser behind PLAY. The parser is
// tested directly so CI runners stay blissfully silent — nobody needs to hear
// a windows-latest agent perform scales.
#include "TestKit.h"

SUB(checkNear(DOUBLE actual, DOUBLE expected, STRING name))
  check(std::fabs(actual - expected) <= 1.0, name);
END_SUB

FUNCTION(main() AS INTEGER)
  // ----- defaults: T120, L4, O4, MN ------------------------------------------
  LET(single = __MML_PARSE("C"))
  check(LIST_COUNT(single) == 1, "one note parsed");
  checkNear(LIST_ITEM(single, 1).frequency, 523.25, "default octave 4: C is C5");
  checkNear(LIST_ITEM(single, 1).totalMilliseconds, 500, "T120 L4 quarter is 500ms");
  checkNear(LIST_ITEM(single, 1).soundMilliseconds, 437.5, "MN articulation is 7/8");

  // ----- pitch ------------------------------------------------------------------
  checkNear(LIST_ITEM(__MML_PARSE("O3 C"), 1).frequency, 261.63, "octave 3 holds middle C");
  checkNear(LIST_ITEM(__MML_PARSE("N37"), 1).frequency, 261.63, "N37 is middle C");
  checkNear(LIST_ITEM(__MML_PARSE("A"), 1).frequency, 880, "A in the default octave");
  checkNear(LIST_ITEM(__MML_PARSE("O4 > C"), 1).frequency, 1046.5, "> shifts an octave up");
  checkNear(LIST_ITEM(__MML_PARSE("O5 < C"), 1).frequency, 523.25, "< shifts an octave down");
  checkNear(LIST_ITEM(__MML_PARSE("O0 < C"), 1).frequency, 32.7, "< clamps at octave 0");
  checkNear(LIST_ITEM(__MML_PARSE("C#"), 1).frequency, 554.37, "# is sharp");
  checkNear(LIST_ITEM(__MML_PARSE("C+"), 1).frequency, 554.37, "+ is sharp too");
  checkNear(LIST_ITEM(__MML_PARSE("D-"), 1).frequency, 554.37, "- is flat");

  // ----- length and tempo ----------------------------------------------------------
  checkNear(LIST_ITEM(__MML_PARSE("L8 C"), 1).totalMilliseconds, 250, "L sets the default length");
  checkNear(LIST_ITEM(__MML_PARSE("C8"), 1).totalMilliseconds, 250, "explicit note length");
  checkNear(LIST_ITEM(__MML_PARSE("C4."), 1).totalMilliseconds, 750, "dotted note");
  checkNear(LIST_ITEM(__MML_PARSE("C4.."), 1).totalMilliseconds, 875, "double dotted note");
  checkNear(LIST_ITEM(__MML_PARSE("T60 C"), 1).totalMilliseconds, 1000, "tempo slows the quarter");

  // ----- rests -----------------------------------------------------------------------
  LET(rest = LIST_ITEM(__MML_PARSE("P4"), 1))
  check(rest.frequency == 0, "P is a rest");
  checkNear(rest.totalMilliseconds, 500, "rests have lengths");
  check(LIST_ITEM(__MML_PARSE("R8"), 1).frequency == 0, "R rests too");
  check(LIST_ITEM(__MML_PARSE("N0"), 1).frequency == 0, "N0 is a rest");

  // ----- articulation --------------------------------------------------------------------
  LET(legato = LIST_ITEM(__MML_PARSE("ML C"), 1))
  checkNear(legato.soundMilliseconds, legato.totalMilliseconds, "ML legato sounds the full slot");
  checkNear(LIST_ITEM(__MML_PARSE("MS C"), 1).soundMilliseconds, 375, "MS staccato sounds 3/4");

  // ----- robustness ------------------------------------------------------------------------
  check(LIST_COUNT(__MML_PARSE("c d e")) == 3, "lowercase works");
  check(LIST_COUNT(__MML_PARSE("C ?! C")) == 2, "junk is graciously ignored");
  check(LIST_COUNT(__MML_PARSE("")) == 0, "empty tune");
  check(LIST_COUNT(__MML_PARSE("T999 C")) == 1, "out-of-range tempo is ignored, note still plays");
  checkNear(LIST_ITEM(__MML_PARSE("T999 C"), 1).totalMilliseconds, 500, "tempo stays at default");

  // ----- a recognisable tune parses end to end ----------------------------------------------
  check(LIST_COUNT(__MML_PARSE("T180 O4 L8 E E P8 E P8 C E P8 G")) == 9, "the coin-up classic");

  // SOUND and PLAY themselves stay untested here: CI runners deserve silence.
  END_OF_TESTS
END_FUNCTION
