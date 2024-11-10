#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "globales.h"



int main(int argc, char** argv) {
    // pthread_t hilos[atoi(argv[3])];
    printf("yo uqe se ya0");
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
    printf("yo uqe se ya0");
    pthread_join(schedulerThread, NULL);
    printf("yo uqe se ya");
    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    pthread_join(procesoThread, NULL);

    return 0;
}

