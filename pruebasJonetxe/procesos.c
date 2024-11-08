#include "globales.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    while(1) {
        sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
        PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
        nuevoProceso->pid = (rand() % 1000) * 100;
        nuevoProceso->siguiente = NULL;
        nuevoProceso->tiempoVida = rand() % 30;
        pthread_mutex_lock(&mutex);
        num_processes++;
        nuevoProceso->siguiente = colaProcesos;
        colaProcesos = nuevoProceso;
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
