CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: semantics

semantics: main.o debug.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o ast.o
			${CC} main.o debug.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o ast.o -o semantics

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

semantics.o: semantics.c
			 ${CC} ${CFLAGS} semantics.c

symbol_table.o: symbol_table.c
				${CC} ${CFLAGS} symbol_table.c

token.o: token.c
		 ${CC} ${CFLAGS} token.c

ast.o: ast.c
		${CC} ${CFLAGS} ast.c

.PHONY: clean

clean:
	rm -f *.o *.gch *~
