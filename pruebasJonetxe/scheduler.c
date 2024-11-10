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
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&cond2, &mutex);
      // int threadNum = 0;
      PCB *temp = colaProcesos;
      // while (temp != NULL && threadNum < num_threads) { //iterar por la cola de procesos por si se libera algun hilo
      //   printf("Scheduler: Created thread %d for process %d\n", threadNum,
      //          temp->pid);
      //   int quantum = 3;
      //   threadArray[threadNum].process = *temp;
      //   int tiempoRestante = threadArray[threadNum].process.tiempoVida;
      //   if (tiempoRestante > 0) {
      //     if (tiempoRestante > quantum) {
      //       threadArray[threadNum].process.tiempoVida -= quantum;
      //       printf("Proceso %d ejecutado %d segundos\n",
      //              threadArray[threadNum].process.pid, quantum);
      //     } else {
      //       printf("Proceso %d ejecutado %d tiempo ha finalizado\n",
      //              threadArray[threadNum].process.pid, tiempoRestante);
      //       threadArray[threadNum].process.tiempoVida = 0;
      //     }
      //     usleep(quantum * 1000);
      //   }
      //   if (threadArray[threadNum].process.tiempoVida == 0) {
      //     printf("Scheduler: Process %d has completed execution\n",
      //            threadArray[threadNum].process.pid);
      //   }
      //   temp = temp->siguiente;
      //   threadNum++;
      // }
      // if (threadNum == num_threads) {
      //   printf("Scheduler: CPU is full with %d threads.\n", num_threads);
      // }

      for(int i = 0;  i < num_threads; i++){
        if(threadArray[i].process.pid == 0 && temp != NULL){
          threadArray[i].process = *temp;
          temp = temp->siguiente;
          printf("se ha metido el proceso %d al thread %d\n", threadArray[i].process.pid, threadArray[i].id);
          printf("hola0");
        }
      }
    
      int quantum = 3;
      printf("hola0.5");
      for(int i = 0;  i < num_threads; i++){
        printf("hola0.7");
        if(threadArray[i].process.tiempoVida > quantum){
          threadArray[i].process.tiempoVida -= quantum;
          printf("hola1");
          if(temp != NULL){
            printf("hola2");
            PCB *nuevoCola = &threadArray[i].process;
            threadArray[i].process = *temp;
            PCB *temp2 = temp;
            temp = temp->siguiente;
            while(temp2!=NULL && temp2->siguiente != NULL){
              if(temp2 != NULL){
                  *temp2->siguiente = *nuevoCola;
                  printf("1se ha metido el proceso %d al thread %d\n", temp2->pid, threadArray[i].id);
                  break;
              }
              temp2 = temp2->siguiente;
            }
            }

        }
        else{
          printf("hola");
          threadArray[i].process.tiempoVida = 0;
          printf("el proceso %d ha finalizado\n", threadArray[i].process.pid);
          if(temp != NULL){
            threadArray[i].process = *temp;
            temp = temp->siguiente;
            printf("2se ha metido el proceso %d al thread %d\n", threadArray[i].process.pid, threadArray[i].id);
            }
        }
      }

      pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

