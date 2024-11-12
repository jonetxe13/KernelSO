#include "globales.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

void meterSiEstaVacioOAcabado(int num_threads, Thread* threadArray, PCB *temp, int num_cpu, int num_core){
      for(int i = 0;  i < num_threads; i++){
        if(threadArray[i].process.pid == 0 && temp != NULL){
          threadArray[i].process = *temp;
          PCB *liberar = temp;
          temp = temp->siguiente;
          free(liberar);
          colaProcesos = temp; 
          printf("se ha metido el proceso %d al cpu %d, core %d thread %d\n", threadArray[i].process.pid, num_cpu, num_core, threadArray[i].id);
        }
      }
}
void roundRobin(int num_threads, Thread* threadArray){
        int quantum = 3;
      for (int i = 0; i < num_threads; i++) {
          if (threadArray[i].process.pid == 0) continue;

          if (threadArray[i].process.tiempoVida > quantum) {
              threadArray[i].process.tiempoVida -= quantum;
              printf("proceso %d su nuevo tiempo de vida es %d\n", threadArray[i].process.pid, threadArray[i].process.tiempoVida);

              PCB *nuevoCola = malloc(sizeof(PCB));
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

              if (colaProcesos != NULL) {
                threadArray[i].process = *colaProcesos;
                PCB *temp = colaProcesos;
                colaProcesos = colaProcesos->siguiente;
                free(temp);
                printf("Nuevo proceso %d asignado al thread %d\n", threadArray[i].process.pid, threadArray[i].id);
              } else {
                threadArray[i].process.pid = 0;
              }
        } else {
            threadArray[i].process.tiempoVida = 0;
            printf("proceso %d ha terminado\n", threadArray[i].process.pid);
            threadArray[i].process.pid = 0;
        }
      }
}

// void inicializarThreadsACero(Core *core){
//
// }

void* scheduler(void* args){

   void** arg_array = (void**)args; 
   int num_cpus = atoi((char*)arg_array[0]); 
   int num_cores = atoi((char*)arg_array[1]); 
   int num_threads = atoi((char*)arg_array[2]); 
   Machine *maquina = (Machine*)&arg_array[3];

    printf("numero de CPUs: %d \n numero de Nucleos: %d \n numero de threads: %d \n", num_cpus, num_cores, num_threads);

    // Thread *threadArray = threadArgs->threads;
    // int num_threads = argumentos[2];
    // for(int i = 0; i < num_cpus; i++){
    //   for(int j = 0; j < num_cores; j++){
    //     inicializarThreadsACero();
    //   }
    // }
    while (1) {
    // printf("hola justo al principio loop");
      pthread_mutex_lock(&mutex);
    // printf("hola dentro del lock");
      pthread_cond_wait(&cond2, &mutex);
      PCB *temp = colaProcesos;
      printf("hola0");

      for(int i = 0; i < num_cpus; i++)
        for(int j = 0; j < num_cores; j++)
          meterSiEstaVacioOAcabado(num_threads, maquina->cpus[i].cores[j].threads, temp, i, j);
      
      printf("hola\n");
      for(int i = 0; i < num_cpus; i++)
        for(int j = 0; j < num_cores; j++)
          roundRobin(num_threads, maquina->cpus[i].cores[j].threads);

      // printf("hola3\n");
      pthread_mutex_unlock(&mutex);
      // printf("unlocked?\n");
    }
    pthread_exit(NULL);
    return NULL;
}

