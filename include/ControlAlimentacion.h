#ifndef CONTROL_ALIMENTACION_H
#define CONTROL_ALIMENTACION_H

#include <Arduino.h>
#include "Mascota.h"
#include "LEDManager.h"
#include "LCD.h"
#include "VisualizacionMascota.h"
#include "RFIDReader.h"
#include "Teclado.h"

class ControlAlimento {
public:
  static const int MAX_MASCOTAS = 10;

  Mascota* mascotas[MAX_MASCOTAS];
  
  int cantidadMascotas = 0;

  LEDManager ledManager;
  LCD LcdManager;
  RFIDReader RFIDLector;
  Teclado teclado;

  VisualizacionMascota listaVisualizacion[MAX_MASCOTAS];

  unsigned long ultimoTiempoDispensacion = 0;
  unsigned long ultimoTiempoRFID = 0;
  const unsigned long intervaloDispensacion = 1000;
  const unsigned long intervaloRFID = 200;

  ControlAlimento() : ledManager(), LcdManager(), RFIDLector(10, 9), teclado() {}

  void iniciar() {
    RFIDLector.begin();
    mascotas[0] = new Mascota("Firulais", "36 37 19 04", 7);
    mascotas[1] = new Mascota("Rafita", "9C A7 C0 01", 6);
    mascotas[1]->setTiempo(8);
    cantidadMascotas = 2;

    Serial.println("Sistema de alimentación iniciado");
    LcdManager.limpiarDisplay();
    LcdManager.escribirNormal("Sistema Listo");
  }

  void verificarRFID() {
  // Control de intervalo de lectura
  if (millis() - ultimoTiempoRFID < intervaloRFID) return;
  ultimoTiempoRFID = millis();

  // Intentar leer un RFID
  String uid = RFIDLector.leerRFIDNoBloqueante();
  Serial.println("UID leído: " + uid);
  if (uid != "") {
    Serial.println("UID leído: " + uid);
    Mascota* mascota = buscarMascotaPorRFID(uid);

    if (mascota != nullptr) {
      mascota->marcarPresencia();
      Serial.println("Gato detectado: " + mascota->getNombre());

      LcdManager.limpiarDisplay();

      //if (mascota->cuencoLleno()) {
        LcdManager.escribirNormal("Acceso permitido");
        dispensar();
        //ledManager.parpadear(mascota->getCuenco(), 5000);
        //mascota->setCuencoLleno(false); // Reiniciar estado del cuenco
      //} else {
        //LcdManager.escribirNormal("Aun no es tiempo");
      //}
    } else {
      // UID no está registrado
      Serial.println("RFID no reconocido: " + uid);
      LcdManager.limpiarDisplay();
      LcdManager.escribirNormal("Gato no registrado");
    }
  } else {
    // No se detectó RFID, verificar si alguna mascota ya está presente
    verificarPresenciaMascotas();
  }
}


void dispensar() {
  if (teclado.obtenerTecla() == '*') {
    mostrarMenuPrincipal();
    return;
  }

  unsigned long ahora = millis();
  if (ahora - ultimoTiempoDispensacion < intervaloDispensacion) return;
  ultimoTiempoDispensacion = ahora;

  bool huboDispensacion = false;

  for (int i = 0; i < cantidadMascotas; i++) {
    if (teclado.obtenerTecla() == '*') {
      mostrarMenuPrincipal();
      return;
    }

    Mascota* m = mascotas[i];
    if (!m) continue;

    if (m->estaGatoPresente() && (m->esTiempoDeDosis() || !m->getCuencoEstado())) {
      int gramos = m->dispensarDosis();
      if (gramos > 0) {
        Serial.println("¡DISPENSANDO! Gato: " + m->getNombre() + ", Cantidad: " + String(gramos) + "g");
        ledManager.parpadear(m->getCuenco());
        guardarEnVisualizacion("G" + String(i + 1), gramos, 'D', i);
        m->inicioDispensacion++;
        LcdManager.limpiarDisplay();
        LcdManager.escribirNormal("Dispensando: " + m->getNombre());
        huboDispensacion = true;
      }
    } else {
      m->inicioDispensacion = 0;
      int tiempoRestante = m->estaGatoPresente() ? m->getTiempoAnteDosis() : 0;
      char tipoVisual = m->estaGatoPresente() ? 'T' : 'A';
      guardarEnVisualizacion("G" + String(i + 1), tiempoRestante, tipoVisual, i);
    }

    if (teclado.obtenerTecla() == '*') {
      mostrarMenuPrincipal();
      return;
    }
  }


    LcdManager.escribirDisplay(listaVisualizacion);



  if (teclado.obtenerTecla() == '*') {
    mostrarMenuPrincipal();
  }
}


  void ejecutarCiclo() {
    verificarRFID();
    dispensar();
  }

  Mascota* buscarMascotaPorRFID(const String& uid) {
    for (int i = 0; i < cantidadMascotas; i++) {
      if (mascotas[i] && mascotas[i]->coincideRFID(uid)) {
        return mascotas[i];
      }
    }
    return nullptr;
  }

  void verificarPresenciaMascotas() {
    for (int i = 0; i < cantidadMascotas; i++) {
      if (mascotas[i]) mascotas[i]->marcarAusencia();
    }
  }

  void guardarEnVisualizacion(const String& nombre, int numero, char estado, int i) {
    if (i >= 0 && i < MAX_MASCOTAS) {
      listaVisualizacion[i] = { nombre, numero, estado };
    }
  }

  void mostrarHistorial() {
    Serial.println(F("----- Estado del Sistema -----"));
    Serial.println(F("Gato           | Estado    | Info"));
    Serial.println(F("------------------------------------"));

    for (int i = 0; i < cantidadMascotas; i++) {
      if (mascotas[i]) {
        String nombre = mascotas[i]->getNombre();
        String estado = mascotas[i]->estaGatoPresente() ? "PRESENTE" : "AUSENTE";
        String info = mascotas[i]->estaGatoPresente() ?
                      "Próx: " + String(mascotas[i]->getTiempoAnteDosis()) + "s" :
                      "Total: " + String(mascotas[i]->getTotalAlimentoConsumido()) + "g";

        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%-13s | %-9s | %s",
                 nombre.c_str(), estado.c_str(), info.c_str());
        Serial.println(buffer);
      }
    }

    Serial.println(F("------------------------------------"));
  }

  void identificarGatoPorRFID() {
    LcdManager.limpiarDisplay();
    LcdManager.escribirNormal("Esperando Gato...");
    String uid = RFIDLector.leerRFIDNoBloqueante();

    if (uid == "") {
      LcdManager.limpiarDisplay();
      LcdManager.escribirNormal("No se detectó RFID");
    } else {
      Mascota* mascota = buscarMascotaPorRFID(uid);
      if (mascota) {
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

  // MENÚS

  void mostrarMenuPrincipal() {
    mostrarMenuPrincipalLCD();
    while (true) {
      char tecla = teclado.obtenerTecla();
      Serial.println("Tecla presionada: " + String(tecla));
      if (!tecla) continue;

      switch (tecla) {
        case '1':
          mostrarHistorial();
          return;
        case '2':
          resetearValores();
          return;
        case '3':
          mostrarMenuGatos();
          return;
        case '4':
          Serial.println("Saliendo del menú...");
          return;
        default:
          LcdManager.limpiarDisplay();
          LcdManager.escribirNormal("Opción inválida");
          delay(700);
          mostrarMenuPrincipalLCD();
          break;
      }
    }
  }

  void resetearValores() {
    for (int i = 0; i < cantidadMascotas; i++) {
      if (mascotas[i]) mascotas[i]->resetearValoresPredeterminados();
    }
    LcdManager.limpiarDisplay();
    LcdManager.escribirNormal("Valores reseteados");
    delay(500);
  }

  void mostrarMenuPrincipalLCD() {
    LcdManager.limpiarDisplay();
    LcdManager.setCursor(0, 0);
    LcdManager.escribirNormal("1.His  3.C.Val");
    LcdManager.setCursor(0, 1);
    LcdManager.escribirNormal("2.Rst  4.Ex");
  }

  void mostrarMenuGatos() {
    mostrarMenuGatosLCD();
    while (true) {
      char tecla = teclado.obtenerTecla();
      if (!tecla) continue;

      int opcion = tecla - '0';
      if (opcion >= 1 && opcion <= cantidadMascotas) {
        mostrarMenuOpcionesGato(opcion - 1);
        return;
      } else {
        LcdManager.limpiarDisplay();
        LcdManager.escribirNormal("Opción inválida");
        delay(700);
        mostrarMenuGatosLCD();
      }
    }
  }

  void mostrarMenuGatosLCD() {
    LcdManager.limpiarDisplay();
    String linea1 = "", linea2 = "";

    for (int i = 0; i < cantidadMascotas; i++) {
      if (i < 2)
        linea1 += String(i + 1) + "." + mascotas[i]->getNombre().substring(0, 3) + " ";
      else if (i < 4)
        linea2 += String(i + 1) + "." + mascotas[i]->getNombre().substring(0, 3) + " ";
    }

    LcdManager.setCursor(0, 0);
    LcdManager.escribirNormal(linea1);
    LcdManager.setCursor(0, 1);
    LcdManager.escribirNormal(linea2);
  }

  void mostrarMenuOpcionesGato(int indice) {
    LcdManager.limpiarDisplay();
    LcdManager.escribirNormal("Opciones gato...");
    delay(1000);
    // Aquí puedes expandir con submenús: cambiar dosis, ver consumo, etc.
  }

};

#endif // CONTROL_ALIMENTACION_H
