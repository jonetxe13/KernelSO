#include "globales.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

// Función para crear un nuevo proceso
PCB* crearNuevoProceso() {
    PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
    if (nuevoProceso == NULL) {
        perror("Error al asignar memoria para nuevoProceso");
        exit(EXIT_FAILURE);
    }
    nuevoProceso->pid = (rand() % 1000) * 100;
    nuevoProceso->siguiente = NULL;
    nuevoProceso->tiempoVida = rand() % 20 + 1;
    nuevoProceso->nice = rand() % 10 + 1;
    return nuevoProceso;
}

// Función para agregar un proceso a la cola
void agregarProcesoACola(PCB *nuevoProceso) {
    pthread_mutex_lock(&mutex);
    num_processes++;
    if (colaProcesos == NULL) {
        colaProcesos = nuevoProceso;
    } else {
        PCB *ultimo = colaProcesos;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = nuevoProceso;
    }
    printf("Generado proceso con PID %d, tiempo de vida %d, nice %d\n", 
           nuevoProceso->pid, nuevoProceso->tiempoVida, nuevoProceso->nice);
    pthread_mutex_unlock(&mutex);
}

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    int count = 0;
    while (count < 10) {
        // sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
        PCB *nuevoProceso = crearNuevoProceso();
        agregarProcesoACola(nuevoProceso);
        count++;
    }
    return NULL;
}