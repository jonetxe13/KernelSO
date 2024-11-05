#include <math.h>
#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
// #include "clock.c"
#include "globales.h"


void *PrintTask(void *processId) { 
    int process= *(int *)processId; 
    printf("Hello World! It's me, thread #%d!\n", process); 
    sleep(3); // simulación de la duración del proceso
    free(processId);
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
                } 
                temp = temp->siguiente; 
            } 
            if (threadNum == num_threads) { 
                printf("Scheduler: CPU is full with %d threads.\n", num_threads); 
            }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}


int main(int argc, char** argv) {
    // pthread_t hilos[atoi(argv[3])];
    Thread threads[atoi(argv[2])];
    ThreadArgs threadArgs = {threads, atoi(argv[2])};
    
    if (argc == 1){
        printf("tienes que meter un argumento melon");
        exit(1);
    }
    float frequencia = atof(argv[1]);
    pthread_t relojThread, timerThread, procesoThread, schedulerThread;

    // Crear threads
    if (pthread_create(&relojThread, NULL, clockNuestro, NULL)) {
        fprintf(stderr, "Error creando el thread del reloj\n");
        return 1;
    }
    if (pthread_create(&timerThread, NULL, timer, &frequencia)) {
        fprintf(stderr, "Error creando el thread del timer\n");
        return 1;
    }
    if (pthread_create(&procesoThread, NULL, generarProcesos, NULL)) {
        fprintf(stderr, "Error creando el thread del generador de procesos\n");
        return 1;
    }
    if (pthread_create(&schedulerThread, NULL, scheduler, &threadArgs)) {
        fprintf(stderr, "Error creando el thread de un proceso");
        return 1;
    }
    pthread_join(schedulerThread, NULL);
    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    // pthread_join(procesoThread, NULL);

    return 0;
}

