void loop() {
  usb.Task();
  //when pressing enter (positive edge) display the keyboard menu. Print start screen when leaving the keyboard menu  
  if (arrKeyPressed[USB_ENTER]==true && arrKeyPressedLast[USB_ENTER]==false){
   arrKeyPressedLast[USB_ENTER]=true;
   KeyboardMenu();
   StartScreen();
  }
}
