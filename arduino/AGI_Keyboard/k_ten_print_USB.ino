void TenPrintUSB() {
  int n = 20;
  int spacing = (4096/n)-1;
  int x = 0;
  int y = 0;
  unsigned long lastTimeMillis = millis();
  int timeout = 0;
  XYscope.plotStart();
  while (true) {
    usb.Task();
    if (timeout < millis()) {
      
      if (x == 0 && y == 0) {
        XYscope.plotClear();
        spacing = (4096/n)-1;
      }
      
      if (random(2) == 1) XYscope.plotLine(x, y, x + spacing, y + spacing);
      else XYscope.plotLine(x, y + spacing, x + spacing, y);
      
      if (x >= 4095 - (2*spacing)) {
        x = 0;
        if (y >= 4095 - (2*spacing)) {
          y = 0;
          timeout = millis() + 1500U;
        } else {
          y = y + spacing;
        }
      } else {
        timeout = millis() + 10U;
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
      if(n<1)n=1;
    }
  }
}
