#ifndef CONTROL_ALIMENTACION_H
#define CONTROL_ALIMENTACION_H

#include <Arduino.h>
#include "Mascota.h"
#include "LEDManager.h"
#include "LCD.h"
#include "VisualizacionMascota.h"
#include "RFIDReader.h"

class ControlAlimento
{
public:
    static const int MAX_MASCOTAS = 10;

    Mascota *mascotas[MAX_MASCOTAS];
    int cantidadMascotas = 0;

    LEDManager ledManager;
    LCD LcdManager;
    RFIDReader RFIDLector;

    VisualizacionMascota listaVisualizacion[MAX_MASCOTAS];

    // Control de tiempo para dispensar cada minuto
    unsigned long ultimoTiempoDispensacion = 0;
    unsigned long ultimoTiempoRFID = 0;
    const unsigned long intervaloDispensacion = 1000; // 1 segundo
    const unsigned long intervaloRFID = 200; // 200ms para lectura RFID

    ControlAlimento() : ledManager(), LcdManager(), RFIDLector(9, 8) {}

    void iniciar() {
        // Inicializar RFID
        RFIDLector.begin();
        
        // Crear mascotas de prueba
        mascotas[0] = new Mascota("Firulais", "UID12345", 7);
        mascotas[1] = new Mascota("Rafita", "9C A7 C0 01", 6);
        Serial.println("["+mascotas[1]->getRFID()+"]");
        mascotas[1]->setTiempo(8);
        cantidadMascotas = 2;
        
        Serial.println("Sistema de alimentación iniciado");
        LcdManager.limpiarDisplay();
        LcdManager.escribirNormal("Sistema Listo");
    }

    // Verificación constante de RFID sin bloqueos
    void verificarRFID() {
        unsigned long ahora = millis();
        if (ahora - ultimoTiempoRFID >= intervaloRFID) {
            ultimoTiempoRFID = ahora;
            
            String uid = RFIDLector.leerRFIDNoBloqueante();
            
            if (uid != "") {
                // Se detectó una tarjeta RFID
                Mascota* mascotaEncontrada = buscarMascotaPorRFID(uid);
                
                if (mascotaEncontrada != nullptr) {
                    // Gato válido encontrado
                    mascotaEncontrada->marcarPresencia();
                    Serial.println("Gato válido detectado: " + mascotaEncontrada->getNombre());
                    
                    // Mostrar en LCD
                    LcdManager.limpiarDisplay();
                    LcdManager.escribirNormal("Gato: " + mascotaEncontrada->getNombre());
                } else {
                    // RFID no reconocido
                    Serial.println("RFID no reconocido: " + uid);
                    LcdManager.limpiarDisplay();
                    LcdManager.escribirNormal("Gato no registrado");
                }
            } else {
                // No hay tarjeta presente, verificar timeouts de presencia
                verificarPresenciaMascotas();
            }
        }
    }

    // Buscar mascota por RFID
    Mascota* buscarMascotaPorRFID(String uid) {
        for (int i = 0; i < cantidadMascotas; i++) {
            if (mascotas[i] != nullptr && mascotas[i]->coincideRFID(uid)) {
                return mascotas[i];
            }
        }
        return nullptr;
    }

    // Verificar si las mascotas siguen presentes
    void verificarPresenciaMascotas() {
        for (int i = 0; i < cantidadMascotas; i++) {
            if (mascotas[i] != nullptr && !mascotas[i]->estaGatoPresente()) {
                // La mascota ya no está presente por timeout
                mascotas[i]->marcarAusencia();
            }
        }
    }

    // Función dispensar mejorada con verificación de gato válido
    void dispensar() {
        unsigned long ahora = millis();
        if (ahora - ultimoTiempoDispensacion >= intervaloDispensacion) {
            ultimoTiempoDispensacion = ahora;
            
            bool hayDispensacion = false;

            for (int i = 0; i < cantidadMascotas; i++) {
                Mascota *m = mascotas[i];
                
                if (m == nullptr) continue;

                // CONDICIÓN CRÍTICA: Solo dispensar si el gato está presente Y es tiempo de dosis
                if (m->estaGatoPresente() && m->esTiempoDeDosis()) {
                    int gramos = m->dispensarDosis();
                    if (gramos > 0) {
                        Serial.println("¡DISPENSANDO! Gato: " + m->getNombre() + 
                                    ", Cantidad: " + String(gramos) + "g");
                        
                        // Activar LED (simular compuerta)
                        ledManager.parpadear(m->getCuenco());
                        
                        // Registrar dispensación
                        guardarEnVisualizacion("G"+String(i+1), gramos, 'D', i);
                        m->inicioDispensacion++;
                        hayDispensacion = true;
                        
                        // Mostrar en LCD
                        LcdManager.limpiarDisplay();
                        LcdManager.escribirNormal("Dispensando: " + m->getNombre());
                    }
                } else {
                    // No es tiempo de dispensar o gato no presente
                    m->inicioDispensacion = 0;
                    
                    if (m->estaGatoPresente()) {
                        int tiempoRestante = m->getTiempoAnteDosis();
                        guardarEnVisualizacion("G"+String(i+1), tiempoRestante, 'T', i);
                    } else {
                        // Gato no presente
                        guardarEnVisualizacion("G"+String(i+1), 0, 'A', i); // A = Ausente
                    }
                }
            }

            // Actualizar visualización
            if (!hayDispensacion) {
                LcdManager.escribirDisplay(listaVisualizacion);
            }
            
            mostrarHistorial();
        }
    }

    // Función principal que integra todo
    void ejecutarCiclo() {
        verificarRFID();  // Verificar RFID constantemente
        dispensar();      // Verificar y dispensar si corresponde
    }

    void guardarEnVisualizacion(String nombre, int numero, char estado, int i) {
        listaVisualizacion[i].nombre = nombre;
        listaVisualizacion[i].numero = numero;
        listaVisualizacion[i].estado = estado;
    }

    void mostrarHistorial() {
        Serial.println(F("----- Estado del Sistema -----"));
        Serial.println(F("Gato           | Estado    | Info"));
        Serial.println(F("------------------------------------"));

        for (int i = 0; i < cantidadMascotas; i++) {
            if (mascotas[i] != nullptr) {
                String nombre = mascotas[i]->getNombre();
                String estado = mascotas[i]->estaGatoPresente() ? "PRESENTE" : "AUSENTE";
                String info = mascotas[i]->estaGatoPresente() ? 
                    "Próx: " + String(mascotas[i]->getTiempoAnteDosis()) + "s" : 
                    "Total: " + String(mascotas[i]->getTotalAlimentoConsumido()) + "g";

                char buffer[50];
                snprintf(buffer, sizeof(buffer), "%-13s | %-9s | %s", 
                        nombre.c_str(), estado.c_str(), info.c_str());
                Serial.println(buffer);
            }
        }
        Serial.println(F("------------------------------------"));
    }

    // Función para identificación manual (mantener para compatibilidad)
    void identificarGatoPorRFID() {
        LcdManager.limpiarDisplay();
        LcdManager.escribirNormal("Esperando Gato...");
        
        String uid = RFIDLector.leerRFIDNoBloqueante();
        
        if (uid == "") {
            LcdManager.limpiarDisplay();
            LcdManager.escribirNormal("No se detectó RFID");
        } else {
            Mascota* mascota = buscarMascotaPorRFID(uid);
            if (mascota != nullptr) {
                mascota->marcarPresencia();
                LcdManager.limpiarDisplay();
                LcdManager.escribirNormal("Gato identificado: " + mascota->getNombre());
            } else {
                Serial.println("Gato no encontrado.");
                LcdManager.limpiarDisplay();
                LcdManager.escribirNormal("Gato no registrado");
            }
        }
    }

};

#endif // CONTROL_ALIMENTACION_H
