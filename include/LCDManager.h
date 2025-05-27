// LCDManager.h
#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <LiquidCrystal_I2C.h>

class LCDManager {
  public:
    LCDManager();
    void escribir(const String& texto, int col = 0, int fila = 0);
    void limpiar();
    void setCursor(int col, int fila);
    void mostrarTiempoRestante(int segundos);
    void mostrarNombreMascota(const String& nombre);
    void mostrarCantidad(int gramos);
  private:
    LiquidCrystal_I2C lcd;
};

#endif