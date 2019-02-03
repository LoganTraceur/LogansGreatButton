/*
    Simple Example of LogansGreatButton Library

    In this example, you can see how easy it is to use this powerful button library.
    
    NOTE: THE BUTTON CALLBACK METHODS ARE IN THE SECOND TAB 
    Not only does it make it easier to keep your code clean but it solves a problem where the IDE requires the callback methods to be declared before the constructor

    The circuit:
    For this example just connect a button to pin2 and ground.

    Created 26/01/2019
    By Logan K

    https://github.com/LoganTraceur/LogansGreatButton

*/

// Required so that the picky Arduino IDE knows that these methods will be declared later (In the second tab). In a future version, this will not be necessary.
extern void onButtonActionPressed();
extern void onButtonPressShortRelease();
extern void onButtonPressLongStart();
extern void onButtonPressLongRelease();
extern void onButtonHoldStart();
extern void onButtonHoldContinuous();
extern void onButtonHoldRelease();
extern void onMultiClicks();
extern void onButtonShiftStart();
extern void onButtonShiftRelease();

// Class: Button
#include <LogansGreatButton.h>
#define BUTTON_PIN    2
LogansGreatButton button(BUTTON_PIN, onButtonActionPressed, onButtonPressShortRelease, onButtonPressLongStart, onButtonPressLongRelease, onButtonHoldStart, onButtonHoldContinuous, onButtonHoldRelease, onMultiClicks, onButtonShiftStart, onButtonShiftRelease);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Welcome to LogansGreatButton example!");
}

void loop() {
  // put your main code here, to run repeatedly:
  button.LOOPButtonController();
  
}