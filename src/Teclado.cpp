// Teclado.cpp
#include "Teclado.h"

const byte FILAS = 4;
const byte COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {9, 8, 7, 6};
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2};

Teclado::Teclado() : keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS) {}

int Teclado::leerNumero(int min, int max, const char* mensaje) {
  int valor = 0;
  bool terminado = false;
  Serial.println(mensaje);
  while (!terminado) {
    char key = keypad.getKey();
    if (key && isDigit(key)) {
      valor = valor * 10 + (key - '0');
      Serial.print(key);
    } else if (key == '#') { // Confirmar con #
      if (valor >= min && valor <= max) {
        terminado = true;
      } else {
        Serial.println("\nValor fuera de rango. Intenta de nuevo.");
        valor = 0;
      }
    } else if (key == '*') { // Borrar con *
      valor = 0;
      Serial.println("\nBorrado.");
    }
  }
  Serial.println();
  return valor;
}

int Teclado::leerTiempo() {
  return leerNumero(10, 30, "Introduce el tiempo (10-30 s) y confirma con #:");
}

int Teclado::leerGramos() {
  return leerNumero(1, 10, "Introduce la cantidad (1-10 g) y confirma con #:");
}