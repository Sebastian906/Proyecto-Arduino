#ifndef MASCOTA_H
#define MASCOTA_H

#include <Arduino.h>
#include <EEPROMManager.h>

class Mascota {
private:
    String nombre;
    String rfidUID;

    int idEEPROM;                     // Posición base en EEPROM para esta mascota
    unsigned int totalAlimentoConsumido; // Total acumulado de alimento
    unsigned int Cuenco;
    bool CuencoEstado;

    //Objeto
    EEPROMManager EEPROM;



public:
    unsigned int tiempoEntreDosis;    // En segundos (10-30)
    unsigned int tiempoEntreDosisActual;    // En segundos (10-30)
    unsigned int inicioDispensacion = 0;
    unsigned int cantidadGramos;      // En gramos (1-10)
    unsigned int cantidadGramosActual;      // En gramos (1-10)
    bool isDosis;

    // Constructor
    Mascota();
    Mascota(String nombre, String rfidUID, int cuenco);
    
    // Métodos para configurar parámetros
    bool setTiempo(unsigned int tiempo);
    bool setComida(unsigned int gramos);
    void setNombre(String nombre);
    void setRFID(String rfidUID);
    void setIdEEPROM(int id);
    
    // Getters
    String getNombre() const;
    String getRFID() const;
    unsigned int getCuenco() const;
    unsigned int getTiempo() const;
    unsigned int getComida() const;
    int getIdEEPROM() const;
    unsigned long getTiempoUltimaDosis() const;
    unsigned int getTotalAlimentoConsumido() const;
    
    // Métodos de dosificación
    bool esTiempoDeDosis() const;
    int dispensarDosis();
    unsigned int getTiempoAnteDosis();
    
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