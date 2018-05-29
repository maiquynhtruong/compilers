#include "code_gen.h"
#include <llvm-c/Analysis.h>

extern Token *current_token;

LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {

}

LLVMValueRef code_gen_constant(EntryAST *entry) {
    // lookup in language reference
}

LLVMValueRef code_gen_variable(EntryAST *entry) {
    EntryAST *node = find_entry(entry->varAST->name);

    // if (node != NULL) return // create a LLVMValueRef
}

LLVMValueRef code_gen_binary_op(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {
    LLVMValueRef lhs = code_gen(entry->binOpAST->lhs, module, builder);
    LLVMValueRef rhs = code_gen(entry->binOpAST->rhs, module, builder);

    if (lhs == NULL || rhs == NULL) return NULL;

    switch (entry->binOpType) {
        case BO_PLUS:
            return LLVMBuildFAdd(builder, lhs, rhs, "addtmp");
        case BO_MINUS:
            return LLVMBuildFSub(builder, lhs, rhs, "subtmp");
    	case BO_DIVIDE:
            return LLVMBuildFMul(builder, lhs, rhs, "multmp");
    	case BO_MULTIPLY:
            return LLVMBuildFDiv(builder, lhs, rhs, "divtmp");
    }
    return NULL;
}

LLVMValueRef code_gen_proc_call(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {
    LLVMValueRef func = LLVMGetNamedFunction(module, entry->protoAST->name);

    if (func == NULL) return NULL;
    if (LLVMCountParams(func) != entry->protoAST->argc) return NULL;

    LLVMValueRef *args = malloc(sizeof(LLVMValueRef) * entry->callProcAST->argc);
    unsigned int i, arg_cnt = entry->callProcAST->argc;
    for (i = 0; i < arg_cnt; i++) {
        args[i] = code_gen(entry->callProcAST->args[i], module, builder);
        if (args[i] == NULL) {
            free(args);
            return NULL;
        }
    }
    return LLVMBuildCall(builder, func, args, argc, "calltmp");
}

LLVMTypeRef code_gen_param(ParamAST *param) {
    switch (param->type->typeClass) {
        case TC_INT: case TC_BOOL: case TC_CHAR:
            return LLVMInt32Type();
        case TC_FLOAT:
            return LLVMFloatType();
        case TC_ARRAY:
            return LLVMArrayType(code_gen_param(type->elementType), (unsigned) type->arraySize);
        case TC_STRING: // string is array of char. MAX_STRING_LENGTH from symbol_table.h
            return LLVMArrayType(LLVMInt32Type(), MAX_STRING_LENGTH);
        default:
            return LLVMVoidType();
    }
}

LLVMValueRef code_gen_prototype(EntryAST *entry, LLVMModuleRef module) {
    unsigned int i, argc = entry->protoAST->argc;

    // take the existing definition if exists
    LLVMValueRef func = LLVMGetNamedFunction(module, entry->protoAST->name);
    if (func != NULL) {
        if (LLVMCountParams(func) != argc) {
            fprintf(stderr, "Existing function exists with different parameter count");
            return NULL;
        }
        if (LLVMCountBasicBlocks(func) != ) { // function is empty
            fprintf(stderr, "Existing function exists with a body");
            return NULL;
        }
    } else { // create new function definition
        LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * argc);
        for (i = 0; i < argc; i++) {
            params[i] = code_gen_param(entry->protoAST->args[i]);
        }
        LLVMTypeRef funcType = LLVMFunctionType (LLVMVoidType(), params, argc, 0);
    }

    func = LLVMAddFunction(module, entry->protoAST->name, funcType);
    LLVMSetLinkage(func, LLVMExternalLinkage);

}

LLVMValueRef code_gen_procedure_declaration(EntryAST *prototype, EntryAST *body) {

}

LLVMValueRef code_gen_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock) {

}

void free_entry_AST(EntryAST *entry) {

}
