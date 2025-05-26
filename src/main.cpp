#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN  9
#define SS_PIN   10

LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección 0x27, pantalla 16x2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del lector

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);

  lcd.init();         // Inicializa el LCD
  lcd.backlight();    // Enciende la luz de fondo

  lcd.setCursor(0, 0);
  lcd.print("Acerca tarjeta...");
  Serial.println("Acerca una tarjeta RFID...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Construir UID como string hexadecimal con ceros a la izquierda
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uidString += " ";
  }
  uidString.toUpperCase();  // Pone el UID en mayúsculas

  // Mostrar en el monitor serial
  Serial.print("UID: ");
  Serial.println(uidString);

  // Mostrar en el LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID:");
  lcd.setCursor(0, 1);
  lcd.print(uidString);

  delay(2000);  // Espera 2 segundos antes de limpiar
  if(uidString =="9C A7 C0 01"){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acceso permitido");
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acerca tarjeta...");

  // Finaliza la comunicación con la tarjeta
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
