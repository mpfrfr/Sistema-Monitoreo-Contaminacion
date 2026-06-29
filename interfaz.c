#include <stdio.h>
#include "interfaz.h"
#include "config.h"
#include "alertas.h"
#include "utilidades.h"

void mostrar_menu(void) {
    printf("\n");
    separador();
    printf("%37s\n", "MENU PRINCIPAL");
    separador();
    printf("  1. Ingresar datos actuales de todas las zonas\n");
    printf("  2. Modificar el dia 30 (dato mas reciente) de una zona\n");
    printf("  3. Mostrar estado de todas las zonas\n");
    printf("  4. Ver detalle de una zona\n");
    printf("  5. Verificar alertas (incluye contexto climatico)\n");
    printf("  6. Ver recomendaciones por zona\n");
    printf("  7. Ver predicciones y mensaje OMS\n");
    printf("  8. Generar reporte (archivo)\n");
    printf("  9. Guardar datos historicos\n");
    printf("  0. Salir\n");
    separador();
}

void mostrar_zonas(void) {
    printf("\n");
    separador();
    printf("%37s\n"," ZONAS DISPONIBLES\n");
    separador();
    printf("  1. Zona Norte\n");
    printf("  2. Zona Sur\n");
    printf("  3. Zona Este\n");
    printf("  4. Zona Oeste\n");
    printf("  5. Zona Centro\n");
    separador();
}

void mostrar_zona(const Zona *z, int idx) {
    printf("\n");
    separador();
    printf("  ZONA %d: %s\n", idx + 1, z->nombre);
    separador();

    printf("  NIVEL ACTUAL (ug/m3):\n");
    printf("    CO2:   %8.2f  (limite: %.0f)  %s\n", z->actual.co2,  LIMITE_CO2,  z->actual.co2  > LIMITE_CO2  ? "[EXCEDE]" : "[OK]");
    printf("    SO2:   %8.2f  (limite: %.0f)   %s\n", z->actual.so2,  LIMITE_SO2,  z->actual.so2  > LIMITE_SO2  ? "[EXCEDE]" : "[OK]");
    printf("    NO2:   %8.2f  (limite: %.0f)   %s\n", z->actual.no2,  LIMITE_NO2,  z->actual.no2  > LIMITE_NO2  ? "[EXCEDE]" : "[OK]");
    printf("    PM2.5: %8.2f  (limite: %.0f)   %s\n", z->actual.pm25, LIMITE_PM25, z->actual.pm25 > LIMITE_PM25 ? "[EXCEDE]" : "[OK]");

    printf("\n  PROMEDIO HISTORICO 30 DIAS (ponderado):\n");
    printf("    CO2: %.2f  SO2: %.2f  NO2: %.2f  PM2.5: %.2f\n",
           z->promedio_historico.co2, z->promedio_historico.so2,
           z->promedio_historico.no2, z->promedio_historico.pm25);

    printf("\n  PREDICCION PROXIMAS 24 H:\n");
    printf("    CO2: %.2f  SO2: %.2f  NO2: %.2f  PM2.5: %.2f\n",
           z->prediccion.co2, z->prediccion.so2,
           z->prediccion.no2, z->prediccion.pm25);

    printf("\n  CLIMA ACTUAL:\n");
    printf("    Temperatura: %.1f C  |  Viento: %.1f km/h  |  Humedad: %.1f%%\n",
           z->clima.temperatura, z->clima.viento, z->clima.humedad);

    printf("\n  ESTADO: %s\n",
           (supera_limite(z->actual) || supera_limite(z->prediccion))
           ? "*** ALERTA ACTIVA ***"
           : "Dentro de parametros normales");
    separador();
}

void mostrar_todos(const Zona zonas[], int n) {
    printf("\n");
    separador();
    printf("  RESUMEN DE TODAS LAS ZONAS\n");
    separador();
    printf("  %-14s | %-8s | %-8s | %-8s | %-8s | %s\n",
           "Zona", "CO2", "SO2", "NO2", "PM2.5", "Estado");
    printf("  -------------|---------|---------|---------|---------|----------\n");

    for (int i = 0; i < n; i++) {
        const Zona *z = &zonas[i];
        printf("  %-14s | %7.1f | %7.1f | %7.1f | %7.1f | %s\n",
               z->nombre,
               z->actual.co2, z->actual.so2, z->actual.no2, z->actual.pm25,
               supera_limite(z->actual) ? "ALERTA" : "OK");
    }
    separador();
}
