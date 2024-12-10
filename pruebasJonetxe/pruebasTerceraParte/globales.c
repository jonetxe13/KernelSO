#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "globales.h"

// Definiciones de las variables globales
uint32_t memoriaFisica[MEMORY_ARRAY_SIZE];
uint32_t* kernel_space = &memoriaFisica[0];
uint32_t* user_space = &memoriaFisica[KERNEL_SIZE / sizeof(uint32_t)];

PCB *colaProcesos = NULL;
SchedulerArgs *sched_args;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int num_processes = 0;
int ticks = 0;
int frecuenciaTicks = 0;

void imprimirColaProcesos(PCB *colaProcesos) { 
    PCB *temp = colaProcesos; 
    while (temp != NULL) { 
        printf("PID %d nice %d -> ", temp->pid, temp->nice); 
        temp = temp->siguiente; 
    } 
    printf("NULL\n");
}

void ordenarColaProcesos(PCB **colaProcesos) {
    if (!colaProcesos || !(*colaProcesos)) {
        return;
    }

    PCB *sorted = NULL; // Lista ordenada
    PCB *current = *colaProcesos;

    while (current != NULL) {
        PCB *next = current->siguiente; // Guardar el siguiente nodo

        // Insertar current en la lista ordenada
        if (sorted == NULL || current->nice < sorted->nice) {
            current->siguiente = sorted;
            sorted = current;
        } else {
            PCB *temp = sorted;
            while (temp->siguiente != NULL && temp->siguiente->nice <= current->nice) {
                temp = temp->siguiente;
            }
            current->siguiente = temp->siguiente;
            temp->siguiente = current;
        }

        current = next; // Avanzar al siguiente nodo
    }

    *colaProcesos = sorted; // Actualizar la cola original
    printf("Cola después de ordenar:\n");
    // imprimirColaProcesos(*colaProcesos);
}

void agregarProcesoACola(PCB *nuevoProceso) {
    pthread_mutex_lock(&mutex);
    if (colaProcesos == NULL) {
        colaProcesos = nuevoProceso;
        printf("agregarProcesoACola: Añadido PID %d como primera entrada\n", nuevoProceso->pid);
    } else {
        PCB *ultimo = colaProcesos;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = nuevoProceso;
        printf("agregarProcesoACola: Añadido PID %d al final de la cola\n", nuevoProceso->pid);
    }
    pthread_mutex_unlock(&mutex);
}
