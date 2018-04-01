#include "symbol_table.h"

void checkNewIdentifier(char *name);
Entry *checkDeclaredIdentifier(char *name);
Entry *checkDeclaredVariable(char *name);
Entry *checkDeclaredProcedure(char *name);
Entry *checkDeclaredLValueIdentifier(char *name);