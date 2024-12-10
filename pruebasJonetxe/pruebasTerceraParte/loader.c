#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void cargarEnMemoria(PCB* proceso, uint32_t virtual_address, uint32_t value) {
    uint32_t page_number = virtual_address >> 12; // Páginas de 4KB
    uint32_t offset = virtual_address & 0xFFF;

    if(((uint32_t*)proceso->mm.pgb)[page_number] == 0) {
        static uint32_t next_frame = KERNEL_SIZE / sizeof(uint32_t);
        ((uint32_t*)proceso->mm.pgb)[page_number] = next_frame++;
    }

    uint32_t frame_number = ((uint32_t*)proceso->mm.pgb)[page_number];
    uint32_t physical_address = (frame_number << 12) | offset;

    if(physical_address / sizeof(uint32_t) >= MEMORY_ARRAY_SIZE) {
        fprintf(stderr, "Error: dirección física fuera de límites\n");
        return;
    }

    memoriaFisica[physical_address / sizeof(uint32_t)] = value;
}

int Loader() {
    int totalProgramas = 10; // Número de programas a cargar
    for(int i = 0; i < totalProgramas; i++) {
        PCB* proceso = cargarProceso(i);
        if(proceso == NULL) {
            return -1;
        }
        // Añadir el proceso a la cola de procesos
        pthread_mutex_lock(&mutex);
        if(colaProcesos == NULL) {
            colaProcesos = proceso;
        } else {
            PCB *ultimo = colaProcesos;
            while(ultimo->siguiente != NULL) {
                ultimo = ultimo->siguiente;
            }
            ultimo->siguiente = proceso;
        }
        pthread_mutex_unlock(&mutex);
        imprimirColaProcesos(colaProcesos);
    }
    return 0;
}

PCB* cargarProceso(int pid) {
    // Create and initialize PCB
    PCB* nuevoProceso = (PCB*)malloc(sizeof(PCB));
    if (!nuevoProceso) {
        fprintf(stderr, "Error al asignar memoria para el PCB\n");
        return NULL;
    }
    nuevoProceso->pid = pid;
    nuevoProceso->tiempoVida = rand() % 10 + 1;
    nuevoProceso->nice = rand() % 30 + 1;
    nuevoProceso->siguiente = NULL;

    // Allocate page table in kernel space
    uint32_t* page_table = (uint32_t*)(kernel_space + (pid * NUM_PAGES));
    memset(page_table, 0, sizeof(uint32_t) * NUM_PAGES);
    nuevoProceso->mm.pgb = page_table;

    // Open executable file
    char filename[256];
    sprintf(filename, "prog%03d.elf", pid);
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo %s\n", filename);
        free(nuevoProceso);
        return NULL;
    }

    // Initialize variables for loading segments
    char line[256];
    uint32_t virtual_address = 0;
    // int loading_data = 0;

    // Read and load segments
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, ".text", 5) == 0) {
            // Parsing code segment
            sscanf(line, ".text %x", &virtual_address);
            nuevoProceso->mm.code = (void*)(uintptr_t)virtual_address;
            // loading_data = 0;
        } else if (strncmp(line, ".data", 5) == 0) {
            // Parsing data segment
            sscanf(line, ".data %x", &virtual_address);
            nuevoProceso->mm.data = (void*)(uintptr_t)virtual_address;
            // loading_data = 1;
        } else if (line[0] != '\n' && line[0] != '\0') {
            // Cargar instrucciones o datos en memoria
            uint32_t value;
            if (sscanf(line, "%x", &value) != 1) {
                fprintf(stderr, "Error al parsear el valor de datos\n");
                free(nuevoProceso);
                fclose(file);
                return NULL;
            }
            cargarEnMemoria(nuevoProceso, virtual_address, value);
            virtual_address += sizeof(uint32_t);
        }
    }

    fclose(file);

    // Add PCB to process queue
    // pthread_mutex_lock(&mutex);
    // if (!colaProcesos) {
    //     colaProcesos = nuevoProceso;
    // } else {
    //     PCB* temp = colaProcesos;
    //     while (temp->siguiente) {
    //         temp = temp->siguiente;
    //     }
    //     temp->siguiente = nuevoProceso;
    // }
    // pthread_mutex_unlock(&mutex);

    return nuevoProceso;
}
