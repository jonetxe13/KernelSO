#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "globales.h"

// Thread para gestionar el reloj del sistema
void* clockNuestro(){
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        ticks++;
        // printf("clock tick\n");
        pthread_cond_broadcast(&cond);
        // printf("clock signal\n");
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

