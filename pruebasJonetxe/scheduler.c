#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* scheduler(void* args){

    ThreadArgs *threadArgs = (ThreadArgs *)args;
    Thread *threadArray = threadArgs->threads;
    int num_threads = threadArgs->num_threads;
    while (1) {
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&cond2, &mutex);
      int threadNum = 0;
      PCB *temp = colaProcesos;
      while (temp != NULL && threadNum < num_threads) {
        printf("Scheduler: Created thread %d for process %d\n", threadNum,
               temp->pid);
        int quantum = 3;
        threadArray[threadNum].process = *temp;
        int tiempoRestante = threadArray[threadNum].process.tiempoVida;
        if (tiempoRestante > 0) {
          if (tiempoRestante > quantum) {
            threadArray[threadNum].process.tiempoVida -= quantum;
            printf("Proceso %d ejecutado %d segundos\n",
                   threadArray[threadNum].process.pid, quantum);
          } else {
            printf("Proceso %d ejecutado %d tiempo ha finalizado\n",
                   threadArray[threadNum].process.pid, tiempoRestante);
            threadArray[threadNum].process.tiempoVida = 0;
          }
          usleep(quantum * 1000);
        }
        if (threadArray[threadNum].process.tiempoVida == 0) {
          printf("Scheduler: Process %d has completed execution\n",
                 threadArray[threadNum].process.pid);
        }
        temp = temp->siguiente;
        threadNum++;
      }
      if (threadNum == num_threads) {
        printf("Scheduler: CPU is full with %d threads.\n", num_threads);
      }
      pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

