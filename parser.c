#include "scanner.h"
#include "parser.h"
#include "error.h"

Token* look_ahead;
int main() {

}

void match_token(TokenType type) {
    if (look_ahead->type != type) {
        perror("Syntax error...\n");
        exit(1);
    } else {
        // consume next token
        // move on to next token
    }
}
