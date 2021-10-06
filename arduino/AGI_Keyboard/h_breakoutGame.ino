void breakoutGame() {
  int width = 4095;
  int height = 4095;
  bool wait = true;
  int colums = 9;   //(horizontal amount of blocks)
  int rows = 4;     //(vertical amount of blocks)
  int heightfield = 1000; //Height of the field, in wich the blocks are generated
  int SpaceToTop = 600; //Distance to the top, for the space in wich blocks are generated
  int Block [colums][rows];
  int xpaddle = 500;  //width of the paddle
  int ypaddle = 100;  //thickness of the paddle
  int zpaddle = 200;  //height of the paddle above "ground"
  int x1paddle = (width - xpaddle) / 2;
  int x2paddle = (width - xpaddle) / 2 + xpaddle;
  int y1paddle = zpaddle;
  int y2paddle = zpaddle + ypaddle;
  bool missed;
  int r = 50;       //radius of ball
  double posx = width / 2;       //X-position of ball
  double posy = zpaddle + ypaddle + r;         //Y-position of ball
  double xvel;
  double yvel;
  double ballspeed = 1;  //the lower, the more often the position of the ball gets calculated, therefore usable to speed up or slow down gamespeed
  //double maxspeed = minspeed * 2;
  bool changes;
  int state;
  int x;
  int y;
  int drawspeed = 20 / ballspeed; //the number of rounds for calculating the positions and states before updating the screen -> if the game is slow, make minspeed/maxspeed higher and/or reduce rows/colums

  while (wait) {                  //initialization
    XYscope.plotClear();
    XYscope.plotStart();
    XYscope.plotRectangle(0, 0, width, height);
    int ixEmptyCourt = XYscope.XYlistEnd;
    posx = width / 2;
    posy = posy = zpaddle + ypaddle + r;
    xvel = random(0, ballspeed * 100);
    xvel = xvel / 100;
    yvel = ballspeed - sqrt(xvel * xvel);
    for (x = 0; x < colums; x++) {
      for (y = 0; y < rows; y++) {
        Block[x][y] = 1;
      }
    }
    int ixBlocks;
    for (x = 0; x < colums; x++) {
      for (y = 0; y < rows; y++) {
        int x_1 = (width / colums) * x;
        int y_1 = (height - SpaceToTop - heightfield) + (heightfield / rows) * y;
        int x_2 = (width / colums) * x + (width / colums);
        int y_2 = (height - SpaceToTop - heightfield) + ((heightfield / rows) * y + (heightfield / rows));
        XYscope.plotRectangle(x_1, y_1, x_2, y_2);
        ixBlocks = XYscope.XYlistEnd;
      }
    }
    XYscope.plotRectangle(x1paddle, y1paddle, x2paddle, y2paddle);
    int ixPaddle = XYscope.XYlistEnd;

    //main loop
    while (wait) {
      usb.Task();

      if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false) {
        arrKeyPressedLast[USB_NUM_PLUS] = true;
        double ballspeedLast = ballspeed;
        double XrelativeSpeed = xvel / ballspeed;
        ballspeed = ballspeed + 1;
        if (ballspeed > drawspeed) {
          ballspeed = drawspeed;
        }
        xvel = XrelativeSpeed * ballspeed;
        if (yvel < 0) {
          yvel = -1 * (ballspeed - sqrt(xvel * xvel));
        } else {
          yvel = ballspeed - sqrt(xvel * xvel);
        }
      }
      if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false) {
        arrKeyPressedLast[USB_NUM_MINUS] = true;
        double ballspeedLast = ballspeed;
        double XrelativeSpeed = xvel / ballspeed;
        ballspeed = ballspeed - 1;
        if (ballspeed < 1) {
          ballspeed = 1;
        }
        xvel = XrelativeSpeed * ballspeed;
        if (yvel < 0) {
          yvel = -1 * (ballspeed - sqrt(xvel * xvel));
        } else {
          yvel = ballspeed - sqrt(xvel * xvel);
        }
      }

      for (int i = 0; i < drawspeed; i++) {
        //check, if ball collides with a block
        for (x = 0; x < colums; x++) {
          for (y = 0; y < rows; y++) {
            int x_1 = (width / colums) * x;
            int y_1 = (height - SpaceToTop - heightfield) + (heightfield / rows) * y;
            int x_2 = (width / colums) * x + (width / colums);
            int y_2 = (height - SpaceToTop - heightfield) + ((heightfield / rows) * y + (heightfield / rows));
            if (posx + r >= x_1 && posx + r <= x_2 && posy + r >= y_1 && posy + r <= y_2 && Block[x][y] == 1 && changes == false) {  //impact from left or bottom
              Block[x][y] = 0;
              changes = true;
              if ((posx + r) - x_1 < (posy + r) - y_1) {  //the smaller value is the side of impact
                xvel = -xvel;
              } else {
                yvel = -yvel;
              }
            } else if (posx - r <= x_2 && posx - r >= x_1 && posy + r >= y_1 && posy + r <= y_2 && Block[x][y] == 1 && changes == false) { //impact from right or bottom
              Block[x][y] = 0;
              changes = true;
              if (x_2 - (posx - r) < (posy + r) - y_1) { //the smaller value is the side of impact
                xvel = -xvel;
              } else {
                yvel = -yvel;
              }
            } else if (posx + r >= x_1 && posx + r <= x_2 && posy - r <= y_2 && posy - r >= y_1 && Block[x][y] == 1 && changes == false) { //impact from left or top
              Block[x][y] = 0;
              changes = true;
              if ((posx + r) - x_1 < y_2 - (posy - r)) { //the smaller value is the side of impact
                xvel = -xvel;
              } else {
                yvel = -yvel;
              }
            } else if (posx - r <= x_2 && posx - r >= x_1 && posy - r <= y_2 && posy - r >= y_1 && Block[x][y] == 1 && changes == false) { //impact from right or top
              Block[x][y] = 0;
              changes = true;
              if (x_2 - (posx - r) < y_2 - (posy - r)) { //the smaller value is the side of impact
                xvel = -xvel;
              } else {
                yvel = -yvel;
              }
            }
          }
        }

        //movement of the platform
        if (arrKeyPressed[80] == true) {    //80 = Left Arrow Key
          x1paddle = x1paddle - width / (width / 4);
          if (x1paddle < 0) {
            x1paddle = 0;
          }
          x2paddle = x1paddle + xpaddle;
        }
        if (arrKeyPressed[79] == true) { //79 = Left Arrow Key
          x2paddle = x2paddle + width / (width / 4);
          if (x2paddle > width) {
            x2paddle = width;
          }
          x1paddle = x2paddle - xpaddle;
        }

        //movement of the ball
        posx = posx + xvel;
        posy = posy + yvel;
        int TopLimit = height - r;
        int BotLimit = 0 + r;
        int RightLimit = width - r;
        int LeftLimit = 0 + r;

        //checking for impacts
        if (posy >= TopLimit) {  //Top Wall Impact
          posy = +TopLimit - (abs(yvel) - abs(posy - TopLimit));
          yvel = -yvel;
        }
        if (posy <= BotLimit) {  //Bottom wall Impact
          posy = BotLimit + (abs(yvel) - abs(posy - BotLimit));
          yvel = -yvel;
        }
        //Check Left-Right collisions with walls...
        if (posx >= RightLimit) {  //Right Wall Impact
          posx = RightLimit - (abs(xvel) - abs(posx - RightLimit));
          xvel = -xvel;
        }
        if (posx <= LeftLimit) { //Left Wall Impact
          posx = LeftLimit + (abs(xvel) - abs(posx - LeftLimit));
          xvel = -xvel;
        }
        //check paddle impact
        if (posy <= zpaddle + ypaddle) {
          if (missed == false) {
            if (posx + r >= x1paddle && posx - r <= x2paddle && yvel < 0) {
              yvel = -yvel;
              xvel = (posx - ((x1paddle + x2paddle) / 2)) / (xpaddle / 2) * ((ballspeed / 3) * 2);              //Value between -2/3 ballspeed and 2/3 ballspeed
              yvel = ballspeed - sqrt(xvel * xvel);
            } else {
              missed = true;
            }
          }
        } else {
          missed = false;
        }
      }

      //if ball collided with a block, then generate new grid of blocks
      if (changes == true) {
        XYscope.XYlistEnd = ixEmptyCourt;
        for (x = 0; x < colums; x++) {
          for (y = 0; y < rows; y++) {
            int x_1 = (width / colums) * x;
            int y_1 = (height - SpaceToTop - heightfield) + (heightfield / rows) * y;
            int x_2 = (width / colums) * x + (width / colums);
            int y_2 = (height - SpaceToTop - heightfield) + ((heightfield / rows) * y + (heightfield / rows));
            if (Block[x][y] == 1) {
              XYscope.plotRectangle(x_1, y_1, x_2, y_2);
              ixBlocks = XYscope.XYlistEnd;
              changes = false;
            }
          }
        }
        if (changes == true) {      //last block hit - reset
          changes = false;
          ixBlocks = ixEmptyCourt;
          break;
        }
      }
      XYscope.XYlistEnd = ixBlocks;
      XYscope.plotRectangle(x1paddle, y1paddle, x2paddle, y2paddle);
      //      ixPaddle = XYscope.XYlistEnd;
      //      XYscope.XYlistEnd = ixPaddle;
      XYscope.plotCircle(posx, posy, r);

      if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
        break;
      }
    }
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }
  }
}
