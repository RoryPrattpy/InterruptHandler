#define incrementBtn D8
#define stopBtn D7

#include "oled-wing-adafruit.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;

volatile bool interrupt = false;
bool interuptsAllowed = true;
int counter = 0;
bool readButtonValue = true;

void setup() {
  pinMode(incrementBtn, INPUT_PULLUP);
  pinMode(stopBtn, INPUT_PULLUP);

  attachInterrupt(incrementBtn, isr, FALLING);

  display.setup();

  resetDisplay();
	display.display();
}

void loop() {
  if (interrupt) {
    counter++;

    interrupt = false;
  }

  if (!digitalRead(stopBtn) && readButtonValue) {
    if (interuptsAllowed) {
      noInterrupts();
      interuptsAllowed = false;
    } else {
      interrupts();
      interuptsAllowed = true;
    }
    readButtonValue = false;
    interrupt = false;
  }

  if (digitalRead(stopBtn)) {
    readButtonValue = true;
  }

  resetDisplay();
  updateDisplay();
}

void isr() {
  interrupt = true;
}

void resetDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
}

void updateDisplay() {
  display.print("Counter: ");
  display.print(counter);
  display.display();
}