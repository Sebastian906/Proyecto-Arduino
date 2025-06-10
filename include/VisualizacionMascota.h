// include/VisualizacionMascota.h
#ifndef VISUALIZACION_MASCOTA_H
#define VISUALIZACION_MASCOTA_H

#include <Arduino.h>

struct VisualizacionMascota {
    String nombre;
    int numero;
    char estado; // 'D' = Dispensando, 'T' = Temporizando
};

#endif
