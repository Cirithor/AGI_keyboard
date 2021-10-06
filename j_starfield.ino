void StarfieldUSB() {
  Serial.println("**** press a key to stop STARFIELD DEMO ****");
  //variables
  bool wait = true;
  float r = 0;
  int b = 0; //brightness
  //coordinates of stars in 3D
  float star_x[64];
  float star_y[64];
  float star_z[64];

  //location of stars on the screen
  int star_screenx[64];
  int star_screeny[64];

  int Center_of_screen_X = 2047;
  int Center_of_screen_Y = 2047;

  unsigned long lastTimeMillis = millis();
  double timeout = 10;

  //program

  //initialise positions of stars
  for (int i = 0; i < 63; i++) {
    star_x[i] = random (-2000, 2000);
    star_y[i] = random (-2000, 2000);
    star_z[i] = random (100, 500);
  }
  while (wait)
  {
    usb.Task();
    if (lastTimeMillis + int(timeout) < millis()) {
      lastTimeMillis = millis();
      XYscope.plotStart();
      XYscope.plotClear();
      for (int i = 0; i < 63; i++) {
        //move the star closer
        star_z[i] = star_z[i] - 5;

        //calculate screen coordinates
        star_screenx[i] = star_x[i] / star_z[i] * 100 + Center_of_screen_X;
        star_screeny[i] = star_y[i] / star_z[i] * 100 + Center_of_screen_Y;

        //let stars appear bigger, if there are closer
        r = map(star_z[i], 0, 500, 7, 2);
        //r = pow(r, 4) / 200;
        b = map (star_z[i], 0, 500, 100, 0);
        if ((star_screenx[i] + r > 4095) || (star_screenx[i] - r < 0) || (star_screeny[i] + r > 4095) || (star_screeny[i] - r < 0) || (star_z[i] < 1)) {
          star_x[i] = random (-2000, 2000);
          star_y[i] = random (-2000, 2000);
          star_z[i] = random (100, 500);
          r = map(star_z[i], 0, 500, 8, 1);
          //r = pow(r, 4) / 200;
          b = map (star_z[i], 0, 500, 100, 0);
        }
        //XYscope.setGraphicsIntensity(b);
        //draw the starfield
        XYscope.plotCircle (star_screenx[i], star_screeny[i], r);
      }
      XYscope.plotEnd();
    }
    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }
    if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false) {
      arrKeyPressedLast[USB_NUM_PLUS] = true;
      timeout = timeout * 0.7;
      if (timeout < 1) timeout = 1;
    }
    if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false) {
      arrKeyPressedLast[USB_NUM_MINUS] = true;
      timeout = timeout / 0.7;
    }
  }//while-loop
  XYscope.setGraphicsIntensity(100);
}
