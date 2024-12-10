#include "globales.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

extern PCB *colaProcesos;
extern pthread_mutex_t mutex;

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    int count = 0;
    printf("generarProcesos: Iniciando generación de procesos\n");
    while(count < 10) {
        PCB *nuevoProceso = cargarProceso(count);
        if (nuevoProceso == NULL) {
            fprintf(stderr, "Error al cargar el proceso %d\n", count);
            return NULL;
        }
        agregarProcesoACola(nuevoProceso);
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
        count++;
    }
    printf("generarProcesos: Finalizada generación de procesos\n");
    return NULL;
}
