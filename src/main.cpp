#include <Arduino.h>
#include "ControlAlimentacion.h"

ControlAlimento controlAlimento;
Mascota mascotas[10];                // Array de mascotas

// Prototipos de funciones auxiliares
String obtenerUID();
void dispensarAlimento(Mascota &mascota);
void generarInforme();

void setup() {
    Serial.begin(9600);
    
    // Esperar a que se establezca la conexión serial
    while (!Serial) {
        ; // Esperar conexión serial (necesario para algunas placas)
    }
    
    Serial.println("=== INICIANDO SISTEMA DE ALIMENTACIÓN ===");
    
    // Inicializar el sistema
    controlAlimento.iniciar();
    
    Serial.println("Sistema listo. Acerque tarjeta RFID...");
}

void loop() {
    // Ejecutar ciclo principal que integra:
    // 1. Verificación constante de RFID (no bloqueante)
    // 2. Verificación de tiempo de dispensación
    // 3. Dispensación solo si gato válido está presente
    controlAlimento.ejecutarCiclo();
    
    // Pequeña pausa para no saturar el procesador
    delay(50);
}