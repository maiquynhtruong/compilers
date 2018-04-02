#include "symbol_table.h"

void checkNewIdentifier(char *name);
Entry *checkDeclaredIdentifier(char *name);
Entry *checkDeclaredVariable(char *name);
Entry *checkDeclaredProcedure(char *name);
Entry *checkDeclaredLValueIdentifier(char *name);

void checkIntType(Type *type);
void checkCharType(Type *type);
void checkArrayType(Type *type);
void checkBasicType(Type *type);
void checkEqualType(Type *type1, Type *type2);