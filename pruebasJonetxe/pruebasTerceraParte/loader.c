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
    }
    return 0;
}

PCB* cargarProceso(int pid) {
    char filename[256];
    sprintf(filename, "prog%03d.elf", pid);
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    PCB* nuevoProceso = (PCB*)malloc(sizeof(PCB));
    if(nuevoProceso == NULL) {
        fprintf(stderr, "Error al asignar memoria para el proceso\n");
        fclose(file);
        return NULL;
    }
    nuevoProceso->pid = pid;
    nuevoProceso->siguiente = NULL;

    // Inicializar MM y cargar segmentos
    // Asignar direcciones virtuales base para código y datos
    uint32_t code_base = 0x00000000;
    uint32_t data_base = 0x00400000;
    nuevoProceso->mm.code = (void*)(uintptr_t)code_base;
    nuevoProceso->mm.data = (void*)(uintptr_t)data_base;

    // Crear tabla de páginas para el proceso
    #define NUM_PAGINAS 1024
    uint32_t* page_table = (uint32_t*)malloc(sizeof(uint32_t) * NUM_PAGINAS);
    if(page_table == NULL) {
        fprintf(stderr, "Error al asignar memoria para la tabla de páginas\n");
        free(nuevoProceso);
        fclose(file);
        return NULL;
    }
    memset(page_table, 0, sizeof(uint32_t) * NUM_PAGINAS);
    nuevoProceso->mm.pgb = page_table;

    // Leer y cargar código y datos en memoria física
    char line[256];
    uint32_t virtual_address = code_base;
    int loading_data = 0;

    while(fgets(line, sizeof(line), file)) {
        if(strncmp(line, ".text", 5) == 0) {
            sscanf(line, ".text %x", &virtual_address);
            nuevoProceso->mm.code = (void*)(uintptr_t)virtual_address;
            loading_data = 0;
        } else if(strncmp(line, ".data", 5) == 0) {
            sscanf(line, ".data %x", &virtual_address);
            nuevoProceso->mm.data = (void*)(uintptr_t)virtual_address;
            loading_data = 1;
        } else {
            uint32_t value;
            sscanf(line, "%x", &value);
            cargarEnMemoria(nuevoProceso, virtual_address, value);
            virtual_address += sizeof(uint32_t);
        }
    }

    fclose(file);
    return nuevoProceso;
}
