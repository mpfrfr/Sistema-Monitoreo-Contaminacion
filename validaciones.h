#ifndef VALIDACIONES_H
#define VALIDACIONES_H

void limpiar_buffer(void);
int  es_numero_valido(const char *cadena, int permitir_decimal);
int    leer_entero(const char *mensaje, int min, int max);
double leer_double(const char *mensaje, double min, double max);
void   leer_cadena(const char *mensaje, char *destino, int maxlen);

#endif
