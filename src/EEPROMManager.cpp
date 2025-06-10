#include "EEPROMManager.h"
#include "EEPROM.h"


EEPROMManager::EEPROMManager() {
#if defined(ESP8266) || defined(ESP32)
    EEPROM.begin(EEPROM.length());
#endif
}




uint16_t EEPROMManager::guardarParametros(uint8_t tiempoEntreDosis, uint8_t cantidadGramos, int16_t direccionExistente) {
    uint16_t direccion;

    // Validar si la dirección pasada es válida
    if (direccionExistente >= 0 && static_cast<uint16_t>(direccionExistente + 1) < static_cast<uint16_t>(EEPROM.length())) {
        direccion = static_cast<uint16_t>(direccionExistente);
        Serial.println("Sobrescribiendo parámetros existentes...");
    } else {
        // Buscar una nueva dirección si la actual es inválida
        direccion = buscarSiguienteDireccionLibre();
        if (direccion + 1 >= EEPROM.length()) {
            Serial.println("Error: no hay espacio suficiente en EEPROM.");
            return UINT16_MAX;
        }
        Serial.println("Guardando en nueva dirección...");
    }

    // Guardar los datos
    EEPROM.write(direccion, tiempoEntreDosis);
    EEPROM.write(direccion + 1, cantidadGramos);

    Serial.print("Parámetros guardados en dirección ");
    Serial.println(direccion);
    return direccion;
}


uint16_t EEPROMManager::buscarSiguienteDireccionLibre() {
    // Saltamos dirección reservada
    const uint16_t inicio = 0;
    const uint16_t fin = EEPROM.length() - 2;  // Se escriben 2 bytes por mascota

    for (uint16_t i = inicio; i <= fin; i += 2) {
        uint8_t val1 = EEPROM.read(i);
        uint8_t val2 = EEPROM.read(i + 1);
        if (val1 == 0xFF && val2 == 0xFF) {  // EEPROM virgen usualmente contiene 0xFF
            return i;
        }
    }

    return EEPROM.length();  // Sin espacio disponible
}

ParametrosMascota EEPROMManager::cargarParametros(uint16_t direccion) {
    ParametrosMascota resultado{0, 0, false};

    if (direccion + 1 >= EEPROM.length()) {
        Serial.println("Error: dirección fuera de rango.");
        return resultado;
    }

    resultado.tiempoEntreDosis = EEPROM.read(direccion);
    resultado.cantidadGramos = EEPROM.read(direccion + 1);
    resultado.exito = true;

    Serial.print("Parámetros cargados desde dirección ");
    Serial.println(direccion);
    return resultado;
}

void EEPROMManager::establecerValoresPredeterminados() {
    // Dirección 0 como predeterminada para el gato principal
    EEPROM.write(0, 60);  // 60 minutos entre dosis
    EEPROM.write(1, 20);  // 20 gramos por dosis
}

void EEPROMManager::restaurarValoresPredeterminados() {
    establecerValoresPredeterminados();
    Serial.println("Valores predeterminados restaurados.");
}

bool EEPROMManager::estaInicializada() {
    return EEPROM.read(DIRECCION_INICIALIZADA) == MARCA_INICIALIZADA;
}

void EEPROMManager::marcarComoInicializada() {
    EEPROM.write(DIRECCION_INICIALIZADA, MARCA_INICIALIZADA);
}
