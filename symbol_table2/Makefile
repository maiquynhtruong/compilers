CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: symtab

symtab: main.o parser.o scanner.o symbol_table.o
		${CC} main.o parser.o scanner.o symbol_table.o -o symtab

main.o: main.c
		${CC} ${CFLAGS} main.c

parser.o: parser.c
			${CC} ${CFLAGS} parser.c

scanner.o: scanner.c
			${CC} ${CFLAGS} scanner.c

symbol_table.o: symbol_table.c
				${CC} ${CFLAGS} symbol_table.c
.PHONY: clean

clean:
	rm -f *.o *.gch *~
