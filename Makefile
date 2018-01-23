DEPS=scanner.h token.h error.h
OBJ=scanner.o token.o error.o
CFLAGS= -Wall
CC=gcc
LIBS=-lm
all: scanner

scanner: scanner.o token.o error.o
	${CC} ${CFLAGS} -o scanner scanner.o token.o error.o

scanner.o: scanner.h scanner.c

token.o: token.h token.c

error.o: error.h error.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
