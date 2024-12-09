# Definir los archivos objeto
OBJS = main.o globales.o procesos.o loader.o scheduler.o timer.o clock.o
PROMETHEUS_OBJS = prometheus/prometheus.o

# Target por defecto: compilar ambos programas
all: programa prometheus

# Regla para el ejecutable "programa"
programa: $(OBJS)
	gcc -o programa $(OBJS) -lpthread

# Regla para el ejecutable "prometheus"
prometheus: $(PROMETHEUS_OBJS)
	gcc -o prometheus/prometheus $(PROMETHEUS_OBJS)

# Reglas para compilar cada archivo .o
main.o: main.c globales.h
	gcc -c main.c

globales.o: globales.c globales.h
	gcc -c globales.c

procesos.o: procesos.c globales.h
	gcc -c procesos.c

loader.o: loader.c globales.h
	gcc -c loader.c

scheduler.o: scheduler.c globales.h
	gcc -c scheduler.c

timer.o: timer.c globales.h
	gcc -c timer.c

clock.o: clock.c globales.h
	gcc -c clock.c

prometheus/prometheus.o: prometheus/prometheus.c
	gcc -c prometheus/prometheus.c -o prometheus/prometheus.o

# Regla para limpiar archivos compilados
clean:
	rm -f programa prometheus/prometheus $(OBJS) $(PROMETHEUS_OBJS)