all: clcg4.h clcg4.c assignment4.c
	gcc -I. -Wall -O3 -c clcg4.c -o clcg4.o
	mpicc -I. -Wall -O3 assignment4.c clcg4.o -pthread -o assignment4
