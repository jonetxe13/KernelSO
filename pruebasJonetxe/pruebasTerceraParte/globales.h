#include <pthread.h>
// Estructura para el PCB (Process Control Block)
typedef struct {
    void* code;  // Puntero a la dirección virtual del segmento de código
    void* data;  // Puntero a la dirección virtual del segmento de datos
    void* pgb;   // Puntero a la dirección física de la tabla de páginas
} MM;

typedef struct PCB {
    int pid;                // Identificador del proceso
    struct PCB *siguiente;  // Puntero al siguiente proceso en la cola
    int tiempoVida;
    int nice;
    MM mm;                  // Estructura de memoria
} PCB;

typedef struct { 
    int id; 
    PCB process; 
} Thread;

typedef struct { 
    int id; 
    Thread* threads;
} Core;

typedef struct { 
    int id; 
    Core* cores;
} CPU;

typedef struct { 
    int id; 
    CPU* cpus;
} Machine;

typedef struct { 
    Thread* threads; 
    int num_threads; 
} ThreadArgs;

typedef struct {
    float hercios;
    int num_cpus;
    int num_cores;
    int num_threads;
    Machine *maquina;
} SchedulerArgs;

// Variables globales
extern PCB *colaProcesos;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern pthread_cond_t cond2;
extern int num_processes;
extern int ticks;
extern int frecuenciaTicks;

extern void* memoriaFisica;
extern void* kernel_space;

// Prototipo de la función Loader
int Loader();
PCB* cargarProceso(int pid);

void* clockNuestro();
void* timer(void* arg);
void* generarProcesos(void* arg);
void* scheduler(void* arg);
void ordenarColaProcesos(PCB **colaProcesos);
void imprimirColaProcesos(PCB *colaProcesos);
