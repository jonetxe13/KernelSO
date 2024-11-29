
#include "globales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    // ...código para cargar código y datos en memoria física...
    fclose(file);
    return nuevoProceso;
}