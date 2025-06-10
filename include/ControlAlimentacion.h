#ifndef CONTROL_ALIMENTACION_H
#define CONTROL_ALIMENTACION_H

#include <Arduino.h>
#include "Mascota.h"
#include "LEDManager.h"
#include "LCD.h"
#include "VisualizacionMascota.h"
#include "RFIDReader.h"
#include "Teclado.h"

class ControlAlimento
{
public:
  static const int MAX_MASCOTAS = 10;

  Mascota *mascotas[MAX_MASCOTAS];
  int cantidadMascotas = 0;
  bool presionadoMenu = false;

  LEDManager ledManager;
  LCD LcdManager;
  RFIDReader RFIDLector;
  Teclado teclado;

  VisualizacionMascota listaVisualizacion[MAX_MASCOTAS];

  // Control de tiempo para dispensar cada minuto
  unsigned long ultimoTiempoDispensacion = 0;
  const unsigned long intervalo = 1000; // 1 segundo

  ControlAlimento() : ledManager(), LcdManager(), RFIDLector(9, 8), teclado() {}

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
    Serial.println("[" + mascotas[1]->getRFID() + "]");
    mascotas[1]->setTiempo(8);
    cantidadMascotas = 2;
  }

  void dispensar()
  {
    if (teclado.obtenerTecla() == '*')
    {
      MenuPrincipal();
    }
    unsigned long ahora = millis();
    if (ahora - ultimoTiempoDispensacion >= intervalo)
    {
      ultimoTiempoDispensacion = ahora;

      for (int i = 0; i < cantidadMascotas; i++)
      {
        if (teclado.obtenerTecla() == '*')
        {
          MenuPrincipal();
        }
        Mascota *m = mascotas[i];

        if (m->esTiempoDeDosis())
        {
          int gramos = m->dispensarDosis();
          Serial.println("Dispensando comida para " + String(gramos));
          ledManager.parpadear(m->getCuenco());
          guardarEnVisualizacion("G" + String(i + 1), gramos, 'D', i);
          m->inicioDispensacion++;
          if (teclado.obtenerTecla() == '*')
          {
            MenuPrincipal();
          }
        }
        else
        {
          m->inicioDispensacion = 0;
          int tiempoRestante = m->getTiempoAnteDosis();
          guardarEnVisualizacion("G" + String(i + 1), tiempoRestante, 'T', i);
          if (teclado.obtenerTecla() == '*')
          {
            MenuPrincipal();
          }
        }
      }

      LcdManager.escribirDisplay(listaVisualizacion);
      mostrarHistorial();
      if (teclado.obtenerTecla() == '*')
      {
        MenuPrincipal();
      }
    }
  }

  void guardarEnVisualizacion(String nombre, int numero, char estado, int i)
  {
    if (teclado.obtenerTecla() == '*')
    {
      MenuPrincipal();
    }
    listaVisualizacion[i].nombre = nombre;
    listaVisualizacion[i].numero = numero;
    listaVisualizacion[i].estado = estado;
  }

  void mostrarHistorial()
  {
    Serial.println(F("----- Historial de Mascotas -----"));
    Serial.println(F("RFID           | Nombre        | Total (g)"));
    Serial.println(F("--------------------------------------------"));

    for (int i = 0; i < cantidadMascotas; i++)
    {
      if (mascotas[i] != nullptr)
      {
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

  void AntesMenu()
  {
  }

  void MenuPrincipal()
  {

    MenuPrincipalLCD();

    while (true)
    {
      char tecla = teclado.obtenerTecla();

      if (tecla)
      {
        Serial.print("Tecla presionada: ");
        Serial.println(tecla);

        switch (tecla)
        {
        case '1':
          mostrarHistorial();
          return;
        case '2':
          Reset();
          return;
        case '3':
          MenuGatos();
          return;
        case '4':
          Serial.println("Salir...");
          return;
        default:
          LcdManager.limpiarDisplay();
          LcdManager.setCursor(0, 0);
          LcdManager.escribirNormal("Opcion invalida");
          delay(700);
          MenuPrincipalLCD();
          // Vuelve a mostrar el menú tras el error
          break;
        }
      }
    }
  }


  void Reset()
  {
    for (int i = 0; i < cantidadMascotas; i++)
    {
      if (mascotas[i] != nullptr)
      {
        mascotas[i]->resetearValoresPredeterminados();
      }
    }
    LcdManager.limpiarDisplay();
    LcdManager.escribirNormal("Valores reseteados");
    delay(500);
  }



  void MenuPrincipalLCD()
  {
    LcdManager.limpiarDisplay();
    LcdManager.setCursor(0, 0);
    LcdManager.escribirNormal("1.His  3.C.Val");
    LcdManager.setCursor(0, 1);
    LcdManager.escribirNormal("2.Rst  4.Ex");
  }

  void MenuGatos()
  {
    MenuGatosLCD(cantidadMascotas);
    // Bucle bloqueante para esperar a que el usuario seleccione un número 1-4
    while (true)
    {
      char tecla = teclado.obtenerTecla();
      if (!tecla)
        continue;

      Serial.print("Gato seleccionado: ");
      int opcion = tecla - '0';
      Serial.println(opcion);

      if (opcion >= 1 && opcion <= cantidadMascotas)
      {
        // Aquí puedes acceder a la mascota con índice (opcion - 1)

        MenuOpcionesGato(opcion - 1);
      }
      else
      {
        // Si la opción no es 1–4, mostramos “Opción inválida” brevemente
        LcdManager.limpiarDisplay();
        LcdManager.setCursor(0, 0);
        LcdManager.escribirNormal("Opcion invalida");
        delay(700);
        // Redibujamos nuevamente la lista de gatos
        LcdManager.limpiarDisplay();
        MenuGatosLCD(cantidadMascotas);
        break;
      }
    }
  }

  void MenuGatosLCD(int count)
  {
    // count = número de mascotas en la lista (puede ser 2, 3 ó 4)
    // Limpiamos pantalla antes de dibujar
    LcdManager.limpiarDisplay();

    // Cadenas que vamos a imprimir en cada línea del LCD (16 cols × 2 filas)
    String linea1 = "";
    String linea2 = "";

    // Construir línea 1: aquí van las mascotas índice 0 y 1 (si existen)
    for (int i = 0; i < count && i < 2; i++)
    {
      // i = 0 → “1.<nombre>”
      // i = 1 → “2.<nombre>”
      String etiqueta = String(i + 1) + "." + listaVisualizacion[i].nombre;
      if (i == 0)
      {
        linea1 += etiqueta;
      }
      else
      {
        // Para el segundo elemento de la primera línea, queremos que empiece
        // aproximadamente en la columna 8. Calculamos cuántos espacios poner:
        int espacios = 8 - linea1.length();
        if (espacios < 1)
          espacios = 1;
        for (int s = 0; s < espacios; s++)
          linea1 += " ";
        linea1 += etiqueta;
      }
    }

    // Construir línea 2: aquí van las mascotas índice 2 y 3 (si existen)
    for (int i = 2; i < count && i < 4; i++)
    {
      // i = 2 → “3.<nombre>”
      // i = 3 → “4.<nombre>”
      String etiqueta = String(i + 1) + "." + listaVisualizacion[i].nombre;
      if (i == 2)
      {
        linea2 += etiqueta;
      }
      else
      {
        // Para el segundo elemento de la segunda línea, idem columna 8
        int espacios = 8 - linea2.length();
        if (espacios < 1)
          espacios = 1;
        for (int s = 0; s < espacios; s++)
          linea2 += " ";
        linea2 += etiqueta;
      }
    }

    // Ahora imprimimos ambas líneas en el LCD
    LcdManager.setCursor(0, 0);
    LcdManager.escribirNormal(linea1);

    LcdManager.setCursor(0, 1);
    LcdManager.escribirNormal(linea2);
  }

  void MenuOpcionesGato(int index)
  {
    // Mostrar el submenú
    LcdManager.limpiarDisplay();
    LcdManager.setCursor(0, 0);
    LcdManager.escribirNormal("1.Temporiz 2.Com");
    LcdManager.setCursor(0, 1);
    LcdManager.escribirNormal("3.Salir");

    while (true)
    {
      char tecla = teclado.obtenerTecla();
      if (!tecla)
        continue;

      Serial.print("Opcion seleccionada: ");
      Serial.println(tecla);

      switch (tecla)
      {
      case '1':
        Serial.print("Temporizador del gato ");
        Serial.println(index + 1); 
        int tiempo = teclado.leerTiempo();
        Serial.println(tiempo);
        mascotas[index]->setTiempo(tiempo);

        return;

      case '2':
        Serial.print("Comida para el gato ");
        Serial.println(index + 1);
        int gramos = teclado.leerGramos();
        Serial.println(gramos);
        mascotas[index]->setComida(gramos);
        return;

      case '3':
        Serial.println("Salir...");
        return;

      default:
        LcdManager.limpiarDisplay();
        LcdManager.setCursor(0, 0);
        LcdManager.escribirNormal("Opcion invalida");
        delay(700);
        LcdManager.limpiarDisplay();
        LcdManager.setCursor(0, 0);
        LcdManager.escribirNormal("1.Temporiz 2.Com");
        LcdManager.setCursor(0, 1);
        LcdManager.escribirNormal("3.Salir");
        break;
      }
    }
  }
};

#endif // CONTROL_ALIMENTACION_H
