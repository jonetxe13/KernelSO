#include <pthread.h>
// Estructura para el PCB (Process Control Block)
typedef struct task_struct {
    int pid;                // Identificador del proceso
    struct task_struct *siguiente;  // Puntero al siguiente proceso en la cola
    int tiempoVida;
} PCB;

typedef struct { 
    int id; 
    PCB process; 
} Thread;

typedef struct { 
    int id; 
    Thread threads[];
} Core;

typedef struct { 
    int id; 
    Core cores[];
} CPU;

typedef struct { 
    int id; 
    CPU cpus[];
} Machine;

typedef struct { 
    Thread* threads; 
    int num_threads; 
} ThreadArgs;

// Variables globales
extern PCB *colaProcesos;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern pthread_cond_t cond2;
extern int num_processes;
extern int ticks;

void* clockNuestro();
void* timer(void* arg);
void* generarProcesos(void* arg);
void* scheduler(void* arg);
