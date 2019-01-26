/*
LogansGreatButton - Library for operating a single button.
Created by Logan Krantz 2016/10/15.
*/

#include "Arduino.h"
#include "LogansGreatButton.h"

#ifdef DEBUG_BUTTON_ON
//#define DEBUG_PRINT(Message) Serial.print(String("ButtonState: ") + _buttonCurrentState +"\t" + Message); 
#define DEBUG_PRINT(Message) Serial.print(Message); 
#else
#define DEBUG_PRINT(Message)
#endif // DEBUG_BUTTON




// Create callback type
typedef void(*callBack) ();

// Global Private Variables
uint32_t _lastButtonPressTime = 0;					// This variable is used to help the button respond to presses
uint8_t _buttonCurrentState = 0;					// This variable keeps track of the type of button press. It goes through 3 levels. PRESS, LONG, HOLD
boolean _isStartOfHold = true;						// A variable to control onHoldStart, to ensure it is only run once per hold;
boolean _isStartOfShift = true;						// A variable to control onShiftStart, to ensure it is only run once per hold;

// Magic Number Avoiders
const uint8_t BUTTON_STATE_WAITING = 0;
const uint8_t BUTTON_STATE_PRESSED = 1;
const uint8_t BUTTON_STATE_LONG_PRESSED = 2;
const uint8_t BUTTON_STATE_HOLD = 3;
const uint8_t BUTTON_STATE_SHIFT = 4;				// Like holding shift on the keyboard: This is a special state set by the user that allows "shift clicks"

const uint16_t DELAY_DEBOUNCE = 250;				// This avoids debounce or accidental double clicks
const uint16_t DELAY_BEFORE_LONG_PRESS = 750;		// How long must a button be held down for it to be considered a long press
const uint16_t DELAY_BEFORE_BUTTON_HOLD = 1500;		// How long before hold is registered
													// AND the Time available to activate shift mode.
													// This value should be longer then a long press.

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Constuctor

LogansGreatButton::LogansGreatButton(uint8_t buttonPin,
	callBack onActionPressed,
	callBack onPressShort,
	callBack onPressLongStart,
	callBack onPressLongRelease,
	callBack onHoldStart,
	callBack onHoldContinuous,
	callBack onHoldRelease,
	callBack onShiftStart,
	callBack onShiftRelease)
{
	// Setup Button Output and interrupts
	_PIN_BUTTON = buttonPin;
	pinMode(buttonPin, INPUT_PULLUP);
	digitalWrite(buttonPin, HIGH);			// Really force that pull up!
	//attachInterrupt(digitalPinToInterrupt(buttonPin), object.interruptLink, CHANGE);		// This sets up the interupt function. (0-> means pin 2, interupt-> is the name of my method, CHANGE-> says that is what the arduino is monitoring)


	// Setup CallBackFunctions
	_CallBackActionPressed = onActionPressed;
	_CallBackPressShort = onPressShort;
	_CallBackPressLongStart = onPressLongStart;
	_CallBackPressLongRelease = onPressLongRelease;
	_CallBackHoldStart = onHoldStart;
	_CallBackHoldContinuous = onHoldContinuous;
	_CallBackHoldRelease = onHoldRelease;
	_CallBackShiftStart = onShiftStart;
	_CallBackShiftRelease = onShiftRelease;

	// Re-assigning Start Booleans avoids errors for some reason
	_isStartOfHold = true;
	_isStartOfShift = true;
}

void LogansGreatButton::checkIfPressedOrReleased()
{
	boolean currentState = digitalRead(_PIN_BUTTON);
	static boolean previousState = currentState;							// HIGH = off, as the have set the pin to PULLUP mode. Which reverses the signals
	//Serial.println(String("Button Pin State: ") + digitalRead(_PIN_BUTTON));	// DEBUG: Should be High(1)

	if (currentState != previousState)
	{
		previousState = currentState;
		// BUTTON PRESSED
		if (isButtonPressed())
		{
			if (millis() - _lastButtonPressTime > DELAY_DEBOUNCE)	// Debounce: Simulates a delay without stopping the rest of the sketch
			{
				_lastButtonPressTime = millis();						// Button Press accepted, reset time
				_buttonCurrentState = BUTTON_STATE_PRESSED;
				_CallBackActionPressed();
				DEBUG_PRINT("\n_CallBackActionPressed")
			}
		}
		// BUTTON RELEASED	
		else if (isButtonReleased())
		{
			buttonActionReleased();
		}

	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Loop Controller
// PUT IN LOOP. This method controls seemless multitasking without the use of delay
void LogansGreatButton::LOOPButtonController()
{
	// This method has Several responsibilites. in order.
	//1. To check if there hase been a press or release event
	checkIfPressedOrReleased();


	if (_buttonCurrentState != BUTTON_STATE_WAITING)		// Only run a check if button is currently being held (CPU saver)
	{
		// 2. Check whether a shift state has been activeated by the user elsewhere
		if (_buttonCurrentState == BUTTON_STATE_SHIFT)
		{
			// Register start of shift for callback.
			// Otherwise do nothing, this state is manually set by the user 
			if (_isStartOfShift)
			{
				_CallBackShiftStart();
				_isStartOfShift = false;
				DEBUG_PRINT("\n_CallBackShiftStart");
			}
			//_CallBackShiftContinuous();					// Runs Continuously while button is held down during a shift event
			//DEBUG_PRINT("\n_CallBackShiftContinuous");
		}
		// 3. OR Acknowlage the button being held
		else if (isButtonHeld())							// Hold is slightly longer then long, and therefore tested first
		{
			if (_isStartOfHold)								// Only happen once at the begining of a hold
			{
				_buttonCurrentState = BUTTON_STATE_HOLD;
				_CallBackHoldStart();
				_isStartOfHold = false;
				DEBUG_PRINT("\n_CallBackHoldStart");
			}
			_CallBackHoldContinuous();						// Runs Continuously while button is held down
			DEBUG_PRINT(".")
		}
		// 4. OR Acknowlage the long press
		else if (isButtonLongPress())						// Long press is slightly shorter then hold, and therefore tested second using an else
		{
			_buttonCurrentState = BUTTON_STATE_LONG_PRESSED;
			_CallBackPressLongStart();
			DEBUG_PRINT("\n_CallBackPressLongStart");
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Weird Shift State, boolean mutator

// Like the shift key on a keyboard. Calling this method quickly before the beginning of a HoldStart sets the "SHIFT" state and make "start" and "release" callbacks.
boolean LogansGreatButton::isShiftStateReady_ThenShiftMode()
{
	if (_buttonCurrentState == BUTTON_STATE_SHIFT				// TRUE IF Already in Shift State
		|| _buttonCurrentState == BUTTON_STATE_PRESSED			// OR button is pressed (but not yet longPress)
		|| _buttonCurrentState == BUTTON_STATE_LONG_PRESSED		// OR button is Long Pressed
		)
	{
		_buttonCurrentState = BUTTON_STATE_SHIFT;		// This activates shift state which simply stops any other mode from activating.
		return true;
	}
	else
	{
		// The button is being not been pressed, or has been pressed for to long and so shift click is not "ready". Therefore do nothing.
		return false;
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Accessor
uint32_t LogansGreatButton::getLastPressTime()
{
	return _lastButtonPressTime;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Release events
void LogansGreatButton::buttonActionReleased()
{
	//buzzerCurrentMode = BUZZER_MODE_OFF;

	// This method is run when the button is released and responds specifically to the stype of button press
	switch (_buttonCurrentState)
	{
	case BUTTON_STATE_WAITING:									// Waiting does not have a release event		
		break;
	case BUTTON_STATE_PRESSED:
	{
		_CallBackPressShort();
		DEBUG_PRINT("\n_CallBackPressShort");
	}
	break;
	case BUTTON_STATE_LONG_PRESSED:
	{
		_CallBackPressLongRelease();	// This mode may have a release event to undo the prevously held state
		DEBUG_PRINT("\n_CallBackPressLongRelease");
	}
	break;
	case BUTTON_STATE_HOLD:
	{
		_isStartOfHold = true;				// Reset _isStartOfHold so that is ready to accept the start on a new hold next time
		_CallBackHoldRelease();				// This mode may have a release event to undo the prevously held state
		DEBUG_PRINT("\n_CallBackHoldRelease");
	}
	break;
	case BUTTON_STATE_SHIFT:
	{
		_isStartOfShift = true;				// Reset _isStartOfShift so that is ready to accept the start on a new shift next time
		_buttonCurrentState = BUTTON_STATE_WAITING;	// This needs to be done before _CallBackShiftRelease otherwise isButtonInShiftMode() returns the wrong value when called in _CallBackShiftRelease
		_CallBackShiftRelease();			// This callback allows a user set release event
		DEBUG_PRINT("\n_CallBackShiftRelease");
	}
	break;
	default: break;
	}

	_buttonCurrentState = BUTTON_STATE_WAITING;						// Reset the button state ready for next press
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Boolean Button Tests: These improve readability of the code above

boolean LogansGreatButton::isButtonHeld()
{
	return (millis() > _lastButtonPressTime + DELAY_BEFORE_BUTTON_HOLD);
}

boolean LogansGreatButton::isButtonLongPress()
{
	// The button has be held long enough
	// And the long press has not already been registered
	return (_buttonCurrentState != BUTTON_STATE_LONG_PRESSED && millis() > _lastButtonPressTime + DELAY_BEFORE_LONG_PRESS);
}

boolean LogansGreatButton::isButtonInShiftMode()
{
	return (_buttonCurrentState == BUTTON_STATE_SHIFT);
}

boolean LogansGreatButton::isButtonPressed()
{
	// Note the value LOW. This counter intuative code is because we activated the pullup resistor to reduce noise
	return (digitalRead(_PIN_BUTTON) == LOW);
}

boolean LogansGreatButton::isButtonReleased()
{
	// Note the value HIGH. This counter intuative code is because we activated the pullup resistor to reduce noise
	return (digitalRead(_PIN_BUTTON) == HIGH);
}


// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Interrupt

//void LogansGreatButton::interruptButton()
//{
//	// BUTTON PRESSED
//	// Accept button press after debounce delay
//	if (millis() - _lastButtonPressTime > DELAY_DEBOUNCE)	// Debounce: Simulates a delay without stopping the rest of the sketch
//	{
//		if (isButtonPressed())
//		{
//			_lastButtonPressTime = millis();									// Button Press accepted, reset time
//			_buttonCurrentState = BUTTON_STATE_PRESSED;
//			_CallBackActionPressed();
//		}
//	}
//
//	// BUTTON RELEASED
//	if (isButtonReleased())
//	{
//		buttonActionReleased();
//	}
//}
