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
    const unsigned long intervalo = 1000; // 1 segundo

    ControlAlimento() : ledManager(), LcdManager(), RFIDLector(9, 8) {}

    void identificarGatoPorRFID()
    {
        LcdManager.limpiarDisplay();
        LcdManager.escribirNormal("Esperando Gato...");
        String uid = RFIDLector.leerRFID2();
        if (uid == "")
        {
            LcdManager.limpiarDisplay();
            LcdManager.escribirNormal("No se detectó RFID");
        }
        else
        {
            for (int i = 0; i < cantidadMascotas; i++)
            {
                if (mascotas[i]->coincideRFID(uid))
                {
                    LcdManager.limpiarDisplay();
                    LcdManager.escribirNormal("Gato identificado: " + mascotas[i]->getNombre());
                }
            }
            Serial.println("Gato no encontrado.");
        }
    }

    void iniciar()
    {
        mascotas[0] = new Mascota("Firulais", "UID12345", 7);
        mascotas[1] = new Mascota("Rafita", "9C A7 C0 01", 6);
        Serial.println("["+mascotas[1]->getRFID()+"]");
        mascotas[1]->setTiempo(8);
        cantidadMascotas = 2;
    }

    void dispensar()
    {
        unsigned long ahora = millis();
        if (ahora - ultimoTiempoDispensacion >= intervalo)
        {
            ultimoTiempoDispensacion = ahora;

            for (int i = 0; i < cantidadMascotas; i++)
            {
                Mascota *m = mascotas[i];

                if (m->esTiempoDeDosis())
                {
                    int gramos = m->dispensarDosis();
                    Serial.println("Dispensando comida para " + String(gramos));
                    ledManager.parpadear(m->getCuenco());
                    guardarEnVisualizacion("G"+String(i+1), gramos, 'D', i);
                    m->inicioDispensacion++;
                }
                else
                {
                    m->inicioDispensacion = 0;
                    int tiempoRestante = m->getTiempoAnteDosis();
                    guardarEnVisualizacion("G"+String(i+1), tiempoRestante, 'T', i);
                }
            }

            LcdManager.escribirDisplay(listaVisualizacion);
            mostrarHistorial();
        }
    }

    void guardarEnVisualizacion(String nombre, int numero, char estado, int i)
    {
        listaVisualizacion[i].nombre = nombre;
        listaVisualizacion[i].numero = numero;
        listaVisualizacion[i].estado = estado;
    }


    void mostrarHistorial() {
        Serial.println(F("----- Historial de Mascotas -----"));
        Serial.println(F("RFID           | Nombre        | Total (g)"));
        Serial.println(F("--------------------------------------------"));

        for (int i = 0; i < cantidadMascotas; i++) {
            if (mascotas[i] != nullptr) {
                String rfid = mascotas[i]->getRFID();
                String nombre = mascotas[i]->getNombre();
                int total = mascotas[i]->getTotalAlimentoConsumido();

                // Formato fijo para columnas
                char buffer[50];
                snprintf(buffer, sizeof(buffer), "%-14s | %-13s | %4d", 
                        rfid.c_str(), nombre.c_str(), total);
                Serial.println(buffer);
            }
        }

        Serial.println(F("--------------------------------------------"));
    }


};

#endif // CONTROL_ALIMENTACION_H
