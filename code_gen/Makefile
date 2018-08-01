CC=clang
LD=clang++
CFLAGS=-g `llvm-config --cflags` -c -Wall
CPPFLAGS +=  -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/
CXX=$(CC)++
LDFLAGS=`llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native executionengine`

all: codegen

codegen: main.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o code_gen.o
			# ${CC} main.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o
			$(LD) main.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o code_gen.o $(LDFLAGS) -o codegen.out

codegen.bc: codegen
	./codegen.out tests/test-array.src

codegen.ll: codegen.bc
	llvm-dis codegen.bc

main.o: main.c
		${CC} ${CFLAGS} main.c

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

code_gen.o: code_gen.c
	 ${CC} ${CFLAGS} code_gen.c

.PHONY: clean

clean:
	rm -f *.gch *.bc *.ll *.o *.out *.s
