CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: parser

parser: main.o parser.o scanner.o token.o error.o reader.o
		${CC} main.o parser.o scanner.o token.o error.o reader.o -o parser.out

main.o: main.c
		${CC} ${CFLAGS} main.c

scanner.o: scanner.c
			${CC} ${CFLAGS} scanner.c

parser.o: parser.c
			${CC} ${CFLAGS} parser.c

token.o: token.c
			${CC} ${CFLAGS} token.c

error.o: error.c
		${CC} ${CFLAGS} error.c

reader.o: reader.c
		 ${CC} ${CFLAGS} reader.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
