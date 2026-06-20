#include "config.h"
#include "estructuras.h"
#include "validaciones.h"
#include "utilidades.h"
#include "zonas.h"
#include "alertas.h"
#include "predicciones.h"
#include "reportes.h"
#include "interfaz.h"

int main(void)
{
    Zona zonas[NUM_ZONAS];
    int opcion;
    int idx;

    const char *nombres_default[NUM_ZONAS] = {
        "Zona Norte",
        "Zona Sur",
        "Zona Este",
        "Zona Oeste",
        "Zona Centro"};

    printf("\n");
    separador();
    printf("        SISTEMA DE MONITOREO DE CONTAMINACION DEL AIRE\n");
    separador();

    for (int i = 0; i < NUM_ZONAS; i++)
    {
        inicializar_zona(&zonas[i], nombres_default[i]);
    }

    int datos_previos = cargar_datos_historicos(zonas, NUM_ZONAS);

    if (!datos_previos)
    {
        int registros = cargar_historico_base(zonas, NUM_ZONAS);
        printf("[INFO] Se registraron %d datos historicos (29 dias x %d zonas)\n",
               registros, NUM_ZONAS);
        printf("[INFO] Falta registrar el dia 30 (el mas reciente) de cada zona.\n");
        printf("[INFO] Use la opcion 2 del menu para registrarlo.\n");
    }

    for (int i = 0; i < NUM_ZONAS; i++)
    {
        zonas[i].promedio_historico = calcular_promedio_historico(&zonas[i]);
        zonas[i].prediccion = calcular_prediccion(&zonas[i]);
    }

    do
    {
        mostrar_menu();
        opcion = leer_entero("Seleccione una opcion", 0, 9);

        switch (opcion)
        {
        case 1:
            for (int i = 0; i < NUM_ZONAS; i++)
            {
                printf("\n--- Datos actuales para: %s ---\n", zonas[i].nombre);
                ingresar_datos_actuales(&zonas[i]);
                ingresar_clima(&zonas[i]);
                zonas[i].promedio_historico = calcular_promedio_historico(&zonas[i]);
                zonas[i].prediccion = calcular_prediccion(&zonas[i]);
            }
            printf("\n[OK] Datos actuales registrados.\n");
            break;

        case 2:
            mostrar_zonas();
            idx = leer_entero("Numero de zona", 1, NUM_ZONAS) - 1;
            ingresar_dia_mas_reciente(&zonas[idx]);
            zonas[idx].promedio_historico = calcular_promedio_historico(&zonas[idx]);
            zonas[idx].prediccion = calcular_prediccion(&zonas[idx]);
            printf("\n[OK] Dia 30 registrado. Historico actualizado.\n");
            break;

        case 3:
            mostrar_todos(zonas, NUM_ZONAS);
            break;

        case 4:
            mostrar_zonas();
            idx = leer_entero("Numero de zona", 1, NUM_ZONAS) - 1;
            mostrar_zona(&zonas[idx], idx);
            break;

        case 5:
            separador();
            printf("  VERIFICACION DE ALERTAS\n");
            separador();
            for (int i = 0; i < NUM_ZONAS; i++)
            {
                verificar_alertas(&zonas[i], i);
            }
            break;

        case 6:
            mostrar_zonas();
            idx = leer_entero("Numero de zona", 1, NUM_ZONAS) - 1;
            mostrar_recomendaciones(&zonas[idx]);
            break;

        case 7:
            mostrar_zonas();
            idx = leer_entero("Numero de zona", 1, NUM_ZONAS) - 1;
            mostrar_predicciones(&zonas[idx], idx);
            break;

        case 8:
            generar_reporte(zonas, NUM_ZONAS);
            break;

        case 9:
            guardar_datos_historicos(zonas, NUM_ZONAS);
            printf("\n[OK] Datos guardados en '%s'.\n", ARCHIVO_DATOS);
            break;

        case 0:
            guardar_datos_historicos(zonas, NUM_ZONAS);
            printf("\nDatos guardados. Hasta luego.\n\n");
            break;

        default:
            printf("\n[ERROR] Opcion no valida.\n");
        }

    } while (opcion != 0);

    return 0;
}
