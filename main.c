#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Estructura para el PCB (Process Control Block)
typedef struct PCB {
    int pid;                // Identificador del proceso
    // char estado[20];        // Estado del proceso
    // int prioridad;          // Prioridad del proceso
    // int tiempo_cpu;         // Tiempo de CPU usado
    // int tiempo_espera;      // Tiempo de espera
    struct PCB *siguiente;  // Puntero al siguiente proceso en la cola
} PCB;

// Variables globales
PCB *colaProcesos = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ticks = 0;

// Thread para gestionar el reloj del sistema
void* gestionarReloj(void* arg) {
    while(1) {
        usleep(1000); // Simula un ciclo de reloj
        ticks++;
        pthread_cond_signal(&cond);
    }
    return NULL;
}

// Thread para el Timer
void* gestionarTimer(int arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (ticks % (1/arg) != 0) {  // Ajusta el intervalo de ticks según sea necesario
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Timer tick\n");
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Thread para el Generador de Procesos
void* generarProcesos(void* arg) {
    while(1) {
        sleep(rand() % 5 + 1); // Genera un proceso cada 1-5 segundos
        PCB *nuevoProceso = (PCB*)malloc(sizeof(PCB));
        nuevoProceso->pid = rand() % 1000;
        // strcpy(nuevoProceso->estado, "Nuevo");
        // nuevoProceso->prioridad = rand() % 10;
        // nuevoProceso->tiempo_cpu = 0;
        // nuevoProceso->tiempo_espera = 0;
        nuevoProceso->siguiente = NULL;

        pthread_mutex_lock(&mutex);
        nuevoProceso->siguiente = colaProcesos;
        colaProcesos = nuevoProceso;
        printf("Generado proceso con PID %d\n", nuevoProceso->pid);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void reloj(int frequencia){
	int suma = 0;
	while(){
		suma++;
		if(suma==1/frequencia) 
	}
}

int main(int argc, char** argv) {
    srand(time(NULL));

    pthread_t relojThread, timerThread, procesoThread;

	reloj(argv[1])
    // Crear threads
    if (pthread_create(&relojThread, NULL, gestionarReloj, NULL)) {
        fprintf(stderr, "Error creando el thread del reloj\n");
        return 1;
    }
    if (pthread_create(&timerThread, NULL, gestionarTimer(argv[2]), NULL)) {
        fprintf(stderr, "Error creando el thread del timer\n");
        return 1;
    }
    if (pthread_create(&procesoThread, NULL, generarProcesos, NULL)) {
        fprintf(stderr, "Error creando el thread del generador de procesos\n");
        return 1;
    }

    // Unirse a los threads (para que el programa no termine inmediatamente)
    pthread_join(relojThread, NULL);
    pthread_join(timerThread, NULL);
    pthread_join(procesoThread, NULL);

    return 0;
}

