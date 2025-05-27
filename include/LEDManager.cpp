// LEDManager.cpp
#include "LEDManager.h"
#include <Arduino.h>

LEDManager::LEDManager(int pin) : ledPin(pin) {
  pinMode(ledPin, OUTPUT);
}

void LEDManager::parpadear(int gramos) {
  for (int i = 0; i < gramos; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}