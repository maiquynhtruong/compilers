#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

Type *create_type() {
    Type *type = (Type *) malloc(sizeof(Type *));
    return type;
}

EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs) {
    EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST *));
    BinaryOpAST *binOpAST = (BinaryOpAST *) malloc(sizeof(BinaryOpAST));
    entryAST->entryType = ET_BIN_OP;
    binOpAST->binOpType = type;
    binOpAST->lhs = lhs;
    binOpAST->rhs = rhs;
    entryAST->binOpAST = binOpAST;
    return entryAST;
}

EntryAST *create_variable(char *name, Type *type, ConstantAST *value) {
    EntryAST *varAST = (EntryAST *) malloc(sizeof(EntryAST *));
    if (type->typeClass != TC_ARRAY) {
        if (value != NULL) {

        } // else throw some error
    } // else do something for variable AST
    return varAST;
}
