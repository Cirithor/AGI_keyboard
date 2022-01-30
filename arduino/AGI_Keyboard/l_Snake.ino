#define SNAKE_WIDTH 4096
#define SNAKE_HEIGHT SNAKE_WIDTH
#define SNAKE_X_MAX 16
#define SNAKE_Y_MAX 14

unsigned long SNAKE_GAME_DELAY = 400;      // in ms

#define SNAKE_TYPE_SNAKE 1
#define SNAKE_TYPE_OFF   2
#define SNAKE_TYPE_FOOD  3
#define SNAKE_TYPE_BLOOD 4

const byte SNAKE_DIRECTION_NONE  = 0;
const byte SNAKE_DIRECTION_UP    = 1;
const byte SNAKE_DIRECTION_DOWN  = 2;
const byte SNAKE_DIRECTION_LEFT  = 3;
const byte SNAKE_DIRECTION_RIGHT = 4;

#define SNAKE_GAME_STATE_RUNNING 1
#define SNAKE_GAME_STATE_END     2
#define SNAKE_GAME_STATE_INIT    3
#define SNAKE_GAME_STATE_PAUSED  4

#define SNAKE_MAX_TAIL_LENGTH 100
#define SNAKE_MIN_TAIL_LENGTH 3


struct snakeCoords {
  int x;
  int y;
};

int snakeCellWidth = SNAKE_WIDTH / SNAKE_X_MAX;
byte snakeUserDirection;
byte snakeUserDirectionBuffer;
byte snakeGameState;
snakeCoords snakeHead;
snakeCoords snakeTail[SNAKE_MAX_TAIL_LENGTH];
snakeCoords snakeFood;
unsigned long snakeLastDrawUpdate = 0;
unsigned int snakeWormLength = 0;
int snakePointerField = 0;
int snakePointerFood = 0;
int snakeScore = 0;

void Snake() {
  snakeResetScreen(true);
  snakeGameState = SNAKE_GAME_STATE_END;


  while (true)
  {
    switch (snakeGameState)
    {
      case SNAKE_GAME_STATE_INIT:
        snakeInitGame();
        break;
      case SNAKE_GAME_STATE_RUNNING:
        snakeCheckButtonPressed();
        snakeUpdateGame();
        break;
      case SNAKE_GAME_STATE_END:
        snakeCheckButtonPressed();
        break;
      case SNAKE_GAME_STATE_PAUSED:
        snakeCheckButtonPressed();
    }
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }
  }
}

void snakeResetScreen(bool screenInfo)
{
  XYscope.plotClear();
  XYscope.plotStart();
  XYscope.plotRectangle(0, 0, SNAKE_WIDTH - 1, SNAKE_WIDTH - 2 * snakeCellWidth);
  XYscope.printSetup(100, 3600, 400); //X;Y;Size
  XYscope.print((char *)"SCORE:");
  snakePointerField = XYscope.XYlistEnd;
  if (screenInfo) {
    XYscope.printSetup(1150, 2700, 600); //X;Y;Size
    XYscope.print((char *)"SNAKE");
    XYscope.printSetup(500, 1900, 250); //X;Y;Size
    XYscope.print((char *)"Press ENTER to Start");
    XYscope.printSetup(100, 600, 170); //X;Y;Size
    XYscope.print((char *)"ARROW KEYS - MOVE");
    XYscope.printSetup(100, 350, 170); //X;Y;Size
    XYscope.print((char *)"ESC        - EXIT");
    XYscope.printSetup(100, 100, 170); //X;Y;Size
    XYscope.print((char *)"SPACE      - PAUSE");
  }
}

void snakeInitGame()
{
  snakeResetScreen(false);
  snakeHead.x = SNAKE_X_MAX / 2;
  snakeHead.y = SNAKE_Y_MAX / 2;
  snakeFood.x = -1;
  snakeFood.y = -1;
  snakeWormLength = SNAKE_MIN_TAIL_LENGTH;
  snakeUserDirectionBuffer = SNAKE_DIRECTION_RIGHT;
  snakeUserDirection = SNAKE_DIRECTION_RIGHT;
  snakeScore = 0;

  for (int i = 0; i < SNAKE_MAX_TAIL_LENGTH; i++) {
    snakeTail[i].x = -1;
    snakeTail[i].y = -1;
  }
  snakeUpdateFood();
  snakeGameState = SNAKE_GAME_STATE_RUNNING;
}

unsigned long snakeGameDelay()
{
  unsigned long tempLength = snakeWormLength;
  unsigned long tempDelay;
  if (snakeWormLength > (SNAKE_MAX_TAIL_LENGTH / 2)) tempLength = SNAKE_MAX_TAIL_LENGTH / 2;
  //scaling speed from SNAKE_GAME_DELAY to 100ms depending on the level
  tempDelay = map (tempLength, 0u, SNAKE_MAX_TAIL_LENGTH / 2, SNAKE_GAME_DELAY, 100u);
  return tempDelay;
}

void snakeUpdateGame()
{
  if (millis() - snakeLastDrawUpdate <= snakeGameDelay()) return;
  if (snakeGameState == SNAKE_GAME_STATE_PAUSED) return;
  
  snakeUserDirection = snakeUserDirectionBuffer;
  switch (snakeUserDirection) {
    case SNAKE_DIRECTION_RIGHT:
      if (snakeHead.x >= 0) {
        snakeHead.x++;
      }
      break;
    case SNAKE_DIRECTION_LEFT:
      if (snakeHead.x < SNAKE_X_MAX) {
        snakeHead.x--;
      }
      break;
    case SNAKE_DIRECTION_DOWN:
      if (snakeHead.y > 0) {
        snakeHead.y--;
      }
      break;
    case SNAKE_DIRECTION_UP:
      if (snakeHead.y < SNAKE_Y_MAX - 1) {
        snakeHead.y++;
      }
      break;
  }
  //    Serial.print("HeadX: ");
  //    Serial.print(snakeHead.x);
  //    Serial.print(" HeadY: ");
  //    Serial.print(snakeHead.y);
  //    Serial.println();
  if (snakeIsCollision() == true) {
    snakeEndGame();
    return;
  }

  snakeUpdateTail();

  if (snakeHead.x == snakeFood.x && snakeHead.y == snakeFood.y) {
    if (snakeWormLength < SNAKE_MAX_TAIL_LENGTH) {
      snakeWormLength++;
    }
    snakeUpdateScore();
    snakeUpdateFood();
  }
  snakeDraw(snakeHead.x, snakeHead.y, SNAKE_TYPE_SNAKE);
  snakeLastDrawUpdate = millis();

}

void snakeEndGame()
{
  snakeGameState = SNAKE_GAME_STATE_END;
  snakeDraw(snakeHead.x, snakeHead.y, SNAKE_TYPE_BLOOD);
}

void snakeUpdateTail()
{
  for (int i = snakeWormLength - 1; i > 0; i--) {
    snakeTail[i].x = snakeTail[i - 1].x;
    snakeTail[i].y = snakeTail[i - 1].y;
  }
  snakeTail[0].x = snakeHead.x;
  snakeTail[0].y = snakeHead.y;
}

void snakeUpdateFood()
{
  bool found = true;
  do {
    found = true;
    snakeFood.x = random(SNAKE_X_MAX);
    snakeFood.y = random(SNAKE_Y_MAX);
    for (int i = 0; i < snakeWormLength; i++) {
      if (snakeTail[i].x == snakeFood.x && snakeTail[i].y == snakeFood.y) {
        found = false;
      }
    }
  } while (found == false);
  //  Serial.print("FoodX: ");
  //  Serial.print(snakeFood.x);
  //  Serial.print(" FoodY: ");
  //  Serial.print(snakeFood.y);
  //  Serial.println();
  snakeDraw(snakeFood.x, snakeFood.y, SNAKE_TYPE_FOOD);
}

void snakeUpdateScore()
{
  snakeScore++;
}

bool snakeIsCollision()
{
  if (snakeHead.x < 0 || snakeHead.x >= SNAKE_X_MAX) {
    //    Serial.print("Collision Horizontal Bounds");
    return true;
  }
  if (snakeHead.y < 0 || snakeHead.y >= SNAKE_Y_MAX) {
    //    Serial.print("Collision vertical bounds");
    return true;
  }
  for (int i = 1; i < snakeWormLength - 1; i++) {
    if (snakeTail[i].x == snakeHead.x && snakeTail[i].y == snakeHead.y) {
      //      Serial.print("Collision Snake Body");
      return true;
    }
  }
  return false;
}


void snakeCheckButtonPressed()
{
  usb.Task();
  if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false) {
    arrKeyPressedLast[USB_ENTER] = true;
    if (snakeGameState == SNAKE_GAME_STATE_END) {
      snakeGameState = SNAKE_GAME_STATE_INIT;
    }
    if (snakeGameState == SNAKE_GAME_STATE_PAUSED) {
      snakeGameState = SNAKE_GAME_STATE_RUNNING;
    }
  }
  if (arrKeyPressed[USB_SPACE] == true && arrKeyPressedLast[USB_SPACE] == false) {
    arrKeyPressedLast[USB_SPACE] = true;
    if (snakeGameState == SNAKE_GAME_STATE_RUNNING) {
      snakeGameState = SNAKE_GAME_STATE_PAUSED;
      snakeCheckPaused(snakeGameState);
    } 
    else if (snakeGameState == SNAKE_GAME_STATE_PAUSED){
      snakeGameState = SNAKE_GAME_STATE_RUNNING;
      snakeCheckPaused(snakeGameState);
    }
  }

  if (snakeGameState == SNAKE_GAME_STATE_PAUSED) return;

  //update snake direction not allowing 180° turn
  if (arrKeyPressed[USB_ARROWUP] == true && arrKeyPressedLast[USB_ARROWUP] == false) {
    arrKeyPressedLast[USB_ARROWUP] = true;
    if (snakeGameState == SNAKE_GAME_STATE_RUNNING && snakeUserDirection != SNAKE_DIRECTION_DOWN) {
      snakeUserDirectionBuffer = SNAKE_DIRECTION_UP;
    }
  } else   if (arrKeyPressed[USB_ARROWDOWN] == true && arrKeyPressedLast[USB_ARROWDOWN] == false) {
    arrKeyPressedLast[USB_ARROWUP] = true;
    if (snakeGameState == SNAKE_GAME_STATE_RUNNING && snakeUserDirection != SNAKE_DIRECTION_UP) {
      snakeUserDirectionBuffer = SNAKE_DIRECTION_DOWN;
    }
  } else   if (arrKeyPressed[USB_ARROWRIGHT] == true && arrKeyPressedLast[USB_ARROWRIGHT] == false) {
    arrKeyPressedLast[USB_ARROWRIGHT] = true;
    if (snakeGameState == SNAKE_GAME_STATE_RUNNING && snakeUserDirection != SNAKE_DIRECTION_LEFT) {
      snakeUserDirectionBuffer = SNAKE_DIRECTION_RIGHT;
    }
  } else   if (arrKeyPressed[USB_ARROWLEFT] == true && arrKeyPressedLast[USB_ARROWLEFT] == false) {
    arrKeyPressedLast[USB_ARROWLEFT] = true;
    if (snakeGameState == SNAKE_GAME_STATE_RUNNING && snakeUserDirection != SNAKE_DIRECTION_RIGHT) {
      snakeUserDirectionBuffer = SNAKE_DIRECTION_LEFT;
    }
  }
}

void snakeDraw(int x, int y, byte type)
{
  switch (type) {
    case SNAKE_TYPE_SNAKE:
      XYscope.XYlistEnd = snakePointerFood;
      break;
    case SNAKE_TYPE_OFF:
      XYscope.XYlistEnd = snakePointerFood;
      break;
    case SNAKE_TYPE_FOOD:
      //redraw Score and food
      XYscope.XYlistEnd = snakePointerField;
      XYscope.printSetup(1600, 3600, 400);
      XYscope.print(int(snakeScore));
      XYscope.plotCircle((snakeFood.x * snakeCellWidth) + (snakeCellWidth / 2), (snakeFood.y * snakeCellWidth) + (snakeCellWidth / 2), snakeCellWidth / 3);
      snakePointerFood = XYscope.XYlistEnd;
      break;
    case SNAKE_TYPE_BLOOD:
      XYscope.XYlistEnd = snakePointerFood;
      break;
  }

  int x1;
  int x2;
  int y1;
  int y2;

  //redraw snake tail
  for (int i = 1; i < snakeWormLength; i++) {
    if (snakeTail[i].x > -1) {
      x1 = snakeTail[i].x * snakeCellWidth;
      x2 = x1 + snakeCellWidth - 1;
      y1 = snakeTail[i].y * snakeCellWidth;
      y2 = y1 + snakeCellWidth - 1;
      XYscope.plotRectangle(x1 + 5, y1 + 5, x2 - 5, y2 - 5);
    }
  }

  if (snakeTail[0].x >= 0) {
    //boundries of snake head
    x1 = snakeTail[0].x * snakeCellWidth;
    x2 = x1 + snakeCellWidth - 1;
    y1 = snakeTail[0].y * snakeCellWidth;
    y2 = y1 + snakeCellWidth - 1;

    //draw half an ellipse as snakehead
    if (snakeUserDirection == SNAKE_DIRECTION_UP) {
      XYscope.plotLine(x1, y1, x2, y1);
      XYscope.plotEllipse(x1 + ((x2 - x1) / 2), y1, (snakeCellWidth - 1) / 2, snakeCellWidth - 1, B00001111);
    }
    if (snakeUserDirection == SNAKE_DIRECTION_DOWN) {
      XYscope.plotLine(x1, y2, x2, y2);
      XYscope.plotEllipse(x1 + ((x2 - x1) / 2), y2, (snakeCellWidth - 1) / 2, snakeCellWidth - 1, B11110000);
    }
    if (snakeUserDirection == SNAKE_DIRECTION_LEFT) {
      XYscope.plotLine(x2, y1, x2, y2);
      XYscope.plotEllipse(x2, y1 + ((y2 - y1) / 2), snakeCellWidth - 1, (snakeCellWidth - 1) / 2, B11000011);
    }
    if (snakeUserDirection == SNAKE_DIRECTION_RIGHT) {
      XYscope.plotLine(x1, y1, x1, y2);
      XYscope.plotEllipse(x1, y1 + ((y2 - y1) / 2), snakeCellWidth - 1, (snakeCellWidth - 1) / 2, B00111100);
    }
    //draw double square as snakehead
    //XYscope.plotRectangle(x1, y1, x2, y2);
    //XYscope.plotRectangle(x1 + 30, y1 + 30, x2 - 30, y2 - 30);
  }
}

void snakeCheckPaused(byte gameState)
{
  static int snakePointerSnake;
  if (gameState != SNAKE_GAME_STATE_PAUSED) {
    XYscope.XYlistEnd = snakePointerSnake;
    return;
  }
  snakePointerSnake = XYscope.XYlistEnd;
  XYscope.printSetup(2300, 3600, 400); //X;Y;Size
  XYscope.print((char *)"PAUSED");
}
