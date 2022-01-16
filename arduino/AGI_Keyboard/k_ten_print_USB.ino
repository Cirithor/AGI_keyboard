void TenPrintUSB() {
  int n = 20;
  int spacing = (4096 / n) - 1;
  int x = 0;
  int y = 0;
  unsigned long lastTimeMillis = millis();
  double nextDelay = 10;
  int timeout = 0;
  XYscope.plotStart();
  while (true) {
    usb.Task();
    if (timeout < millis()) {

      if (x == 0 && y == 0) {
        XYscope.plotClear();
        spacing = (4096 / n) - 1;
      }
      if (n > 20) XYscope.setGraphicsIntensity(80);
      if (n > 30) XYscope.setGraphicsIntensity(60);
      if (random(2) == 1) XYscope.plotLine(x, y, x + spacing, y + spacing);
      else XYscope.plotLine(x, y + spacing, x + spacing, y);
      XYscope.setGraphicsIntensity(100);

      if (x >= 4095 - (2 * spacing)) {
        x = 0;
        if (y >= 4095 - (2 * spacing)) {
          y = 0;
          timeout = millis() + 1500U;
        } else {
          y = y + spacing;
        }
      } else {
        timeout = millis() + (unsigned long)nextDelay;
        x = x + spacing;
      }
    }



    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      break;
    }
    if (arrKeyPressed[USB_NUM_PLUS] == true && arrKeyPressedLast[USB_NUM_PLUS] == false) {
      arrKeyPressedLast[USB_NUM_PLUS] = true;
      n++;
    }
    if (arrKeyPressed[USB_NUM_MINUS] == true && arrKeyPressedLast[USB_NUM_MINUS] == false) {
      arrKeyPressedLast[USB_NUM_MINUS] = true;
      n--;
      if (n < 1)n = 1;
    }
    if (arrKeyPressed[USB_NUM_SLASH] == true && arrKeyPressedLast[USB_NUM_SLASH] == false) {
      arrKeyPressedLast[USB_NUM_SLASH] = true;
      nextDelay = nextDelay * 1.2;
      if (nextDelay > 20) nextDelay = 20;
    }
    if (arrKeyPressed[USB_NUM_STAR] == true && arrKeyPressedLast[USB_NUM_STAR] == false) {
      arrKeyPressedLast[USB_NUM_STAR] = true;
      nextDelay = nextDelay * 0.8;
      if (nextDelay < 1) nextDelay = 1;
    }
  }
}
