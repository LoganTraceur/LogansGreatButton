/*
    Full Example of LogansGreatButton Library
    In this example, you can see how easy it is to use this powerful button library.
    
    The circuit:
    For this example just connect a button to pin2 and ground.
    Created 26/01/2019
    By Logan K
    https://github.com/LoganTraceur/LogansGreatButton
*/

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

//_______________________________________________________________________________________________________________________________________________________________
// EXAMPLE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// What the button does in different situations

void onButtonActionPressed() 
{
	Serial.println();
	Serial.println("The button has been PRESSED. What will the future hold?");
}

void onButtonPressShortRelease()
{
	Serial.println("End  of a SHORT Press");
}

void onButtonPressLongStart()
{
	Serial.println("Start of a LONG Press");
}

void onButtonPressLongRelease()
{
	Serial.println("End  of a LONG Press");
}

void onButtonHoldStart()
{
	Serial.println("Start of a HOLD Press");
}

void onButtonHoldContinuous()
{
	Serial.println("The button is being held. This message will be resend continuously.");
}

void onButtonHoldRelease()
{
	Serial.println("End of a HOLD Press");
}

void onMultiClicks()
{
	switch (button.getNumberOfMultiClicks())
	{
		// One click is considered a onButtonPressShortRelease(). 
		case 2:		Serial.println("The button has been clicked Twice"); break;
		case 3:		Serial.println("The button has been clicked Three Times"); break;
		case 4:		Serial.println("The button has been clicked Four Times"); break;
		default:	Serial.println(String("The button has been clicked ") + button.getNumberOfMultiClicks() + " times"); break;
	}
}

// SHIFT: Actions for the start and end of the shift state
// Ask if the button is able to be in shift mode and if it is, do that instead of long or short releases.
// A limbo state if you will.

// To use Place the following method just after you turn a dial or click a button.
// This simple if statement allows you to split your logic paths. 
// First it asks the button if it is being held down, and if so automatically sets it into shift mode so you can now shift click other buttons or spin a dial.
void EXAMPLE_METHOD_ButtonClicked_Or_DialTurned()
{
  if (button.isShiftStateReady_ThenShiftMode())
  {
    // Shift state ready and now activated. Shift click away.
  }
  else
  {
    // No shift state. Act normal. Button is not held down, or has been held down too long before this method was called.
  }
}

// At the start of a shift event, tell the user by flashing a light or making a buzz
void onButtonShiftStart()
{

}

// When the shift event ends, what should happen? (More Flashing and buzzing)
void onButtonShiftRelease()
{

}