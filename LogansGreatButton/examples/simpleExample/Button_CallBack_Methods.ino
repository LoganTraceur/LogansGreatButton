//_______________________________________________________________________________________________________________________________________________________________
// EXAMPLE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// What the button does in different situations

void onButtonActionPressed() 
{
  Serial.println("The Button has been pressed. What will the future hold?");
}

void onButtonPressShort()
{
  Serial.println("The Button was released fast enough to be a short press.");
}

void onButtonPressLongStart()
{
  Serial.println("The Button was held long enought to trigger the start of a long press.");
}

void onButtonPressLongRelease()
{
  Serial.println("The Button was held long enough but released before a hold. A Long Reslease it is!");
}

void onButtonHoldStart()
{
  Serial.println("The Button was held so long, its the start of a hold event.");
}

void onButtonHoldContinuous()
{
  Serial.println("The Button is being held. This message will be resent continiuoly.");
}

void onButtonHoldRelease()
{
  Serial.println("The button, was being held and has now been released!");
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