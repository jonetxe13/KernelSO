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
        nuevoProceso->pid = rand() % 1000;
        nuevoProceso->siguiente = NULL;
        pthread_mutex_lock(&mutex);
        num_processes++;
        nuevoProceso->siguiente = colaProcesos;
        colaProcesos = nuevoProceso;
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
        
        // PCB *temp = colaProcesos;
        // printf("Lista de procesos:\n");
        // while (temp != NULL) {
        //     printf("PID %d\n", temp->pid);
        //     temp = temp->siguiente;
        // }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
