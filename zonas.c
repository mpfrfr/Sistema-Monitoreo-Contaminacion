#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zonas.h"
#include "config.h"
#include "validaciones.h"

void inicializar_zona(Zona *z, const char *nombre) {
    strncpy(z->nombre, nombre, MAX_NOMBRE - 1);
    z->nombre[MAX_NOMBRE - 1] = '\0';

    memset(&z->actual,             0, sizeof(Contaminantes));
    memset(&z->clima,              0, sizeof(FactoresClimaticos));
    memset(&z->prediccion,         0, sizeof(Contaminantes));
    memset(&z->promedio_historico, 0, sizeof(Contaminantes));

    for (int d = 0; d < DIAS_HISTORICOS; d++) {
        memset(&z->historico[d], 0, sizeof(Contaminantes));
    }
}

void ingresar_datos_actuales(Zona *z) {
    printf("  CO2  (ug/m3, limite OMS %.0f): ", LIMITE_CO2);
    z->actual.co2  = leer_double("  CO2  (ug/m3)", 0.0, 50000.0);
    z->actual.so2  = leer_double("  SO2  (ug/m3)", 0.0, 5000.0);
    z->actual.no2  = leer_double("  NO2  (ug/m3)", 0.0, 5000.0);
    z->actual.pm25 = leer_double("  PM2.5(ug/m3)", 0.0, 1000.0);
}

void ingresar_clima(Zona *z) {
    printf("  --- Factores Climaticos ---\n");
    z->clima.temperatura = leer_double("  Temperatura (C)", -20.0, 60.0);
    z->clima.viento      = leer_double("  Velocidad del viento (km/h)", 0.0, 300.0);
    z->clima.humedad     = leer_double("  Humedad (%)", 0.0, 100.0);
}

void ingresar_dia_mas_reciente(Zona *z) {
    for (int d = DIAS_HISTORICOS - 1; d >= 1; d--) {
        z->historico[d] = z->historico[d - 1];
    }

    printf("\nRegistro del dia 30 (dato mas reciente) para: %s\n", z->nombre);
    z->historico[0].co2  = leer_double("    CO2  (ug/m3)", 0.0, 50000.0);
    z->historico[0].so2  = leer_double("    SO2  (ug/m3)", 0.0, 5000.0);
    z->historico[0].no2  = leer_double("    NO2  (ug/m3)", 0.0, 5000.0);
    z->historico[0].pm25 = leer_double("    PM2.5(ug/m3)", 0.0, 1000.0);
}

int cargar_historico_base(Zona zonas[], int n) {
    FILE *fp = fopen(ARCHIVO_HISTORICO_BASE, "r");
    if (fp == NULL) {
        printf("[ADVERTENCIA] No se encontro '%s'. No hay historico base disponible.\n",
               ARCHIVO_HISTORICO_BASE);
        return 0;
    }

    char linea[256];
    int  registros_leidos = 0;

    while (fgets(linea, sizeof(linea), fp) != NULL) {
        if (linea[0] == '#' || linea[0] == '\n') continue;

        int    zona_idx, posicion;
        double co2, so2, no2, pm25;

        int campos = sscanf(linea, "%d;%d;%lf;%lf;%lf;%lf",
                             &zona_idx, &posicion, &co2, &so2, &no2, &pm25);

        if (campos != 6) continue;
        if (zona_idx < 0 || zona_idx >= n) continue;
        if (posicion < 1 || posicion >= DIAS_HISTORICOS) continue;

        zonas[zona_idx].historico[posicion].co2  = co2;
        zonas[zona_idx].historico[posicion].so2  = so2;
        zonas[zona_idx].historico[posicion].no2  = no2;
        zonas[zona_idx].historico[posicion].pm25 = pm25;

        registros_leidos++;
    }

    fclose(fp);
    return registros_leidos;
}

Contaminantes calcular_promedio_historico(const Zona *z) {
    Contaminantes resultado = {0};
    double peso_total = 0.0;

    for (int d = 0; d < DIAS_HISTORICOS; d++) {
        double peso = (d < 7) ? PESO_RECIENTE : PESO_ANTIGUO;
        resultado.co2  += z->historico[d].co2  * peso;
        resultado.so2  += z->historico[d].so2  * peso;
        resultado.no2  += z->historico[d].no2  * peso;
        resultado.pm25 += z->historico[d].pm25 * peso;
        peso_total += peso;
    }

    if (peso_total > 0.0) {
        resultado.co2  /= peso_total;
        resultado.so2  /= peso_total;
        resultado.no2  /= peso_total;
        resultado.pm25 /= peso_total;
    }

    return resultado;
}

double factor_climatico(const FactoresClimaticos *c) {
    double f = 1.0;

    if (c->temperatura > 25.0) {
        f += (c->temperatura - 25.0) * 0.01;
    }

    if (c->viento > 10.0) {
        f -= (c->viento - 10.0) * 0.005;
    }

    if (c->humedad > 70.0) {
        f -= (c->humedad - 70.0) * 0.002;
    }

    if (f < 0.3) f = 0.3;
    if (f > 2.0) f = 2.0;

    return f;
}

Contaminantes calcular_prediccion(const Zona *z) {
    Contaminantes pred;
    double        fc = factor_climatico(&z->clima);

    pred.co2  = z->promedio_historico.co2  * fc;
    pred.so2  = z->promedio_historico.so2  * fc;
    pred.no2  = z->promedio_historico.no2  * fc;
    pred.pm25 = z->promedio_historico.pm25 * fc;

    return pred;
}

void guardar_datos_historicos(const Zona zonas[], int n) {
    FILE *fp = fopen(ARCHIVO_DATOS, "wb");
    if (fp == NULL) {
        printf("[ADVERTENCIA] No se pudo guardar en '%s'.\n", ARCHIVO_DATOS);
        return;
    }
    fwrite(zonas, sizeof(Zona), n, fp);
    fclose(fp);
}


int cargar_datos_historicos(Zona zonas[], int n) {
    FILE *fp = fopen(ARCHIVO_DATOS, "rb");
    if (fp == NULL) {
        return 0;
    }
    size_t leidos = fread(zonas, sizeof(Zona), n, fp);
    fclose(fp);
    if (leidos == (size_t)n) {
        printf("[INFO] Datos historicos cargados correctamente desde '%s'.\n", ARCHIVO_DATOS);
        return 1;
    }
    printf("[ADVERTENCIA] El archivo de datos puede estar incompleto.\n");
    return 1;
}
