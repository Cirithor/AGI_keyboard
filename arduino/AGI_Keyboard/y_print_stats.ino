void PrintStatsToConsole(void) {
  //  Routine to send memory usage status reports out to the
  //  serial monitor port.  Use this to observe how various graphics
  //  objects consume the graphics list RAM memory.  Routine also
  //  also displays current clock speeds in use so as you change
  //  them and observe performance differences, you can zero in on
  //  what works best for your scope.
  //
  //  Passed Parameters NONE
  //
  //  Returns: NOTHING
  //
  //  20170811 Ver 0.0  E.Andrews First cut
  //
  Serial.print("\n STATS..............\n MaxBuffSize: ");
  Serial.print(XYscope.MaxBuffSize);
  Serial.print("  Total Array Used: ");
  Serial.print((XYscope.XYlistEnd - 1));
  Serial.print(" (");
  Serial.print((XYscope.XYlistEnd * 100) / XYscope.MaxBuffSize);
  Serial.println(" %)");
  Serial.print("     DMA Clock Freq: ");
  Serial.print(XYscope.DmaClkFreq_Hz / 1000);
  Serial.print(" Khz");
  Serial.print(" (Period: ");
  Serial.print(XYscope.DmaClkPeriod_us);
  Serial.println(" us)");
  Serial.print("   Point Clock Freq: ");
  Serial.print(XYscope.DmaClkFreq_Hz / 2000);
  Serial.print(" Khz");
  Serial.print(" (Period: ");
  Serial.print(XYscope.DmaClkPeriod_us * 2);
  Serial.println(" us)");
  Serial.print("       Refresh Freq: ");
  Serial.print(float(1000000) / float(XYscope.ActiveRefreshPeriod_us), 1);
  Serial.print(" Hz (Period: ");
  Serial.print(XYscope.ActiveRefreshPeriod_us / 1000);
  Serial.print(" ms)\n");
  Serial.print("Graphics Int: ");
  Serial.print(XYscope.getGraphicsIntensity(), DEC);
  Serial.print(" %   Text Int: ");
  Serial.print(XYscope.getTextIntensity(), DEC);
  Serial.println(" %");
  Serial.print("Font Spcng Mode = ");
  if (XYscope.getFontSpacing() == 0)
    Serial.print("PROP");
  else
    Serial.print("MONO");
  Serial.print(", Second_Hand_Enab = ");
  if (enabSecondHand)
    Serial.print("YES,");
  else
    Serial.print("NO,");
  Serial.print(" Scrn_Sav_Secs: ");
  Serial.print(XYscope.getScreenSaveSecs());
  Serial.println();
}
