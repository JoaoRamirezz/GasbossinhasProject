#define CUSTOM_SETTINGS
#define INCLUDE_LEDCONTROL_MODULE
#define LED 13
#include <DabbleESP32.h>
unsigned long lasttime=0;
void setup() {
  Serial.begin(115200);
  Dabble.begin("MyEsp32");
  pinMode(LED, OUTPUT);
}

void loop() {
  Dabble.processInput();
  Serial.print("State:");
  Serial.print(LedControl.getpinState());
  digitalWrite(LED, LedControl.getpinState());
  }
