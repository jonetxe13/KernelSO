#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

void* scheduler(void* args){

    ThreadArgs *threadArgs = (ThreadArgs *)args;
    Thread *threadArray = threadArgs->threads;
    int num_threads = threadArgs->num_threads;
    for(int i = 0; i < num_threads; i++){
      threadArray[i].process.pid=0;
      threadArray[i].id=i;
    }
    while (1) {
    // printf("hola justo al principio loop");
      pthread_mutex_lock(&mutex);
    // printf("hola dentro del lock");
      pthread_cond_wait(&cond2, &mutex);
      PCB *temp = colaProcesos;
      printf("hola0");
      for(int i = 0;  i < num_threads; i++){
        if(threadArray[i].process.pid == 0 && temp != NULL){
          threadArray[i].process = *temp;
          PCB *liberar = temp;
          temp = temp->siguiente;
          free(liberar);
          colaProcesos = temp; 
            printf("se ha metido el proceso %d al thread %d\n", threadArray[i].process.pid, threadArray[i].id);
        }
      }
    
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

      // printf("hola3\n");
      pthread_mutex_unlock(&mutex);
      // printf("unlocked?\n");
    }
    pthread_exit(NULL);
    return NULL;
}

