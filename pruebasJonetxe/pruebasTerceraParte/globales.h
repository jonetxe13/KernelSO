#include <pthread.h>
#include <stdint.h>

#define PHYSICAL_MEMORY_SIZE (1 << 24) // 24 bits de dirección, equivalente a 16 MB
#define KERNEL_SIZE (PHYSICAL_MEMORY_SIZE / 4) // El kernel ocupa la primera cuarta parte

// Número de elementos en el array
#define MEMORY_ARRAY_SIZE (PHYSICAL_MEMORY_SIZE / sizeof(uint32_t))

extern uint32_t memoriaFisica[MEMORY_ARRAY_SIZE]; // Array que representa la memoria física

// Asignar la primera cuarta parte al kernel
extern uint32_t* kernel_space;

// El espacio de usuario comienza después del kernel
extern uint32_t* user_space;

// Estructura para una entrada de la TLB
typedef struct {
    unsigned int page_number;   // Número de página virtual
    unsigned int frame_number;  // Número de marco físico
    int valid;                  // Bit de validez
} TLBEntry;

// Estructura para la TLB
typedef struct {
    int num_entries;            // Número de entradas en la TLB
    TLBEntry entries[16]; // Array de entradas
} TLB;

// Estructura para la MMU
typedef struct {
    void* PTBR;   // Registro apuntador a la tabla de páginas
    TLB tlb;      // Estructura de la TLB
} MMU;

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

// Estructura para el hilo hardware (Thread)
typedef struct {
    int id;
    PCB process;
    MMU mmu;       // Estructura de la MMU
    void* RI;      // Registro de instrucción
    void* PC;      // Contador de programa
} Thread;

// Estructuras para Core, CPU y Machine
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

// Prototipo de la función Loader
int Loader();
PCB* cargarProceso(int pid);

void* clockNuestro();
void* timer(void* arg);
void* generarProcesos(void* arg);
void* scheduler(void* arg);
void ordenarColaProcesos(PCB **colaProcesos);
void imprimirColaProcesos(PCB *colaProcesos);
