#include <stdio.h>
#include "debug.h"

void print_type_class(Type *type) {
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
            printf("TypeClass: Array\n"); break;
        default:
            printf("TypeClass: Unknown\n") ;break;
    }
}

void print_constant_value(ConstantValue *value) {
    switch (value->typeClass) {
        case TC_INT:
            printf("%d\n: ", value->intVal); break;
    	// case TC_FLOAT:
            // printf("TypeClass: Float\n"); break;
    	case TC_STRING:
            printf("\"%s\"\n", value->stringVal); break;
    	case TC_BOOL:
            printf("%d\n", value->boolVal); break;
    	case TC_CHAR:
            printf("'%c'\n", value->charVal); break;
    	// case TC_ARRAY:
            // printf("\n"); break;
        default:
            printf("Unknow value\n");break;
    }
}

void print_entry(Entry *entry) {
    switch (entry->entryType) {
        case ET_CONSTANT:
            printf("Constant: %s\n", entry->name);
            print_constant_value(entry->constAttrs->constantValue);
            break;
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
