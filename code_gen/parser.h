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
TypeClass parse_type_mark();
void parse_statement_list();
void parse_statement();
EntryAST *parse_param();
void parse_param_list(EntryAST **entry);
void parse_assignment_statement();
void parse_if_statement();
void parse_loop_statement();
void parse_return_statement();
void parse_procedure_call();
TypeAST *parse_destination();
LLVMValueRef *parse_argument_list(EntryAST *proc);
LLVMValueRef parse_argument(TypeAST *param);
TypeAST *parse_expression();
TypeAST *parse_expression_arith_op(TypeAST *expType);
TypeAST *parse_arith_op();
TypeAST *parse_arith_op_relation(TypeAST *arithOpAST);
TypeAST *parse_relation();
TypeAST *parse_relation_term(TypeAST *relationType);
TypeAST *parse_term();
TypeAST *parse_term_factor(TypeAST *termType);
TypeAST *parse_factor();
LLVMValueRef parse_indexes();

#endif
