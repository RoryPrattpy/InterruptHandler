/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/CTD_2023/InterruptHandler/src/InterruptHandler.ino"
void setup();
void loop();
void isr();
void resetDisplay();
void updateDisplay();
#line 1 "/Users/admin/CTD_2023/InterruptHandler/src/InterruptHandler.ino"
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

  Serial.begin(9600);
  while (!Serial.isConnected()) {}
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
  Serial.println(digitalRead(stopBtn));

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