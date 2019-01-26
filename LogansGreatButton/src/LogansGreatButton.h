/*
LogansGreatButton - Library for operating a single button.
Created by Logan Krantz 2016/10/15.
*/

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define DEBUG_BUTTON_ON 1			// Uncomment to get a serial.print message for every button event
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
//_______________________________________________________________________________________________________________________________________________________________
// EXAMPLE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// What the button does in different situations

void onButtonActionPressed() 
{

}

void onButtonPressShort()
{

}

void onButtonPressLongStart()
{

}

void onButtonPressLongRelease()
{

}

void onButtonHoldStart()
{

}

void onButtonHoldContinuous()
{

}

void onButtonHoldRelease()
{

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


*/



#ifndef LogansGreatButton_h
#define LogansGreatButton_h

typedef void(*callBack) ();

/*
A Class to easily and powerfully control a button
Use callbacks for most button actions
And to use SHIFT clicks use "if (button.isShiftStateReady_ThenShiftMode()) {"

// Class: Button
#include <LogansGreatButton.h>
#define BUTTON_PIN		2
LogansGreatButton button(BUTTON_PIN, onButtonActionPressed, onButtonPressShort, onButtonPressLongStart, onButtonPressLongRelease, onButtonHoldStart, onButtonHoldContinuous, onButtonHoldRelease, onButtonShiftStart, onButtonShiftRelease);

button.LOOPButtonController();
*/
class LogansGreatButton
{
public:
	// Constructor
	LogansGreatButton(uint8_t buttonPin,
		callBack onActionPressed,
		callBack onPressShort,
		callBack onPressLongStart,
		callBack onPressLongRelease,
		callBack onHoldStart,
		callBack onHoldContinuous,
		callBack onHoldRelease,
		callBack onShiftStart,
		callBack onShiftRelease);

	// Controller. Place this method in the controller so the code runs.
	void LOOPButtonController();

	// Like the shift key on a keyboard. Calling this method quickly before the beginning of a HoldStart sets the "SHIFT" state and make "start" and "release" callbacks.
	// This function can be used by calling if (isShiftStateReady_ThenShiftMode()) {
	boolean isShiftStateReady_ThenShiftMode();

	// Accessors

	// Returns the millis() when the button was last pressed.
	uint32_t getLastPressTime();

	// Returns true if the button is currently held down.
	boolean isButtonPressed();		

	// Returns true if shift mode has successfully been activated using button.isShiftStateReady_ThenShiftMode() elsewhere.
	boolean isButtonInShiftMode();

private:
	//void interruptButton();
	void checkIfPressedOrReleased();
	void buttonActionReleased();

	
	// Callback Methods, This allows the user to use their own methods when a button event happens
	callBack
		_CallBackActionPressed,
		_CallBackPressShort,
		_CallBackPressLongStart,
		_CallBackPressLongRelease,
		_CallBackHoldStart,
		_CallBackHoldContinuous,
		_CallBackHoldRelease,
		_CallBackShiftStart,
		_CallBackShiftRelease;

	// Methods to improve readability of the code
	boolean
		isButtonHeld(),
		isButtonLongPress(),
		isButtonReleased();
	

	// Global Private Variables
	uint8_t _PIN_BUTTON;
	uint32_t _lastButtonPressTime;					// This variable is used to help the button respond to presses
	uint8_t _buttonCurrentState;					// This variable keeps track of the type of button press. It goes through 3 levels. PRESS, LONG, HOLD
	boolean _isStartOfHold;							// A variable to control onHoldStart, to ensure it is only run once per hold;
	boolean _isStartOfShift;						// A variable to control onShiftStart, to ensure it is only run once per hold;
};



#endif // LogansGreatButton_h