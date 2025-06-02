#ifndef EEPROMMANAGER_H
#define EEPROMMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

// Estructura para encapsular los parámetros de una mascota
struct ParametrosMascota {
    uint8_t tiempoEntreDosis;
    uint8_t cantidadGramos;
    bool exito;  // indica si la carga fue exitosa
};

class EEPROMManager {
private:
    static const int DIRECCION_INICIALIZADA = 1020;
    static const byte MARCA_INICIALIZADA = 0xAA;

    void establecerValoresPredeterminados();
    void marcarComoInicializada();
    bool estaInicializada();
    uint16_t buscarSiguienteDireccionLibre();

public:
    EEPROMManager();

    // Guarda los parámetros de una mascota y devuelve la dirección utilizada
    uint16_t guardarParametros(uint8_t tiempoEntreDosis, uint8_t cantidadGramos,int16_t direccionExistente);

    // Carga los parámetros desde EEPROM
    ParametrosMascota cargarParametros(uint16_t direccion);

    // Restaura valores predeterminados en EEPROM
    void restaurarValoresPredeterminados();
};

#endif

