CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: semantics

semantics: main.o debug.o error.o parser.o reader.o scanner.o symbol_table.o token.o
			${CC} main.o debug.o error.o parser.o reader.o scanner.o symbol_table.o token.o -o semantics

main.o: main.c
		${CC} ${CFLAGS} main.c

debug.o: debug.c
		${CC} ${CFLAGS} debug.c

error.o: error.c
		${CC} ${CFLAGS} error.c

parser.o: parser.c
			${CC} ${CFLAGS} parser.c

reader.o: reader.c
			${CC} ${CFLAGS} reader.c

scanner.o: scanner.c
			${CC} ${CFLAGS} scanner.c

symbol_table.o: symbol_table.c
				${CC} ${CFLAGS} symbol_table.c

token.o: token.c
		${CC} ${CFLAGS} token.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
