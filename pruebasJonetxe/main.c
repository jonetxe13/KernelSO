#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// void scheduler_call();
// Estructura para el PCB (Process Control Block)
typedef struct PCB {
    int pid;                // Identificador del proceso
    struct PCB *siguiente;  // Puntero al siguiente proceso en la cola
} PCB;

// Variables globales
PCB *colaProcesos = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int num_processes = 0;
int ticks = 0;

void* scheduler(void* threadid){
    long tid = (long)threadid;
    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond2, &mutex);
        static int index = 0;
        if (index >= num_processes) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        PCB p = colaProcesos[index++];
        pthread_mutex_unlock(&mutex);

        printf("Thread #%ld está ejecutando el proceso #%d\n", tid, p.pid);
        sleep(3); // simulación de la duración del proceso
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
        num_processes++;
        pthread_mutex_lock(&mutex);
        nuevoProceso->siguiente = colaProcesos;
        colaProcesos = nuevoProceso;
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
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
    srand(time(NULL));
    pthread_t thread[atoi(argv[2])];
    if (argc == 1){
        printf("tienes que meter un argumento melon");
        exit(1);
    }
    float frequencia = atof(argv[1]);
    pthread_t relojThread, timerThread, procesoThread;

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
    for (int i =0; i<atoi(argv[2]); i++){
        if (pthread_create(&thread[i], NULL, scheduler, NULL)) {
            fprintf(stderr, "Error creando el thread de un proceso");
            return 1;
        }
    }
    for (int i =0; i<atoi(argv[2]); i++){
        pthread_join(thread[i], NULL);
    }
    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    // pthread_join(procesoThread, NULL);

    return 0;
}

