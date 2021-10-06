//=======================================================
// DIAGNOSTIC ROUTINES
//=======================================================
void WaitForAnyKey(String msg) {
  // Routine outputs 'msg' to serial monitor port using Serial.print then halts execution and
  // waits for Opr to hit any key.  Then it will return so execution can continue.
  // Used as a diagnostic tool to provide a 'halt execution and wait' function for
  // debugging use
  //
  //  Passed Parameters
  //    msg -   This string is output to the Serial Monitor port, then
  //        execution halts until the operator touches any key.
  //
  //  Returns: NOTHING
  //
  //  20170708 Ver 0.0  E.Andrews First cut
  //

  boolean wait;
  Serial.print(msg);
  Serial.println(F(" <-WAITING! (...Hit any Key to CONTINUE...)"));
  wait = true;
  while (wait) {
    if (Serial.available()) {
      int d = 0;
      while (d >= 0) {
        d = Serial.read();  //clean out the buffer (Retrieved data is NOT USED)
      }           //...keep reading until buffer is empty; note, Serial.read will return as -1 when no data is available
      wait = false;
      Serial.println(F("_KP_"));
      delay(100);
    }

  }
}
void ShowMemory() {
  // Routine outputs an estimate of the total RAM in active use to the serial monitor port.
  // Used during development to observe RAM consumption.
  //
  //  Passed Parameters NONE
  //
  //  Returns: NOTHING
  //
  //  20170725 Ver 0.0  E.Andrews First cut
  //

  char *heapend = sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi = mallinfo();
  printf("\n DUE Dynamic ram used: %d\n", mi.uordblks);
  printf("  DUE Program static ram used %d\n", &_end - ramstart);
  printf("  DUE Stack ram used %d\n\n", ramend - stack_ptr);
  printf("     ... My guess at free mem: %d\n",
         stack_ptr - heapend + mi.fordblks);
}
