void Terrain() {
  //variables
  const int width = 4095;
  const int height = 4095;
  const int Center_of_screen_X = 2047;
  const int Center_of_screen_Y = 2047;
  float d = 1000; //distance of projection screen
  float dFar = 10000; //maximum distance in 3D space
  int terrainOffset = -2500;
  int terrainOffsetOld;
  int terrainAltitude = 2000;
  int squareSize = 2000;
  int xLines = 14;
  int yLines = 6;
  int arrayLength = xLines * yLines;
  int b = 0; //brightness
  bool terrainOffsetChanged = false;
  //coordinates of stars in 3D

  //3D Coordinates
  float terrain_x[arrayLength];
  float terrain_y[arrayLength];
  float terrain_z[arrayLength];

  float terrain_x_rotated[arrayLength];
  float terrain_y_rotated[arrayLength];
  float terrain_z_rotated[arrayLength];

  //2D Projection Coordinates
  int terrain_screenx[arrayLength];
  int terrain_screeny[arrayLength];


  unsigned long lastTimeMillis = 0;
  double timeout = 100;

  //program

  //initialise positions of stars
  for (int y = 0; y < yLines; y++) {
    for (int x = 0; x < xLines; x++) {
      int i = xLines * y + x;
      float a = fmap (x * squareSize , 0, (xLines - 1) * squareSize, -1, 1);
      if (a < 0) a = a * -1;

      terrain_x[i] = x * squareSize - (xLines - 1) * squareSize / 2;
      terrain_y[i] = terrainOffset + random(terrainAltitude, 2 * terrainAltitude) * a;
      terrain_z[i] = y * squareSize + d;

    }
  }

  XYscope.plotStart();
  while (true)
  {
    usb.Task();
    if (lastTimeMillis + int(timeout) < millis()) {
      lastTimeMillis = millis();
      XYscope.plotClear();
      for (int y = 0; y < yLines; y++) {
        for (int x = 0; x < xLines; x++) {
          int i = xLines * y + x;

          terrain_screenx[i] = terrain_x[i] * (d / terrain_z[i]) + Center_of_screen_X;
          terrain_screeny[i] = terrain_y[i] * (d / terrain_z[i]) + Center_of_screen_Y;

          if (x > 0) {
            //horizontal lines
            if (terrain_screenx[i - 1] >= 0 && terrain_screenx[i - 1] <= width) {
              if (terrain_screenx[i] >= 0 && terrain_screenx[i] <= width) {
                if (terrain_screeny[i - 1] >= 0 && terrain_screeny[i - 1] <= height) {
                  if (terrain_screeny[i] >= 0 && terrain_screeny[i] <= height) {
                    if (terrain_z[i] > d && terrain_z[i - 1] > d) {
                      XYscope.plotLine(terrain_screenx[i], terrain_screeny[i], terrain_screenx[i - 1], terrain_screeny[i - 1]);
                    }
                  }
                }
              }
            }
          }
          //vertical Lines
          if (y > 0) {
            if (terrain_screenx[i - xLines] >= 0 && terrain_screenx[i - xLines] <= width) {
              if (terrain_screenx[i] >= 0 && terrain_screenx[i] <= width) {
                if (terrain_screeny[i - xLines] >= 0 && terrain_screeny[i - xLines] <= height) {
                  if (terrain_screeny[i] >= 0 && terrain_screeny[i] <= height) {
                    if (terrain_z[i - xLines] > d) {
                      XYscope.plotLine(terrain_screenx[i], terrain_screeny[i], terrain_screenx[i - xLines], terrain_screeny[i - xLines]);
                    }
                  }
                }
              }
            }
          }
        }
      }

      bool invisible = true;

      for (int y = 0; y < yLines; y++) {
        for (int x = 0; x < xLines; x++) {
          int i = xLines * y + x;
          terrain_z[i] = terrain_z[i] - 100;
          if (y == 0) {
            //Serial.println(terrain_z[i]);
            if (terrain_z[i] > d) {
              invisible = false;
            }
          }
        }
      }

      //Serial.println(invisible);
      //shift terrain arrays to remove verticies closer than the projection distance 'd' and add new verticies at the back
      if (invisible) {
        for (int y = 1; y < yLines; y++) {
          for (int x = 0; x < xLines; x++) {
            int i = xLines * y + x;
            //            Serial.print(terrain_z[i]);
            //            Serial.print(" ");
            terrain_x[i - xLines] = terrain_x[i];
            terrain_y[i - xLines] = terrain_y[i];
            terrain_z[i - xLines] = terrain_z[i];
            //            Serial.println(terrain_z[i]);
            if (y == yLines - 1) {
              //Serial.print(terrain_z[i]);
              //Serial.print(" ");
              float a = fmap (x * squareSize , 0, (xLines - 1) * squareSize, -1, 1);
              if (a < 0) a = a * -1;
              //              Serial.print(x);
              //              Serial.print(" ");
              //              Serial.println(a);
              if (a < 0) a = a * -1;
              terrain_x[i] = terrain_x[i - xLines];
              terrain_y[i] = terrainOffset + random(terrainAltitude, 2 * terrainAltitude) * a;
              terrain_z[i] = terrain_z[i - xLines] + squareSize;
              //Serial.println(terrain_z[i]);
            }
          }
        }
      }
    }

    if (terrainOffsetChanged) {
      for (int y = 0; y < yLines; y++) {
        for (int x = 0; x < xLines; x++) {
          int i = xLines * y + x;
          terrain_y[i] = terrain_y[i] - terrainOffsetOld + terrainOffset;
        }
      }
      terrainOffsetChanged = false;
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
    if (arrKeyPressed[USB_NUM_2] == true && arrKeyPressedLast[USB_NUM_2] == false) {
      arrKeyPressedLast[USB_NUM_2] = true;
      terrainOffsetOld = terrainOffset;
      terrainOffset = terrainOffset - 100;
      terrainOffsetChanged = true;
    }
    if (arrKeyPressed[USB_NUM_8] == true && arrKeyPressedLast[USB_NUM_8] == false) {
      arrKeyPressedLast[USB_NUM_8] = true;
      terrainOffsetOld = terrainOffset;
      terrainOffset = terrainOffset + 100;
      terrainOffsetChanged = true;
    }

    if (arrKeyPressed[USB_PAGEUP] == true && arrKeyPressedLast[USB_PAGEUP] == false) {
      arrKeyPressedLast[USB_PAGEUP] = true;
      terrainAltitude = terrainAltitude + 100;
    }

    if (arrKeyPressed[USB_PAGEDOWN] == true && arrKeyPressedLast[USB_PAGEDOWN] == false) {
      arrKeyPressedLast[USB_PAGEDOWN] = true;
      terrainAltitude = terrainAltitude - 100;
    }
    //music();
  }//while-loop
}

float fmap(float x, float a, float b, float c, float d)
{
  float f = x / (b - a) * (d - c) + c;
  return f;
}
