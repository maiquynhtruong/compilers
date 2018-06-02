#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

Token* next_token();
Token* next_valid_token();
void print_token(Token *token);

int scan(char *file_name);

#endif
