#include "ControlAlimentacion.h"

ControlAlimento sistema;

void setup() {
  Serial.begin(9600);
  sistema.iniciar();
}

void loop() {
  sistema.verificarRFID();
  //sistema.ejecutarCiclo();
}
