#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class RFIDReader {
  private:
    MFRC522 mfrc522;
    String ultimoRFIDLeido;
    unsigned long tiempoUltimaLectura;
    static const unsigned long TIMEOUT_LECTURA = 100; // 100ms timeout
    bool hayTarjetaPresente;

  public:
    RFIDReader(byte ssPin, byte rstPin)
        : mfrc522(ssPin, rstPin), ultimoRFIDLeido(""), 
          tiempoUltimaLectura(0), hayTarjetaPresente(false) {}

    void begin() {
      SPI.begin();
      mfrc522.PCD_Init();
      delay(4); // Tiempo de estabilización
    }

    // Lectura no bloqueante con timeout
    String leerRFIDNoBloqueante() {
        unsigned long tiempoInicio = millis();
        
        // Verificar si hay tarjeta nueva con timeout
        while (millis() - tiempoInicio < TIMEOUT_LECTURA) {
            if (mfrc522.PICC_IsNewCardPresent()) {
                if (mfrc522.PICC_ReadCardSerial()) {
                    // Construir UID
                    String rfidTag = "";
                    for (byte i = 0; i < mfrc522.uid.size; i++) {
                        if (mfrc522.uid.uidByte[i] < 0x10) rfidTag += "0";
                        rfidTag += String(mfrc522.uid.uidByte[i], HEX);
                        if (i < mfrc522.uid.size - 1) rfidTag += " ";
                    }
                    
                    rfidTag.toUpperCase();
                    
                    // Finalizar comunicación
                    mfrc522.PICC_HaltA();
                    mfrc522.PCD_StopCrypto1();
                    
                    ultimoRFIDLeido = rfidTag;
                    tiempoUltimaLectura = millis();
                    hayTarjetaPresente = true;
                    
                    return rfidTag;
                }
            }
            delay(10); // Pequeña pausa para no saturar
        }
        
        // No se encontró tarjeta en el tiempo límite
        return "";
    }

    // Verificar si hay tarjeta presente actualmente
    bool hayTarjeta() {
        // Si la última lectura fue hace más de 2 segundos, verificar de nuevo
        if (millis() - tiempoUltimaLectura > 2000) {
            String lectura = leerRFIDNoBloqueante();
            return (lectura != "");
        }
        return hayTarjetaPresente;
    }

    // Obtener último RFID leído
    String getUltimoRFID() {
        return ultimoRFIDLeido;
    }

    // Limpiar estado
    void limpiarEstado() {
        ultimoRFIDLeido = "";
        hayTarjetaPresente = false;
        tiempoUltimaLectura = 0;
    }

    // Función de prueba (mantener para compatibilidad)
    String leerRFID2() {
        return "9C A7 C0 01";
    }
  };

#endif

