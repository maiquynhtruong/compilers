DEPS=token.h token.h
OBJ=scanner.o token.o
%.o: %.c $(DEPS)
    gcc -c -o $@
compiler: $(OBJ)
    gcc -o $@ $^ 
