#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symbol_table.h"
Entry *lookupObject(char *name);
void checkNewIdentifier(char *name);
Entry *checkDeclaredIdentifier(char *name);
Entry *checkDeclaredVariable(char *name);
Entry *checkDeclaredProcedure(char *name);
Entry *checkDeclaredLValueIdentifier(char *name);

void checkIntType(Type *type);
void checkCharType(Type *type);
void checkStringType(Type *type);
void checkFloatType(Type *type);
void checkBoolType(Type *type);
void checkTypeEquality(Type *type1, Type *type2);
#endif
