TokenType next_token(Token *token);
void init_scanner(char *file_name);
TokenType check_reserved_word(char *tokenStr);
void print_token(Token *token);
void skip_star_comment();

