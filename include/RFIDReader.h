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
    static const unsigned long TIMEOUT_LECTURA = 100;     // 100 ms
    static const unsigned long TIMEOUT_SIN_TARJETA = 2000; // 2 s

  public:
    RFIDReader(byte ssPin, byte rstPin)
      : mfrc522(ssPin, rstPin),
        ultimoRFIDLeido(""),
        tiempoUltimaLectura(0) {}

    void begin() {
      SPI.begin();
      mfrc522.PCD_Init();
      delay(4); // Esperar estabilización
      Serial.println("RFID listo. Acerca una tarjeta...");
    }

    String leerRFIDNoBloqueante() {
      unsigned long inicio = millis();

      while (millis() - inicio < TIMEOUT_LECTURA) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          String uid = "";
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
            uid += String(mfrc522.uid.uidByte[i], HEX);
            if (i < mfrc522.uid.size - 1) uid += " ";
          }

          uid.toUpperCase();  // Esto no modifica en lugar. Hay que asignarlo.

          ultimoRFIDLeido = uid;
          tiempoUltimaLectura = millis();

          // Finaliza la comunicación con la tarjeta
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();

          return uid;
        }
        delay(5);
      }

      return "";
    }

    bool hayTarjeta() {
      // Si ha pasado demasiado tiempo desde la última lectura válida
      if (millis() - tiempoUltimaLectura > TIMEOUT_SIN_TARJETA) {
        String nuevaLectura = leerRFIDNoBloqueante();
        return (nuevaLectura != "");
      }
      return true;
    }

    String getUltimoRFID() {
      return ultimoRFIDLeido;
    }

    void limpiarEstado() {
      ultimoRFIDLeido = "";
      tiempoUltimaLectura = 0;
    }
};

#endif
