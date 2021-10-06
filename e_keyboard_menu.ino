void KeyboardMenu() {
  //=======================================================
  // add new programs here and at the bottom
  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

  const char* str[] = {
    /*1*/    "Tetris",
    /*2*/    "Game of Life",
    /*3*/    "Breakout",
    /*4*/    "Clock",
    /*5*/    "Starfield",
    /*6*/    "10Print",
    /*7*/    //    "new",
    /*8*/    //    "new",
    /*9*/    //    "new",
  };

  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // number of items in an array
#define ARRAY_SIZE(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))
  int strLen = (ARRAY_SIZE(str));
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
  int FontSize = 400;         //font size
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
      if (listLen <= strLen) {
        temp = listLen;
      } else {
        temp = strLen;
      }
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

    //show less Options at he same time
    if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false && listLen > 5) {
      arrKeyPressedLast[USB_NUM_MINUS] = true;
      listLen--;
      drawList = true;
      if (listLen < lastPosCurser) {
        int temp = lastPosCurser - listLen;
        lastPosCurser = listLen;
        listOffset = listOffset + temp;
      }
    }
    //show more Options at he same time
    if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false && listLen < strLen) {
      arrKeyPressedLast[USB_NUM_PLUS] = true;
      listLen++;
      drawList = true;
      if (listOffset + listLen > strLen) {
        int temp = listOffset + listLen - strLen;
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
    if (arrKeyPressed[USB_ARROWUP] == true && arrKeyPressedLast[USB_ARROWUP] == false) {   //Pfeiltaste nach oben
      arrKeyPressedLast[USB_ARROWUP] = true;
      if (lastPosCurser > 1) {
        lastPosCurser--;
        drawCursor = true;
      } else if (listOffset > 0) {
        listOffset--;
        drawList = true;
      }
    }

    //start choosed function
    if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false) {
      arrKeyPressedLast[USB_ENTER] = true;
//=======================================================
// add new programs here and at the top
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
      switch (selection) {
        case 1:
          /*1*/          Tetris();
          break;
        case 2:
          /*2*/          GOLMenu();
          break;
        case 3:
          /*3*/          breakoutGame();
          break;
        case 4:
          /*4*/          Clock();
          break;
        case 5:
          /*5*/          StarfieldUSB();
          break;
        case 6:
          /*6*/          TenPrintUSB();
          break;
        case 7:
          /*7*/
          break;
        case 8:
          /*8*/
          break;
        case 9:
          /*9*/
          break;
        default:
          ;
      }
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
      drawHeader = true;
      drawList = true;
      drawCursor = true;
    }
  }
}
