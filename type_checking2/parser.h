#include "scanner.h"
#include "symbol_table.h"
void match_token(TokenType type);
void start_parsing();
void parse_program();
void parse_program_body();
void parse_declarations();
void parse_declaration();
void parse_proc_declaration(Entry *entry);
void parse_var_declaration(Entry *entry);
void parse_type_mark();
void parse_statements();
void parse_statement();
void parse_param();
void parse_param_list();
void parse_param_list_param();
void parse_assignment();
void parse_if();
void parse_loop();
void parse_return();
void parse_procedure_call();
void parse_destination();
void parse_argument_list();
void parse_argument_list_expression();
void parse_expression();
void parse_expression_arith_op();
void parse_arith_op();
void parse_arith_op_relation();
void parse_relation();
void parse_relation_term();
void parse_term();
void parse_term_factor();
void parse_factor();
Type *compileType()