CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: semantics

scanner: debug.o main.o semantics.o
				${CC} debug.o debug.o semantics.o -o semantics

main.o: main.c
				${CC} ${CFLAGS} main.c

debug.o: debug.c
					${CC} ${CFLAGS} debug.c

semantics.o: semantics.c
						${CC} ${CFLAGS} semantics.c
.PHONY: clean

clean:
	rm -f *.o *.gch *~
