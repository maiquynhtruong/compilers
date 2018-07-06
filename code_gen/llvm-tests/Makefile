CC=gcc
LD=g++
CFLAGS=-g `llvm-config --cflags`
CPPFLAGS +=  -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/
CXX=$(CC)++
LDFLAGS=`llvm-config --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs`

# Generating the object file is a no-brainer:

# gcc `llvm-config --cflags` -c fac.c
# Linking is a little trickier. Even though you are writing C code, you have to use a C++ linker.
#
# g++ `llvm-config --libs --cflags --ldflags core analysis executionengine jit interpreter native` fac.o -o fac
# All set!

all: sum

sum: sum.o
	$(LD) $< $(LDFLAGS) -o $@.out

sum.o: sum.c
	$(CC) $(CFLAGS) -c sum.c

sum.bc: sum
	./sum 0 0

sum.ll: sum.bc
	llvm-dis $<

.PHONY: clean
clean:
	-rm -f *.bc *.ll *.o
