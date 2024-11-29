#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "globales.h"

PCB *colaProcesos = NULL;
SchedulerArgs *sched_args;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int num_processes = 0;
int ticks = 0;
int frecuenciaTicks = 0;

void* memoriaFisica = NULL;
void* kernel_space = NULL;

void imprimirColaProcesos(PCB *colaProcesos) { 
    PCB *temp = colaProcesos; 
    while (temp != NULL) { 
        printf("PID %d nice %d -> ", temp->pid, temp->nice); 
        temp = temp->siguiente; 
    } 
    printf("NULL\n");
}

void ordenarColaProcesos(PCB **colaProcesos) {
    PCB *sorted = NULL;
    while (*colaProcesos != NULL) {
        PCB *minPrev = NULL;
        PCB *minNode = *colaProcesos;
        PCB *current = *colaProcesos;

        while (current->siguiente != NULL) {
            if (current->siguiente->nice < minNode->nice) {
                minPrev = current;
                minNode = current->siguiente;
            }
            current = current->siguiente;
        }

        if (minPrev != NULL) {
            minPrev->siguiente = minNode->siguiente;
        } else {
            *colaProcesos = minNode->siguiente;
        }

        minNode->siguiente = sorted;
        sorted = minNode;
    }
    *colaProcesos = sorted;
    printf("cola despues de ordenar: \n");
    imprimirColaProcesos(*colaProcesos);
}
