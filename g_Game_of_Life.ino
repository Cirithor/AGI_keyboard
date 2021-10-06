int width = 4095;   //plot fiel goes from 0..4095
int height = width;  //...and is square
int cellsAmount = 32;   //initial number of cells per row and per colum
const int maxCellsAmount = 48; //the grid is at max 48x48 (needed for array)
float probabilityOfAliveAtStart = 15; //chance of Cell being alive at random start in percent
int interval = 500; //initial redraw time in milliseconds

int lastRecordedTime = 0;
int cellSize = (width + 1) / cellsAmount; //there are height/cellSize cells
int cells[maxCellsAmount][maxCellsAmount];
int cellsBuffer[maxCellsAmount][maxCellsAmount];
bool reset;

//Run Lenght Encoded (RLE) starting grids. To add more you need to add the RLE String and add a Name-String to the corresponding number
const char* GOL[] = {
  /*0*/  "bo$2bo$3o!",
  /*1*/  "2b3o3b3o2b2$o4bobo4bo$o4bobo4bo$o4bobo4bo$2b3o3b3o2b2$2b3o3b3o2b$o4bobo4bo$o4bobo4bo$o4bobo4bo2$2b3o3b3o!",
  /*2*/  "24bo11b$22bobo11b$12b2o6b2o12b2o$11bo3bo4b2o12b2o$2o8bo5bo3b2o14b$2o8bo3bob2o4bobo11b$10bo5bo7bo11b$11bo3bo20b$12b2o!",
  /*3*/  "2bo4bo2b$2ob4ob2o$2bo4bo!",
  /*4*/  "3b2o$3bobo$2bo4bo5bo$3bobob2o2b2ob2o$3bo3bo7bo$12bobo2$3bo7b3o$2b3o7bo2$bobo$o7bo3bo$2ob2o2b2obobo$2bo5bo4bo$10bobo$11b2o!",
  /*5*/  "2b2o11b2o$2b2o11b2o$bo2bo9bo2bo$2bobo9bobo2$b3o11b3o$bo2bo9bo2bo$o2bo11bo2bo$b2o13b2o2$b2o13b2o$o2bo11bo2bo$bo2bo9bo2bo$b3o11b3o2$2bobo9bobo$bo2bo9bo2bo$2b2o11b2o$2b2o11b2o!",
  /*6*/  "2o24b2o$2o24b2o$18b2o8b$17bo2bo7b$18b2o8b$14bo13b$13bobo12b$12bo3bo11b$12bo2bo12b2$12bo2bo12b$11bo3bo12b$12bobo13b$13bo14b$8b2o18b$7bo2bo17b$8b2o18b$2o24b2o$2o24b2o!",
  /*7*/  "7b2o4b$7bobo3b$2bo4bob2o2b$b2o5bo4b$o2bo9b$3o10b2$10b3o$9bo2bo$4bo5b2ob$2b2obo4bo2b$3bobo7b$4b2o!",
  /*8*/  "8b3o10b$7bo2bo10b$7bo2bo10b$7b2o12b4$17b3ob$17bo2bo$20bo$3o15b3o$o20b$o2bo17b$b3o17b4$12b2o7b$10bo2bo7b$10bo2bo7b$10b3o!",
  /*9*/  "2b2ob2o$5bo$o2b3o$obobo4bo$2b2o3b3o$3o3b2o$o4bobobo$4b3o2bo$4bo$3b2ob2o!",
  /*10*/ "2bo2bobob$2obob3ob$bo6bo$2o5bob2$bo5b2o$o6bob$b3obob2o$bobo2bo!",
  /*11*/ "bo5bob$obo3bobo$o2bobo2bo$2bo3bo2b$2b2ob2o!",
  /*12*/ "20b2o$20b2o4$9b2o$8bo2bo10b2o$9b2o11bo$22bo12bo$23bo10bobo$34bobo$35bo7$32bo2bo$33b3o$2o38b2o$2o38b2o$6b3o$6bo2bo7$6bo$5bobo$5bobo10bo$6bo12bo$19bo11b2o$18b2o10bo2bo$31b2o4$20b2o$20b2o!",
  /*13*/
};

void GOLMenu() {
  const char* str[] = {
    "Random",
    /*0*/    "Glider",
    /*1*/    "Pulsar",
    /*2*/    "Gosper Gun",
    /*3*/    "Penta Decathlon",
    /*4*/    "48P22.1",
    /*5*/    "68p32.1",
    /*6*/    "Achim's P144",
    /*7*/    "Achim's P16",
    /*8*/    "Gabriel's P138",
    /*9*/    "Jasons P6",
    /*10*/    "Kok's Galaxy",
    /*11*/    "Tumbler",
    /*12*/    "60P312",
    /*13*/
  };

  // number of items in an array
  #define ARRAY_SIZE3(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))
  int strLen = (ARRAY_SIZE3(str));
  bool drawHeader = true;     //if "true" redraw header
  bool drawList = true;       //if "true" redraw list
  bool drawCursor = true;     //if "true" redraw cursor
  int width = 4096;
  int height = 4096;
  int heightList = 3584;      //height for beginning the list
  int ixMenu = 0;             //plot-array-end after the header "Menu"
  int ixList = 0;             //plot-array-end after listing the choices
  int lastPosCurser = 1;      //current cursor position
  int listLen = 9;            //number of listings on the screen
  int listOffset = 0;         //when scrolling: the number of listings not shown on the screen
  int FontSize = 300;         //font size
  int selection = 0;          //current cursor selection based on the string array 'str' starting with '0'.
  int temp = 0;

  while (true) {
//=======================================================
//  drawing the menu
//=======================================================
    //draw header 
    if (drawHeader) {
      XYscope.plotClear();
      XYscope.plotStart();
      XYscope.printSetup(1500, height / 8 * 7, 500); //X;Y;Size
      XYscope.print((char *)"Menu");
      ixMenu =  XYscope.XYlistEnd;
      drawHeader = false;
    }
    //draw list to chose from
    if (drawList) {
      XYscope.XYlistEnd = ixMenu;
      //checking if more or less options are available then the set number of listings on the screen
      if (listLen <= strLen) {temp = listLen;} else {temp = strLen;}
        for (int i = 1; i <= (temp); i++) {
          XYscope.printSetup(width / 8, heightList - heightList / (listLen)*i, FontSize);
          XYscope.print((char *)str[i - 1 + listOffset]);
        }
      ixList = XYscope.XYlistEnd;
      drawList = false;
      drawCursor = true;
    }
    //draw cursor
    if (drawCursor) {
      XYscope.XYlistEnd = ixList;
      XYscope.plotCircle(width / 16, heightList - heightList / (listLen) * (lastPosCurser) + FontSize / 2, FontSize / 4);
      selection = lastPosCurser + listOffset;
      drawCursor = false;
    }
    
//=======================================================
//  interacting with the menu
//=======================================================
    usb.Task();
    
    //go back to the previous menu
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }

    //show less options at he same time
    if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false && listLen > 5) {
      arrKeyPressedLast[USB_NUM_MINUS] = true;
      listLen--;
      drawList = true;
      if (listLen < lastPosCurser) {
        temp = lastPosCurser - listLen;
        lastPosCurser = listLen;
        listOffset = listOffset + temp;
      }
    }
    //show more options at he same time
    if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false && listLen < strLen) {
      arrKeyPressedLast[USB_NUM_PLUS] = true;
      listLen++;
      drawList = true;
      if (listOffset + listLen > strLen) {
        temp = listOffset + listLen - strLen;
        listOffset = listOffset - temp;
        lastPosCurser = lastPosCurser + temp;
      }
    }

    //select the option below
    if (arrKeyPressed[USB_ARROWDOWN] == true && arrKeyPressedLast[USB_ARROWDOWN] == false) {
      arrKeyPressedLast[USB_ARROWDOWN] = true;
      if (strLen < listLen) {
        if (lastPosCurser < strLen) {
          lastPosCurser++;
          drawCursor = true;
        }
      } else {
        if (lastPosCurser < listLen) {
          lastPosCurser++;
          drawCursor = true;
        } else if (listOffset < strLen - listLen) {
          listOffset++;
          drawList = true;
        }
      }
    }
    //select the option above
    if (arrKeyPressed[USB_ARROWUP] == true && arrKeyPressedLast[USB_ARROWUP] == false) {
      arrKeyPressedLast[USB_ARROWUP] = true;
      if (lastPosCurser > 1) {
        lastPosCurser--;
        drawCursor = true;
      } else if (listOffset > 0) {
        listOffset--;
        drawList = true;
      }
    }

    //start choosed Game of Life
    if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false) {
      arrKeyPressedLast[USB_ENTER] = true;
      //run selected Game of Life
      GameOfLife2(selection);
      //redraw the Game of Life menu after exiting Game Of Life
      drawHeader = true;
      drawList = true;
      drawCursor = true;
    }
  }
}


/* Decode run length encoded initial grid layout and center the active cells.
   Will decode twice.
   first run: dimensionKnown = false -> find out the x and y dimensons of the active cells
   second run: dimensionKnown = true -> push living cells centered in the cells[][]-array

   valid string format: (*numbers*)[b/o/$]!
   if no number is declared, the following stat counts as one

 * *numbers*  - number of following dead cells / alive cells / rows
   b          - dead cell
   o          - living cell
   $          - jump to next row
   !          - end of string

   example 'Glider':  bo$2bo$3o!
                      -> "b" one dead cell "o" one living cell, "$" jump one row, "2b" two dead cells, "o" one living cell, "$" jump one row, "3o" three living cells, "!" End
   Output if dead cells are "." and living cells are "o":
   .o
   ..o
   ooo

*/

void RLD(String compressed) {
  bool RLD = true;
  int i = 0;  //index of string
  int j = 0;  //temporary index of string
  int x = 0;  //x-pos of grid (0 = left)
  int y = 0;  //y-pos of grid (0 = down)
  int number = 0; //number in a string
  int xOff = 0; //offset for positioning the cells in the center of the screen
  int yOff = 0;
  bool dimensionsKnown = false; //gets set to true the second round
  char currentChar;  //the current read char from the string

  while (true) {    
    currentChar = compressed.charAt(i);
    //find out start and end of a number in a string
    if (isDigit(currentChar)) {
      j = i;
      while (isDigit(currentChar)) {
        i++;
        currentChar = compressed.charAt(i);
      }
      i--;
      //j is now the index of the most significant digit in the number    (Example: bo581b! -> j would be the index of number '5')
      //i is now the index of the least significant digit in the number   (Example: bo581b! -> i would be the index of number '1')
      //i >= j
      int exponent = 0;
      const int base = 10;
      number = 0;
      //calculate number out of single digits
      for (int k = i; k >= j; k--) {
        //the base is 10 and the exponent is the current step beginning with 0 -> 10^0 = 1 , 10^1 = 10 , and so on...
        //digit "0" is ASCII 48, therefore to get the number value subtract 48 from the char digit
        number = number + (pow(base, exponent) * (int(compressed.charAt(k)) - 48));
        exponent++;
      }
      i++;
    }
    if (currentChar == 'b' || currentChar == 'o') {        //cell is dead or alive
      if (number > 0) {                                   //set more than one cell active/inactive at the time
        for (int k = 0; k < number; k++) {
          if (currentChar == 'b') {
            if (dimensionsKnown) cells[x + xOff][y + yOff] = 0;
            else Serial.print('.');
          }
          if (currentChar == 'o') {
            if (dimensionsKnown) cells[x + xOff][y + yOff] = 1;
            else Serial.print('O');
          }
          x++;
        }
        number = 0;
      } else {                                           //set only one cell active/inactive at the time
        if (currentChar == 'b') {
          if (dimensionsKnown) cells[x + xOff][y + yOff] = 0;
          else Serial.print('.');
        }
        if (currentChar == 'o') {
          if (dimensionsKnown) cells[x + xOff][y + yOff] = 1;
          else Serial.print('O');
        }
        x++;
      }
      i++;
    }

    if (currentChar == '$') {                              //next row
      if (number > 0) {                                    //jump more than one row
        for (int k = 0; k < number; k++) {
          y++;
          if(!dimensionsKnown) Serial.println();
        }
        number = 0;
      } else {                                             //jump only one row
        y++;
        if(!dimensionsKnown) Serial.println();
      }
      if (xOff < x && !dimensionsKnown) xOff = x;          //if x is bigger than the previous x's than save the value
      x = 0;
      i++;
    }

    if (currentChar == '!') {   //End Decompression
      if (dimensionsKnown) break;
      yOff = y;
      xOff = (maxCellsAmount - xOff) / 2;         //calculate the offset to center the active cell structure to the screen
      yOff = (maxCellsAmount - yOff) / 2;
      dimensionsKnown = true;
      i = 0;
      x = 0;
      y = 0;
    }
  }//Decompression loop
  Serial.println("End");
}


void GameOfLife2(int initialStart) {
  while (true) {
    // Initialization of cells Arrays
    for (int x = 0; x < maxCellsAmount; x++) {
      for (int y = 0; y < maxCellsAmount; y++) {
        cells[x][y] = 0;
        cellsBuffer[x][y] = 0;
      }
    }

    if (initialStart == 1) {       //choosed random initial start
      for (int x = 0; x < maxCellsAmount; x++) {
        for (int y = 0; y < maxCellsAmount; y++) {
          float state = random (100);
          if (state > probabilityOfAliveAtStart) {
            state = 0;
          } else {
            state = 1;
          }
          cells[x][y] = int(state); // Save state of each cell
        }
      }
    //choosed another initial start from list
    } else RLD(GOL[initialStart - 2]);


    bool pause = false;
//=======================================================
//  MAIN LOOP
//=======================================================
    while (true)
    {
      XYscope.autoSetRefreshTime();
      usb.Task();
      //draw next generation after set interval OR if paused, draw next generation when enter is pressed
      if ((millis() - lastRecordedTime > interval && !pause) || (pause && (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false))) {  
        arrKeyPressedLast[USB_ENTER] = true;
        
        //Plot game state
        XYscope.plotClear();
        XYscope.plotStart();
        int xScreenOffset = (maxCellsAmount - cellsAmount) / 2;
        int yScreenOffset = (maxCellsAmount - cellsAmount) / 2;
        for (int x = xScreenOffset; x < xScreenOffset + cellsAmount; x++) {
          for (int y = yScreenOffset; y < yScreenOffset + cellsAmount; y++) {
            if (cells[x][y] == 1) {
              XYscope.plotRectangle((x - xScreenOffset) * cellSize, height - ((y - yScreenOffset) * cellSize), (x - xScreenOffset) * cellSize + cellSize - 1, height - ((y - yScreenOffset) * cellSize + cellSize - 1));
            }
          }
        }


        //buffer current state
        for (int x = 0; x < maxCellsAmount; x++) {
          for (int y = 0; y < maxCellsAmount; y++) {
            cellsBuffer[x][y] = cells[x][y];
          }
        }
        lastRecordedTime = millis();
        // Visit each cell:
        for (int x = 0; x < maxCellsAmount; x++) {
          for (int y = 0; y < maxCellsAmount; y++) {
            // And visit all the neighbours of each cell
            int neighbours = 0; // We'll count the neighbours
            for (int xx = x - 1; xx <= x + 1; xx++) {
              for (int yy = y - 1; yy <= y + 1; yy++) {
                if (((xx >= 0) && (xx < maxCellsAmount)) && ((yy >= 0) && (yy < maxCellsAmount))) { // Make sure you are not out of bounds
                  if (!((xx == x) && (yy == y))) { // Make sure to to check against self
                    if (cellsBuffer[xx][yy] == 1) {
                      neighbours ++; // Check alive neighbours and count them
                    }
                  } // End of if
                } // End of if
              } // End of yy loop
            } //End of xx loop
            // We've checked the neigbours: apply rules!
            if (cellsBuffer[x][y] == 1) { // The cell is alive: kill it if necessary
              if (neighbours < 2 || neighbours > 3) {
                cells[x][y] = 0; // Die unless it has 2 or 3 neighbours
              }
            } else { // The cell is dead: make it live if necessary
              if (neighbours == 3 ) {
                cells[x][y] = 1; // Only if it has 3 neighbours
              }
            } // End of if
          } // End of y loop
        } // End of x loop
      }//redraw time


//=======================================================
//  Keyboard Controls
//=======================================================
      //restart
      if (arrKeyPressed[USB_R] == true && arrKeyPressedLast[USB_R] == false) {
        arrKeyPressedLast[USB_R] = true;
        break;
      }
      //exit to Game of Life menu
      if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
        break;
      }
      //show less cells on the screen
      if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false) {
        arrKeyPressedLast[USB_NUM_MINUS] = true;
        cellsAmount--;
        if (cellsAmount < 1) cellsAmount = 1;
        cellSize = (width + 1) / cellsAmount;
      }
      //show more cells on the screen
      if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false) {
        arrKeyPressedLast[USB_NUM_PLUS] = true;
        cellsAmount++;
        if (cellsAmount > maxCellsAmount) cellsAmount = maxCellsAmount;
        cellSize = (width + 1) / cellsAmount;
      }
      //pause the game
      if (arrKeyPressed[USB_PAUSE] == true && arrKeyPressedLast[USB_PAUSE] == false) {
        arrKeyPressedLast[USB_PAUSE] = true;
        pause = !pause;
      }
      //decrease game speed
      if (arrKeyPressed[USB_NUM_SLASH] == true && arrKeyPressedLast[USB_NUM_SLASH] == false) {
        arrKeyPressedLast[USB_NUM_SLASH] = true;
        interval = interval * 0.7;
        if (interval < 10) interval = 10;
      }
      //increase game speed
      if (arrKeyPressed[USB_NUM_STAR] == true && arrKeyPressedLast[USB_NUM_STAR] == false) {
        arrKeyPressedLast[USB_NUM_STAR] = true;
        interval = interval / 0.7;
        if (interval > 10000) interval = 10000;
      }
    }//main loop
    
    //still exiting to Game of Life menu
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }
  }//reset loop
}
