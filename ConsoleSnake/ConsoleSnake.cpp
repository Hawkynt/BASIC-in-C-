// ============================================================================
// CONSOLE SNAKE — the flagship BASIC.h specimen, now in colour.
// Pure text mode (LOCATE/COLOR/PRINT, no framebuffer): a bordered arena,
// a score line, golden power-ups that come and go, and enemies that spawn
// as you feast and stumble around drunkenly. Arrows steer (they arrive as
// DOS scan codes on every OS), ESC gives up.
// ============================================================================
#include "../BASIC.h"

CONST(WIDTH = 40)
CONST(HEIGHT = 20)
CONST(MAX_LEN = 200)
CONST(MAX_ENEMIES = 4)

// playfield (1..WIDTH, 1..HEIGHT) -> screen (row y+2, column x+1)
SUB(drawAt(INTEGER x, INTEGER y, STRING glyph, INTEGER colour))
  COLOR(colour, 0)
  LOCATE(y + 2, x + 1)
  PRINT(glyph)
END_SUB

SUB(drawBorder())
  COLOR(8, 0)
  FOR(x, 0 TO WIDTH + 1)
    LOCATE(2, x + 1)
    PRINT("#")
    LOCATE(HEIGHT + 3, x + 1)
    PRINT("#")
  NEXT
  FOR(y, 0 TO HEIGHT + 1)
    LOCATE(y + 2, 1)
    PRINT("#")
    LOCATE(y + 2, WIDTH + 2)
    PRINT("#")
  NEXT
END_SUB

SUB(drawScore(INTEGER score, INTEGER enemies))
  COLOR(15, 0)
  LOCATE(1, 1)
  PRINT("SCORE ", score, "   ENEMIES ", enemies, "  ")
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
  SET(powerX = 1)
  SET(powerY = 1)
  LET(lastTailX = 1)
  LET(lastTailY = 1)
  LET(tick = 0)

  RANDOMIZE_TIMER()
  SET(snakeX[0] = WIDTH / 2)
  SET(snakeY[0] = HEIGHT / 2)
  SET(fruitX = CINT(RND() * (WIDTH - 1)) + 1)
  SET(fruitY = CINT(RND() * (HEIGHT - 1)) + 1)
  FOR(i, 0 TO MAX_ENEMIES - 1)
    SET(enemyAlive[i] = FALSE)
  NEXT

  CLS()
  drawBorder();
  DO
    INCR(tick)

    // ----- draw ------------------------------------------------------------
    drawAt(lastTailX, lastTailY, " ", 7);
    drawAt(fruitX, fruitY, "O", 12);            // bright red fruit
    IF(powerActive) THEN
      IF((tick MOD 4) < 2) THEN                 // a blinking golden prize
        drawAt(powerX, powerY, "$", 14);
      ELSE
        drawAt(powerX, powerY, "$", 6);
      ENDIF
    ENDIF
    FOR(index, 0 TO length - 1)
      IF(index == 0) THEN
        drawAt(snakeX[0], snakeY[0], "@", 10);  // bright green head
      ELSE
        drawAt(snakeX[index], snakeY[index], "o", 2);
      ENDIF
    NEXT
    FOR(i, 0 TO MAX_ENEMIES - 1)
      IF(enemyAlive[i]) THEN
        drawAt(enemyX[i], enemyY[i], "%", 13);  // magenta menace
      ENDIF
    NEXT
    drawScore(score, enemyCount);

    // ----- input -----------------------------------------------------------
    SELECT(INKEY())
      CASE(72) // up
        IF(NOT(direction == 2)) THEN
          SET(direction = 0)
        ENDIF
      END_CASE
      CASE(77) // right
        IF(NOT(direction == 3)) THEN
          SET(direction = 1)
        ENDIF
      END_CASE
      CASE(80) // down
        IF(NOT(direction == 0)) THEN
          SET(direction = 2)
        ENDIF
      END_CASE
      CASE(75) // left
        IF(NOT(direction == 1)) THEN
          SET(direction = 3)
        ENDIF
      END_CASE
      CASE(27) // ESC: surrender with dignity
        COLOR(7, 0)
        LOCATE(HEIGHT + 5, 1)
        END
      END_CASE
    END_SELECT

    // ----- move snake --------------------------------------------------------
    SET(lastTailX = snakeX[length - 1])
    SET(lastTailY = snakeY[length - 1])
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
    IF(snakeX[0] < 1) THEN
      SET(snakeX[0] = WIDTH)
    ENDIF
    IF(snakeX[0] > WIDTH) THEN
      SET(snakeX[0] = 1)
    ENDIF
    IF(snakeY[0] < 1) THEN
      SET(snakeY[0] = HEIGHT)
    ENDIF
    IF(snakeY[0] > HEIGHT) THEN
      SET(snakeY[0] = 1)
    ENDIF

    // ----- enemies wander drunkenly every other tick ---------------------------
    IF((tick MOD 2) == 0) THEN
      FOR(i, 0 TO MAX_ENEMIES - 1)
        IF(enemyAlive[i]) THEN
          drawAt(enemyX[i], enemyY[i], " ", 7);
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
          IF(enemyX[i] < 1) THEN
            SET(enemyX[i] = WIDTH)
          ENDIF
          IF(enemyX[i] > WIDTH) THEN
            SET(enemyX[i] = 1)
          ENDIF
          IF(enemyY[i] < 1) THEN
            SET(enemyY[i] = HEIGHT)
          ENDIF
          IF(enemyY[i] > HEIGHT) THEN
            SET(enemyY[i] = 1)
          ENDIF
        ENDIF
      NEXT
    ENDIF

    // ----- power-up lifecycle ---------------------------------------------------
    IF(powerActive) THEN
      DECR(powerTimer)
      IF(powerTimer <= 0) THEN
        drawAt(powerX, powerY, " ", 7);
        SET(powerActive = FALSE)
      ENDIF
    ELSEIF(RND() < 0.02f)
      SET(powerX = CINT(RND() * (WIDTH - 1)) + 1)
      SET(powerY = CINT(RND() * (HEIGHT - 1)) + 1)
      SET(powerTimer = 70)
      SET(powerActive = TRUE)
    ENDIF

    // ----- collisions --------------------------------------------------------------
    IF(snakeX[0] == fruitX AND snakeY[0] == fruitY) THEN
      INCR(score)
      INCR(fruitsEaten)
      INCR(pendingGrowth)
      SET(fruitX = CINT(RND() * (WIDTH - 1)) + 1)
      SET(fruitY = CINT(RND() * (HEIGHT - 1)) + 1)
      IF((fruitsEaten MOD 5) == 0 AND enemyCount < MAX_ENEMIES) THEN
        // every fifth fruit invites another party crasher
        FOR(i, 0 TO MAX_ENEMIES - 1)
          IF(NOT enemyAlive[i]) THEN
            SET(enemyX[i] = CINT(RND() * (WIDTH - 1)) + 1)
            SET(enemyY[i] = CINT(RND() * (HEIGHT - 1)) + 1)
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
        LET(text = "GAME OVER - a wild % got you!")
        COLOR(13, 0)
        LOCATE(HEIGHT / 2 + 2, (WIDTH - LEN(text)) / 2)
        PRINT(text)
        COLOR(7, 0)
        SLEEP(2500)
        LOCATE(HEIGHT + 5, 1)
        END
      ENDIF
    NEXT

    FOR(index, 1 TO length - 1)
      IF(snakeX[0] == snakeX[index] AND snakeY[0] == snakeY[index]) THEN
        LET(text = "GAME OVER - You ate yourself!")
        COLOR(12, 0)
        LOCATE(HEIGHT / 2 + 2, (WIDTH - LEN(text)) / 2)
        PRINT(text)
        COLOR(7, 0)
        SLEEP(2500)
        LOCATE(HEIGHT + 5, 1)
        END
      ENDIF
    NEXT

    IF(pendingGrowth > 0 AND length < MAX_LEN) THEN
      SET(snakeX[length] = snakeX[length - 1])
      SET(snakeY[length] = snakeY[length - 1])
      INCR(length)
      DECR(pendingGrowth)
    ENDIF

    // the feast quickens the pulse: faster with every point, floor at 45ms
    LET(delay = 110 - score * 3)
    IF(delay < 45) THEN
      SET(delay = 45)
    ENDIF
    SLEEP(delay)
  LOOP_FOREVER
END_FUNCTION
