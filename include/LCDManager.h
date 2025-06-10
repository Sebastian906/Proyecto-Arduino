#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "VisualizacionMascota.h"

class LCDManager {
  public:
    LCDManager();
    
    void escribir(const String& texto, int col = 0, int fila = 0);
    void limpiar();
    void setCursor(int col, int fila);
    void mostrarTiempoRestante(int segundos);
    void mostrarNombreMascota(const String& nombre);
    void mostrarCantidad(int gramos);
    void escribirDisplay(const VisualizacionMascota lista[2]);

  private:
    LiquidCrystal_I2C lcd;
    const int maxCols = 16;
    const int maxRows = 2;
};

#endif
