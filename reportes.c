#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reportes.h"
#include "config.h"
#include "alertas.h"
#include "predicciones.h"

void generar_reporte(const Zona zonas[], int n) {
    FILE *fp = fopen(ARCHIVO_REPORTE, "w");
    if (fp == NULL) {
        printf("[ERROR] No se pudo crear el reporte '%s'.\n", ARCHIVO_REPORTE);
        return;
    }

    time_t ahora = time(NULL);
    char   timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&ahora));

    fprintf(fp, "=======================================================\n");
    fprintf(fp, "            REPORTE DE CALIDAD DEL AIRE\n");
    fprintf(fp, "                Generado: %s\n", timestamp);
    fprintf(fp, "=======================================================\n\n");

    fprintf(fp, "Limites OMS (ug/m3):\n");
    fprintf(fp, "  CO2:   %.0f | SO2:  %.0f | NO2: %.0f | PM2.5: %.0f\n\n",
            LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);

    for (int i = 0; i < n; i++) {
        const Zona *z = &zonas[i];

        double razon_co2  = z->prediccion.co2  / LIMITE_CO2;
        double razon_so2  = z->prediccion.so2  / LIMITE_SO2;
        double razon_no2  = z->prediccion.no2  / LIMITE_NO2;
        double razon_pm25 = z->prediccion.pm25 / LIMITE_PM25;
        double razon_max  = razon_co2;
        if (razon_so2  > razon_max) razon_max = razon_so2;
        if (razon_no2  > razon_max) razon_max = razon_no2;
        if (razon_pm25 > razon_max) razon_max = razon_pm25;

        fprintf(fp, "-------------------------------------------------------\n");
        fprintf(fp, "                  Zona %d: %s\n", i + 1, z->nombre);
        fprintf(fp, "-------------------------------------------------------\n");

        fprintf(fp, "Nivel actual:\n");
        fprintf(fp, "  CO2=%.2f  SO2=%.2f  NO2=%.2f  PM2.5=%.2f\n",
                z->actual.co2, z->actual.so2, z->actual.no2, z->actual.pm25);

        fprintf(fp, "Promedio historico (30 dias, ponderado):\n");
        fprintf(fp, "  CO2=%.2f  SO2=%.2f  NO2=%.2f  PM2.5=%.2f\n",
                z->promedio_historico.co2, z->promedio_historico.so2,
                z->promedio_historico.no2, z->promedio_historico.pm25);

        fprintf(fp, "Prediccion (proximas 24 h):\n");
        fprintf(fp, "  CO2=%.2f  SO2=%.2f  NO2=%.2f  PM2.5=%.2f\n",
                z->prediccion.co2, z->prediccion.so2,
                z->prediccion.no2, z->prediccion.pm25);
        fprintf(fp, "  Mensaje OMS: %s\n", clasificar_nivel_oms(razon_max));

        fprintf(fp, "Clima: Temp=%.1fC  Viento=%.1f km/h  Humedad=%.1f%%\n",
                z->clima.temperatura, z->clima.viento, z->clima.humedad);

        fprintf(fp, "Alertas: %s | Prediccion: %s\n",
                supera_limite(z->actual)     ? "ACTIVA" : "ok",
                supera_limite(z->prediccion) ? "ACTIVA" : "ok");

        fprintf(fp, "\n");
    }

    fprintf(fp, "=======================================================\n");
    fprintf(fp, "                  Fin del reporte\n");
    fprintf(fp, "=======================================================\n");

    fclose(fp);
    printf("\n[OK] Reporte generado: '%s'\n", ARCHIVO_REPORTE);
    system("start reporte_contaminacion.txt");
}
