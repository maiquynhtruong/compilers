DEPS=token.h
OBJ=scanner.o
%.o: %.c $(DEPS)
    gcc -c -o $@
scanner: $(OBJ)
    gcc -o $@ $^ 
