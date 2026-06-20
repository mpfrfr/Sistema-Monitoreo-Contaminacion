#include <stdio.h>
#include "alertas.h"
#include "config.h"

int supera_limite(Contaminantes c) {
    return (c.co2  > LIMITE_CO2  ||
            c.so2  > LIMITE_SO2  ||
            c.no2  > LIMITE_NO2  ||
            c.pm25 > LIMITE_PM25);
}

void mostrar_contexto_climatico(const FactoresClimaticos *c) {
    if (c->viento < 10.0 && c->humedad > 70.0) {
        printf("  [CLIMA] Viento bajo + humedad alta: el aire esta estancado y los\n");
        printf("          contaminantes tienden a acumularse en la zona.\n");
    } else if (c->viento < 10.0) {
        printf("  [CLIMA] Viento bajo: la dispersion de contaminantes es limitada.\n");
    } else if (c->viento > 20.0) {
        printf("  [CLIMA] Viento fuerte: favorece la dispersion, lo que reduce el\n");
        printf("          riesgo real frente al valor medido o previsto.\n");
    }

    if (c->temperatura > 30.0) {
        printf("  [CLIMA] Temperatura alta: favorece la formacion de smog\n");
        printf("          fotoquimico, intensificando el efecto del NO2.\n");
    }
}

void verificar_alertas(const Zona *z, int idx) {
    printf("\n[Zona %d] %s\n", idx + 1, z->nombre);

    int alerta_actual = supera_limite(z->actual);
    int alerta_pred    = supera_limite(z->prediccion);

    if (!alerta_actual && !alerta_pred) {
        printf("  Estado: OK - Dentro de los limites aceptables.\n");
        return;
    }

    if (alerta_actual) {
        printf("  *** ALERTA ACTUAL ***\n");
        if (z->actual.co2  > LIMITE_CO2 ) printf("  - CO2:  %.2f ug/m3 (limite %.0f)\n", z->actual.co2,  LIMITE_CO2);
        if (z->actual.so2  > LIMITE_SO2 ) printf("  - SO2:  %.2f ug/m3 (limite %.0f)\n", z->actual.so2,  LIMITE_SO2);
        if (z->actual.no2  > LIMITE_NO2 ) printf("  - NO2:  %.2f ug/m3 (limite %.0f)\n", z->actual.no2,  LIMITE_NO2);
        if (z->actual.pm25 > LIMITE_PM25) printf("  - PM2.5:%.2f ug/m3 (limite %.0f)\n", z->actual.pm25, LIMITE_PM25);
    }

    if (alerta_pred) {
        printf("  *** ALERTA PREDICCION (proximas 24 h) ***\n");
        if (z->prediccion.co2  > LIMITE_CO2 ) printf("  - CO2:  %.2f ug/m3 previsto\n", z->prediccion.co2);
        if (z->prediccion.so2  > LIMITE_SO2 ) printf("  - SO2:  %.2f ug/m3 previsto\n", z->prediccion.so2);
        if (z->prediccion.no2  > LIMITE_NO2 ) printf("  - NO2:  %.2f ug/m3 previsto\n", z->prediccion.no2);
        if (z->prediccion.pm25 > LIMITE_PM25) printf("  - PM2.5:%.2f ug/m3 previsto\n", z->prediccion.pm25);
    }

    mostrar_contexto_climatico(&z->clima);
}

void mostrar_recomendaciones(const Zona *z) {
    printf("\n--- Recomendaciones para: %s ---\n", z->nombre);

    int hay_problema = 0;

    if (z->actual.co2 > LIMITE_CO2 || z->prediccion.co2 > LIMITE_CO2) {
        printf("  [CO2]  Reducir trafico vehicular; promover transporte publico.\n");
        hay_problema = 1;
    }
    if (z->actual.so2 > LIMITE_SO2 || z->prediccion.so2 > LIMITE_SO2) {
        printf("  [SO2]  Cierre temporal de industrias con alta emision de azufre.\n");
        hay_problema = 1;
    }
    if (z->actual.no2 > LIMITE_NO2 || z->prediccion.no2 > LIMITE_NO2) {
        printf("  [NO2]  Restriccion vehicular en horario pico. Evitar quemas.\n");
        hay_problema = 1;
    }
    if (z->actual.pm25 > LIMITE_PM25 || z->prediccion.pm25 > LIMITE_PM25) {
        printf("  [PM2.5] Suspender actividades al aire libre. Uso obligatorio de mascarilla.\n");
        printf("  [PM2.5] Activar protocolos de construccion para reducir polvo.\n");
        hay_problema = 1;
    }

    if (!hay_problema) {
        printf("  Los niveles de contaminacion estan dentro de los limites.\n");
        printf("  Continuar con monitoreo periodico.\n");
    } else {
        mostrar_contexto_climatico(&z->clima);
    }
}
