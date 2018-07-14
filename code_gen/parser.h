#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "symbol_table.h"

void match_token(TokenType type);
int parse(char *file_name);

void parse_body_block();
void parse_program();
void parse_declaration_list();
void parse_proc_declaration(int isGlobal);
void parse_var_declaration(int isGlobal);
TypeAST *parse_type_mark();
void parse_statement_list();
void parse_statement();
void parse_param();
void parse_param_list();
void parse_assignment_statement();
void parse_if_statement();
void parse_loop_statement();
void parse_return_statement();
void parse_procedure_call();
TypeClass parse_destination();
LLVMValueRef *parse_argument_list(EntryAST *proc);
LLVMValueRef parse_argument(EntryAST *param);
TypeClass parse_expression();
TypeClass parse_expression_arith_op(TypeClass expType);
TypeClass parse_arith_op();
TypeClass parse_arith_op_relation(TypeClass arithOpAST);
TypeClass parse_relation();
TypeClass parse_relation_term(TypeClass relationType);
TypeClass parse_term();
TypeClass parse_term_factor(TypeClass termType);
TypeClass parse_factor();
void parse_indexes();
// TODO: To be implemented later
// Type *int_to_bool(Type *intType);
// Type *bool_to_int(Type *boolType);

#endif
