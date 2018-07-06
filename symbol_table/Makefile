CFLAGS= -c -Wall
CC=gcc
LIBS=-lm

all: symtab

symtab: debug.o main.o symbol_table.o
		${CC} debug.o main.o symbol_table.o -o symtab.out

main.o: main.c
		${CC} ${CFLAGS} main.c

debug.o: debug.c
		 ${CC} ${CFLAGS} debug.c

symbol_table.o: symbol_table.c
				${CC} ${CFLAGS} symbol_table.c
.PHONY: clean

clean:
	rm -f *.o *.gch *.out *~
