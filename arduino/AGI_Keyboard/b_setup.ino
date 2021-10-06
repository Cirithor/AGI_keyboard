void setup() {
  delay(300);
  // Mainline program SETUP routine.
  //
  //  Passed Parameters NONE
  //
  //  Returns: NOTHING
  //
  //  20170708 Ver 0.0  E.Andrews First cut
  //            (Updated throughout development cycle without version change)

  //Set Serial Monitor must be setup to agree with this initializtion.
  //  [X] Autoscroll
  //  "No Line Ending"
  //  "115200 baud"
  Serial.begin(115200);

  //Send startup messages out to Serial monitor port...
  Serial.println("");
  Serial.print(startMsg);
  Serial.print(" (");
  Serial.print(CompileDateStamp);
  Serial.print(" ");
  Serial.print(CompileTimeStamp);
  Serial.println(")");


 
  XYscope.setScreenSaveSecs(0);

  double DmaFreq = 800000;    //800000 (Hz) is the default startup value for the DMA frequency.
  //You can try various alternate values to find an optimal
  //value that works best for your scope, setup, & application.

  XYscope.begin(DmaFreq);

  //Timer3 is used as the CRT refresh timer.  This timer is setup inside of XYscope.begin( ).
  //However, paintCRT_ISR must be "attached" to timer 3.  To be properly link to the
  //refresh-screen XYscope interupt service routine, it must be linked in the Arduino
  //setup() code as follows:

  Timer3.attachInterrupt(paintCrt_ISR);

  //Here is just some stuff to paint onto CRT at startup
  StartScreen();

  //Send option menu out to PC via Serial.print
  Serial.println();
  PrintStatsToConsole();

}
