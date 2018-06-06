#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symbol_table.h"
#include "ast.h"

Entry *lookup(char *name);
void check_new_identifier(char *name);
Entry *check_declared_identifier(char *name);
Entry *check_declared_variable(char *name);
Entry *check_declared_procedure(char *name);
Entry *check_declared_destination(char *name);

void check_int_type(Type *type);
void check_char_type(Type *type);
void check_string_type(Type *type);
void check_float_type(Type *type);
void check_bool_type(Type *type);
void check_type_equality(Type *type1, Type *type2);

#endif
