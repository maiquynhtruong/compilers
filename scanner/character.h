#ifndef CHARACTER_H
#define CHARACTER_H

typedef enum {
    C_SPACE,
    C_LETTER,
    C_DIGIT,
    C_SLASH,
    C_STAR,
    C_SINGLE_QUOTE,
    C_DOUBLE_QUOTE,
    C_COLON,
    C_SEMI_COLON,
    C_COMMA,
    C_PLUS,
    C_MINUS,
    C_LESS_THAN,
    C_EQUAL,
    C_GREATER_THAN,
    C_EXCLAIMATION,
    C_LEFT_PAREN,
    C_RIGHT_PAREN,
    C_LEFT_BRACKET,
    C_RIGHT_BRACKET,
    C_PERIOD,
    C_UNKNOWN
} CharClass;

#endif
