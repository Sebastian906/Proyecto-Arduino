// LCDManager.cpp
#include "LCDManager.h"

LCDManager::LCDManager() : lcd(0x27, 16, 2) {
  lcd.init();
  lcd.backlight();
}

void LCDManager::escribir(const String& texto, int col, int fila) {
  lcd.setCursor(col, fila);
  lcd.print(texto);
}

void LCDManager::limpiar() {
  lcd.clear();
}

void LCDManager::setCursor(int col, int fila) {
  lcd.setCursor(col, fila);
}

void LCDManager::mostrarTiempoRestante(int segundos) {
  lcd.setCursor(0, 0);
  lcd.print("Tiempo: ");
  lcd.print(segundos);
  lcd.print("s   ");
}

void LCDManager::mostrarNombreMascota(const String& nombre) {
  lcd.setCursor(0, 1);
  lcd.print("Mascota: ");
  lcd.print(nombre);
}

void LCDManager::mostrarCantidad(int gramos) {
  lcd.setCursor(0, 1);
  lcd.print("Cantidad: ");
  lcd.print(gramos);
  lcd.print("g   ");
}