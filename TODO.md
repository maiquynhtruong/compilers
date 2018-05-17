# Todo tasks
- Figure out a way to receive return value for the getBool, getInteger, etc functions
- Implement enum ParamType for in and out
- How to declare and keep track of an array (hint: something to do with compileType() in parser)?
- Look at llvm documentation
- The parser should have at least one resync point to try to recover from a parsing error.
- Optional period at end of file. In case there is no period, parse end of file

- Type Checking
  - At least for expressions and statements, your parsing rules will now have to be expanded to return the type result for the construct just parsed. The upper rules will use that type information to assert type checks at its level.
  - A full symbol table complete with scoping data must be constructed.
  - You must be able to define a scope and remove a scope as the parse is made. You can achieve scoping by having nested symbol tables or by chaining together the entries in the symbol table and placing scope entry points that can be used to control how symbols are removed when your parser leaves a scope

- Code Generation
  - use the LLVM backend optimizer and code generator. In this case your code generation phase would really be a translator to the LLVM intermediate form
