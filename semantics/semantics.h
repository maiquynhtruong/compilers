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

void check_int_type(EntryAST *entryAST);
void check_char_type(EntryAST *entryAST);
void check_string_type(EntryAST *entryAST);
void check_float_type(EntryAST *entryAST);
void check_int_float_type(EntryAST *entryAST);
void check_basic_type(EntryAST *entryAST);
void check_bool_type(EntryAST *entryAST);
void check_type_equality(EntryAST *entryAST1, EntryAST *entryAST2);

#endif
