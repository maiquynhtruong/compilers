#include <stdio.h>
#include "debug.h"

void print_type(TypeAST *type) {
    assert_ast("A type");
    printf("TypeClass: ");
    switch (type->typeClass) {
        case TC_INT:
            printf("Int"); break;
    	case TC_FLOAT:
            printf("Float"); break;
    	case TC_STRING:
            printf("String"); break;
    	case TC_BOOL:
            printf("Bool"); break;
    	case TC_CHAR:
            printf("Char"); break;
    	case TC_ARRAY:
            printf("Array. Size: %d. ", type->arraySize);
            print_type(type->elementType->typeAST);
            break;
        default:
            printf("Unknown");break;
    }
    printf("\n");
}

void print_variable(VariableAST *varAST) {
    assert_ast("A variable");
    printf("Name: %s\n", varAST->name);

    print_type(varAST->typeAST->typeClass);

    printf("Value: ");
    FactorAST *value = varAST->value->factorAST;
    switch (varAST->typeAST->typeClass) {
        case TC_INT:
            printf("%d", value->intVal); break;
    	case TC_FLOAT:
            printf("%f", value->floatVal); break;
    	case TC_STRING:
            printf("\"%s\"", value->stringVal); break;
    	case TC_BOOL:
            printf("%d", value->boolVal); break;
    	case TC_CHAR:
            printf("'%c'", value->charVal); break;
        default:
            printf("Unknown value"); break;
    }
    printf("\n");
}

void print_program(ProgramAST *progAST) {
    assert_ast("A program\n");
}

void print_procedure(ProcedureAST *procAST) {
    assert_ast("A procedure\n");
}

void print_bin_op(BinaryOpAST *binOpAST) {
    assert_ast("A Binary Operation\n");
}

void print_statement(StatementAST *statAST) {
    assert_ast("A statment\n");
}

void print_param(ParamAST *paramAST) {
    assert_ast("A parameter\n");
}

void print_entry(EntryAST *entry) {
    switch (entry->entryType) {
    	case ET_VARIABLE:
            print_variable(entry->varAST); break;
    	case ET_TYPE_MARK:
            print_type(entry->typeAST); break;
        case ET_PARAMTER:
            print_param(entry->paramAST); break;
    	case ET_PROCEDURE:
            print_procedure(entry->procAST); break;
    	case ET_PROGRAM:
            print_program(entry->progAST); break;
        case ET_BIN_OP:
            print_bin_op(entry->binOpAST); break;
        case ET_STATEMENT:
            print_statement(entry->stmtAST); break;
        default: break;
    }
}

void print_entry_list(EntryNodeAST *entryList) {
    EntryNodeAST *node = entryList;
    while (node != NULL) {
        print_entry(node->entryAST);
        node = node->next;
    }
}

void print_scope(Scope *scope) {
    print_entry_list(scope->entryList);
}

void print_symbol_table() {

}
