#ifndef ALERTAS_H
#define ALERTAS_H

#include "estructuras.h"

int  supera_limite(Contaminantes c);
void mostrar_contexto_climatico(const FactoresClimaticos *c);
void verificar_alertas(const Zona *z, int idx);
void mostrar_recomendaciones(const Zona *z);

#endif
