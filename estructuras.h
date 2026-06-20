#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "config.h"

typedef struct {
    double co2;
    double so2;
    double no2;
    double pm25;
} Contaminantes;

typedef struct {
    double temperatura;
    double viento;
    double humedad;
} FactoresClimaticos;

typedef struct {
    char           nombre[MAX_NOMBRE];
    Contaminantes  actual;
    FactoresClimaticos clima;
    Contaminantes  historico[DIAS_HISTORICOS];  
    Contaminantes  prediccion;
    Contaminantes  promedio_historico;
} Zona;

#endif
