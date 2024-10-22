#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// void scheduler_call();
// Estructura para el PCB (Process Control Block)
// typedef struct PCB {
//     int pid;                // Identificador del proceso
//     // char estado[20];        // Estado del proceso
//     // int prioridad;          // Prioridad del proceso
//     // int tiempo_cpu;         // Tiempo de CPU usado
//     // int tiempo_espera;      // Tiempo de espera
//     struct PCB *siguiente;  // Puntero al siguiente proceso en la cola
// } PCB;

// Variables globales
// PCB *colaProcesos = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ticks = 0;


// Thread para el Generador de Procesos
// void* generarProcesos(void* arg) {
//     while(1) {
//         sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
//         PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
//         nuevoProceso->pid = rand() % 1000;
//         // strcpy(nuevoProceso->estado, "Nuevo");
//         // nuevoProceso->prioridad = rand() % 10;
//         // nuevoProceso->tiempo_cpu = 0;
//         // nuevoProceso->tiempo_espera = 0;
//         nuevoProceso->siguiente = NULL;
//
//         pthread_mutex_lock(&mutex);
//         nuevoProceso->siguiente = colaProcesos;
//         colaProcesos = nuevoProceso;
//         printf("Generado proceso con PID %d\n", nuevoProceso->pid);
//         pthread_mutex_unlock(&mutex);
//     }
//     return NULL;
// }

// Thread para el Timer
void* timer(void* arg) {
    float* hercios=(float*)arg;
    int frequenciaTicks=(int)(1.0/ *hercios);
    printf("hercios: %f", *hercios);
    printf("hercios: %d",frequenciaTicks);
    while(1) {
        pthread_mutex_lock(&mutex);
            // printf("ticks: %d",ticks);
        while (ticks < frequenciaTicks) {  // Ajusta el intervalo de ticks según sea necesario
            pthread_cond_wait(&cond, &mutex);
        }
        // printf("Timer tick\n");
        // printf("Frequencia: %d\n", frequenciaTicks);
        // printf("Tick: %d", ticks);
        printf("Timer tick\n");
        ticks = 0;
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
        printf("clock tick\n");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        // printf("ticks: %d",ticks);
    }
    return NULL;
}

int main(int argc, char** argv) {
    srand(time(NULL));

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
    // if (pthread_create(&procesoThread, NULL, generarProcesos, NULL)) {
    //     fprintf(stderr, "Error creando el thread del generador de procesos\n");
    //     return 1;
    // }

    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    // pthread_join(procesoThread, NULL);

    return 0;
}

