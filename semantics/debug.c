#include <stdio.h>
#include "debug.h"

void print_type(TypeAST *type) {
    switch (type->typeClass) {
        case TC_INT:
            printf("TypeClass: Int\n"); break;
    	case TC_FLOAT:
            printf("TypeClass: Float\n"); break;
    	case TC_STRING:
            printf("TypeClass: String\n"); break;
    	case TC_BOOL:
            printf("TypeClass: Bool\n"); break;
    	case TC_CHAR:
            printf("TypeClass: Char\n"); break;
    	case TC_ARRAY:
            printf("TypeClass: Array. Size: %d. ", type->arraySize);
            print_type_class(type->elementType);
            break;
        default:
            printf("TypeClass: Unknown\n");break;
    }
}

void print_variable(VariableAST *varAST) {
    printf("Name: %s\n", varAST->name);
    print_type(varAST->typeAST);
    switch (varAST->value->typeClass) {
        case TC_INT:
            printf("Value: %d\n", value->val.intVal); break;
    	case TC_FLOAT:
            printf("Value: %f\n", value->val.floatVal); break;
    	case TC_STRING:
            printf("Value: \"%s\"\n", value->val.stringVal); break;
    	case TC_BOOL:
            printf("Value: %d\n", value->val.boolVal); break;
    	case TC_CHAR:
            printf("Value: '%c'\n", value->val.charVal); break;
        default:
            printf("Unknown value\n"); break;
    }
}

void print_entry(EntryAST *entry) {
    switch (entry->entryType) {
    	case ET_VARIABLE:
            printf("Variable: %s\n", entry->name);
            print_type_class(entry->varAttrs->type);
            break;
    	case ET_TYPE_MARK:
            printf("Type Mark: %s\n", entry->name);
            print_type_class(entry->typeAttrs->type);
            break;
        case ET_PARAMTER:
            printf("Parameter: %s\n", entry->name);
            print_type_class(entry->paramAttrs->type);
            break;
    	case ET_PROCEDURE:
            printf("Procedure: %s\n", entry->name);
            print_scope(entry->procAttrs->scope);
            break;
    	case ET_PROGRAM:
            printf("Program: %s\n", entry->name);
            print_scope(entry->progAttrs->scope);
            break;
        default: break;
    }
}

void print_entry_list(EntryNode *entryList) {
    EntryNode *node = entryList;
    while (node != NULL) {
        print_entry(node->entry);
        printf("\n");
        node = node->next;
    }
}

void print_scope(Scope *scope) {
    print_entry_list(scope->entryList);
}
