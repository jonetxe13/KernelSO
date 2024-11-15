#include "globales.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    int count = 0;
    while(count < 10) {
        // sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
        PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
        nuevoProceso->pid = (rand() % 1000) * 100;
        nuevoProceso->siguiente = NULL;
        nuevoProceso->tiempoVida = rand() % 20 + 1;
        nuevoProceso->nice = rand() % 10 + 1;
        pthread_mutex_lock(&mutex);
        num_processes++;
        if(colaProcesos == NULL) {
            colaProcesos = nuevoProceso;
        } else {
            PCB *ultimo = colaProcesos;
            while(ultimo->siguiente != NULL) {
                ultimo = ultimo->siguiente;
            }
            ultimo->siguiente = nuevoProceso;
        }
        printf("Generado proceso con PID %d y tiempo de vida %d y su nice %d\n", nuevoProceso->pid, nuevoProceso->tiempoVida, nuevoProceso->nice);
        count++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
