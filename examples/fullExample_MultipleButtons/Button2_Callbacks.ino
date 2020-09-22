//_______________________________________________________________________________________________________________________________________________________________
// EXAMPLE BUTTON TWO
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// What the button does in different situations

void onButtonActionPressed_B2() 
{
  Serial.println();
  Serial.println("Button 2: The button has been PRESSED. What will the future hold?");
}

void onButtonPressShortRelease_B2()
{
  Serial.println("Button 2: End  of a SHORT Press");
}

void onButtonPressLongStart_B2()
{
  Serial.println("Button 2: Start of a LONG Press");
}

void onButtonPressLongRelease_B2()
{
  Serial.println("Button 2: End  of a LONG Press");
}

void onButtonHoldStart_B2()
{
  Serial.println("Button 2: Start of a HOLD Press");
}

void onButtonHoldContinuous_B2()
{
  Serial.println("Button 2: The button is being held. This message will be resend continuously.");
}

void onButtonHoldRelease_B2()
{
  Serial.println("Button 2: End of a HOLD Press");
}

void onMultiClicks_B2()
{
  switch (button1.getNumberOfMultiClicks())
  {
    // One click is considered a onButtonPressShortRelease(). 
    case 2:   Serial.println("Button 2: The button has been clicked Twice"); break;
    case 3:   Serial.println("Button 2: The button has been clicked Three Times"); break;
    case 4:   Serial.println("Button 2: The button has been clicked Four Times"); break;
    default:  Serial.println(String("Button 2: The button has been clicked ") + button2.getNumberOfMultiClicks() + " times"); break;
  }
}

// SHIFT: Actions for the start and end of the shift state
// Ask if the button is able to be in shift mode and if it is, do that instead of long or short releases.
// A limbo state if you will.

// To use Place the following method just after you turn a dial or click a button.
// This simple if statement allows you to split your logic paths. 
// First it asks the button if it is being held down, and if so automatically sets it into shift mode so you can now shift click other buttons or spin a dial.
void EXAMPLE_METHOD2_ButtonClicked_Or_DialTurned()
{
  if (button2.isShiftStateReady_ThenShiftMode())
  {
    // Shift state ready and now activated. Shift click away.
  }
  else
  {
    // No shift state. Act normal. Button is not held down, or has been held down too long before this method was called.
  }
}

// At the start of a shift event, tell the user by flashing a light or making a buzz
void onButtonShiftStart_B2()
{

}

// When the shift event ends, what should happen? (More Flashing and buzzing)
void onButtonShiftRelease_B2()
{

}
