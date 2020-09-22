 /*
    Full Example of LogansGreatButton Library
    In this example, you can see how easy it is to use this powerful button library.
    
    The circuit:
    For this example just connect a button to pin2 and ground.
    Created 26/01/2019
    By Logan K
    https://github.com/LoganTraceur/LogansGreatButton
    
    Modified with Dynamic Callbacks by HackThatNow 5/11/2019
*/


#include <LogansGreatButton.h>

#define BUTTON_1_PIN 1
LogansGreatButton button1(BUTTON_1_PIN);

#define BUTTON_2_PIN 2
LogansGreatButton button2(BUTTON_2_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to LogansGreatButton example!");

  // Setup button 1 callbacks
  button1.onActionPressed(onButtonActionPressed_B1);
	button1.onPressShortRelease(onButtonPressShortRelease_B1);
	button1.onPressLongStart(onButtonPressLongStart_B1);
	button1.onPressLongRelease(onButtonPressLongRelease_B1);
	button1.onHoldStart(onButtonHoldStart_B1);
	button1.onHoldContinuous(onButtonHoldContinuous_B1);
	button1.onHoldRelease(onButtonHoldRelease_B1);
	button1.onMultiClick(onMultiClicks_B1);
	button1.onShiftStart(onButtonShiftStart_B1);
	button1.onShiftRelease(onButtonShiftRelease_B1);

  // Setup button 2 callbacks
  button2.onActionPressed(onButtonActionPressed_B2);
  button2.onPressShortRelease(onButtonPressShortRelease_B2);
  button2.onPressLongStart(onButtonPressLongStart_B2);
  button2.onPressLongRelease(onButtonPressLongRelease_B2);
  button2.onHoldStart(onButtonHoldStart_B2);
  button2.onHoldContinuous(onButtonHoldContinuous_B2);
  button2.onHoldRelease(onButtonHoldRelease_B2);
  button2.onMultiClick(onMultiClicks_B2);
  button2.onShiftStart(onButtonShiftStart_B2);
  button2.onShiftRelease(onButtonShiftRelease_B2);
}

void loop() {
  button1.LOOPButtonController();
  button2.LOOPButtonController();
}
