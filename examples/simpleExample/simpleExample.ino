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

#define BUTTON_PIN 2
LogansGreatButton button(BUTTON_PIN);

void onPressShortRelease(void)
{
  Serial.println("Short Press");
}

void onPressLongStart(void)
{
  Serial.println("Long Press");
}

void setup() {
  Serial.begin(115200);

  button.onPressShortRelease(onPressShortRelease);
  button.onPressLongStart(onPressLongStart);
}

void loop() {
  button.LOOPButtonController();
}
