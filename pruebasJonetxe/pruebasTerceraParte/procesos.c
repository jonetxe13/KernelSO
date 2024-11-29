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
    while(count < 10) {
        // Usar el Loader para cargar el proceso
        PCB *nuevoProceso = cargarProceso(count);
        if (nuevoProceso == NULL) {
            fprintf(stderr, "Error al cargar el proceso %d\n", count);
            return NULL;
        }
        pthread_mutex_lock(&mutex);
        // Añadir el proceso a la cola
        if(colaProcesos == NULL) {
            colaProcesos = nuevoProceso;
        } else {
            PCB *ultimo = colaProcesos;
            while(ultimo->siguiente != NULL) {
                ultimo = ultimo->siguiente;
            }
            ultimo->siguiente = nuevoProceso;
        }
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
        count++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
