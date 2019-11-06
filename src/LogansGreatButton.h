/*
LogansGreatButton - Library for operating a single button.
Created by Logan Krantz 2016/10/15.
*/

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define DEBUG_BUTTON_ON 1			// Uncomment to get a serial.print message for every button event
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef LogansGreatButton_h
#define LogansGreatButton_h

#include <Arduino.h>
typedef void(*callBack)();


// Required so that the picky Arduino IDE knows that these methods will be declared later by the user (see second tab of the example)
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





/*
A Class to easily and powerfully control a button
Use callbacks for most button actions
And to use SHIFT clicks use "if (button.isShiftStateReady_ThenShiftMode()) {"
// Class: Button
#include <LogansGreatButton.h>
#define BUTTON_PIN		2
LogansGreatButton button(BUTTON_PIN, onButtonActionPressed, onButtonPressShortRelease, onButtonPressLongStart, onButtonPressLongRelease, onButtonHoldStart, onButtonHoldContinuous, onButtonHoldRelease, onButtonShiftStart, onButtonShiftRelease);
button.LOOPButtonController();
*/
class LogansGreatButton
{
public:

	
	// Constructor
	LogansGreatButton(uint8_t buttonPin);
	
	LogansGreatButton(uint8_t buttonPin,
		callBack onActionPressed,
		callBack onPressShortRelease,
		callBack onPressLongStart,
		callBack onPressLongRelease,
		callBack onHoldStart,
		callBack onHoldContinuous,
		callBack onHoldRelease,
		callBack onMultiClicks,
		callBack onShiftStart,
		callBack onShiftRelease);
	
	// Controller. Place this method in the controller so the code runs.
	void LOOPButtonController();
	
	// Callback setters
	void onActionPressed(callBack ptr_onActionPressed);
	void onPressShortRelease(callBack ptr_onPressShortRelease);
	void onPressLongStart(callBack ptr_onPressLongStart);
	void onPressLongRelease(callBack ptr_onPressLongRelease);
	void onHoldStart(callBack ptr_onHoldStart);
	void onHoldContinuous(callBack ptr_onHoldContinuous);
	void onHoldRelease(callBack ptr_onHoldRelease);
	void onMultiClick(callBack ptr_onMultiClick);
	void onShiftStart(callBack ptr_onShiftStart);
	void onShiftRelease(callBack ptr_onShiftRelease);

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

	// Returns the current number of multiclicks
	uint16_t getNumberOfMultiClicks();
	

private:
	//void interruptButton();
	void checkIfPressedOrReleased();
	void buttonActionReleased(); 
	void releaseOfShortOrMultiClicks();
	
	// Callback Methods, This allows the user to use their own methods when a button event happens
	callBack
		_CallBackActionPressed,
		_CallBackPressShortRelease,
		_CallBackPressLongStart,
		_CallBackPressLongRelease,
		_CallBackHoldStart,
		_CallBackHoldContinuous,
		_CallBackHoldRelease,
		_CallBackMultiClicks,
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
	uint32_t _millisAtStartOfLoop;					// This variable is set to millis() at the beginning of each loop. This limits the calls to millis() to a max of once per loop
	uint16_t _numberOfMultiClicks;					// This variable counts how many times the button has been pressed in a multiclick event
	boolean _isStartOfHold;							// A variable to control onHoldStart, to ensure it is only run once per hold;
	boolean _isStartOfShift;						// A variable to control onShiftStart, to ensure it is only run once per hold;
};



#endif // LogansGreatButton_h
