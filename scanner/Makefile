CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: scanner

scanner: main.o token.o error.o reader.o
		${CC} main.o token.o error.o reader.o -o scanner

main.o: main.c
		${CC} ${CFLAGS} main.c

scanner.o: scanner.c
			${CC} ${CFLAGS} scanner.c

token.o: token.c
		${CC} ${CFLAGS} token.c

error.o: error.c
		${CC} ${CFLAGS} error.c

reader.o: reader.c
			${CC} ${CFLAGS} reader.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
