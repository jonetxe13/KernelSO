#!/bin/bash

gcc main.c clock.c globales.c procesos.c timer.c scheduler.c -o main -lpthread -lm

./main 1 1 1 1