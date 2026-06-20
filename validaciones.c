#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int es_numero_valido(const char *cadena, int permitir_decimal) {
    if (cadena == NULL || *cadena == '\0') return 0;

    int i            = 0;
    int tiene_punto  = 0;
    int tiene_digito = 0;

    if (cadena[0] == '-') {
        i = 1;
        if (cadena[1] == '\0') return 0;
    }

    for (; cadena[i] != '\0'; i++) {
        if (isdigit((unsigned char)cadena[i])) {
            tiene_digito = 1;
        } else if (cadena[i] == '.' && permitir_decimal) {
            if (tiene_punto) return 0;
            tiene_punto = 1;
        } else {
            return 0;
        }
    }
    return tiene_digito;
}

int leer_entero(const char *mensaje, int min, int max) {
    char buffer[64];
    int  valor;

    while (1) {
        printf("%s [%d-%d]: ", mensaje, min, max);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "\n[ERROR] No se pudo leer la entrada. Saliendo.\n");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (!es_numero_valido(buffer, 0)) {
            printf("[ERROR] Ingrese solo numeros enteros (sin letras ni puntos).\n");
            continue;
        }

        valor = atoi(buffer);

        if (valor < min || valor > max) {
            printf("[ERROR] El valor debe estar entre %d y %d.\n", min, max);
            continue;
        }

        return valor;
    }
}

double leer_double(const char *mensaje, double min, double max) {
    char   buffer[64];
    double valor;

    while (1) {
        printf("%s [%.1f - %.1f]: ", mensaje, min, max);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "\n[ERROR] No se pudo leer la entrada. Saliendo.\n");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (!es_numero_valido(buffer, 1)) {
            printf("[ERROR] Ingrese un numero valido (use punto como decimal, ej: 12.5).\n");
            continue;
        }

        valor = atof(buffer);

        if (valor < min || valor > max) {
            printf("[ERROR] El valor debe estar entre %.1f y %.1f.\n", min, max);
            continue;
        }

        return valor;
    }
}

void leer_cadena(const char *mensaje, char *destino, int maxlen) {
    char buffer[256];

    while (1) {
        printf("%s: ", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "\n[ERROR] No se pudo leer la entrada.\n");
            exit(EXIT_FAILURE);
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            printf("[ERROR] El campo no puede estar vacio.\n");
            continue;
        }

        strncpy(destino, buffer, maxlen - 1);
        destino[maxlen - 1] = '\0';
        return;
    }
}
