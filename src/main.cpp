//Prueba Keypad y LEDS el codigo anterior este en main3.bk


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Arduino.h>

// Configuración del LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección 0x27, 16 columnas x 2 filas

// Configuración del Keypad 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4};    // Filas
byte colPins[COLS] = {3, 2, A0, A1};  // Columnas
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Configuración de LEDs
const int led1 = A2;
const int led2 = A3;

void setup() {
  Serial.begin(9600);
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Keypad Test");
  lcd.setCursor(0, 1);
  lcd.print("Presiona una tecla");
  
  // Configurar LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

void loop() {
  char key = keypad.getKey(); // Leer tecla presionada
  
  if (key) {
    Serial.print("Tecla presionada: ");
    Serial.println(key);
    
    // Mostrar tecla en LCD
    lcd.clear();
    lcd.print("Tecla: ");
    lcd.print(key);
    
    // Encender LEDs según la tecla
    switch (key) {
      case 'A':
      case 'B':
      case 'C':
      case 'D':
        digitalWrite(led1, HIGH); // LED1 ON para letras
        digitalWrite(led2, LOW);
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        digitalWrite(led2, HIGH); // LED2 ON para números
        digitalWrite(led1, LOW);
        break;
      case '*':
      case '#':
        digitalWrite(led1, HIGH); // Ambos LEDs ON para * y #
        digitalWrite(led2, HIGH);
        break;
      default:
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
    }
    
    delay(300); // Pequeña pausa para evitar rebotes
  }
}