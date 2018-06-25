#ifndef PARSER_H
#define PARSER_H

#include "token.h"

void match_token(TokenType type);
int parse(char *file_name);

EntryAST *parse_body_block();
EntryAST *parse_program();
EntryNodeAST *parse_declaration_list();
EntryAST *parse_proc_declaration(int isGlobal);
EntryAST *parse_var_declaration(int isGlobal);
EntryAST *parse_type_mark();
EntryNodeAST *parse_statement_list();
EntryAST *parse_statement();
EntryAST *parse_param();
EntryNodeAST *parse_param_list();
EntryAST *parse_assignment_statement();
EntryAST *parse_if_statement();
EntryAST *parse_loop_statement();
EntryAST *parse_return_statement();
EntryAST *parse_procedure_call();
EntryAST *parse_destination();
EntryNodeAST *parse_argument_list();
void parse_argument(EntryAST *paramType);
EntryAST *parse_expression();
EntryAST *parse_expression_arith_op(EntryAST *expAST);
EntryAST *parse_arith_op();
EntryAST *parse_arith_op_relation(EntryAST *arithOpAST);
EntryAST *parse_relation();
EntryAST *parse_relation_term(EntryAST *relationAST);
EntryAST *parse_term();
EntryAST *parse_term_factor(EntryAST *termAST);
EntryAST *parse_factor();
EntryAST *parse_indexes();
// TODO: To be implemented later
// Type *int_to_bool(Type *intType);
// Type *bool_to_int(Type *boolType);

#endif
