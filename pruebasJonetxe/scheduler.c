#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *PrintTask(void *processId) { 
    int process= *(int *)processId; 
    printf("Hello World! It's me, thread #%d!\n", process); 
    sleep(3); // simulación de la duración del proceso
    pthread_exit(NULL); 
}

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
                        pthread_create(&threadArray[threadNum].thread, NULL, PrintTask, (void*) &pid_copy); 
                        printf("Scheduler: Created thread %d for process %d\n", threadNum, temp->pid); 
                        threadNum++; 
                        temp = temp->siguiente; 
                        colaProcesos = colaProcesos->siguiente;
                } 
            } 
            if (threadNum == num_threads) { 
                printf("Scheduler: CPU is full with %d threads.\n", num_threads); 
            }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

