all: clcg4.h clcg4.c assignment4.c
	gcc -I. -O3 -c clcg4.c -o clcg4.o
	mpixlc -I. -O3 assignment4.c clcg4.o -lpthread -o assignment4.xl

