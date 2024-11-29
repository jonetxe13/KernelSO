
OBJS = main.o globales.o procesos.o loader.o scheduler.o timer.o clock.o prometheus/prometheus.o

programa: $(OBJS)
	gcc -o programa $(OBJS) -lpthread

main.o: main.c
	gcc -c main.c

globales.o: globales.c
	gcc -c globales.c

procesos.o: procesos.c
	gcc -c procesos.c

loader.o: loader.c
	gcc -c loader.c

scheduler.o: scheduler.c
	gcc -c scheduler.c

timer.o: timer.c
	gcc -c timer.c

clock.o: clock.c
	gcc -c clock.c

prometheus/prometheus.o: prometheus/prometheus.c
	gcc -c prometheus/prometheus.c -o prometheus/prometheus.o

clean:
	rm -f programa $(OBJS)