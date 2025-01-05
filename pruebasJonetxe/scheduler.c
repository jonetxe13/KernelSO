#include "globales.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

// Assuming frequenciaTicks is a global variable, define it here if not included in "globales.h"
extern int frecuenciaTicks;

void meterSiEstaVacioOAcabado(int num_threads, Thread* threadArray, int num_cpu, int num_core) {
    for (int i = 0; i < num_threads; i++) {
        if (threadArray[i].process.pid == 0 && colaProcesos != NULL) {
            threadArray[i].process = *colaProcesos;
            threadArray[i].process.siguiente = NULL;

            PCB *liberar = colaProcesos;
            colaProcesos = colaProcesos->siguiente;
            free(liberar);

            printf("Proceso %d asignado al CPU %d, core %d, thread %d con nice %d\n", 
                   threadArray[i].process.pid, num_cpu, num_core, threadArray[i].id, threadArray[i].process.nice);
        }
    }
}

void roundRobin(int num_threads, Thread* threadArray) {
    imprimirColaProcesos(colaProcesos);
    int quantum = frecuenciaTicks;

    for (int i = 0; i < num_threads; i++) {
        if (threadArray[i].process.pid == 0) continue;

        if (threadArray[i].process.tiempoVida > quantum) {
            threadArray[i].process.tiempoVida -= quantum;
            threadArray[i].process.nice -= quantum / 2;
            printf("Proceso %d: nuevo tiempo de vida %d, nice %d\n", 
                   threadArray[i].process.pid, threadArray[i].process.tiempoVida, threadArray[i].process.nice);

            PCB *nuevoCola = malloc(sizeof(PCB));
            if (nuevoCola == NULL) {
                perror("Error al asignar memoria para nuevoCola");
                exit(EXIT_FAILURE);
            }
            *nuevoCola = threadArray[i].process;
            nuevoCola->siguiente = NULL;

            if (colaProcesos != NULL) {
                PCB *ultimo = colaProcesos;
                while (ultimo->siguiente != NULL) {
                    ultimo = ultimo->siguiente;
                }
                ultimo->siguiente = nuevoCola;
            } else {
                colaProcesos = nuevoCola;
            }

            threadArray[i].process.pid = 0;
            threadArray[i].process.tiempoVida = 0;
        } else {
            printf("Proceso %d ha terminado\n", threadArray[i].process.pid);
            threadArray[i].process.pid = 0;
            threadArray[i].process.tiempoVida = 0;
        }
    }
}

void* scheduler(void* args) {
    void** arg_array = (void**)args;
    int num_cpus = atoi((char*)arg_array[0]);
    int num_cores = atoi((char*)arg_array[1]);
    int num_threads = atoi((char*)arg_array[2]);
    Machine *maquina = (Machine*)arg_array[3];

    if (num_cpus <= 0 || num_cores <= 0 || num_threads <= 0) {
        fprintf(stderr, "Configuración de scheduler inválida: CPUs=%d, Cores=%d, Threads=%d\n", num_cpus, num_cores, num_threads);
        pthread_exit(NULL);
    }

    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond2, &mutex);

        ordenarColaProcesos(&colaProcesos);

        for (int i = 0; i < num_cpus; i++) {
            for (int j = 0; j < num_cores; j++) {
                if (maquina->cpus[i].cores[j].threads == NULL) {
                    fprintf(stderr, "Array de threads es NULL para CPU %d, Core %d\n", i, j);
                    continue;
                }
                roundRobin(num_threads, maquina->cpus[i].cores[j].threads);
            }
        }

        for (int i = 0; i < num_cpus; i++) {
            for (int j = 0; j < num_cores; j++) {
                if (maquina->cpus[i].cores[j].threads == NULL) {
                    fprintf(stderr, "Array de threads es NULL para CPU %d, Core %d\n", i, j);
                    continue;
                }
                meterSiEstaVacioOAcabado(num_threads, maquina->cpus[i].cores[j].threads, i, j);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
    return NULL;
}