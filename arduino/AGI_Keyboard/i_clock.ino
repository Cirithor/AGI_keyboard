//================ START OF SIMULATED CLOCK ROUTINES ========================//
//DEFINE Global (Gbl) Variables used to draw the clock features

float Gbl_X_Center, Gbl_Y_Center, Gbl_Center_Radius, Gbl_MinHubCenterRadius;
float Gbl_Hour_HandLength, Gbl_Hour_EndRadius, Gbl_Hour_End_X_Center, Gbl_Hour_End_Y_Center;
float Gbl_Min_HandLength, Gbl_Min_EndRadius, Gbl_Min_End_X_Center, Gbl_Min_End_Y_Center;
float Gbl_Sec_HandLength, Gbl_Sec_End_X, Gbl_Sec_End_Y;
int Gbl_HR_ListPtr, Gbl_MIN_ListPtr, Gbl_SEC_ListPtr;

void ClockHandsSetup(int X_Center, int Y_Center, int Center_Radius, int MinCenterRadius, int Hour_Length, int Min_Length, int Sec_Length) {
  //  Routine to set global variables used to drive the clock hands display
  //
  //  Calling parameters (All coordinates, radius, and length parameters are in "pixels")
  //    X_Center, Y-Center - These are the coordinates of the center of the clock
  //    Center_Radius - Defines the radius of the center of the Hour hand
  //    MinCenterRadius - Defines the radius for center of the Minute hand
  //    Hour_Length, Min_Length - Defines the length of the Hour/Minute hands
  //
  //  Returns:
  //    NOTHING
  //
  //  20171106 Ver 0.0  E.Andrews First cut

  Gbl_X_Center = X_Center;
  Gbl_Y_Center = Y_Center;
  Gbl_Center_Radius = Center_Radius;
  Gbl_MinHubCenterRadius = MinCenterRadius;
  Gbl_Hour_HandLength = Hour_Length;
  Gbl_Min_HandLength = Min_Length;
  Gbl_Sec_HandLength = Sec_Length;


}
void DrawClockMinHand(float CurrentMinutes) {
  //  CLOCK routine that plots a minute-hand
  //
  //  Passed Parameters:
  //    float Minutes This is the current minutes value, 0<= Minutes <= 59
  //            If this value is out of range, 0 will be the minutes value used
  //
  //  This routine uses the following global parameters:
  //    float Gbl_X_Center, Gbl_Y_Center  - This is the XY center of the clock face.
  //    float Gbl_Center_Radius     - Radius of the central clock face 'circle'
  //    float Gbl_Min_HandLength      - This is the length of the clock arm
  //    int XYscope.ListEnd         - Standard XY_List array pointer
  //
  //  Returns: NOTHING
  //
  //  20171106 Ver 0.0  E.Andrews First cut
  //
  int X0, Y0, X1, Y1;
  float mins;
  double TwoPi = atan(1) * 8;

  //Figure out the Theta, the ANGLE (in radians) of the hand based on the passed parameter.FYI: 0=12 o'clock, Pi=6 o'clock, etc...
  mins = CurrentMinutes;
  if (mins<0 or mins>59) mins = 0;      //Bound the incoming value
  double theta = TwoPi * mins / 60.;    //This is the angle of the Minute hand
  //theta = const_2pi / 60. * seconds;  //This is the angle of the Second Hand
  //theta = const_2pi / 12. * hours;    //This is the angle of the Hour Hand

  //Calculate the Coordinates for the center point of the END of the hand
  //Gbl_Hour_End_X_Center = Gbl_X_Center + Gbl_Hour_HandLength * sin(theta);  //=+(Xc0)+Hand_Len*SIN(Theta)
  //Gbl_Hour_End_Y_Center = Gbl_Y_Center + Gbl_Hour_HandLength * cos(theta);  //=+(Yc0)+Hand_Len*COS(Theta)
  Gbl_Min_End_X_Center = Gbl_X_Center + Gbl_Min_HandLength * sin(theta);  //=+(Xc0)+Hand_Len*SIN(Theta)
  Gbl_Min_End_Y_Center = Gbl_Y_Center + Gbl_Min_HandLength * cos(theta);  //=+(Yc0)+Hand_Len*COS(Theta)



  //Calculate the Coordinates for LINE#1 of arm and draw this line  (min and hour hand only)
  X0 = int(Gbl_X_Center - Gbl_MinHubCenterRadius * cos(theta));         //+=Xc0-Radius_1*COS(Theta)
  Y0 = int(Gbl_Y_Center + Gbl_MinHubCenterRadius * sin(theta));         //=+Yc0-Radius_1*SIN(Theta)
  X1 =  int(Gbl_Min_End_X_Center);      //=+(Xc1_)-Radius_2*COS(Theta)
  Y1 =  int(Gbl_Min_End_Y_Center);      //=Yc1_+Radius_2*SIN(Theta)
  XYscope.plotLine(X0, Y0, X1, Y1);

  //Calculate the Coordinates for LINE#2 of arm and draw this line  (min and hour hand only)
  X0 = int(Gbl_X_Center + Gbl_MinHubCenterRadius * cos(theta));         //=+Xc0+Radius_1*COS(Theta)
  Y0 = int(Gbl_Y_Center - Gbl_MinHubCenterRadius * sin(theta));         //=+Yc0-Radius_1*SIN(Theta)
  X1 =  int(Gbl_Min_End_X_Center);      //=+Xc1_+Radius_2*COS(Theta)
  Y1 =  int(Gbl_Min_End_Y_Center);      //=+Yc1_-Radius_2*SIN(Theta)
  XYscope.plotLine(X1, Y1, X0, Y0); //Reverse order of vector for best plot quality
}
void DrawClockHourHand(float CurrentHour) {
  //  CLOCK routine that plots the Hour-hand
  //
  //  Passed Parameters:
  //    float Hours   This is the current Hours value, 0<= Hour <= 12
  //            If this value is out of range, 12 will be the Hours value used
  //            Note: An Hours value such as 3.5 will put hour hand at 3:30 position.
  //
  //  This routine uses the following global parameters:
  //    float Gbl_X_Center, Gbl_Y_Center  - This is the XY center of the clock face.
  //    float Gbl_Center_Radius     - Radius of the central clock face 'circle'
  //    float Gbl_Hour_HandLength     - This is the length of the clock arm
  //    float Gbl_Hour_EndRadius      - Radius of the circle plotted at the 'end' of the clock arm
  //    int XYscope.ListEnd         - Standard XY_List array pointer
  //
  //  Returns: NOTHING
  //
  //  20171106 Ver 0.0  E.Andrews First cut
  //
  int X0, Y0, X1, Y1;
  float Hours;
  double TwoPi = atan(1) * 8;

  //Figure out the Theta, the ANGLE (in radians) of the hand based on the passed parameter.FYI: 0=12 o'clock, Pi=6 o'clock, etc...
  Hours = CurrentHour;
  if (Hours<0 or Hours>12) Hours = 12;      //Bound the incoming value
  //double theta = TwoPi * mins/60.;      //This is the angle of the Minute hand
  //double theta = TwoPi * seconds / 60.;   //This is the angle of the Second Hand
  double theta = TwoPi * Hours / 12.;     //This is the angle of the Hour Hand

  //Calculate the Coordinates for the center point of the END of the hand
  Gbl_Hour_End_X_Center = Gbl_X_Center + Gbl_Hour_HandLength * sin(theta);  //=+(Xc0)+Hand_Len*SIN(Theta)
  Gbl_Hour_End_Y_Center = Gbl_Y_Center + Gbl_Hour_HandLength * cos(theta);  //=+(Yc0)+Hand_Len*COS(Theta)
  //Gbl_Min_End_X_Center = Gbl_X_Center + Gbl_Min_HandLength * sin(theta);  //=+(Xc0)+Hand_Len*SIN(Theta)
  //Gbl_Min_End_Y_Center = Gbl_Y_Center + Gbl_Min_HandLength * cos(theta);  //=+(Yc0)+Hand_Len*COS(Theta)



  //Calculate the Coordinates for LINE#1 of arm and draw this line  (min and hour hand only)
  X0 = int(Gbl_X_Center - Gbl_Center_Radius * cos(theta));          //+=Xc0-Radius_1*COS(Theta)
  Y0 = int(Gbl_Y_Center + Gbl_Center_Radius * sin(theta));          //=+Yc0-Radius_1*SIN(Theta)
  X1 =  int(Gbl_Hour_End_X_Center);     //=+(Xc1_)-Radius_2*COS(Theta)
  Y1 =  int(Gbl_Hour_End_Y_Center);     //=Yc1_+Radius_2*SIN(Theta)
  XYscope.plotLine(X0, Y0, X1, Y1);


  //Calculate the Coordinates for LINE#2 of arm and draw this line  (min and hour hand only)
  X0 = int(Gbl_X_Center + Gbl_Center_Radius * cos(theta));          //=+Xc0+Radius_1*COS(Theta)
  Y0 = int(Gbl_Y_Center - Gbl_Center_Radius * sin(theta));          //=+Yc0-Radius_1*SIN(Theta)
  X1 =  int(Gbl_Hour_End_X_Center);     //=+Xc1_+Radius_2*COS(Theta)
  Y1 =  int(Gbl_Hour_End_Y_Center);     //=+Yc1_-Radius_2*SIN(Theta)
  XYscope.plotLine(X1, Y1, X0, Y0); //Reverse order of vector for best plot quality
}
void DrawClockSecHand (int Secs) {
  //  Clock Routine that plots the SECONDS-hand
  //
  //  Passed Parameters:
  //    int Secs    This is the current Secs value, 0<= Secs <= 59
  //            If this value is out of range, 0 will be used
  //
  //  This routine uses the following global parameters:
  //    float Gbl_X_Center, Gbl_Y_Center  - This is the XY center of the clock face.
  //    float Gbl_Center_Radius     - Radius of the central clock face 'circle'
  //    float Gbl_Secs_HandLength     - This is the length of the clock arm
  //    int XYscope.ListEnd         - Standard XY_List array pointer
  //
  //  Returns: NOTHING
  //
  //  20171106 Ver 0.0  E.Andrews First cut
  //
  double TwoPi = atan(1) * 8;
  float angle = (TwoPi * Secs) / (60);

  int X_end = Gbl_X_Center + Gbl_Sec_HandLength * sin(angle);
  int Y_end = Gbl_Y_Center + Gbl_Sec_HandLength * cos(angle);
  //Plot a simple vector (line) as the second hand
  XYscope.plotLine(Gbl_X_Center, Gbl_Y_Center, X_end, Y_end);
}

int EnterTime() {
  int hhmmss = 0;
  int heightNumbers = 4096 / 8 * 5;
  int fontSize = 500;
  int fontWidth = (fontSize * 0.6);
  int xfirstcharacter = 850;
  usb.Task();
  XYscope.plotClear();
  XYscope.plotStart();
  XYscope.printSetup(400, 4096 / 8 * 7, fontSize); //X;Y;Size
  XYscope.print((char *)"Enter Time:");
  int digit = 0; //used to point to the correct digits[] index
  int ixEnterTime;
  char digits [9]; //used to hold the current char-array with the user input
  String tempStr;
  bool digitChanged = false;
  bool backslash = false;


  ixEnterTime = XYscope.XYlistEnd;
  XYscope.printSetup(xfirstcharacter, heightNumbers, fontSize); //X;Y;Size
  XYscope.print((char *)"  :  :  ");
  XYscope.plotLine(xfirstcharacter, heightNumbers, xfirstcharacter + fontWidth, heightNumbers);


  for (int i = 0; i <= 8; i++) digits [i] = 0;  //clear Array
  for (int i = 0; i <= 7; i++) {                //fill Array with template "  :  :  " and leaving a zero at the end for string conversion
    digits [i] = ' ';
    if ((i + 1)  % 3 == 0) digits[i] = ':';
    Serial.print(char(digits[i]));
  }

  while (true) {
    usb.Task();
    int currentKey = -1;
    //if a key between 0 and 9 is pressed, then the corresponding interger value is saved in the variable 'currentKey'
    for (int i = 48; i <= 57; i++) {
      if (arrASCIIKeyPressed[i] && !arrASCIIKeyPressedLast[i]) {
        arrASCIIKeyPressedLast[i] = true;
        currentKey = i - 48;
        break;
      }
    }

    /* the integer 'digit' is used in combination of the array digits[] for guaranteed correct user input and plotting the currently entered time on the screen

       digit value:   0 1 2 3 4 5 6 7 8
       String format: h h : m m : s s 0

       digits [2] and digits [5] are always the ':' for plotting
       digits [8] is always zero for conversion of the char-array digits [] to a zero-terminated string
    */


    //allow only valid numbers
    if (currentKey >= 0) {
      if (digit == 0 && currentKey <= 2) digitChanged = true;
      if (digit == 1 && ((digits[0] - 48 == 2 && currentKey <= 3) || digits[0] - 48 < 2)) digitChanged = true;
      if (digit == 3 && currentKey <= 5) digitChanged = true;
      if (digit == 4 ) digitChanged = true;
      if (digit == 6 && currentKey <= 5) digitChanged = true;
      if (digit == 7 ) digitChanged = true;
    }


    if (digitChanged) {                                                   //true, if valid number entered or backslash is pressed
      if (backslash) {                                                    //if backslash pressed
        if ((digit % 3) == 0) digits[digit - 2] = ' ';                    //replace the last set number with a space
        else digits[digit - 1] = ' ';
      }
      else digits[digit] = currentKey + 48;                               //else convert the digit from int to its ASCII equivalent and store it into the char-array
      tempStr = digits;                                                   //move the char-array into a string
      XYscope.XYlistEnd = ixEnterTime;                                    //erease current displayed time
      XYscope.printSetup(xfirstcharacter, heightNumbers, fontSize);
      XYscope.print((char*)tempStr.c_str());                              //print current time input
      //draw underline for next number input
      if (backslash) {                                                    //if backslash pressed
        digit--;                                                        //go to previously set number
        if ((digit + 1) % 3 == 0) digit--;                              //watch out for colons
      } else {                                                            //else
        digit++;                                                        //go to the next number
        if ((digit + 1) % 3 == 0) digit++;                              //watch out for colons
      }

      if (digit > 7) digit = 8;                                           //if all numbers are entered set digit to 8
      if (digit < 0) digit = 0;                                           //dont't go below zero

      if (digit <= 7) XYscope.plotLine(xfirstcharacter + ((digit) * fontWidth), heightNumbers, xfirstcharacter + ((digit + 1) * fontWidth), heightNumbers);      //Plot underline for next Number input -> no line if all numbers entered

      currentKey = -1;                                                    //reset currentKey
      digitChanged = false;                                               //reset flags
      backslash = false;
    }

    if (arrKeyPressed[USB_BACKSPACE] == true && arrKeyPressedLast[USB_BACKSPACE] == false) {
      arrKeyPressedLast[USB_BACKSPACE] = true;
      digitChanged = true;
      backslash = true;
    }

    if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
      arrKeyPressedLast[USB_ESC] = true;
      return -1;
    }
    if (arrKeyPressed[USB_ENTER] == true && arrKeyPressedLast[USB_ENTER] == false && digit == 8) {
      arrKeyPressedLast[USB_ENTER] = true;
      tempStr.remove(5, 1);       //remove last colon
      tempStr.remove(2, 1);       //remove first colon
      hhmmss = tempStr.toInt();   //convert string to int
      break;
    }
  }
  return hhmmss;
}


void Clock (void) {
  int hhmmss = EnterTime();
  //Serial.print(hhmmss);
  if  (hhmmss >= 0) {
    int ss = ((hhmmss / 1U) % 10) + (10 * ((hhmmss / 10U) % 10));
    int mm = ((hhmmss / 100U) % 10) + (10 * ((hhmmss / 1000U) % 10));
    int hh = ((hhmmss / 10000U) % 10) + (10 * ((hhmmss / 100000U) % 10));
    Serial.print(hh);
    Serial.print("h");
    Serial.print(mm);
    Serial.print("m");
    Serial.print(ss);
    Serial.println("s");
    //  This routine shows the basics of a CRT clock application
    //  This shows how DrawClockHourHand(Hr), DrawClockMinuteHand(Min),
    //  and DrawClockSecondHand(Sec) routines can be written and used.
    //  Take note of how the XYscope.XYlistEnd is managed to animate and
    //  redraw the hands. Also, note that ClockHandsSetup(...) is a routine
    //  that is used to define the location and size parameters for hands.
    //
    //  Passed Parameters NONE
    //
    //  Returns: NOTHING
    //
    //  20171106 Ver 0.0  E.Andrews First cut
    //            (Minor updates throughout development without version change)

    XYscope.plotClear();
    XYscope.plotStart();
    int xCtr = 2047;
    int yCtr = 2047;
    int CenterRadius = 200;
    int Min_HubCenterRadius = 100;
    int ClockRadius = 1900;
    int HR_armRadius = ClockRadius - 500;
    int MN_armRadius = ClockRadius;
    int SEC_armRadius = ClockRadius + 120;
    int xRadius = 1900;
    int yRadius = 1900;
    //Initialize ClockHands global variables

    ClockHandsSetup(xCtr, yCtr, CenterRadius, Min_HubCenterRadius, HR_armRadius, MN_armRadius, SEC_armRadius);

    //  Here is where we draw the fixed, CLOCK FACE parts...
    float numTestPoints = 12; //We want to have 12 text Hour Markers
    float testAngle = 0;
    int patternCount = 0;
    float angleStep = 2 * 3.141526 / numTestPoints;
    testAngle = testAngle - angleStep;
    const int charHt = 500;
    int HoursIntensity = 0;
    int OldTextIntensity = XYscope.getTextIntensity();
    XYscope.setGraphicsIntensity(100);
    for (patternCount = 0; patternCount < numTestPoints; patternCount++) {
      HoursIntensity = HoursIntensity + 20; //Just for fun and test purposes, change
      //intensity of every hour value. Start at
      //20% and step brightness for each HOUR.
      //A real CRT clock will probably want to
      // have one intensity for all hour markers.
      HoursIntensity = 150; //Comment this line out if you want to plot each hour at different level
      if (HoursIntensity > 255)  HoursIntensity = 255;
      testAngle = testAngle + angleStep;
      int xP, yP;
      xP = int(xCtr + xRadius * sin(testAngle));
      yP = int(yCtr + yRadius * cos(testAngle));

      XYscope.plotCircle(xP, yP, 40); //Plot small circles at HOUR marks
      // Now Plot Hour numbers 1-12.  Change in text-plot-radius
      // and offsets were needed to get the numbers in the right place...
      int xDelta = -350;
      int yDelta = -350;
      int xOffset = -charHt / 2;
      int yOffset = -charHt / 2;
      xP = int(xCtr + xOffset + (xRadius + xDelta) * sin(testAngle));
      yP = int(yCtr + yOffset + (yRadius + yDelta) * cos(testAngle));

      //Plot HOURS text
      if (patternCount > 0 && patternCount < 7)
        XYscope.printSetup(xP + 150, yP, charHt, HoursIntensity);
      else
        XYscope.printSetup(xP, yP, charHt, HoursIntensity);
      if (patternCount == 0) {
        XYscope.printSetup(xP, yP, charHt, HoursIntensity);
        XYscope.print((char *)"12");
      } else
        XYscope.print(patternCount);
    }
    //Restore prior Text Intensity
    XYscope.setTextIntensity(OldTextIntensity);

    //Draw short dashed lines for each of the SECOND marks
    numTestPoints = 60;
    angleStep = 2 * 3.141526 / numTestPoints;
    for (patternCount = 0; patternCount < numTestPoints; patternCount++) {
      testAngle = testAngle + angleStep;
      int xP1, yP1, xP2, yP2;
      xP1 = int(xCtr + (xRadius + 50) * sin(testAngle));
      yP1 = int(yCtr + (yRadius + 50) * cos(testAngle));
      xP2 = int(xCtr + (xRadius - 50) * sin(testAngle));
      yP2 = int(yCtr + (yRadius - 50) * cos(testAngle));

      //Note: Here we skip plotting the SECONDS-MARK at each of the HOUR marks
      if ((patternCount + 1) % 5 != 0)
        XYscope.plotLine(xP1, yP1, xP2, yP2);
    }
    //Draw the center hubs of the HOURS and MINITE hands...
    XYscope.plotCircle(xCtr, yCtr, CenterRadius - 5);     //Hour Hand Center Circle ('-5' just makes it look nicer!)
    XYscope.plotCircle(xCtr, yCtr, Min_HubCenterRadius - 5);  //Minute Hand Center Circle
    XYscope.plotCircle(xCtr, yCtr, 10);           //Second-Hand Center Circle


    int  Gbl_HR_ListPtr = XYscope.XYlistEnd;
    int  Gbl_SEC_ListPtr = XYscope.XYlistEnd;
    float TestSec = ss;
    float TempTestSec = ss;
    float TestHour = hh;
    float TestMin = mm;
    float TestHourTwelve;
    unsigned long lastTimeChecked = 0UL;
    unsigned long interval = 1000UL;
    if (TestHour >= 12) TestHourTwelve = TestHour - 12;
    else TestHourTwelve = TestHour;

    XYscope.XYlistEnd = Gbl_HR_ListPtr;
    DrawClockHourHand(TestHourTwelve + TestMin / 60);
    DrawClockMinHand(TestMin);
    Gbl_SEC_ListPtr = XYscope.XYlistEnd;
    DrawClockSecHand(TestSec);
    //Draw Digital Readout of Time as Well
    XYscope.printSetup(1050, 2300, 450, 100);
    if (TestHour < 10)XYscope.print((char *) "0");
    XYscope.print(int(TestHour));
    if (TestMin < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
    XYscope.print(int(TestMin));
    if (TestSec < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
    XYscope.print(int(TestSec));

    while (true) {

      for (TestSec = 0; TestSec <= 60; TestSec++) {

        if (lastTimeChecked + interval <= millis()) {
          lastTimeChecked = millis();
          if (TempTestSec > 0) {
            TestSec = TempTestSec;
            TempTestSec = 0;
          }
          //          if (TestHour >= 12) TestHourTwelve = TestHour - 12;
          //          else TestHourTwelve = TestHour;

          XYscope.XYlistEnd = Gbl_SEC_ListPtr;
          DrawClockSecHand(TestSec);

          //Draw Digital Readout of Time as well...
          XYscope.printSetup(1050, 2300, 450, 100);
          if (TestHour < 10)XYscope.print((char *) "0");
          XYscope.print(int(TestHour));
          if (TestMin < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
          XYscope.print(int(TestMin));
          if (TestSec < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
          XYscope.print(int(TestSec));
        } else if (TestSec > 0) {
          usb.Task();
          TestSec--;
          if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
            break;
          }
        }
      }
      if (arrKeyPressed[USB_ESC] == true && arrKeyPressedLast[USB_ESC] == false) {
        arrKeyPressedLast[USB_ESC] = true;
        break;
      }
      TestSec = 0;
      TestMin++;
      if (TestMin > 59) {
        TestMin = 0;
        TestHour++;
        if (TestHour > 23) TestHour = 0;
      }
      if (TestHour >= 12) TestHourTwelve = TestHour - 12;
      else TestHourTwelve = TestHour;
      XYscope.XYlistEnd = Gbl_HR_ListPtr;
      DrawClockHourHand(TestHourTwelve + TestMin / 60);
      DrawClockMinHand(TestMin);
      Gbl_SEC_ListPtr = XYscope.XYlistEnd;
      DrawClockSecHand(TestSec);
      //Draw Digital Readout of Time as Well
      XYscope.printSetup(1050, 2300, 450, 100);
      if (TestHour < 10)XYscope.print((char *) "0");
      XYscope.print(int(TestHour));
      if (TestMin < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
      XYscope.print(int(TestMin));
      if (TestSec < 10)XYscope.print((char *) ":0"); else XYscope.print((char *) ":");
      XYscope.print(int(TestSec));

    }
  }
}
