// ============================================================================
// CONSOLE SNAKE — the flagship BASIC.h specimen.
// The arena renders as half-block cells (one character = two stacked square
// pixels), so the snake finally looks square instead of 1:2-stretched, and
// it auto-fits whatever terminal you give it. Still no SDL, still just the
// terminal: a bordered arena, a score bar of pips along the top, blinking
// golden power-ups, and magenta enemies that crash the party as you feast.
// Arrows steer (one key press = one INKEY, on every OS), ESC gives up.
// ============================================================================
#include "../BASIC.h"

CONST(FIELD_W = 40)
CONST(FIELD_H = 40)                             // square cells now, so square arena
CONST(MAX_LEN = 400)
CONST(MAX_ENEMIES = 4)
CONST(BAR_Y = 0)                                // score pips live on the top row
CONST(TOP = 2)                                  // border ring starts here

// colours straight from the EGA sixteen
CONST(C_BORDER = 8)
CONST(C_HEAD = 10)
CONST(C_BODY = 2)
CONST(C_FRUIT = 12)
CONST(C_POWER = 14)
CONST(C_POWER_DIM = 6)
CONST(C_ENEMY = 13)
CONST(C_SCORE = 15)

// game cell (0..FIELD_W-1, 0..FIELD_H-1) -> framebuffer pixel
SUB(cell(INTEGER x, INTEGER y, INTEGER colour))
  PSET(x + 1, y + TOP + 1, colour)
END_SUB

SUB(drawScoreBar(INTEGER score))
  LINE(0, BAR_Y, FIELD_W + 1, BAR_Y, 0)
  FOR(i, 0 TO score - 1)
    IF(i <= FIELD_W + 1) THEN
      PSET(i, BAR_Y, C_SCORE)                   // one pip per point, arcade style
    ENDIF
  NEXT
END_SUB

FUNCTION(main() AS INTEGER)
  DIM_ARRAY(snakeX, MAX_LEN AS INTEGER)
  DIM_ARRAY(snakeY, MAX_LEN AS INTEGER)
  DIM_ARRAY(enemyX, MAX_ENEMIES AS INTEGER)
  DIM_ARRAY(enemyY, MAX_ENEMIES AS INTEGER)
  DIM_ARRAY(enemyAlive, MAX_ENEMIES AS BOOLEAN)
  LET(direction = 1)
  LET(length = 1)
  LET(pendingGrowth = 0)
  LET(score = 0)
  LET(fruitsEaten = 0)
  LET(enemyCount = 0)
  LET(powerActive = FALSE)
  LET(powerTimer = 0)
  DIM(powerX AS INTEGER)
  DIM(powerY AS INTEGER)
  DIM(fruitX AS INTEGER)
  DIM(fruitY AS INTEGER)
  SET(powerX = 0)
  SET(powerY = 0)
  LET(tick = 0)
  LET(gameOverText = STRING(""))

  RANDOMIZE_TIMER()
  SET(snakeX[0] = FIELD_W / 2)
  SET(snakeY[0] = FIELD_H / 2)
  SET(fruitX = CINT(RND() * (FIELD_W - 1)))
  SET(fruitY = CINT(RND() * (FIELD_H - 1)))
  FOR(i, 0 TO MAX_ENEMIES - 1)
    SET(enemyAlive[i] = FALSE)
  NEXT

  GRAPHICS(FIELD_W + 2, FIELD_H + TOP + 2)      // arena + border ring + score bar
  LINE_B(0, TOP, FIELD_W + 1, FIELD_H + TOP + 1, C_BORDER)

  DO
    INCR(tick)

    // ----- input: drain the queue, the last direction wins ------------------
    DIM(key AS INTEGER)
    SET(key = INKEY())
    DO_WHILE(key != 0)
      IF(key == 72 AND direction != 2) THEN
        SET(direction = 0)
      ENDIF
      IF(key == 77 AND direction != 3) THEN
        SET(direction = 1)
      ENDIF
      IF(key == 80 AND direction != 0) THEN
        SET(direction = 2)
      ENDIF
      IF(key == 75 AND direction != 1) THEN
        SET(direction = 3)
      ENDIF
      IF(key == 27) THEN                        // ESC: surrender with dignity
        SCREEN(0)
        CLS()
        PRINT("You surrendered with a score of ", score, ".")
        END
      ENDIF
      SET(key = INKEY())
    LOOP

    // ----- move snake --------------------------------------------------------
    LET(tailX = snakeX[length - 1])
    LET(tailY = snakeY[length - 1])
    FOR(index, length - 1 TO 1 STEP -1)
      SET(snakeX[index] = snakeX[index - 1])
      SET(snakeY[index] = snakeY[index - 1])
    NEXT
    IF(direction == 0) THEN
      SET(snakeY[0] = snakeY[0] - 1)
    ENDIF
    IF(direction == 1) THEN
      SET(snakeX[0] = snakeX[0] + 1)
    ENDIF
    IF(direction == 2) THEN
      SET(snakeY[0] = snakeY[0] + 1)
    ENDIF
    IF(direction == 3) THEN
      SET(snakeX[0] = snakeX[0] - 1)
    ENDIF

    // wrap around the arena
    IF(snakeX[0] < 0) THEN
      SET(snakeX[0] = FIELD_W - 1)
    ENDIF
    IF(snakeX[0] >= FIELD_W) THEN
      SET(snakeX[0] = 0)
    ENDIF
    IF(snakeY[0] < 0) THEN
      SET(snakeY[0] = FIELD_H - 1)
    ENDIF
    IF(snakeY[0] >= FIELD_H) THEN
      SET(snakeY[0] = 0)
    ENDIF

    // ----- enemies wander drunkenly every other tick ---------------------------
    IF((tick MOD 2) == 0) THEN
      FOR(i, 0 TO MAX_ENEMIES - 1)
        IF(enemyAlive[i]) THEN
          cell(enemyX[i], enemyY[i], 0);
          LET(roll = CINT(RND() * 3.99f))
          IF(roll == 0) THEN
            SET(enemyY[i] = enemyY[i] - 1)
          ELSEIF(roll == 1)
            SET(enemyX[i] = enemyX[i] + 1)
          ELSEIF(roll == 2)
            SET(enemyY[i] = enemyY[i] + 1)
          ELSE
            SET(enemyX[i] = enemyX[i] - 1)
          ENDIF
          IF(enemyX[i] < 0) THEN
            SET(enemyX[i] = FIELD_W - 1)
          ENDIF
          IF(enemyX[i] >= FIELD_W) THEN
            SET(enemyX[i] = 0)
          ENDIF
          IF(enemyY[i] < 0) THEN
            SET(enemyY[i] = FIELD_H - 1)
          ENDIF
          IF(enemyY[i] >= FIELD_H) THEN
            SET(enemyY[i] = 0)
          ENDIF
        ENDIF
      NEXT
    ENDIF

    // ----- power-up lifecycle ---------------------------------------------------
    IF(powerActive) THEN
      DECR(powerTimer)
      IF(powerTimer <= 0) THEN
        cell(powerX, powerY, 0);
        SET(powerActive = FALSE)
      ENDIF
    ELSEIF(RND() < 0.02f)
      SET(powerX = CINT(RND() * (FIELD_W - 1)))
      SET(powerY = CINT(RND() * (FIELD_H - 1)))
      SET(powerTimer = 90)
      SET(powerActive = TRUE)
    ENDIF

    // ----- collisions --------------------------------------------------------------
    IF(snakeX[0] == fruitX AND snakeY[0] == fruitY) THEN
      INCR(score)
      INCR(fruitsEaten)
      INCR(pendingGrowth)
      SET(fruitX = CINT(RND() * (FIELD_W - 1)))
      SET(fruitY = CINT(RND() * (FIELD_H - 1)))
      IF((fruitsEaten MOD 5) == 0 AND enemyCount < MAX_ENEMIES) THEN
        // every fifth fruit invites another party crasher
        FOR(i, 0 TO MAX_ENEMIES - 1)
          IF(NOT enemyAlive[i]) THEN
            SET(enemyX[i] = CINT(RND() * (FIELD_W - 1)))
            SET(enemyY[i] = CINT(RND() * (FIELD_H - 1)))
            SET(enemyAlive[i] = TRUE)
            INCR(enemyCount)
            EXIT_FOR
          ENDIF
        NEXT
      ENDIF
    ENDIF

    IF(powerActive AND snakeX[0] == powerX AND snakeY[0] == powerY) THEN
      INCR(score, 5)                            // the golden apple pays well
      INCR(pendingGrowth, 2)
      SET(powerActive = FALSE)
    ENDIF

    FOR(i, 0 TO MAX_ENEMIES - 1)
      IF(enemyAlive[i] AND snakeX[0] == enemyX[i] AND snakeY[0] == enemyY[i]) THEN
        SET(gameOverText = "GAME OVER - a wild enemy got you!")
      ENDIF
    NEXT
    FOR(index, 1 TO length - 1)
      IF(snakeX[0] == snakeX[index] AND snakeY[0] == snakeY[index]) THEN
        SET(gameOverText = "GAME OVER - You ate yourself!")
      ENDIF
    NEXT
    IF(LEN(gameOverText) > 0) THEN
      FLIP()
      SLEEP(800)
      SCREEN(0)
      CLS()
      PRINT(gameOverText)
      PRINT("Final score: ", score, " after ", fruitsEaten, " fruits.")
      SLEEP(2000)
      END
    ENDIF

    IF(pendingGrowth > 0 AND length < MAX_LEN) THEN
      SET(snakeX[length] = snakeX[length - 1])
      SET(snakeY[length] = snakeY[length - 1])
      INCR(length)
      DECR(pendingGrowth)
    ENDIF

    // ----- draw ----------------------------------------------------------------
    cell(tailX, tailY, 0);
    cell(fruitX, fruitY, C_FRUIT);
    IF(powerActive) THEN
      IF((tick MOD 4) < 2) THEN                 // a blinking golden prize
        cell(powerX, powerY, C_POWER);
      ELSE
        cell(powerX, powerY, C_POWER_DIM);
      ENDIF
    ENDIF
    FOR(index, 1 TO length - 1)
      cell(snakeX[index], snakeY[index], C_BODY);
    NEXT
    cell(snakeX[0], snakeY[0], C_HEAD);
    FOR(i, 0 TO MAX_ENEMIES - 1)
      IF(enemyAlive[i]) THEN
        cell(enemyX[i], enemyY[i], C_ENEMY);
      ENDIF
    NEXT
    drawScoreBar(score);
    FLIP()

    // the feast quickens the pulse: faster with every point, floor at 45ms
    LET(delay = 110 - score * 3)
    IF(delay < 45) THEN
      SET(delay = 45)
    ENDIF
    SLEEP(delay)
  LOOP_FOREVER
END_FUNCTION
