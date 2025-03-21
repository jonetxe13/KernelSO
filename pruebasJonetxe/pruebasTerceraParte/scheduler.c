#include "globales.h"

// Assuming frequenciaTicks is a global variable, define it here if not included in "globales.h"
extern int frecuenciaTicks;
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

void meterSiEstaVacioOAcabado(int num_threads, Thread* threadArray, int num_cpu, int num_core){
      for(int i = 0;  i < num_threads; i++){
        if(threadArray[i].process.pid == 0 && colaProcesos != NULL){
          threadArray[i].process = *colaProcesos;
          threadArray[i].process.siguiente = NULL;

          PCB *liberar = colaProcesos;
          colaProcesos = colaProcesos->siguiente;

          free(liberar);
          printf("Se ha asignado el proceso %d al CPU %d, Core %d, Thread %d con nice %d\n", 
                 threadArray[i].process.pid, num_cpu, num_core, threadArray[i].id, threadArray[i].process.nice);
        }
      }
}

void roundRobin(int num_threads, Thread* threadArray){
      // printf("roundRobin: Entering roundRobin\n");
      // imprimirColaProcesos(colaProcesos);
      int quantum = frecuenciaTicks;
      for (int i = 0; i < num_threads; i++) {
          if (threadArray[i].process.pid == 0) continue;

          if (threadArray[i].process.tiempoVida > quantum) {
              threadArray[i].process.tiempoVida -= quantum;
              threadArray[i].process.nice -= quantum/2;
              printf("proceso %d su nuevo tiempo de vida es %d y su nice es %d\n", threadArray[i].process.pid, threadArray[i].process.tiempoVida, threadArray[i].process.nice);

              PCB *nuevoCola = malloc(sizeof(PCB));
              if (nuevoCola == NULL) {
                printf("Failed to allocate memory for nuevoCola");
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
            printf("proceso %d ha terminado\n", threadArray[i].process.pid);

            threadArray[i].process.pid = 0;
            threadArray[i].process.tiempoVida = 0;
        }
      }
      // printf("roundRobin: Exiting roundRobin\n");
}

void* scheduler(void* args){

    void** arg_array = (void**)args;
    // Correctly cast the arguments
    int num_cpus = atoi((char*)arg_array[0]); 
    int num_cores = atoi((char*)arg_array[1]); 
    int num_threads = atoi((char*)arg_array[2]); 
    Machine *maquina = (Machine*)arg_array[3]; // Correct cast without taking address

    printf("Scheduler iniciado con %d CPUs, %d Cores por CPU, %d Threads por Core\n", num_cpus, num_cores, num_threads);

    // printf("numero de CPUs: %d \n numero de Nucleos: %d \n numero de threads: %d \n", num_cpus, num_cores, num_threads);
    if (num_cpus <= 0 || num_cores <= 0 || num_threads <= 0) {
        printf("Invalid scheduler configuration: CPUs=%d, Cores=%d, Threads=%d\n", num_cpus, num_cores, num_threads);
        pthread_exit(NULL);
    }

    while (1) {
      pthread_mutex_lock(&mutex);
      imprimirColaProcesos(colaProcesos);
      pthread_cond_wait(&cond2, &mutex);
      ordenarColaProcesos(&colaProcesos);

      for(int i = 0; i < num_cpus; i++){
        for(int j = 0; j < num_cores; j++){
          if (maquina->cpus[i].cores[j].threads == NULL) {
              printf("Array of threads is NULL for CPU %d, Core %d\n", i, j);
              continue;
          }
          roundRobin(num_threads, maquina->cpus[i].cores[j].threads);
        }
      }

      for(int i = 0; i < num_cpus; i++){
        for(int j = 0; j < num_cores; j++){
          if (maquina->cpus[i].cores[j].threads == NULL) {
              printf("Thread array is NULL for CPU %d, Core %d\n", i, j);
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

