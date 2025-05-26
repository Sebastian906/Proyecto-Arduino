#ifndef EEPROMMANAGER_H
#define EEPROMMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Mascota.h"

// Pin para el botón de RESET (ajusta según tu conexión)
#define RESET_BUTTON_PIN 2

class EEPROMManager {
private:
    static const int MAX_MASCOTAS = 10;           // Máximo número de mascotas
    static const int DIRECCION_CONTADOR = 1020;  // Dirección para guardar contador de mascotas
    static const int DIRECCION_INICIALIZADA = 1021; // Dirección para verificar si EEPROM está inicializada
    static const byte MARCA_INICIALIZADA = 0xAA;  // Marca para verificar inicialización
    
    int numMascotas;
    static bool resetPressed;  // Flag para la interrupción
    
public:
    // Constructor
    EEPROMManager();
    
    // Inicialización
    void inicializar();
    void configurarInterrupcionReset();
    
    // Manejo de mascotas
    bool agregarMascota(Mascota& mascota);
    bool eliminarMascota(int indice);
    void cargarTodasLasMascotas(Mascota mascotas[]);
    void guardarTodasLasMascotas(Mascota mascotas[]);
    
    // Valores predeterminados
    void restaurarValoresPredeterminados();
    void establecerValoresPredeterminados();
    
    // Gestión de EEPROM
    void limpiarEEPROM();
    bool estaInicializada();
    void marcarComoInicializada();
    
    // Getters
    int getNumMascotas() const;
    
    // Métodos para manejar el RESET
    static void manejarInterrupcionReset();
    bool sePresionoReset();
    void limpiarFlagReset();
    
    // Utilidades de depuración
    void imprimirEstadoEEPROM();
    void imprimirDatosMascota(int indice);
};

#endif