#ifndef MASCOTA_H
#define MASCOTA_H

#include <Arduino.h>
#include <EEPROM.h>

class Mascota {
private:
    String nombre;
    String rfidUID;
    unsigned int tiempoEntreDosis;    // En segundos (10-30)
    unsigned int cantidadGramos;      // En gramos (1-10)
    int idEEPROM;                     // Posición base en EEPROM para esta mascota
    unsigned long tiempoUltimaDosis;  // Timestamp de la última dosis
    unsigned int totalAlimentoConsumido; // Total acumulado de alimento

    // Constantes para validación
    static const unsigned int MIN_TIEMPO = 10;
    static const unsigned int MAX_TIEMPO = 30;
    static const unsigned int MIN_GRAMOS = 1;
    static const unsigned int MAX_GRAMOS = 10;
    
    // Tamaño de datos por mascota en EEPROM (nombre + rfid + tiempo + gramos + total)
    static const int TAMANO_DATOS_MASCOTA = 64; 

public:
    // Constructor
    Mascota();
    Mascota(String nombre, String rfidUID, int idEEPROM);
    
    // Métodos para configurar parámetros
    bool setTiempo(unsigned int tiempo);
    bool setComida(unsigned int gramos);
    void setNombre(String nombre);
    void setRFID(String rfidUID);
    void setIdEEPROM(int id);
    
    // Getters
    String getNombre() const;
    String getRFID() const;
    unsigned int getTiempo() const;
    unsigned int getComida() const;
    int getIdEEPROM() const;
    unsigned long getTiempoUltimaDosis() const;
    unsigned int getTotalAlimentoConsumido() const;
    
    // Métodos de dosificación
    bool esTiempoDeDosis() const;
    void dispensarDosis();
    void actualizarTiempoUltimaDosis();
    unsigned long getTiempoRestante() const;
    
    // Métodos de persistencia
    void guardarEnEEPROM();
    void cargarDeEEPROM();
    void resetearValoresPredeterminados();
    
    // Método para validar RFID
    bool coincideRFID(String uid) const;
    
    // Método para obtener información de consumo
    String getInfoConsumo() const;
    
    // Método para incrementar el total de alimento
    void incrementarAlimento(unsigned int gramos);
};

#endif