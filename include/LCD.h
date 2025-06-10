#ifndef LCDM2_H
#define LCDM2_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "VisualizacionMascota.h"


// Pines del LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

class LCD {
  private:
    LiquidCrystal lcd;  // Objeto interno del LCD
    int col = 0;
    int row = 0;
    const int maxCols = 16;
    const int maxRows = 2;

  public:
    // Constructor
    LCD() : lcd(rs, en, d4, d5, d6, d7) {
      lcd.begin(maxCols, maxRows);
    }

    // Limpiar pantalla y resetear posición
    void limpiarDisplay() {
      col = 0;
      row = 0;
      lcd.clear();
      delay(200);
    }

    void escribirNormal(const String& texto) {
      lcd.print(texto);
    }

    void escribirDisplay(const VisualizacionMascota lista[2]) {
        delay(500);
        lcd.clear();
        lcd.setCursor(0, 0);
        
        // Línea 1: nombre y estado de las dos mascotas
        for (int i = 0; i < 2; i++) {
            String texto = lista[i].nombre + " " + lista[i].estado;
            lcd.print(texto);

            // Espacio entre columnas si no es el último
            if (i == 0) {
                int espacio = 8 - texto.length(); // espacio hasta el centro
                for (int j = 0; j < espacio; j++) lcd.print(" ");
            }
        }

        // Línea 2: número alineado debajo de cada grupo
        lcd.setCursor(0, 1);
        for (int i = 0; i < 2; i++) {
            String numStr = String(lista[i].numero);
            lcd.print(numStr);

            if (i == 0) {
                int espacio = 8 - numStr.length(); // espacio hasta el centro
                for (int j = 0; j < espacio; j++) lcd.print(" ");
            }
        }
    }


    // Opcional: método para posicionar el cursor manualmente
    void setCursor(int c, int r) {
      if (c >= 0 && c < maxCols && r >= 0 && r < maxRows) {
        col = c;
        row = r;
        lcd.setCursor(col, row);
      }
    }
};

#endif
