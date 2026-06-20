#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define NUM_ZONAS         5
#define DIAS_HISTORICOS   30   
#define DIAS_REGISTRADOS  29   
#define MAX_NOMBRE        50

#define ARCHIVO_DATOS       "datos_historicos.bin"
#define ARCHIVO_HISTORICO_BASE "historico_base.txt"
#define ARCHIVO_REPORTE     "reporte_contaminacion.txt"

#define LIMITE_CO2   4000.0
#define LIMITE_SO2     20.0
#define LIMITE_NO2     25.0
#define LIMITE_PM25    15.0

#define PESO_RECIENTE 3
#define PESO_ANTIGUO  1

#endif
