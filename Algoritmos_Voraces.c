#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --------------- EL PROBLEMA DEL CAMBIO ---------------

void problemaCambio(int cantidad) {
    printf("\n--- Problema del Cambio (N = %d) ---\n", cantidad);
    
    // Monedas disponibles (ya ordenadas para el enfoque voraz)
    int monedas[] = {25, 10, 5, 1};
    int num_tipos = 4;
    int contador_monedas = 0;

    printf("Cambio entregado: ");
       
    for (int i = 0; i < num_tipos; i++) {
        while (cantidad >= monedas[i]) {
            cantidad -= monedas[i]; 
            printf("%d ", monedas[i]);
            contador_monedas++;
        }
    }

    
    printf("\nTotal monedas: %d\n", contador_monedas);
}


// --------------- MOCHILA FRACCIONARIA ---------------

// Estructura para los objetos
typedef struct {
    int id; // Identificador del objeto
    double peso; // Peso del objeto
    double valor; // Valor del objeto
    double ratio; // Valor por unidad de peso
} Objeto;

// Función de comparación para qsort (Orden Descendente por Ratio)
int compararRatio(const void *a, const void *b) {

    // Cast a Objeto pointers,trata a estos punteros como punteros de tipo Objeto
    Objeto *objA = (Objeto *)a;
    Objeto *objB = (Objeto *)b;
    
    // Comparar los ratios para orden descendente (mayor valor a menor)
    if (objB->ratio > objA->ratio) return 1; // devuelve positivo si objB es mayor que objA
    if (objB->ratio < objA->ratio) return -1; // devuelve negativo si objA es mayor que objB
    return 0;
}

// Función para resolver el problema de la mochila fraccionaria
void problemaMochila(int num_articulos, double capacidad_mochila) {
    printf("\n--- Mochila Fraccionaria (%d articulos, Capacidad: %.2f) ---\n", num_articulos, capacidad_mochila);

    // Reservar memoria dinámica para los objetos
    Objeto *inventario = (Objeto *)malloc(num_articulos * sizeof(Objeto));
    
    // Generación de objetos aleatorios para la prueba
    for (int i = 0; i < num_articulos; i++) {
        inventario[i].id = i + 1;
        inventario[i].peso = 1.0 + ((double)rand() / RAND_MAX) * 19.0;   // 1 a 20
        inventario[i].valor = 10.0 + ((double)rand() / RAND_MAX) * 90.0; // 10 a 100
        // Calcular ratio valor/peso (valor de cada gramo)
        inventario[i].ratio = inventario[i].valor / inventario[i].peso;
    }

    // PASO VORAZ: Ordenar usando qsort
    // Ordenar los objetos por su ratio valor/peso en orden descendente
    qsort(inventario, num_articulos, sizeof(Objeto), compararRatio);

    // Variables para el seguimiento del valor y peso acumulados
    double valor_total = 0.0;
    double peso_actual = 0.0;

    // El algoritmo recorre el inventario ya ordenado (del mejor al peor) e intenta meterlos en la mochila
    for (int i = 0; i < num_articulos; i++) {
        // Caso 1: Si cabe el objeto completo
        if (peso_actual + inventario[i].peso <= capacidad_mochila) {
            // Tomar todo el objeto
            peso_actual += inventario[i].peso;
            valor_total += inventario[i].valor;
        } 
        // Caso 2: Si no cabe el objeto completo
        else {
            // Tomar fracción
            double espacio_restante = capacidad_mochila - peso_actual;
            // Calcular el valor parcial basado en la fracción que cabe (regla de tres)
            double valor_parcial = inventario[i].valor * (espacio_restante / inventario[i].peso);
            valor_total += valor_parcial;
            peso_actual += espacio_restante;
            break; // Mochila llena
        }
    }

    // Resultados
    printf("Valor total acumulado: %.2f\n", valor_total);
    printf("Peso final: %.2f / %.2f\n", peso_actual, capacidad_mochila);

    // Liberar memoria, para evitar fugas de memoria
    free(inventario);
}


// --------------- SELECCIÓN DE ACTIVIDADES ---------------

// Estructura para las actividades
typedef struct {
    int id; // Identificador de la actividad
    int inicio; // Tiempo de inicio
    int fin; // Tiempo de finalización
} Actividad;

// Función de comparación para qsort (Orden Ascendente por tiempo Final)
int compararFin(const void *a, const void *b) {

    // Cast a Actividad pointers,trata a estos punteros como punteros de tipo Actividad
    Actividad *actA = (Actividad *)a;
    Actividad *actB = (Actividad *)b;

    // Comparar los tiempos de finalización para orden ascendente
    return (actA->fin - actB->fin);
}

// Función para resolver el problema de selección de actividades
void problemaActividades(Actividad *actividades, int n) {
    printf("\n--- Seleccion de Actividades ---\n");

    // PASO VORAZ 1: Ordenar por tiempo de finalización
    // Ordenar las actividades por su tiempo de finalización en orden ascendente
    qsort(actividades, n, sizeof(Actividad), compararFin);

    printf("Actividades seleccionadas:\n");

    // PASO VORAZ 2: Seleccionar la primera
    // Como ya ordenamos, la primera del arreglo es la que acaba más temprano
    int i = 0;
    printf("Actividad %d [%d - %d]\n", actividades[i].id, actividades[i].inicio, actividades[i].fin);
    
    // Guardar el tiempo de finalización de la última actividad seleccionada
    int ultimo_fin = actividades[i].fin;
    int cuenta = 1;

    // PASO VORAZ 3: Seleccionar siguientes compatibles
    for (int j = 1; j < n; j++) {
        if (actividades[j].inicio >= ultimo_fin) {
            printf("Actividad %d [%d - %d]\n", actividades[j].id, actividades[j].inicio, actividades[j].fin);
            ultimo_fin = actividades[j].fin;
            cuenta++;
        }
    }
    printf("Total actividades: %d\n", cuenta);
}

// --------------- FIN DE LOS ALGORITMOS VORACES ---------------

// --------------- INTERCCION CON EL USUARIO ---------------

int cantidad_de_cambio() {
    int cantidad;
    printf("\nIngrese la cantidad a cambiar (entero positivo): ");
    scanf("%d", &cantidad);
    return cantidad;
}

void ingresarActividadesUsuario() {
    int n;
    printf("\n--- Ingreso Manual de Actividades ---\n");
    printf("¿Cuantas actividades deseas ingresar? ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Numero de actividades invalido.\n");
        return;
    }

    // Reservamos memoria para el arreglo de actividades
    Actividad *misActividades = (Actividad *)malloc(n * sizeof(Actividad));

    printf("Ingresa los tiempos (Inicio Fin) para cada actividad:\n");
    printf("Ejemplo: si inicia a las 5 y termina a las 9, escribe: 5 9\n");

    for (int i = 0; i < n; i++) {
        misActividades[i].id = i + 1;
        printf("Actividad %d: ", i + 1);
        scanf("%d %d", &misActividades[i].inicio, &misActividades[i].fin);
    }

    // Llamamos a la función voraz con los datos del usuario
    problemaActividades(misActividades, n);

    // Liberar la memoria
    free(misActividades);
}

// --------------- MAIN ---------------

int main() {
    srand(time(NULL)); // Semilla para aleatorios

    // 1. Pruebas Cambio
    problemaCambio(67);
    problemaCambio(43);
    // Prueba con entrada del usuario
    problemaCambio(cantidad_de_cambio());

    // 2. Pruebas Mochila
    problemaMochila(10, 50.0);
    problemaMochila(100, 500.0);
    problemaMochila(1000, 5000.0);

    // 3. Pruebas Actividades
    // Definición de actividades de prueba
    Actividad lista[] = {
        {1, 5, 9}, {2, 1, 2}, {3, 3, 4}, {4, 0, 6}, {5, 5, 7}, {6, 8, 9}
    };
    int n_act = sizeof(lista) / sizeof(lista[0]);
    problemaActividades(lista, n_act);
    // Prueba con entrada del usuario
    ingresarActividadesUsuario();

    return 0;
}