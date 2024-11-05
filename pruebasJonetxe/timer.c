#include "globales.h"
#include <stdio.h>

// Thread para el Timer
void* timer(void* arg) {
    float* hercios=(float*)arg;
    int frequenciaTicks=(int)(1.0/ *hercios);
    printf("hercios: %f", *hercios);
    printf("hercios: %d",frequenciaTicks);
    while(1) {
        pthread_mutex_lock(&mutex);
        while (ticks < frequenciaTicks) {  // Ajusta el intervalo de ticks según sea necesario
            pthread_cond_wait(&cond, &mutex);
        }
        // printf("Timer tick\n");
        ticks = 0;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
