# compilers
Final Project for Compiler Design class in Spring 2018. The class project is to build a simple recursive decent (LL(1)) compiler by hand (not using compiler construction tools such as flex or antlr). The file `projectDescription.pdf` gives more details about the project.

This compiler compiles a made-up language. The file `projectLanguageDescription.pdf` in the root folder specifies the grammar and semantics of the language.

There are five development phases: 
- Scanner -> the `scanner` folder
- Parser -> the `parser` folder
- Type Checking -> the `symbol_table`, `semantics` and `type_checking` folders
- Code Generation -> To be implemented
- Runtime -> To be implemented

# How to run
To run any component, type `make` and run whatever file generated, with command line parameters as follows:
- `scanner`: `./scanner ../testPgms/correct/test1.src`
- `parser`: `./parser ../testPgms/correct/test1.src`
- `symbol_table`: `./symtab` (no param)
- `semantics`: `./semantics ../testPgms/correct/test1.src`


