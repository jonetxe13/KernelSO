#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "globales.h"

// Función para inicializar la máquina
void inicializarMaquina(Machine *maquina, int num_cpus, int num_cores, int num_threads) {
    maquina->cpus = malloc(num_cpus * sizeof(CPU));
    for (int i = 0; i < num_cpus; i++) {
        maquina->cpus[i].id = i + 1;
        maquina->cpus[i].cores = malloc(num_cores * sizeof(Core));
        printf("El id de CPU es: %d\n", i + 1);
        for (int j = 0; j < num_cores; j++) {
            maquina->cpus[i].cores[j].id = j + 1;
            maquina->cpus[i].cores[j].threads = malloc(num_threads * sizeof(Thread));
            printf("El id de core es: %d\n", j + 1);
            for (int k = 0; k < num_threads; k++) {
                maquina->cpus[i].cores[j].threads[k].id = k + 1;
                maquina->cpus[i].cores[j].threads[k].process.pid = 0;
                maquina->cpus[i].cores[j].threads[k].process.tiempoVida = 0;
                maquina->cpus[i].cores[j].threads[k].process.siguiente = NULL;
                printf("El id de thread es: %d\n", k + 1);
            }
        }
    }
}

// Función para liberar la memoria de la máquina
void liberarMaquina(Machine *maquina, int num_cpus, int num_cores) {
    for (int i = 0; i < num_cpus; i++) {
        for (int j = 0; j < num_cores; j++) {
            free(maquina->cpus[i].cores[j].threads);
        }
        free(maquina->cpus[i].cores);
    }
    free(maquina->cpus);
}

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <frecuencia> <num_cpus> <num_cores> <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    float frecuencia = atof(argv[1]);
    int num_cpus = atoi(argv[2]);
    int num_cores = atoi(argv[3]);
    int num_threads = atoi(argv[4]);

    Machine maquina;
    inicializarMaquina(&maquina, num_cpus, num_cores, num_threads);

    void* args[4] = {argv[2], argv[3], argv[4], &maquina};

    pthread_t relojThread, timerThread, procesoThread, schedulerThread;

    // Crear threads
    if (pthread_create(&relojThread, NULL, clockNuestro, NULL)) {
        fprintf(stderr, "Error creando el thread del reloj\n");
        return 1;
    }
    if (pthread_create(&timerThread, NULL, timer, &frecuencia)) {
        fprintf(stderr, "Error creando el thread del timer\n");
        return 1;
    }
    if (pthread_create(&procesoThread, NULL, generarProcesos, NULL)) {
        fprintf(stderr, "Error creando el thread del generador de procesos\n");
        return 1;
    }
    if (pthread_create(&schedulerThread, NULL, scheduler, args)) {
        fprintf(stderr, "Error creando el thread del scheduler\n");
        return 1;
    }

    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    pthread_join(procesoThread, NULL);
    pthread_join(schedulerThread, NULL);

    liberarMaquina(&maquina, num_cpus, num_cores);

    return 0;
}
