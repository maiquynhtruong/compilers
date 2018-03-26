CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: compiler

compiler: scanner.o token.o error.o parser.o
	${CC} ${CFLAGS} scanner.o token.o error.o parser.o

scanner.o: scanner.c

token.o: token.h token.c

error.o: error.h error.c

parser.o: parser.h parser.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
