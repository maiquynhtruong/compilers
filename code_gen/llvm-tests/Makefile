CC=clang
LD=clang++
CFLAGS=-g `llvm-config --cflags`
CPPFLAGS +=  -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/
CXX=$(CC)++
LDFLAGS=`llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native executionengine`
# executionengine bitwriter interpreter analysis

# Generating the object file is a no-brainer:

# gcc `llvm-config --cflags` -c fac.c
# Linking is a little trickier. Even though you are writing C code, you have to use a C++ linker.
#
# g++ `llvm-config --libs --cflags --ldflags core analysis executionengine jit interpreter native` fac.o -o fac
# All set!

# all: sum

sum: sum.o
	$(LD) $< $(LDFLAGS) -o $@.out

sum.o: sum.c
	$(CC) $(CFLAGS) -c sum.c

sum.bc: sum
	./sum.out 0 0

sum.ll: sum.bc
	llvm-dis $<

fac: fac.o
	$(LD) fac.o $(LDFLAGS) -o fac.out

fac.o: fac.c
	$(CC) $(CFLAGS) -c fac.c

fac.bc: fac
	./fac.out 1

fac.ll: fac.bc
	llvm-dis fac.bc

printf: printf.o
	$(LD) printf.o $(LDFLAGS) -o printf.out

printf.o: printf.c
	$(CC) $(CFLAGS) -c printf.c

printf.bc: printf
	./printf.out

printf.ll: printf.bc
	llvm-dis printf.bc

.PHONY: clean
clean:
	-rm -f *.bc *.ll *.o *.out
