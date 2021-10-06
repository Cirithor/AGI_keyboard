//=======================================================
//  SPLASH SCREEN DEMO
//=======================================================
void ArduinoSplash() {
  // Locate and plot the Arduino-Logo to screen.
  //
  //  Passed Parameters NONE
  //
  //  Returns: NOTHING
  //
  //  20171008 Ver 0.0  E.Andrews First cut
  //

  int Lx = 1150, Ly = 2000, Lht = 1000;
  XYscope.plotStart();
  XYscope.plotArduinoLogo(Lx, Ly, Lht);

}
//=========== END SPLASH =================================

//v---------------------------------------------------v//
//v----------BEGIN SETUP SPLASH SCREEN ---------------v//
void StartScreen() {
  XYscope.plotEnd();
  ArduinoSplash();        //Paint an Arduino logo
  int xC = 1800, yC = 2800;   //Set values of XY center coordinates for start of text
  int textSize = 400;       //Set Text Size (in pixels)
  bool const UndrLined = true;  //Turn underline ON
  int textBright = 150;
  XYscope.printSetup(xC - 150, yC + 50 + 700, textSize, textBright);
  XYscope.print((char *)"AGI", UndrLined);
  xC = 100;
  yC = 2900;
  textSize = 250;
  XYscope.printSetup(xC + 50, yC + 50 + textSize, textSize, textBright);
  XYscope.setFontSpacing(XYscope.prop);     //Select Proportional Spacing
  if (XYscope.getFontSpacing() != XYscope.mono) //Adjust coordinate in currently active spacing mode is 'prop'
    XYscope.printSetup(xC + 50 + 500, yC + 50 + textSize, textSize,
                       textBright);
  XYscope.print((char *)"Arduino Graphics Interface", false); //(false=No underline)
  XYscope.setFontSpacing(XYscope.mono);

  XYscope.autoSetRefreshTime();
  XYscope.plotRectangle(0, 0, 4095, 4095);

  XYscope.printSetup(1000, 1050, 300, 100);
  XYscope.print((char *)"PRESS ENTER");


  XYscope.printSetup(350, 275, 175, 100);
  XYscope.print(startMsg); XYscope.print((char *)" LibRev:"); XYscope.print(XYscope.getLibRev(), 2);

  //Now put the compile date & time onto the screen
  //The time and data stamp come from #define statements at the top of the program...
  XYscope.printSetup(1100, 100, 150, 100);
  XYscope.print((char *)"(");
  XYscope.print(CompileDateStamp);
  XYscope.print((char *)"  ");
  XYscope.print(CompileTimeStamp);
  XYscope.print((char *)")");
}
//^---------------------------------------------------^//
//^---------------------------------------------------^//
