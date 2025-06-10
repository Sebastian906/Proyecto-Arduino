// LEDManager.cpp
#include "LEDManager.h"
#include <Arduino.h>

LEDManager::LEDManager() {

}

void LEDManager::parpadear(int cuenco, unsigned long tiempoMs) {
    pinMode(cuenco, OUTPUT);           // Configura el pin como salida
    digitalWrite(cuenco, HIGH);        // Enciende el LED del cuenco
    delay(tiempoMs);                        
    digitalWrite(cuenco, LOW);         // Apaga el LED
}
