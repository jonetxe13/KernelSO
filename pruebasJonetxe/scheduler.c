#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* scheduler(void* args){

    ThreadArgs *threadArgs = (ThreadArgs *) args; 
    Thread *threadArray = threadArgs->threads;
    int num_threads = threadArgs->num_threads;

    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond2, &mutex);
        int index = 0;
        int threadNum = 0;
        printf("Scheduler: num_processes = %d, num_threads = %d\n", num_processes, threadNum);
        PCB *temp = colaProcesos; 
        while (temp != NULL) { 
            int *pid_copy = malloc(sizeof(int)); 
            *pid_copy = temp->pid; 
            if (threadNum < num_threads) {
                 printf("Scheduler: Created thread %d for process %d\n", threadNum, temp->pid);
                 int quantum = 3;
                 threadArray[threadNum].process = *temp;

                 while (temp != NULL) {
                    for (int i = 0; i < num_threads; i++) {
                        if (threadArray[i].process.tiempoVida > 0) {
                            if (threadArray[i].process.tiempoVida > quantum) {
                                threadArray[i].process.tiempoVida -= quantum;
                                printf("Proceso %d ejecutado %d segundos\n", threadArray[i].process.pid, quantum);
                            } else {
                                printf("Proceso %d ejecutado %d tiempo ha finalizado\n", threadArray[i].process.pid, threadArray[i].process.tiempoVida);
                                threadArray[i].process.tiempoVida = 0;
                            }
                            // Simula la acción de la CPU
                            usleep(quantum * 1000);
                        }
                    }
                    // Avanza al siguiente proceso
                    temp = temp->siguiente;
                 }

    // Limpiar procesos terminados
    for (int i = 0; i < num_threads; i++) {
        if (threadArray[i].process.tiempoVida == 0) {
            // Manejar el proceso terminado
            printf("Scheduler: Process %d has completed execution\n", threadArray[i].process.pid);
        }
    }

    threadNum++;
 }
            free(pid_copy);
        } 
        if (threadNum == num_threads) { 
            printf("Scheduler: CPU is full with %d threads.\n", num_threads); 
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

