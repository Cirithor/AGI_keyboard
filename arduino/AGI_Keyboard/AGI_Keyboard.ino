#define CompileDate __DATE__
#define CompileTime __TIME__
char CompileDateStamp[] = CompileDate;//This is how we get a compile date and time stamp into the program
char CompileTimeStamp[] = CompileTime;

char startMsg[] = "CRT_SCOPE (Ver_1.60) ";	//Program Revision Text

/*
  CRT_SCOPE.ino  E.Andrews  Brookfield, WI USA

  This program is an EXAMPLE program to demonstrate and test the
  Arduino Graphics Interface (AGI) hardware.  This hardware provides
  an interface between an Arduino DUE processor and an Oscillocope that
  permits XY drawing and display of graphics objects onto the scope CRT.

  Hardwareand software details can be are found in the EXTRA folder.
  This project is urther described in the February 2018 and March 2018
  editions of of the electronics magazine "Nuts & Volts"

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Note: This program was written and tested using Arduino IDE, Version 1.8.2
       Modifications may be required if using other platforms or versions of the IDE.
	   Also, you must select Arduino DUE as the target processor for correct and valid compilation.

  20170311 Ver  0.00	E.Andrews	Development begins
  20170826 Ver  1.00	E.Andrews	All key functions working, General Clean up
  20171122 Ver  1.58 E.Andrews	Cleanup and tuning; Reset master revision level to 1.58
  20180103 Ver  1.59 E.Andrews   Added clarifying comments throughout code base
  20180103 Ver  1.60 E.Andrews   Adapt to Lib-structure, enhance comments, & test to ensure
 								 that package can be compiled within ARDUINO-IDE Ver 1.8.2 as a library

  This mainline requires the use of XYscope library, the set of drivers
  that manage and drive the Arduino Graphics Interface (AGI) hardware.

  XYscope libraries were first created by E. Andrews (March-September 2017) to enable
  an Ardino DUE to drive a X-Y CRT such as an Oscilloscope or XYZ Monitor.

  This program requires the following three (3) XYscope library routines to function properly
	XYscope.h			Include file
	XYscope.cpp			All grapahics control and plotting routines
	VectorFontROM.h		FONT file used by XYscope to display variable sized vector-stroke characters.
						All defined characters can be displayed using XYscope option 'S' and 's'.

*/

#include <Arduino.h>	//Provided as part of the Arduino IDE

#include <DueTimer.h>	//Timer library for DUE; download this library from the Arduino.org site
//Timer library is also available from author at https://github.com/ivanseidel/DueTimer

//UNCOMMENT ONE OF THE FOLLOWING TWO INCLUDE STATEMENTS TO SET THE PATH TO THESE THE LIBRARY PROGRAMS

#include <XYscope.h>	//UNCOMMENT THIS LINE for PUBLIC XYscope Drivers & Graphics Functions for Arduino Graphics Engine (AGI)
//Use this INCLUDE if you store XYscope library routines inside of ...\Library\XYscope folder

//#include "XYscope.h"	//UNCOMMENT THIS LINE for PRIVATE XYscope Drivers & Graphics Functions for Arduino Graphics Engine (AGI)
//Use this INCLUDE if you keep XYscope library routines in same directory as mainline.ino Directory

//Download this library from GitHub



XYscope XYscope;

// 	+---------Begin Critical Interrupt Service Routines ------------+
//	|  These routines MUST be declared as shown at the top of the	|
//	|      user's main line code for all XYscope Projects!			|
// 	+---------------------------------------------------------------+
//	|																|
//	V																V

void DACC_Handler(void) {
  //	DACC_Handler Interrupt Service Routine. This routine
  //	provides a 'wrapper' to link the AVR DAC INTERRUPT
  //	to the 'XYscope-class' ISR routine.
  XYscope.dacHandler();	//Link the AVR DAC ISR/IRQ to the XYscope.
  //It is called whenever the DMA controller
  //'DAC_ready_for_More_data' event occurs
}

void paintCrt_ISR(void) {
  //	paintCrtISR  Interrupt Service Routine. This routine
  //	provides a 'wrapper' to link the Timer3.AttachedInterrupt()
  //	function to the 'XYscope-class' ISR routine.
  XYscope.initiateDacDma();	//Start the DMA transfer to paint the CRT screen
}

//	V																V
//	|																|
// 	+---------- END Critical Interrupt Service Routines ------------+

//	Define/initialize critical global constants and variables

double dacClkRateHz, dacClkRateKHz;

int EndOfSetup_Ptr;
double TimeForRefresh;


char shiftVal = 0;
uint32_t dispTimer = 0;
int MovingX = 2048, MovingY = 2048;


int enabSecondHand;	//Flag to turn "Radar Scope clock Second-hand" demo feature on/off
//0=Disable second-hand animation, <>0=ENABLE second-hand animation


float angle = 0;

#include <malloc.h>	//Required for RAM usage monitoring routines

// The following lines are used for RAM monitoring routines
extern char _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *) 0x20070000;
char *ramend = (char *) 0x20088000;

/*

  Keyboard Controller HID Example

  Shows the output of a USB Keyboard connected to the USB

  controller of an Arduino Due Board.

  created 8 Oct 2012

  by Cristian Maglie

*/

//define keys with OEM Number - probably needs adjusting with other keyboards
//use the serial monitor to get the OEM Number for every key
#define USB_A 4
#define USB_B 5
#define USB_C 6
#define USB_D 7
#define USB_E 8
#define USB_F 9
#define USB_G 10
#define USB_H 11
#define USB_I 12
#define USB_J 13
#define USB_K 14
#define USB_L 15
#define USB_M 16
#define USB_N 17
#define USB_O 18
#define USB_P 19
#define USB_Q 20
#define USB_R 21
#define USB_S 22
#define USB_T 23
#define USB_U 24
#define USB_V 25
#define USB_W 26
#define USB_X 27
#define USB_Y 29
#define USB_Z 28
#define USB_1 30
#define USB_2 31
#define USB_3 32
#define USB_4 33
#define USB_5 34
#define USB_6 35
#define USB_7 36
#define USB_8 37
#define USB_9 38
#define USB_0 39
#define USB_ENTER 40
#define USB_ESC 41
#define USB_BACKSPACE 42
#define USB_TAB 43
#define USB_SPACE 44
#define USB_SS 45         //'ß'
#define USB_ACUTEACCENT 46//'´'
#define USB_UE 47         //'Ü'
#define USB_PLUS 48       //'+'
#define USB_HASHTAG 50    //'#'
#define USB_OE 51         //'Ö'
#define USB_AE 52         //'Ä'
#define USB_CIRCUMFLEX 53 //'^'
#define USB_COMMA 54      //','
#define USB_PERIOD 55     //'.'
#define USB_MINUS 56      //'-'
#define USB_CAPSLOCK 57
#define USB_F1 58
#define USB_F2 59
#define USB_F3 60
#define USB_F4 61
#define USB_F5 62
#define USB_F6 63
#define USB_F7 64
#define USB_F8 65
#define USB_F9 66
#define USB_F10 67
#define USB_F11 68
#define USB_F12 69
#define USB_PRINT 70
#define USB_ROLL 71
#define USB_PAUSE 72
#define USB_INS 73
#define USB_POS1 74
#define USB_PAGEUP 75
#define USB_DEL 76
#define USB_END 77
#define USB_PAGEDOWN 78
#define USB_ARROWRIGHT 79
#define USB_ARROWLEFT 80
#define USB_ARROWDOWN 81
#define USB_ARROWUP 82
#define USB_NUM 83
#define USB_NUM_SLASH 84
#define USB_NUM_STAR 85
#define USB_NUM_MINUS 86
#define USB_NUM_PLUS 87
#define USB_NUM_ENTER 88
#define USB_NUM_1 89
#define USB_NUM_2 90
#define USB_NUM_3 91
#define USB_NUM_4 92
#define USB_NUM_5 93
#define USB_NUM_6 94
#define USB_NUM_7 95
#define USB_NUM_8 96
#define USB_NUM_9 97
#define USB_NUM_0 98
#define USB_NUM_COMMA 99
#define USB_LESSTHEN 100
#define USB_MENUKEY 101


// Require keyboard control library
#include <KeyboardController.h>

// Initialize USB Controller

USBHost usb;

// Attach keyboard controller to USB

KeyboardController keyboard(usb);

/*defining arrays to store the info of pressed keys*/
int const OEMKeys = 101;
bool arrKeyPressed [OEMKeys];
bool arrKeyPressedLast [OEMKeys];

int const ASCIIKeys = 255;
bool arrASCIIKeyPressed [ASCIIKeys];
bool arrASCIIKeyPressedLast [ASCIIKeys];

bool bPrintKey = true;

/* The following functions keyPressed() and keyReleased() intercepts key presses and key releases
    To get a key detection working in your program you need to call 'USB.Task()' in your loop, wich polls the connected keyboard

    Examples:
    query if the A-key is pressed ->            if(arrKeyPressed(USB_A))

    query rising edge of just pressed A-key ->  if(arrKeyPressed(USB_A) == true && arrKeyPressedLast(USB_A) == false) {
                                                arrKeyPressedLast(USB_A) = true;
                                                //do stuff
                                                }

    query falling edge of just released A-key ->  if(arrKeyPressed(USB_A) == false && arrKeyPressedLast(USB_A) == true) {
                                                  arrKeyPressedLast(USB_A) = false;
                                                  //do stuff
                                                  }


*/

void keyPressed() {


  for (int i = 0; i < OEMKeys; i++) arrKeyPressedLast[i] = arrKeyPressed[i];
  for (int i = 0; i < ASCIIKeys; i++) arrASCIIKeyPressedLast[i] = arrASCIIKeyPressed[i];
  arrKeyPressed[keyboard.getOemKey()] = true;
  arrASCIIKeyPressed[keyboard.getKey()] = true;

  if (arrKeyPressed[USB_F1] == true && arrKeyPressedLast[USB_F1] == false) {
    arrKeyPressedLast[USB_F1] = true;
    ShowMemory();
  }

  if (arrKeyPressed[USB_F2] == true && arrKeyPressedLast[USB_F2] == false) {
    arrKeyPressedLast[USB_F2] = true;
    bPrintKey = !bPrintKey;
  }

  if (bPrintKey) {
    Serial.print("Pressed:  ");
    printKey();
  }
}

// This function intercepts key release
void keyReleased() {
  for (int i = 0; i < OEMKeys; i++) arrKeyPressedLast[i] = arrKeyPressed[i];
  for (int i = 0; i < ASCIIKeys; i++) arrASCIIKeyPressedLast[i] = arrASCIIKeyPressed[i];
  arrKeyPressed[keyboard.getOemKey()] = false;
  arrASCIIKeyPressed[keyboard.getKey()] = false;

  if (bPrintKey) {
    Serial.print("Released: ");
    printKey();
  }
}

void printKey() {

  // getOemKey() returns the OEM-code associated with the key

  Serial.print(" key:");

  Serial.print(keyboard.getOemKey());

  // getModifiers() returns a bits field with the modifiers-keys

  int mod = keyboard.getModifiers();

  Serial.print(" mod:");

  Serial.print(mod);

  Serial.print(" => ");

  if (mod & LeftCtrl)

    Serial.print("L-Ctrl ");

  if (mod & LeftShift)

    Serial.print("L-Shift ");

  if (mod & Alt)

    Serial.print("Alt ");

  if (mod & LeftCmd)

    Serial.print("L-Cmd ");

  if (mod & RightCtrl)

    Serial.print("R-Ctrl ");

  if (mod & RightShift)

    Serial.print("R-Shift ");

  if (mod & AltGr)

    Serial.print("AltGr ");

  if (mod & RightCmd)

    Serial.print("R-Cmd ");

  // getKey() returns the ASCII translation of OEM key

  // combined with modifiers.

  Serial.write(keyboard.getKey());

  Serial.println();
}

#define _R    (0)
#define _C0    (16.35)
#define _CS0   (17.32)
#define _D0    (18.35)
#define _DS0   (19.45)
#define _E0    (20.60)
#define _F0    (21.83)
#define _FS0   (23.12)
#define _G0    (24.50)
#define _GS0   (25.96)
#define _A0    (27.50)
#define _AS0   (29.14)
#define _B0    (30.87)
#define _C1    (32.70)
#define _CS1   (34.65)
#define _D1    (36.71)
#define _DS1   (38.89)
#define _E1    (41.20)
#define _F1    (43.65)
#define _FS1   (46.25)
#define _G1    (49.00)
#define _GS1   (51.91)
#define _A1    (55.00)
#define _AS1   (58.27)
#define _B1    (61.74)
#define _C2    (65.41)
#define _CS2   (69.30)
#define _D2    (73.42)
#define _DS2   (77.78)
#define _E2    (82.41)
#define _F2    (87.31)
#define _FS2   (92.50)
#define _G2    (98.00)
#define _GS2   (103.83)
#define _A2    (110.00)
#define _AS2   (116.54)
#define _B2    (123.47)
#define _C3    (130.81)
#define _CS3   (138.59)
#define _D3    (146.83)
#define _DS3   (155.56)
#define _E3    (164.81)
#define _F3    (174.61)
#define _FS3   (185.00)
#define _G3    (196.00)
#define _GS3   (207.65)
#define _A3    (220.00)
#define _AS3   (233.08)
#define _B3    (246.94)
#define _C4    (261.63)
#define _CS4   (277.18)
#define _D4    (293.66)
#define _DS4   (311.13)
#define _E4    (329.63)
#define _F4    (349.23)
#define _FS4   (369.99)
#define _G4    (392.00)
#define _GS4   (415.30)
#define _A4    (440.00)
#define _AS4   (466.16)
#define _B4    (493.88)
#define _C5    (523.25)
#define _CS5   (554.37)
#define _D5    (587.33)
#define _DS5   (622.25)
#define _E5    (659.25)
#define _F5    (698.46)
#define _FS5   (739.99)
#define _G5    (783.99)
#define _GS5   (830.61)
#define _A5    (880.00)
#define _AS5   (932.33)
#define _B5    (987.77)
#define _C6    (1046.50)
#define _CS6   (1108.73)
#define _D6    (1174.66)
#define _DS6   (1244.51)
#define _E6    (1318.51)
#define _F6    (1396.91)
#define _FS6   (1479.98)
#define _G6    (1567.98)
#define _GS6   (1661.22)
#define _A6    (1760.00)
#define _AS6   (1864.66)
#define _B6    (1975.53)
#define _C7    (2093.00)
#define _CS7   (2217.46)
#define _D7    (2349.32)
#define _DS7   (2489.02)
#define _E7    (2637.02)
#define _F7    (2793.83)
#define _FS7   (2959.96)
#define _G7    (3135.96)
#define _GS7   (3322.44)
#define _A7    (3520.00)
#define _AS7   (3729.31)
#define _B7    (3951.07)
#define _C8    (4186.01)
#define _CS8   (4434.92)
#define _D8    (4698.63)
#define _DS8   (4978.03)
#define _E8    (5274.04)
#define _F8    (5587.65)
#define _FS8   (5919.91)
#define _G8    (6271.93)
#define _GS8   (6644.88)
#define _A8    (7040.00)
#define _AS8   (7458.62)
#define _B8    (7902.13)
