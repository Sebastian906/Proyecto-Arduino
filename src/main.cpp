#include <Arduino.h>
#include "ControlAlimentacion.h"

ControlAlimento controlAlimento;

void setup() {
  Serial.begin(9600);
 
  controlAlimento.iniciar();
}

void loop(){
controlAlimento.dispensar();

}

