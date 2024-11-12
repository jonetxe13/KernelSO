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

    // Thread threads[atoi(argv[2])];
    // Core cores[atoi(argv[3])];
    // CPU cpu[atoi(argv[2])];
    Machine maquina;
    for (int i = 0; i<(atoi(argv[2])); i++){
        for (int j = 0; j<(atoi(argv[3])); j++){
            for (int k = 0; k<(atoi(argv[4])); k++){
                maquina.cpus[i].id = i;
                maquina.cpus[i].cores[j].id= j;
                maquina.cpus[i].cores[j].threads[k].id = 0;
            }
        }
    }
    void* args[4] = {argv[2], argv[3], argv[4], &maquina};
    
    if (argc == 3){
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
    if (pthread_create(&schedulerThread, NULL, scheduler, &args)) {
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

