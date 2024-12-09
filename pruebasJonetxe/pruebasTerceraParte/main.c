#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "globales.h"



int main(int argc, char** argv) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s <frecuencia> <num_cpus> <num_cores> <num_threads>\n", argv[0]);
        return 1;
    }

    printf("Inicializando máquina...\n");

    Machine maquina;
    maquina.cpus = malloc(atoi(argv[2]) * sizeof(CPU));
    if (maquina.cpus == NULL) {
        fprintf(stderr, "Error al asignar memoria para CPUs\n");
        return 1;
    }

    for (int i = 0; i < atoi(argv[2]); i++) {
        maquina.cpus[i].id = i + 1;
        maquina.cpus[i].cores = malloc(atoi(argv[3]) * sizeof(Core));
        if (maquina.cpus[i].cores == NULL) {
            fprintf(stderr, "Error al asignar memoria para Cores\n");
            return 1;
        }
        printf("CPU %d inicializado\n", i + 1);

        for (int j = 0; j < atoi(argv[3]); j++) {
            maquina.cpus[i].cores[j].id = j + 1;
            maquina.cpus[i].cores[j].threads = malloc(atoi(argv[4]) * sizeof(Thread));
            if (maquina.cpus[i].cores[j].threads == NULL) {
                fprintf(stderr, "Error al asignar memoria para Threads\n");
                return 1;
            }
            printf("Core %d de CPU %d inicializado\n", j + 1, i + 1);

            for (int k = 0; k < atoi(argv[4]); k++) {
                maquina.cpus[i].cores[j].threads[k].id = k + 1;
                maquina.cpus[i].cores[j].threads[k].process.pid = 0; // Inicialización adicional 
                maquina.cpus[i].cores[j].threads[k].process.tiempoVida = 0; // Inicialización adicional 
                maquina.cpus[i].cores[j].threads[k].process.siguiente = NULL;
                printf("Thread %d de Core %d de CPU %d inicializado\n", k + 1, j + 1, i + 1);
            }
        }
    }

    void* args[4] = {argv[2], argv[3], argv[4], &maquina};

    if (argc == 3) {
        printf("tienes que meter un argumento melon");
        exit(1);
    }

    float frequencia = atof(argv[1]);
    pthread_t relojThread, timerThread, procesoThread, schedulerThread;


    // Cargar programas utilizando el Loader
    if (Loader() != 0) {
        fprintf(stderr, "Error al cargar los programas\n");
        // free(memoriaFisica);
        return 1;
    }

    // Crear threads
    if (pthread_create(&relojThread, NULL, clockNuestro, NULL)) {
        fprintf(stderr, "Error creando el thread del reloj\n");
        // free(memoriaFisica);
        return 1;
    }
    if (pthread_create(&timerThread, NULL, timer, &frequencia)) {
        fprintf(stderr, "Error creando el thread del timer\n");
        // free(memoriaFisica);
        return 1;
    }
    if (pthread_create(&procesoThread, NULL, generarProcesos, NULL)) {
        fprintf(stderr, "Error creando el thread del generador de procesos\n");
        // free(memoriaFisica);
        return 1;
    }
    if (pthread_create(&schedulerThread, NULL, scheduler, &args)) {
        fprintf(stderr, "Error creando el thread de un proceso");
        // free(memoriaFisica);
        return 1;
    }

    printf("Threads creados, esperando a que terminen...\n");

    pthread_join(schedulerThread, NULL);
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    pthread_join(procesoThread, NULL);

    // Liberar memoria
    for (int i = 0; i < atoi(argv[2]); i++) {
        for (int j = 0; j < atoi(argv[3]); j++) {
            free(maquina.cpus[i].cores[j].threads);
        }
        free(maquina.cpus[i].cores);
    }
    free(maquina.cpus);
    // free(memoriaFisica);

    return 0;
}

