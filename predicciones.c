#include <stdio.h>
#include "predicciones.h"
#include "config.h"
#include "utilidades.h"


const char* clasificar_nivel_oms(double razon) {
    if (razon <= 1.0) {
        return "ACEPTABLE: la calidad del aire prevista esta dentro de los limites de la OMS.";
    } else if (razon <= 1.5) {
        return "MODERADO: grupos sensibles (ninos, adultos mayores, personas con\n"
               "             afecciones respiratorias) deberian limitar la exposicion prolongada.";
    } else if (razon <= 2.0) {
        return "DAÑINO PARA GRUPOS SENSIBLES: se recomienda reducir actividades al aire libre.";
    } else if (razon <= 3.0) {
        return "PERJUDICIAL PARA LA SALUD GENERAL: la poblacion en general puede presentar\n"
               "             efectos adversos. Evitar esfuerzo fisico prolongado al aire libre.";
    } else {
        return "PELIGROSO: nivel de emergencia sanitaria segun la OMS. Evitar la\n"
               "             exposicion al aire libre y seguir las indicaciones de las autoridades.";
    }
}

void mostrar_predicciones(const Zona *z, int idx) {
    printf("\n");
    separador();
    printf("  PREDICCIONES PROXIMAS 24 H - %s (Zona %d)\n", z->nombre, idx + 1);
    separador();

    printf("  CO2:   %8.2f ug/m3  (limite OMS %.0f)\n", z->prediccion.co2,  LIMITE_CO2);
    printf("  SO2:   %8.2f ug/m3  (limite OMS %.0f)\n", z->prediccion.so2,  LIMITE_SO2);
    printf("  NO2:   %8.2f ug/m3  (limite OMS %.0f)\n", z->prediccion.no2,  LIMITE_NO2);
    printf("  PM2.5: %8.2f ug/m3  (limite OMS %.0f)\n", z->prediccion.pm25, LIMITE_PM25);

  
    double razon_co2  = z->prediccion.co2  / LIMITE_CO2;
    double razon_so2  = z->prediccion.so2  / LIMITE_SO2;
    double razon_no2  = z->prediccion.no2  / LIMITE_NO2;
    double razon_pm25 = z->prediccion.pm25 / LIMITE_PM25;

    double razon_max = razon_co2;
    if (razon_so2  > razon_max) razon_max = razon_so2;
    if (razon_no2  > razon_max) razon_max = razon_no2;
    if (razon_pm25 > razon_max) razon_max = razon_pm25;

    printf("\n  Mensaje OMS: %s\n", clasificar_nivel_oms(razon_max));
    separador();
}
