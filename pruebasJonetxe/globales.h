#include <pthread.h>
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
extern PCB *colaProcesos;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern pthread_cond_t cond2;
extern int num_processes;
extern int ticks;

void* clockNuestro();
void* timer(void* arg);
void* generarProcesos(void* arg);
