#include "LCDManager.h"

// Dirección por defecto 0x27 para muchos módulos I2C
LCDManager::LCDManager() : lcd(0x27, 16, 2) {
    lcd.init();
    lcd.backlight();
}

void LCDManager::limpiar() {
    lcd.clear();
    delay(200);
}

void LCDManager::escribir(const String& texto, int col, int fila) {
    setCursor(col, fila);
    lcd.print(texto);
}

void LCDManager::setCursor(int col, int fila) {
    if (col >= 0 && col < maxCols && fila >= 0 && fila < maxRows) {
        lcd.setCursor(col, fila);
    }
}

void LCDManager::mostrarTiempoRestante(int segundos) {
    limpiar();
    escribir("Tiempo restante:", 0, 0);
    escribir(String(segundos) + "s", 0, 1);
}

void LCDManager::mostrarNombreMascota(const String& nombre) {
    limpiar();
    escribir("Mascota:", 0, 0);
    escribir(nombre, 0, 1);
}

void LCDManager::mostrarCantidad(int gramos) {
    limpiar();
    escribir("Cantidad:", 0, 0);
    escribir(String(gramos) + "g", 0, 1);
}

void LCDManager::escribirDisplay(const VisualizacionMascota lista[2]) {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);

    // Línea 1: nombre + estado
    for (int i = 0; i < 2; i++) {
        String texto = lista[i].nombre + " " + lista[i].estado;
        lcd.print(texto);
        if (i == 0) {
            int espacio = 8 - texto.length();
            for (int j = 0; j < espacio; j++) lcd.print(" ");
        }
    }

    // Línea 2: número alineado debajo
    lcd.setCursor(0, 1);
    for (int i = 0; i < 2; i++) {
        String numStr = String(lista[i].numero);
        lcd.print(numStr);
        if (i == 0) {
            int espacio = 8 - numStr.length();
            for (int j = 0; j < espacio; j++) lcd.print(" ");
        }
    }
}
