int TetrisLevelSelection() {
  const int width = 4096;                      //"Pixels" of screen width
  const int height = 4096;                     //"Pixels" of screen height
  //level selection
  //XYscope.XYlistEnd = drawPointer [0];
  XYscope.printSetup(100, 3000, 250); //X;Y;Size
  XYscope.print((char *)"SELECT LEVEL");
  //draw grid
  int gridSize = ((width / 2) / 7);
  int gridHeight = 2000;
  //plot horizontal lines
  for (int i = 0; i < 3; i++) {
    XYscope.plotLine(gridSize , gridHeight + (i * gridSize), gridSize * 6, gridHeight + (i * gridSize));
  }
  //plot vertical lines
  for (int i = 1; i < 7; i++) {
    XYscope.plotLine(gridSize * i , gridHeight, gridSize * i, gridHeight + (2 * gridSize));
  }
  //print numbers
  for (int y = 1; y >= 0; y--) {
    for (int x = 1; x < 6; x++) {
      XYscope.printSetup(gridSize * x + 80, (gridHeight + gridSize) - (y * gridSize), 250);
      int n = (y * 5 + x) - 1;
      XYscope.print((int)n);
    }
  }
  //print info
  XYscope.printSetup(100, 600, 170); //X;Y;Size
  XYscope.print((char *)"ARROW KEYS - MOVE");
  XYscope.printSetup(100, 350, 170); //X;Y;Size
  XYscope.print((char *)"ESC        - EXIT");
  XYscope.printSetup(100, 100, 170); //X;Y;Size
  XYscope.print((char *)"SPACE      - PAUSE");

  int drawPointer = XYscope.XYlistEnd;
  //draw first selection square
  int gridPos = 0;
  XYscope.setGraphicsIntensity(100);
  XYscope.plotRectangle(gridSize + 30, gridHeight + gridSize + 30, (2 * gridSize) - 30, (gridHeight + 2 * gridSize) - 30);
  while (true) {
    usb.Task();
    bool refreshSelection = false;
    if (arrKeyPressed[USB_ARROWRIGHT] == true && arrKeyPressedLast[USB_ARROWRIGHT] == false) {
      arrKeyPressedLast[USB_ARROWRIGHT] = true;
      if (gridPos < 9) {
        gridPos++;
        refreshSelection = true;
      }
    }
    if (arrKeyPressed[USB_ARROWLEFT] == true && arrKeyPressedLast[USB_ARROWLEFT] == false) {
      arrKeyPressedLast[USB_ARROWLEFT] = true;
      if (gridPos > 0) {
        gridPos--;
        refreshSelection = true;
      }
    }
    if (arrKeyPressed[USB_ARROWDOWN] == true && arrKeyPressedLast[USB_ARROWDOWN] == false) {
      arrKeyPressedLast[USB_ARROWDOWN] = true;
      if (gridPos <= 4) {
        gridPos = gridPos + 5;
        refreshSelection = true;
      }
    }
    if (arrKeyPressed[USB_ARROWUP] == true && arrKeyPressedLast[USB_ARROWUP] == false) {
      arrKeyPressedLast[USB_ARROWUP] = true;
      if (gridPos >= 5) {
        gridPos = gridPos - 5;
        refreshSelection = true;
      }
    }
    //draw new selection square
    if (refreshSelection == true) {
      refreshSelection = false;
      XYscope.XYlistEnd = drawPointer;
      int column = gridPos;
      int row = 1;
      if (gridPos > 4) {
        column = column - 5;
        row = 0;
      }
      int x_1 = gridSize + (gridSize * column) + 30;
      int y_1 = gridHeight + (row * gridSize) + 30;
      int x_2 = gridSize + (gridSize * (column + 1)) - 30;
      int y_2 = gridHeight + ((row + 1) * gridSize) - 30;
      XYscope.plotRectangle(x_1, y_1, x_2, y_2);
    }

    //confirm selection and break;
    if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false) {
      arrKeyPressedLast[USB_ENTER] = true;
      XYscope.setGraphicsIntensity(30);
      return gridPos;
    }
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      XYscope.setGraphicsIntensity(100);
      return -1;
    }
  }
}

int tetrisNote;
void Tetris() {

  // Pieces definition
  char mPieces [7 /*kind */ ][4 /* rotation */ ][5 /* horizontal blocks */ ][5 /* vertical blocks */ ] =
  {
    // Square => 0
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      }
    },

    // I => 1
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}
      }
    },
    // L => 2
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      }
    },
    // L mirrored => 3
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      }
    },
    // N => 4
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}
      }
    },
    // N mirrored => 5
    {
      {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
      }
    },
    // T => 6
    {
      {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
      },
      {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 2, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
      }
    }
  };

  //  for (int i = 0; i < 7; i++) {
  //    for (int j = 0; j < 4; j++) {
  //      for (int k = 0; k < 5; k++) {
  //        for (int l = 0; l < 5; l++) {
  //          Serial.print(mPieces[i][j][k][l]);
  //        }
  //        Serial.println();
  //      }
  //      Serial.println();
  //    }
  //    Serial.println();
  //  }
  //  Serial.println("end");

  // Displacement of the piece to the position where it is first drawn in the board when it is created
  int mPiecesInitialPosition  [7 /*kind */ ][3 /* position */] =  //position index 0 = shift blocks in x axis / position index 1 = blocks in y axis / position index 2 = how wide is the Tetromino (x-Axis)
  {
    /* Square */

    { 2, 2, 2},

    /* I */

    { 2, 1, 4},

    /* L */

    { 2, 1, 2},

    /* L mirrored */

    { 2, 1, 2},

    /* N */

    { 1, 1, 2},

    /* N mirrored */

    { 1, 1, 2},

    /* T */

    { 2, 1, 2},

  };

  //a X and Y Offset for every Tetromino for the preview window
  double previewOffset [7][2] =
  {
    /* Square */
    { -0.5, -0.5},
    /* I */
    { -0.5, -1},
    /* L */
    {0, -0.5},
    /* L mirrored */
    {0, -0.5},
    /* N */
    {0.5, -1},
    /* N mirrored */
    {0.5, -1},
    /* T */
    {0, -0.5}
  };

  //for every level a fall delay
  //  int delayPerRow [21] {900, 830, 760, 695, 625, 560, 475, 370, 290, 190, 170, 150, 135, 120, 100, 100, 85, 85, 70, 70, 55};
  int delayPerRow [21] {900, 830, 760, 695, 625, 560, 475, 370, 290, 260, 230, 200, 180, 170, 170, 160, 160, 155, 155, 155, 150};

  XYscope.setGraphicsIntensity(30);

  const int width = 4096;                      //"Pixels" of screen width
  const int height = 4096;                     //"Pixels" of screen height
  const int boardWidth = 10;                   //in Blocks
  const int boardHeight = 20;                  //in Blocks
  const int matrixSize = 5;                    //matrix size of mPieces array
  long lastTime = 0;                           //last time the active Tetromino fall one block
  int timer = 700U;                            //delay in ms after active Tetromino fall one block
  int blockSize = 4096 / boardHeight;          //Size in Pixels
  int board [boardWidth][boardHeight];         //0 = block free; 1 = active Tetromino; 2 = active Tetromino (rotation point); 3 = fallen Tetrominos; 4 = just fallen Tetromino
  int boardBuffer [boardWidth][boardHeight];   //0 = block free; 1 = active Tetromino; 2 = active Tetromino (rotation point); 3 = fallen Tetrominos; 4 = just fallen Tetromino
  int rotPoint [2] = {0, 0};                   //rotation point of active block
  int blockSideWidth = width / boardHeight;    //Size of one side of a block in Pixels
  int blockType = 0;                           //wich kind of Tetromino is the active one
  int nextBlockType = 0;                       //wich kind of Tetromino is the next one
  int rotation = 0;                            //rotation of active Tetromino (0 to 3)
  int drawPointer [3] = {0, 0, 0};             //index 0 = Empty Board / index 1 = Fallen solid Blocks / index 2 = Info
  bool blockFalling = false;                   //Tetromino is actively falling
  int rowsCleared = 0;                         //value "Lines"
  int rowsClearedBuffer = rowsCleared;         //Buffer value "Lines"
  int level = 0;                               //value "Level"
  bool bSoftDrop = false;                      //softdropping flag
  int softDrop = 0;                            //number of continous lines softdropping
  long score = 0;                              //game score
  bool gameOver = false;                       //game Over flag

  //variables for redrawing different things
  bool draw_blockMoved = true;                //Tetromino fallen one block, steered to the left or right, rotated
  bool draw_blockFallen = true;               //Tetromino fallen on another block or bottom
  bool draw_lineDeleted = true;               //Line completed and therefore deleted

  //variables for drawing positions
  int xFieldStart = 0;
  int xFieldLength = (height / boardHeight) * boardWidth;
  int xFieldEnd = xFieldStart + xFieldLength;
  int xNextTetromino = xFieldEnd + (((width - 1) - xFieldEnd - (matrixSize * blockSize)) / 2);
  int xNextTetrominoEnd = xNextTetromino + (matrixSize * blockSize);
  int yNextTetromino = 2 * blockSize;
  int yNextTetrominoEnd = yNextTetromino + (matrixSize * blockSize);
  int xLines = xNextTetromino - (blockSize / 2);
  int xLinesEnd = xNextTetrominoEnd + (blockSize / 2);
  int yLines = (yNextTetromino + (matrixSize * blockSize) + (blockSize / 2)) + blockSize;
  int yLinesEnd = yLines + (3 * blockSize);
  int xLevel = xLines;
  int xLevelEnd = xLinesEnd;
  int yLevel = yLinesEnd + (blockSize / 2);
  int yLevelEnd = yLevel + (3 * blockSize);


  //initialize board
  for (int x = 0; x < boardWidth; x++) {
    for (int y = 0; y < boardHeight; y++) {
      board [x][y] = 0;
      boardBuffer [x][y] = 0;
      //      Serial.print(x);
      //      Serial.print(" ");
      //      Serial.print(y);
      //      Serial.print(" ");
      //      Serial.println(board [x][y]);
    }
  }

  //set first drawn random Tetromino
  randomSeed(millis());
  nextBlockType = random (0, 7);

  //draw core game field
  XYscope.plotClear();
  //XYscope.plotRectangle(0, 0, 4095, 4095);
  XYscope.plotRectangle(xFieldStart, 0, xFieldStart + xFieldLength, height - 1);
  //draw preview window
  XYscope.plotRectangle(xNextTetromino - (blockSize / 2), yNextTetromino - (blockSize / 2), xNextTetrominoEnd + (blockSize / 2) , yNextTetrominoEnd + (blockSize / 2));
  //draw Lines
  XYscope.plotRectangle(xLines, yLines, xLinesEnd, yLinesEnd);
  XYscope.printSetup(xLines + 100, (yLinesEnd - yLines) / 2 + yLines, 300); //X;Y;Size
  XYscope.print((char *)"LINES");
  //draw Level
  XYscope.plotRectangle(xLevel, yLevel, xLevelEnd, yLevelEnd);
  XYscope.printSetup(xLevel + 100, (yLevelEnd - yLevel) / 2 + yLevel, 300); //X;Y;Size
  XYscope.print((char *)"LEVEL");
  //draw Score
  int scoreHeight = height - 300 - 100;
  XYscope.printSetup(xLevel + 200, scoreHeight, 300); //X;Y;Size
  XYscope.print((char *)"SCORE");
  XYscope.plotLine(xFieldEnd, scoreHeight, width - 1, scoreHeight);
  XYscope.plotLine(xFieldEnd, scoreHeight - 400, width - 1, scoreHeight - 400);

  drawPointer [0] = XYscope.XYlistEnd;
  drawPointer [1] = XYscope.XYlistEnd;
  drawPointer [2] = XYscope.XYlistEnd;



  /*for debugging*/
  //  for (int y = boardHeight - 1; y >= 0; y--) {
  //    Serial.println();
  //    for (int x = 0; x < boardWidth; x++) {
  //      Serial.print(board [x][y]);
  //    }
  //  } Serial.println("Field initialized");



  while (true) {
    XYscope.XYlistEnd = drawPointer [0];
    level = TetrisLevelSelection();
    if (level < 0) return;
    tetrisNote = 0;
    XYscope.XYlistEnd = drawPointer [0];
    while (true) {
      while (true) {
        usb.Task();

        //exit game
        if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
          arrKeyPressedLast[USB_ESC] = true;
          XYscope.setGraphicsIntensity(100);
          return;
        }

        //pause game
        if (arrKeyPressed[USB_SPACE] == true && arrKeyPressedLast[USB_SPACE] == false) {
          arrKeyPressedLast[USB_SPACE] = true;
          int drawPointer = XYscope.XYlistEnd;
          XYscope.printSetup(150, 3500, 500); //X;Y;Size
          XYscope.print((char *)"PAUSED");
          while (true) {
            usb.Task();
            if (arrKeyPressed[USB_SPACE] == true && arrKeyPressedLast[USB_SPACE] == false) {
              arrKeyPressedLast[USB_SPACE] = true;
              XYscope.XYlistEnd = drawPointer;
              break;
            }
          }
        }

        //let the blocks fall faster
        if (arrKeyPressed[USB_ARROWDOWN] == true && arrKeyPressedLast[USB_ARROWDOWN] == false) {
          bSoftDrop = true;
          timer = 50U;
        }
        else {
          bSoftDrop = false;
          timer = delayPerRow[level];
        }
        if (!bSoftDrop) softDrop = 0;
        /*for debugging*/
        //      Serial.print("Soft-Drop: ");
        //      Serial.println(softDrop);
        if (lastTime + timer < millis() || !blockFalling) {
          lastTime = millis();

          //reset the buffer
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              boardBuffer [x][y] = board [x][y];
            }
          }

          if (!blockFalling) {
            //draw new Tetromino
            draw_blockMoved = true;
            draw_blockFallen = true;
            blockType = nextBlockType;
            nextBlockType = random (0, 7);
            rotation = 0;
            Serial.print("Block Type: ");
            Serial.println(blockType);
            int YInitialPos = mPiecesInitialPosition[blockType][0];
            int XInitialPos = mPiecesInitialPosition[blockType][1];
            int InitialPieceLength = mPiecesInitialPosition[blockType][2];
            for (int y = 0; y < matrixSize; y++) {
              for (int x = 0; x < matrixSize; x++) {
                if (y + YInitialPos < matrixSize && x + XInitialPos < matrixSize) {
                  if ( board [(boardWidth - InitialPieceLength) / 2 + x][(boardHeight - 1) - y] > 0 && mPieces [blockType][rotation][y + YInitialPos][x + XInitialPos] > 0) gameOver = true;
                  if (mPieces [blockType][rotation][y + YInitialPos][x + XInitialPos] > 0) board [(boardWidth - InitialPieceLength) / 2 + x][(boardHeight - 1) - y] = mPieces [blockType][rotation][y + YInitialPos][x + XInitialPos];
                }
              }//Horizontal Loop
            }//Vertical Loop
            if (gameOver) break;
            blockFalling = true;
            break;
          }

          if (blockFalling) {
            for (int x = 0; x < boardWidth; x++) {
              for (int y = 0; y < boardHeight; y++) {
                if (board [x][y] == 1 || board [x][y] == 2) {
                  if (y - 1 >= 0 && board [x][y - 1] == 3) blockFalling = false;
                  if (board [x][0] == 1 || board [x][0] == 2) blockFalling = false;
                }
              }
            }

            if (!blockFalling) {
              //stop soft-dropping
              if (arrKeyPressed[USB_ARROWDOWN] == true) {
                arrKeyPressedLast[USB_ARROWDOWN] = true;
                score = score + softDrop;
              }
              //"solidify" all blocks
              draw_blockFallen = true;
              //drawPointer [1] = drawPointer [2];  //set the "solid Blocks" to the "all Blocks" Pointer
              for (int x = 0; x < boardWidth; x++) {
                for (int y = 0; y < boardHeight; y++) {
                  if (board [x][y] == 1 || board [x][y] == 2) board [x][y] = 4;
                }
              }
              //check for complete rows
              int blocksPerRow = 0;
              int rowsClearedBuffer = rowsCleared;
              for (int y = 0; y < boardHeight; y++) {
                for (int x = 0; x < boardWidth; x++) {
                  if (board [x][y] >= 3) blocksPerRow++;
                }
                if (blocksPerRow == boardWidth) {
                  rowsCleared++;
                  for (int yy = y; yy < (boardHeight - 1); yy++) {
                    for (int xx = 0; xx < boardWidth; xx++) {
                      board [xx][yy] = board [xx][yy + 1];
                    }
                  }
                  y--;
                  draw_lineDeleted = true;
                }
                blocksPerRow = 0;
              }
              //calculating score from cleared lines
              if (rowsCleared - rowsClearedBuffer > 0) {
                int leveltemp = level;
                if (leveltemp > 9) leveltemp = 9;
                if (rowsCleared - rowsClearedBuffer == 1) score = score + (40L * (leveltemp + 1));
                if (rowsCleared - rowsClearedBuffer == 2) score = score + (100L * (leveltemp + 1));
                if (rowsCleared - rowsClearedBuffer == 3) score = score + (300L * (leveltemp + 1));
                if (rowsCleared - rowsClearedBuffer == 4) score = score + (1200L * (leveltemp + 1));
              }
            } else { //fall one block
              if (bSoftDrop) softDrop++;
              for (int x = 0; x < boardWidth; x++) {
                for (int y = 0; y < boardHeight; y++) {
                  if (y - 1 >= 0 && (board [x][y] == 1 || board [x][y] == 2)) {
                    board [x][y - 1] = board [x][y];
                    board [x][y] = 0;
                  }
                }
              }
              draw_blockMoved = true;
            }
          }
          break;
        }
        //do stuff in between falling
        usb.Task();

        //rotate Tetromino
        if (arrKeyPressed[USB_ARROWUP] == true && arrKeyPressedLast[USB_ARROWUP] == false && blockFalling) {
          arrKeyPressedLast[USB_ARROWUP] = true;
          bool validMove = true;
          int tempRotation = rotation;
          if (tempRotation < 3) tempRotation++;
          else tempRotation = 0;
          /*for debugging*/
          //Serial.print("rotation: ");
          //Serial.print(rotation);
          //Serial.print("tempRotation: ");
          //Serial.println(tempRotation);

          //get position of rotation point
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              if (board [x][y] == 2) {
                rotPoint [0] = x;
                rotPoint [1] = y;
              }
            }
          }
          for (int y = 0; y < matrixSize; y++) {  //defining an invalid move
            for (int x = 0; x < matrixSize; x++) {
              int boardXIndex = rotPoint [0] - 2 + x;
              int boardYIndex = rotPoint [1] - 2 + y;
              if (boardXIndex < 0 || boardYIndex < 0 || boardXIndex >= boardWidth || boardYIndex >= boardHeight) {  //if new position would be out of bound
                if (int(mPieces[blockType][tempRotation][(matrixSize - 1) - y][x] == 1) || int(mPieces[blockType][tempRotation][(matrixSize - 1) - y][x] == 2)) {   //and a block would be needed to draw there
                  /*for debugging*/
                  //Serial.println("move not valid: rotation outside of bound");
                  //Serial.print("X Index: ");
                  //Serial.print(boardXIndex);
                  //Serial.print(" Y Index: ");
                  //Serial.println(boardYIndex);
                  validMove = false;
                }
              }
              if (board[boardXIndex][boardYIndex] == 3 && (int(mPieces[blockType][tempRotation][(matrixSize - 1) - y][x] == 1) || int(mPieces[blockType][tempRotation][(matrixSize - 1) - y][x] == 2))) { //if a rotation would end in another block
                /*for debugging*/
                //Serial.println("move not valid: rotation into other block");
                //Serial.print("X Index: ");
                //Serial.print(boardXIndex);
                //Serial.print(" Y Index: ");
                //Serial.println(boardYIndex);
                validMove = false;
              }
            }//Horizontal Loop
          }//Vertical Loop
          if (validMove) {
            draw_blockMoved = true;
            rotation = tempRotation;
            /*for debugging*/
            //Serial.println("in condition 'validMove'");
            //Serial.print("rotation: ");
            //Serial.print(rotation);
            //Serial.print("tempRotation: ");
            //Serial.println(tempRotation);
            for (int y = 0; y < matrixSize; y++) {
              for (int x = 0; x < matrixSize; x++) {
                int boardXIndex = rotPoint [0] - 2 + x;
                int boardYIndex = rotPoint [1] - 2 + y;
                if (boardXIndex >= 0 && boardYIndex >= 0 && boardXIndex < boardWidth && boardYIndex < boardHeight) {
                  /*for debugging*/
                  //Serial.print("X Index:");
                  //Serial.print(boardXIndex);
                  //Serial.print(" Y Index:");
                  //Serial.println(boardYIndex);
                  if (board[boardXIndex][boardYIndex] == 1 || board[boardXIndex][boardYIndex] == 2) board[boardXIndex][boardYIndex] = 0;
                  if (int(mPieces[blockType][rotation][(matrixSize - 1) - y][x]) == 1 || int(mPieces[blockType][rotation][(matrixSize - 1) - y][x]) == 2) {
                    board[boardXIndex][boardYIndex] = int(mPieces[blockType][rotation][(matrixSize - 1) - y][x]);
                    /*for debugging*/
                    //Serial.print("X Index:");
                    //Serial.print(boardXIndex);
                    //Serial.print(" Y Index:");
                    //Serial.print(boardYIndex);
                    //Serial.print(" value:");
                    //Serial.println(int(mPieces[blockType][rotation][y][x]));
                    //Serial.println("!rotation written!");
                  }
                  //Serial.println("rotated");
                  //break; //position out of bound
                }
              }
            }//Horizontal Loop
          }//Vertical Loop
          else {
            Serial.println("move isn't valid");
          }
          break;
        }

        if (arrKeyPressed[USB_ARROWLEFT] == true && arrKeyPressedLast[USB_ARROWLEFT] == false && blockFalling) {
          arrKeyPressedLast[USB_ARROWLEFT] = true;
          bool validMove = true;
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              if (board [0][y] == 1 || board [0][y] == 2) {
                validMove = false;
                Serial.println("move isn't valid: already on left border");
              }
              if (x + 1 >= boardWidth) break;
              if (board [x][y] >= 3 && (board [x + 1][y] == 1 || board [x + 1][y] == 2)) {
                Serial.println("move isn't valid: another Block already on the left");
                validMove = false;
              }
            }
          }
          if (validMove) {
            draw_blockMoved = true;
            for (int x = 0; x < boardWidth; x++) {
              for (int y = 0; y < boardHeight; y++) {
                if (board [x][y] == 1 ||  board [x][y] == 2) {
                  board [x - 1][y] = board [x][y];
                  board [x][y] = 0;
                }
              }
            }
          }
          break;
        }

        if (arrKeyPressed[USB_ARROWRIGHT] == true && arrKeyPressedLast[USB_ARROWRIGHT] == false && blockFalling) {
          arrKeyPressedLast[USB_ARROWRIGHT] = true;
          bool validMove = true;
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              if (board [boardWidth - 1][y] == 1 || board [boardWidth - 1][y] == 2) {
                validMove = false;
                //Serial.println("move isn't valid: already on right border");
              }
              if (x - 1 < 0) break;
              if (board [x][y] >= 3 && (board [x - 1][y] == 1 || board [x - 1][y] == 2)) {
                //Serial.println("move isn't valid: another Block already on the right");
                validMove = false;
              }
            }
          }
          if (validMove) {
            draw_blockMoved = true;
            for (int x = boardWidth - 1; x >= 0; x--) {
              for (int y = 0; y < boardHeight; y++) {
                if (board [x][y] == 1 ||  board [x][y] == 2) {
                  board [x + 1][y] = board [x][y];
                  board [x][y] = 0;
                }
              }
            }
          }
          break;
        }
        tetrisMusic();
      }



      /*send current field status to the serial monitor*/
      //    for (int y = boardHeight - 1; y >= 0; y--) {
      //      Serial.println();
      //      for (int x = 0; x < boardWidth; x++) {
      //        Serial.print(board [x][y]);
      //      }
      //    } Serial.println("end");


      /*  drawPointer: index 0 = Empty Board / index 1 = Fallen solid Blocks / index 2 = Info (Score, Lines, Level, Preview Tetromino
          board [][] == 1 -> active block, board [][] == 2 -> active rotation block, board [][] == 3 -> solid block, board [][] == 4 -> just fallen solid block
      */

      if (draw_blockFallen || draw_lineDeleted || gameOver) {

        /*draw all solid blocks on game field when lines needing to be deleted*/

        if (draw_lineDeleted || gameOver) {
          drawPointer [1] = drawPointer [0];
          drawPointer [2] = drawPointer [0];
          XYscope.XYlistEnd = drawPointer [0];
          //row was cleared (also true if no solid blocks are on the game field)
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              if (gameOver && y == 4) y = 16;      //draw only rows 0 to 4 and 16 to 20 to have room for the "Game Over" text
              if (board [x][y] >= 3) {
                if (board [x][y] == 4) board [x][y] = 3;
                XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 20, y * blockSideWidth + 20, x * blockSideWidth + blockSideWidth + xFieldStart - 20, y * blockSideWidth + blockSideWidth - 20);
                XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 40, y * blockSideWidth + 40, x * blockSideWidth + blockSideWidth + xFieldStart - 40, y * blockSideWidth + blockSideWidth - 40);
              }
            }
          }
          drawPointer [1] = XYscope.XYlistEnd;    //refresh "solid blocks" Pointer after row was deleted      }
        }
        else {
          /*draw new solid blocks on game field if NO lines needing to be deleted*/
          XYscope.XYlistEnd = drawPointer [1];
          drawPointer [2] = drawPointer [1];
          for (int x = 0; x < boardWidth; x++) {
            for (int y = 0; y < boardHeight; y++) {
              if (board [x][y] == 4) {  //just fallen Tetromino
                XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 20, y * blockSideWidth + 20, x * blockSideWidth + blockSideWidth + xFieldStart - 20, y * blockSideWidth + blockSideWidth - 20);
                XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 40, y * blockSideWidth + 40, x * blockSideWidth + blockSideWidth + xFieldStart - 40, y * blockSideWidth + blockSideWidth - 40);
                board [x][y] = 3;
              }
            }
          }
          drawPointer [1] = XYscope.XYlistEnd;
        }



        XYscope.XYlistEnd = drawPointer [1];
        /*draw next Tetromino (preview)*/
        //    int YInitialPos = mPiecesInitialPosition[nextBlockType][0];
        //    int XInitialPos = mPiecesInitialPosition[nextBlockType][1];
        //    int InitialPieceLength = mPiecesInitialPosition[nextBlockType][2];
        for (int y = 0; y < matrixSize; y++) {
          for (int x = 0; x < matrixSize; x++) {
            if (mPieces [nextBlockType][0][y][x] == 1 || mPieces [nextBlockType][0][y][x] == 2) {
              int x_1 = xNextTetromino + (previewOffset[nextBlockType][0] * blockSize) + (x * blockSize);
              int x_2 = xNextTetromino + (previewOffset[nextBlockType][0] * blockSize) + ((x + 1) * blockSize);
              int y_1 = yNextTetromino + (previewOffset[nextBlockType][1] * blockSize) + ((matrixSize - y) * blockSize);
              int y_2 = yNextTetromino + (previewOffset[nextBlockType][1] * blockSize) + ((matrixSize - (y - 1)) * blockSize);
              XYscope.plotRectangle(x_1 + 20 , y_1 + 20, x_2 - 20, y_2 - 20);
              XYscope.plotRectangle(x_1 + 40 , y_1 + 40, x_2 - 40, y_2 - 40);
            }
          }//Horizontal Loop
        }//Vertical Loop


        /*draw lines*/
        //doing all this just to have the number alinged right...
        if (rowsCleared > 9999) rowsCleared = 9999;
        const int linesDigits = 4; //max number of digits
        char linesChar[linesDigits + 1] = {0, 0, 0, 0, 0};  //create char array to store the 4 digit "rowsCleared" number, leaving a 5th zero at the end for string termination)
        itoa(rowsCleared, linesChar, 10);       //convert int to char array -> number "left bound" (starting with index 0)
        /*finding out how much the numbers needed to be shifted*/
        int shiftValue = 0;
        for (int i = 0; i < linesDigits; i++) if (linesChar[i] == 0) shiftValue++;
        /*shift the numbers*/
        for (int i = linesDigits - shiftValue; i >= 0; i--) linesChar[i + shiftValue] = linesChar[i];
        /*fill the rest with spaces*/
        for (int i = 0; i < shiftValue; i++) linesChar[i] = ' ';
        String lines = linesChar; //convert char-array to string
        XYscope.printSetup(xLines + 100, yLines, 300); //X;Y;Size
        XYscope.print((char *)lines.c_str());


        /*draw level*/
        //doing all this just to have the number alinged right...
        if (level < rowsCleared / 10) level = rowsCleared / 10;
        if (level > 20) level = 20;
        const int levelDigits = 2; //max number of digits
        char levelsChar[levelDigits + 1] = {0, 0, 0};
        itoa(level, levelsChar, 10);
        /*finding out how much the numbers needed to be shifted*/
        shiftValue = 0;
        for (int i = 0; i < levelDigits; i++) if (levelsChar[i] == 0) shiftValue++;
        /*shift the numbers*/
        for (int i = levelDigits - shiftValue; i >= 0; i--) levelsChar[i + shiftValue] = levelsChar[i];
        //for (int i = shiftValue; i < levelDigits; i++) levelsChar[i] = levelsChar[i - shiftValue];
        /*fill the rest with spaces*/
        for (int i = 0; i < shiftValue; i++) levelsChar[i] = ' ';
        String levels = levelsChar; //convert char-array to string

        XYscope.printSetup(xLevel + 100, yLevel, 300); //X;Y;Size
        XYscope.print((char *)"  ");
        XYscope.print((char *)levels.c_str());


        /*draw score*/
        //doing all this just to have the number alinged right...
        if (score > 999999) score = 999999;
        const int scoreDigits = 6; //max number of digits
        char scoreChar[scoreDigits + 1] = {0, 0, 0, 0, 0 , 0 , 0};
        itoa(score, scoreChar, 10);
        /*finding out how much the numbers needed to be shifted*/
        shiftValue = 0;
        for (int i = 0; i < scoreDigits; i++) if (scoreChar[i] == 0) shiftValue++;
        /*shift the numbers*/
        for (int i = scoreDigits - shiftValue; i >= 0; i--) scoreChar[i + shiftValue] = scoreChar[i];
        //for (int i = shiftValue; i < levelDigits; i++) levelsChar[i] = levelsChar[i - shiftValue];
        /*fill the rest with spaces*/
        for (int i = 0; i < shiftValue; i++) scoreChar[i] = ' ';
        String score_str = scoreChar; //convert char-array to string

        XYscope.printSetup(xLevel + 100, scoreHeight - 400, 300); //X;Y;Size
        //XYscope.print((char *)"  ");
        XYscope.print((char *)score_str.c_str());

        drawPointer [2] =  XYscope.XYlistEnd;
      }

      if (draw_blockMoved) {
        /*draw active Tetromino*/
        XYscope.XYlistEnd = drawPointer [2];
        for (int x = 0; x < boardWidth; x++) {
          for (int y = 0; y < boardHeight; y++) {
            if (board [x][y] == 1 || board [x][y] == 2) { //only draw active Tetromino
              XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 20, y * blockSideWidth + 20, x * blockSideWidth + blockSideWidth + xFieldStart - 20, y * blockSideWidth + blockSideWidth - 20);
              XYscope.plotRectangle(x * blockSideWidth + xFieldStart + 40, y * blockSideWidth + 40, x * blockSideWidth + blockSideWidth + xFieldStart - 40, y * blockSideWidth + blockSideWidth - 40);
            }
          }
        }
      }

      if (gameOver) {
        XYscope.printSetup(50, 2800, 350); //X;Y;Size
        XYscope.print((char *)"GAME OVER");
        XYscope.printSetup(290, 2500, 200); //X;Y;Size
        XYscope.print((char *)"PRESS ENTER");
        XYscope.printSetup(330, 2300, 200); //X;Y;Size
        XYscope.print((char *)"TO RESTART");
        XYscope.printSetup(370, 2000, 200); //X;Y;Size
        XYscope.print((char *)"PRESS ESC");
        XYscope.printSetup(450, 1800, 200); //X;Y;Size
        XYscope.print((char *)"TO QUIT");
        while (true) {
          usb.Task();
          if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false) {
            arrKeyPressedLast[USB_ENTER] = true;
            //gameOver = false;
            score = 0;
            level = 0;
            rowsCleared = 0;
            for (int x = 0; x < boardWidth; x++) {
              for (int y = 0; y < boardHeight; y++) {
                board [x][y] = 0;
                boardBuffer [x][y] = 0;
              }
            }
            drawPointer [1] = drawPointer [0];
            drawPointer [2] = drawPointer [0];
            XYscope.XYlistEnd = drawPointer [0];
            break;

          }
          if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
            arrKeyPressedLast[USB_ESC] = true;
            XYscope.setGraphicsIntensity(100);
            return;
          }
        }
      }

      draw_lineDeleted = false;
      draw_blockMoved = false;
      draw_blockFallen = false;
      if (gameOver) {
        draw_lineDeleted = true;
        draw_blockMoved = true;
        draw_blockFallen = true;
        gameOver = false;
        break;
      }
    } //main loop
  } //level selection loop
}

void tetrisMusic()
{
  static unsigned long duration;
  //static int note;
  static unsigned long startTime;
  float BPM = 180;
  const int notes = 59 * 2; //number of notes in array times 2

  float lead_notes[] = {
    // part 1
    _E5, _B4, _C5, _D5, _C5, _B4, _A4, _A4, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,
    _D5, _F5, _A5, _G5, _F5, _E5, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,

    // part 2
    _E4, _C4, _D4, _B3, _C4, _A3, _GS3, _B3,
    _E4, _C4, _D4, _B3, _C4, _E4, _A4, _A4, _GS4, _R

  };
  float lead_times[] = {
    // part 1
    1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.5, 0.5, 1.0, 0.5, 0.5, 1.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,

    // part 2
    2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0,
    2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0
  };
  if (startTime + duration < millis()) {
    //after every note an 8 millisecond delay
    if (tetrisNote % 2 == 1) {
      noTone(13);
      duration = 8;
      startTime = millis();
    } else {
      duration = lead_times[tetrisNote / 2] * 1000 / (BPM / 60);
      Serial.println(duration);
      startTime = millis();
      tone(13, lead_notes[tetrisNote / 2], duration);
      //Serial.println(note);
    }
    tetrisNote++;
    if (tetrisNote >= notes * 2) tetrisNote = 0;
  }
}
