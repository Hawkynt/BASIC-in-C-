#include "../BASIC.h"

FUNCTION(main() AS INTEGER)
  CONST(WIDTH, 40)
  CONST(HEIGHT, 20)
  CONST(MAX_LEN, 100)

  DIM_ARRAY(snakeX, MAX_LEN AS INTEGER)
  DIM_ARRAY(snakeY, MAX_LEN AS INTEGER)
  LET(direction, 0)

  RANDOMIZE_TIMER()

  SET(snakeX[0], WIDTH / 2)
  SET(snakeY[0], HEIGHT / 2)
  LET(length, 1)

  DIM(fruitX AS INTEGER)
  DIM(fruitY AS INTEGER)
  SET(fruitX, RND() * WIDTH + 1)
  SET(fruitY, RND() * HEIGHT + 1)
  DO

    // draw screen
    CLS()
    LOCATE(fruitY, fruitX)
    PRINT("O")

    FOR(index, 0, length - 1)
      LOCATE(snakeY[index], snakeX[index])
      IF(index == 0) THEN
        PRINT("@")
      ELSE
        PRINT("#")
      ENDIF
    NEXT
  
    // get input
    SELECT(INKEY())
      CASE(72) // up
        IF(NOT(direction == 2))THEN
          SET(direction, 0)
        ENDIF
      END_CASE
      CASE(77) // right
        IF(NOT(direction == 3))THEN
          SET(direction, 1)
        ENDIF
      END_CASE
      CASE(80) // down
        IF(NOT(direction == 0))THEN
          SET(direction, 2)
        ENDIF
      END_CASE
      CASE(75) // left
        IF(NOT(direction == 1))THEN
          SET(direction, 3)
        ENDIF
      END_CASE
    END_SELECT

    // move snake body
    FOR_STEP(index, length - 1, 1, -1)
      SET(snakeX[index], snakeX[index - 1])
      SET(snakeY[index], snakeY[index - 1])
    NEXT

    // move head
    IF(direction == 0) THEN
      SET(snakeY[0], snakeY[0] - 1)
    ENDIF // up
    IF(direction == 1) THEN
      SET(snakeX[0], snakeX[0] + 1)
    ENDIF // right
    IF(direction == 2) THEN
      SET(snakeY[0], snakeY[0] + 1)
    ENDIF // down
    IF(direction == 3) THEN
      SET(snakeX[0], snakeX[0] - 1)
    ENDIF // left

    // check screen wrapping
    IF(snakeX[0] < 1) THEN
      SET(snakeX[0], WIDTH)
    ENDIF
    IF(snakeX[0] > WIDTH) THEN
      SET(snakeX[0], 1)
    ENDIF
    IF(snakeY[0] < 1) THEN
      SET(snakeY[0], HEIGHT)
    ENDIF
    IF(snakeY[0] > HEIGHT) THEN
      SET(snakeY[0], 1)
    ENDIF

    // check collisions
    IF(snakeX[0] == fruitX AND snakeY[0] == fruitY) THEN
      IF(length < MAX_LEN) THEN
        SET(length, length + 1)
        SET(snakeX[length - 1], snakeX[length - 2])
        SET(snakeY[length - 1], snakeY[length - 2])
      ENDIF
      SET(fruitX, RND() * WIDTH + 1)
      SET(fruitY, RND() * HEIGHT + 1)
    ENDIF

    // check for self-collision
    FOR(index, 1, length - 1)
      IF(snakeX[0] == snakeX[index] AND snakeY[0] == snakeY[index]) THEN
        LET(text, "GAME OVER - You ate yourself!")
        LOCATE(HEIGHT, (WIDTH-LEN(text)) / 2)
        PRINT(text)
        SLEEP(2000)
        END
      ENDIF
    NEXT

    SLEEP(100)
  LOOP_FOREVER
END_FUNCTION
