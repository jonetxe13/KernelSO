#include "globales.h"
#include <stdio.h>

// Thread para el Timer
void* timer(void* arg) {
    float* hercios=(float*)arg;
    frecuenciaTicks = (int)(1.0 / *hercios);
    printf("hercios: %f\n", *hercios);
    frecuenciaTicks = (int)(1.0 / *hercios);
    printf("hercios: %d",frecuenciaTicks);
    while(1) {
        // printf("ni idea ya");
        pthread_mutex_lock(&mutex);
        while (ticks < frecuenciaTicks) {  // Ajusta el intervalo de ticks según sea necesario
            // printf("1llega a los ticks");
            pthread_cond_wait(&cond, &mutex);
            // printf("2llega a los ticks");
        }
        // printf("llega a los ticks");
        // printf("Timer tick\n");
        ticks = 0;
        // printf("1Timer condicion 2\n");
        pthread_cond_signal(&cond2);
        // printf("2Timer condicion 2\n");
        
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
