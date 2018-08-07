# Compilers

Final Project for Compiler Design class in Spring 2018. The project is to build a simple recursive decent with one lookahead (LL(1)) compiler from scratch (without using external compiler frontend tools such as flex or bison). The file `projectDescription.pdf` gives more details about the project.

This compiler compiles a made-up language. The file `projectLanguageDescription.pdf` in the root folder specifies the grammar and semantics of the language. Some example code in this language can be found in `code_gen/tests` folder.

### There are five development phases:

* **Scanner**: Implemented in the `scanner` folder
* **Parser**: the `parser` folder.
* **Type Checking**: the `symbol_table`, `semantics` and `type_checking` folders.
* **Code Generation**: the `code_gen` folder (current development).
* **Runtime**: To be implemented. However, the compiler can still with `lli` tool.

**Future goals:** After this project, I'm hoping to learn more about compiler optimization.

## How to run

Currently, the compiler is at code generation phase. In order to run, go into `code_gen` folder and type:


    $ make
    clang++ main.o error.o parser.o reader.o scanner.o semantics.o symbol_table.o token.o code_gen.o `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native executionengine` -o codegen.out

This will generate `codegen.out` file, the binary code file for the compiler. The compiler takes in one command line parameter: the code file to compile. To compile a file, do like so:

    $ ./codegen.out tests/test-proc.src



Replace `tests/test-proc.src` with a path to a test file. Some example tests can be found in the `tests` folder.

In order to see LLVM IR (almost like assembly), run:

     $ make codegen.ll


This rule will use `llvm-dis` to generate `codegen.ll` file, which contains LLVM IR code.

## Debugging

Debugging information is displayed by calling:


    void assert_scanner(const char *mesg);

    void assert_parser(const char *mesg);

    void assert_symbol_table(const char *mesg);
   
    void assert_semantics(const char *mesg);
   
    void assert_codegen(const char *mesg);


These functions are declared in the `code_gen/error.h` file. Therefore, in order to turn on or off debugging info, uncomment/comment their implementations in the file `code_gen/error.c`.

## Architecture and documentation
I'm working on writing wiki on implementation details and code documentation. More coming soon in wiki section.

## Contributing and Questions
Feel free to create issues and make pull requests :)
