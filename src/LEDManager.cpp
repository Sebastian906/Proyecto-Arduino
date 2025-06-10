// LEDManager.cpp
#include "LEDManager.h"
#include <Arduino.h>

LEDManager::LEDManager() {

}

void LEDManager::parpadear(int cuenco) {
    pinMode(cuenco, OUTPUT);           // Configura el pin como salida
    digitalWrite(cuenco, HIGH);        // Enciende el LED del cuenco
    delay(200);                        
    digitalWrite(cuenco, LOW);         // Apaga el LED
    delay(200);
}
