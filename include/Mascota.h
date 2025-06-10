#ifndef MASCOTA_H
#define MASCOTA_H

#include <Arduino.h>
#include <EEPROMManager.h>

class Mascota {
private:
    String nombre;
    String rfidUID;
    int idEEPROM;
    unsigned int totalAlimentoConsumido; // Total acumulado de alimento
    unsigned int Cuenco;
    bool CuencoEstado;

    // Control de presencia RFID
    bool estaPresente;
    unsigned long tiempoUltimaDeteccion;
    static const unsigned long TIMEOUT_PRESENCIA = 5000;

    //Objeto
    EEPROMManager EEPROM;

public:
    unsigned int tiempoEntreDosis;
    unsigned int tiempoEntreDosisActual;
    unsigned int inicioDispensacion = 0;
    unsigned int cantidadGramos;
    unsigned int cantidadGramosActual;
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
    bool getCuencoEstado() const;
    unsigned int getTiempo() const;
    unsigned int getComida() const;
    int getIdEEPROM() const;
    unsigned long getTiempoUltimaDosis() const;
    unsigned int getTotalAlimentoConsumido() const;

    // Control de presencia RFID
    bool estaGatoPresente() const;
    void marcarPresencia();
    void marcarAusencia();

    // Métodos de dosificación
    bool esTiempoDeDosis() const;
    int dispensarDosis();
    unsigned int getTiempoAnteDosis();
    
    // Métodos de Persistencia
    void guardarEnEEPROM();
    void cargarDeEEPROM();
    void resetearValoresPredeterminados();

    // Método para validar RFID
    bool coincideRFID(String uid) const;

    // Método para obtener información de consumo
    String getInfoConsumo() const;
    
    // Método para incrementar el total de alimento
    void incrementarAlimento(unsigned int gramos);

    // Métodos para el estado del cuenco
    bool cuencoLleno() const;
    void setCuencoLleno(bool lleno);
};

#endif