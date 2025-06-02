#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class RFIDReader {
  private:
    MFRC522 mfrc522;
    String rfidTag;

  public:
    RFIDReader(byte ssPin, byte rstPin) 
      : mfrc522(ssPin, rstPin), rfidTag("") {}

    void begin() {
      SPI.begin();
      mfrc522.PCD_Init();
      delay(4); // Tiempo de estabilización
    }

    // Intenta leer una tarjeta RFID. Retorna "" si no hay tarjeta.
    String leerRFID() {
      // Verifica si hay una nueva tarjeta presente
      if (!mfrc522.PICC_IsNewCardPresent()) return "";
      if (!mfrc522.PICC_ReadCardSerial()) return "";

      // Construir UID como cadena hexadecimal
      rfidTag = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) rfidTag += "0";
        rfidTag += String(mfrc522.uid.uidByte[i], HEX);
        if (i < mfrc522.uid.size - 1) rfidTag += " ";
      }

      rfidTag.toUpperCase();

      // Finaliza la comunicación con la tarjeta
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      return rfidTag;
    }

    String leerRFID2(){
      return "9C A7 C0 01";
    }
};

#endif

