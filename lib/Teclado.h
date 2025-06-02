// Teclado.h
#ifndef TECLADO_H
#define TECLADO_H

#include <Keypad.h>

class Teclado {
  public:
    Teclado();
    int leerTiempo();   // Devuelve tiempo en segundos (10-30)
    int leerGramos();   // Devuelve gramos (1-10)
  private:
    Keypad keypad;
    int leerNumero(int min, int max, const char* mensaje);
};

#endif