#include <math.h>
#include <linux/sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Estructura para el PCB (Process Control Block)
typedef struct task_struct {
    int pid;                // Identificador del proceso
    struct task_struct *siguiente;  // Puntero al siguiente proceso en la cola
} PCB;

typedef struct { 
    pthread_t thread; 
    int id; 
} Thread;

typedef struct { 
    Thread* threads; 
    int num_threads; 
} ThreadArgs;

// Variables globales
PCB *colaProcesos = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int num_processes = 0;
int ticks = 0;

void *PrintTask(void *processId) { 
    int process= *(int *)processId; 
    printf("Hello World! It's me, thread #%d!\n", process); 
    sleep(3); // simulación de la duración del proceso
    free(processId);
    pthread_exit(NULL); 
}

void* scheduler(void* args){
    ThreadArgs *threadArgs = (ThreadArgs *) args; 
    Thread *threadArray = threadArgs->threads;
    int num_threads = threadArgs->num_threads;
    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond2, &mutex);
        int index = 0;
        int threadNum = 0;
        // while( index < num_processes){
        //     PCB p = colaProcesos[index];
        //     int *pid_copy = malloc(sizeof(int));
        //     *pid_copy = p.pid;
        //
            printf("Scheduler: num_processes = %d, num_threads = %d\n", num_processes, threadNum);
            PCB *temp = colaProcesos; 
            while (temp != NULL) { 
                int *pid_copy = malloc(sizeof(int)); 
                *pid_copy = temp->pid; 
                if (threadNum < num_threads) { 
                        pthread_create(&threadArray[threadNum].thread, NULL, PrintTask, (void*) &pid_copy); 
                        printf("Scheduler: Created thread %d for process %d\n", threadNum, temp->pid); 
                        threadNum++; 
                } 
                temp = temp->siguiente; 
            } 
            if (threadNum == num_threads) { 
                printf("Scheduler: CPU is full with %d threads.\n", num_threads); 
            }
            // if(threadNum < num_threads){
            //     pthread_create(&threadArray[threadNum].thread, NULL, PrintTask,(void*) &pid_copy);
            //     printf("Scheduler: Created thread %d for process %d\n", threadNum,p.pid);
            //     threadNum++;
            // }else{
            //     printf("el maximo de procesos ya esta ejecutandose");
            // }
            // index++;
        // }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    while(1) {
        sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
        PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
        nuevoProceso->pid = rand() % 1000;
        nuevoProceso->siguiente = NULL;
        pthread_mutex_lock(&mutex);
        num_processes++;
        nuevoProceso->siguiente = colaProcesos;
        colaProcesos = nuevoProceso;
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
        
        // PCB *temp = colaProcesos;
        // printf("Lista de procesos:\n");
        // while (temp != NULL) {
        //     printf("PID %d\n", temp->pid);
        //     temp = temp->siguiente;
        // }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Thread para el Timer
void* timer(void* arg) {
    float* hercios=(float*)arg;
    int frequenciaTicks=(int)(1.0/ *hercios);
    printf("hercios: %f", *hercios);
    printf("hercios: %d",frequenciaTicks);
    while(1) {
        pthread_mutex_lock(&mutex);
        while (ticks < frequenciaTicks) {  // Ajusta el intervalo de ticks según sea necesario
            pthread_cond_wait(&cond, &mutex);
        }
        // printf("Timer tick\n");
        ticks = 0;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Thread para gestionar el reloj del sistema
void* clockNuestro(){
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        ticks++;
        // printf("clock tick\n");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char** argv) {
    // pthread_t hilos[atoi(argv[3])];
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
    pthread_join(schedulerThread, NULL);
    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    // pthread_join(procesoThread, NULL);

    return 0;
}

