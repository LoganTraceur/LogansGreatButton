/*
LogansGreatButton - Library for operating a single button.
Created by Logan Krantz 2016/10/15.
*/

#include "Arduino.h"
#include "LogansGreatButton.h"

#ifdef DEBUG_BUTTON_ON
	//#define DEBUG_PRINT(Message) Serial.print(String("ButtonState: ") + _buttonCurrentState +"\t" + Message); 
	#define DEBUG_PRINT(Message) Serial.print(Message); 
	uint32_t _multiClickTimes[20];										// An array used for debugging that records the timing of multiclicks
#else
	#define DEBUG_PRINT(Message)
#endif // DEBUG_BUTTON


// Create callback type
typedef void(*callBack) ();

// Global Private Variables
uint32_t _lastButtonPressTime = 0;									// This variable is used to help the button respond to presses
uint32_t _millisAtStartOfLoop = 0;									// This variable is set to millis() at the beginning of each loop. This limits the calls to millis() to a max of once per loop
uint8_t _buttonCurrentState = 0;									// This variable keeps track of the type of button press. It goes through 3 levels. PRESS, LONG, HOLD
uint16_t _numberOfMultiClicks = 0;										// This variable counts how many times the button has been pressed in a multiclick event
boolean _isStartOfHold = true;										// A variable to control onHoldStart, to ensure it is only run once per hold;
boolean _isStartOfShift = true;										// A variable to control onShiftStart, to ensure it is only run once per hold;

// Magic Number Avoiders
const uint8_t BUTTON_STATE_WAITING = 0;
const uint8_t BUTTON_STATE_PRESSED = 1;
const uint8_t BUTTON_STATE_MULTI_CLICK = 2;
const uint8_t BUTTON_STATE_LONG_PRESSED = 3;
const uint8_t BUTTON_STATE_HOLD = 4;
const uint8_t BUTTON_STATE_SHIFT = 5;								// Like holding shift on the keyboard: This is a special state set by the user that allows "shift clicks"

const uint16_t DELAY_DEBOUNCE = 120;								// This avoids debounce or accidental double clicks. Also Minimum time between multiclicks
const uint16_t DELAY_SHORT_OR_MULTI_CLICK = 350;					// If another click is detected before this expires then start counting multiclicks. This is also the delay before a shortClickRelease is fired
const uint16_t DELAY_BEFORE_LONG_PRESS = 750;						// How long must a button be held down for it to be considered a long press
const uint16_t DELAY_BEFORE_BUTTON_HOLD = 1500;						// How long before hold is registered
																	// AND the Time available to activate shift mode.
																	// This value should be longer then a long press.

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Constuctor

LogansGreatButton::LogansGreatButton(uint8_t buttonPin,
	callBack onActionPressed,
	callBack onPressShortRelease,
	callBack onPressLongStart,
	callBack onPressLongRelease,
	callBack onHoldStart,
	callBack onHoldContinuous,
	callBack onHoldRelease,
	callBack onMultiClick,
	callBack onShiftStart,
	callBack onShiftRelease)
{
	// Setup Button Output and interrupts
	_PIN_BUTTON = buttonPin;
	pinMode(buttonPin, INPUT_PULLUP);
	digitalWrite(buttonPin, HIGH);									// Really force that pull up!
	//attachInterrupt(digitalPinToInterrupt(buttonPin), object.interruptLink, CHANGE);		// This sets up the interupt function. (0-> means pin 2, interupt-> is the name of my method, CHANGE-> says that is what the arduino is monitoring)


	// Setup CallBackFunctions
	_CallBackActionPressed = onActionPressed;
	_CallBackPressShortRelease = onPressShortRelease;
	_CallBackPressLongStart = onPressLongStart;
	_CallBackPressLongRelease = onPressLongRelease;
	_CallBackHoldStart = onHoldStart;
	_CallBackHoldContinuous = onHoldContinuous;
	_CallBackHoldRelease = onHoldRelease;
	_CallBackMultiClicks = onMultiClick;
	_CallBackShiftStart = onShiftStart;
	_CallBackShiftRelease = onShiftRelease;

	// Re-assigning Start Booleans avoids errors for some reason
	_isStartOfHold = true;
	_isStartOfShift = true;
}

void LogansGreatButton::checkIfPressedOrReleased()
{
	boolean currentState = digitalRead(_PIN_BUTTON);
	static boolean previousState = currentState;					// HIGH = off, as the have set the pin to PULLUP mode. Which reverses the signals
	//Serial.println(String("Button Pin State: ") + digitalRead(_PIN_BUTTON));	// DEBUG: Should be High(1)
	//Serial.println("Checking for Pressed");

	if (currentState != previousState)
	{
		previousState = currentState;
		// BUTTON PRESSED
		if (isButtonPressed())
		{
			if (_millisAtStartOfLoop - _lastButtonPressTime > DELAY_DEBOUNCE)	// Debounce: Simulates a delay without stopping the rest of the sketch
			{
				_lastButtonPressTime = _millisAtStartOfLoop;					// Button Press accepted, reset time
				#ifdef DEBUG_BUTTON_ON
					_multiClickTimes[_numberOfMultiClicks] = _lastButtonPressTime;
				#endif // DEBUG_BUTTON
				_numberOfMultiClicks = _numberOfMultiClicks + 1;				// The button has been relased after a short time. Add 1 to the multi click counter

				if (_buttonCurrentState == BUTTON_STATE_WAITING)
				{
					// This is the first click in this series
					DEBUG_PRINT("\n_CallBackActionPressed");
					_buttonCurrentState = BUTTON_STATE_PRESSED;
					_CallBackActionPressed();
				}
				else if (_buttonCurrentState == BUTTON_STATE_MULTI_CLICK)
				{
					// This is for subsequent clicks in a series
					//Serial.println(String("#Clicks: ") + _numberOfMultiClicks + "\tTime: " + _multiClickTimes[_numberOfMultiClicks]);
				}
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
	_millisAtStartOfLoop = millis();
	// This method has Several responsibilites. in order.
	//1. To check if there hase been a press or release event
	checkIfPressedOrReleased();
	//Serial.println(String("_buttonCurrentState: ") + _buttonCurrentState);

	if (_buttonCurrentState != BUTTON_STATE_WAITING)				// Only run a check if button is currently being held (CPU saver)
	{
		// 2. If the button has been released in a short time it could be a "Short Press" or a "MultiClick". Either way it can no longer be a LONG, HOLD or SHIFT
		if (_buttonCurrentState == BUTTON_STATE_MULTI_CLICK)
		{
			// Wait until DELAY_SHORT_OR_MULTI_CLICK has expired before resolving the release event
			// This also gives the user time to make another click
			if (_millisAtStartOfLoop > _lastButtonPressTime + DELAY_SHORT_OR_MULTI_CLICK)
			{
				releaseOfShortOrMultiClicks();
			}
		}
		// 3. Check whether a shift state has been activeated by the user elsewhere
		else if (_buttonCurrentState == BUTTON_STATE_SHIFT)
		{
			// Register start of shift for callback.
			// Otherwise do nothing, this state is manually set by the user 
			if (_isStartOfShift)
			{
				DEBUG_PRINT("\n_CallBackShiftStart");
				_CallBackShiftStart();
				_isStartOfShift = false;
			}
			//DEBUG_PRINT("\n_CallBackShiftContinuous");
			//_CallBackShiftContinuous();							// Runs Continuously while button is held down during a shift event
		}
		// 4. OR Acknowlage the button being held
		else if (isButtonHeld())									// Hold is slightly longer then long, and therefore tested first
		{
			if (_isStartOfHold)										// Only happen once at the begining of a hold
			{
				DEBUG_PRINT("\n_CallBackHoldStart");
				_buttonCurrentState = BUTTON_STATE_HOLD;
				_CallBackHoldStart();
				_isStartOfHold = false;
			}
			DEBUG_PRINT(".")
			_CallBackHoldContinuous();								// Runs Continuously while button is held down
		}
		// 5. OR Acknowlage the long press
		else if (isButtonLongPress())								// Long press is slightly shorter then hold, and therefore tested second using an else
		{
			DEBUG_PRINT("\n_CallBackPressLongStart");
			_buttonCurrentState = BUTTON_STATE_LONG_PRESSED;
			_CallBackPressLongStart();
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Weird Shift State, boolean mutator

// Like the shift key on a keyboard. Calling this method quickly before the beginning of a HoldStart sets the "SHIFT" state and make "start" and "release" callbacks.
boolean LogansGreatButton::isShiftStateReady_ThenShiftMode()
{
	if (_buttonCurrentState == BUTTON_STATE_SHIFT					// TRUE IF Already in Shift State
		|| _buttonCurrentState == BUTTON_STATE_PRESSED				// OR button is pressed (but not yet longPress)
		|| _buttonCurrentState == BUTTON_STATE_LONG_PRESSED			// OR button is Long Pressed
		)
	{
		_buttonCurrentState = BUTTON_STATE_SHIFT;					// This activates shift state which simply stops any other mode from activating.
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

uint16_t LogansGreatButton::getNumberOfMultiClicks()
{
	return _numberOfMultiClicks;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Release events
void LogansGreatButton::buttonActionReleased()
{
	// This method is run when the button is released and responds specifically to the stype of button press
	switch (_buttonCurrentState)
	{
	case BUTTON_STATE_WAITING:										// Waiting does not have a release event		
		break;
	case BUTTON_STATE_PRESSED:
	{
		_buttonCurrentState = BUTTON_STATE_MULTI_CLICK;				// Wait for possible multiclicks
	}
	break;
	case BUTTON_STATE_MULTI_CLICK:
	{
		_buttonCurrentState = BUTTON_STATE_MULTI_CLICK;				// Wait for possible multiclicks
	}
	break;
	case BUTTON_STATE_LONG_PRESSED:
	{
		DEBUG_PRINT("\n_CallBackPressLongRelease");
		_buttonCurrentState = BUTTON_STATE_WAITING;					// Reset the button state ready for next press
		_CallBackPressLongRelease();								// This mode may have a release event to undo the prevously held state
	}
	break;
	case BUTTON_STATE_HOLD:
	{
		DEBUG_PRINT("\n_CallBackHoldRelease");
		_isStartOfHold = true;										// Reset _isStartOfHold so that is ready to accept the start on a new hold next time
		_buttonCurrentState = BUTTON_STATE_WAITING;					// Reset the button state ready for next press
		_CallBackHoldRelease();										// This mode may have a release event to undo the prevously held state
	}
	break;
	case BUTTON_STATE_SHIFT:
	{
		DEBUG_PRINT("\n_CallBackShiftRelease");
		_isStartOfShift = true;										// Reset _isStartOfShift so that is ready to accept the start on a new shift next time
		_buttonCurrentState = BUTTON_STATE_WAITING;					// This needs to be done before _CallBackShiftRelease otherwise isButtonInShiftMode() returns the wrong value when called in _CallBackShiftRelease
		_CallBackShiftRelease();									// This callback allows a user set release event
	}
	break;
	default: break;
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Multiclick code

void LogansGreatButton::releaseOfShortOrMultiClicks()
{
	// Time for multiclick expired. Resolve into a release event.
	if (_numberOfMultiClicks == 1)
	{
		DEBUG_PRINT("\n_CallBackPressShortRelease");
		_buttonCurrentState = BUTTON_STATE_WAITING;					// Reset the button state ready for next press
		_CallBackPressShortRelease();
		_numberOfMultiClicks = 0;									// Reset the multiclick counter
	}
	else
	{
		DEBUG_PRINT("\n_CallBackMultiClickRelease: " + String(_numberOfMultiClicks) + " clicks");
		_buttonCurrentState = BUTTON_STATE_WAITING;					// Reset the button state ready for next press
		#ifdef DEBUG_BUTTON_ON
		{
			String debugStr;
			for (int i = 0; i < _numberOfMultiClicks; i++) { debugStr = debugStr + "\n " + _multiClickTimes[i] + "\t" + (_multiClickTimes[i] - _multiClickTimes[i - 1]); }
			Serial.println(debugStr);
		}
		#endif // DEBUG_BUTTON
		_CallBackMultiClicks();
		_numberOfMultiClicks = 0;									// Reset the multiclick counter
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Boolean Button Tests: These improve readability of the code above

boolean LogansGreatButton::isButtonHeld()
{
	return (_millisAtStartOfLoop > _lastButtonPressTime + DELAY_BEFORE_BUTTON_HOLD);
}

boolean LogansGreatButton::isButtonLongPress()
{
	// The button has be held long enough
	// And the long press has not already been registered
	return ((_buttonCurrentState != BUTTON_STATE_LONG_PRESSED) && (_millisAtStartOfLoop > _lastButtonPressTime + DELAY_BEFORE_LONG_PRESS));
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
//	if (_millisAtStartOfLoop - _lastButtonPressTime > DELAY_DEBOUNCE)			// Debounce: Simulates a delay without stopping the rest of the sketch
//	{
//		if (isButtonPressed())
//		{
//			_lastButtonPressTime = _millisAtStartOfLoop;						// Button Press accepted, reset time
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
