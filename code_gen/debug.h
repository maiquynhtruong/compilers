#ifndef DEBUG_H
#define DEBUG_H

#include "symbol_table.h"

void print_type(TypeAST *type);
void print_variable(VariableAST *varAST);
void print_program(ProgramAST *progAST);
void print_procedure(ProcedureAST *procAST);
void print_bin_op(BinaryOpAST *binOpAST);
void print_statement(StatementAST *statAST);
void print_param(ParamAST *paramAST);
void print_entry(EntryAST *entry);
void print_entry_list(EntryNodeAST *entryList);
void print_scope(Scope *scope);
void print_symbol_table();

#endif
