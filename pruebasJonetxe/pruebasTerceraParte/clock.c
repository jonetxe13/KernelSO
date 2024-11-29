#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "globales.h"

// Thread para gestionar el reloj del sistema
void* clockNuestro() {
    while(1) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        ticks++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

