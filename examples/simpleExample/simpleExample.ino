#include <LogansGreatButton.h>

#define BUTTON_PIN 25
LogansGreatButton button(BUTTON_PIN);

void onPressShortRelease(void)
{
  Serial.print("Short Press");
}

void onPressLongStart(void)
{
  Serial.print("Long Press");
}

void setup() {
  Serial.begin(115200);

  button.onPressShortRelease(onPressShortRelease);
  button.onPressLongStart(onPressLongStart);
}

void loop() {
  button.LOOPButtonController();
}
