#ifndef ZONAS_H
#define ZONAS_H

#include "estructuras.h"

void inicializar_zona(Zona *z, const char *nombre);
void ingresar_datos_actuales(Zona *z);
void ingresar_clima(Zona *z);
void ingresar_dia_mas_reciente(Zona *z);

int  cargar_historico_base(Zona zonas[], int n);
void guardar_datos_historicos(const Zona zonas[], int n);
int  cargar_datos_historicos(Zona zonas[], int n);

Contaminantes calcular_promedio_historico(const Zona *z);
Contaminantes calcular_prediccion(const Zona *z);
double        factor_climatico(const FactoresClimaticos *c);

#endif
