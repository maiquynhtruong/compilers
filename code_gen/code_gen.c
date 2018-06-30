#include "code_gen.h"
#include <llvm-c/Analysis.h>

extern Token *current_token;

LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {

}

LLVMValueRef gen_program(EntryAST *entryAST) {
    LLVMValueRef body = code_gen(entryAST->programAST->body, module, builder);
    return body;
}

LLVMValueRef gen_body(EntryAST *entryAST) {
    EntryNodeAST *node = entryAST->bodyAST->declList;
    while (node != NULL) {
        code_gen(node->entryAST, module, builder);
        node = node->next;
    }
    node = entryAST->bodyAST->statementList;
    while (node != NULL) {
        code_gen(node->entryAST, module, builder);
        node = node->next;
    }
}

LLVMTypeRef gen_type(EntryAST *typeAST, LLVMModuleRef module, LLVMBuilderRef builder) {
}

LLVMValueRef gen_variable(EntryAST *varAST, LLVMModuleRef module, LLVMBuilderRef builder) {
    EntryAST *node = find_entry(entry->varAST->name);

    // if (node != NULL) return // create a LLVMValueRef
}

LLVMValueRef gen_binary_op(EntryAST *entryAST, LLVMModuleRef module, LLVMBuilderRef builder) {
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

LLVMValueRef gen_proc_call(EntryAST *procCallAST, LLVMModuleRef module, LLVMBuilderRef builder) {
    LLVMValueRef func = LLVMGetNamedFunction(module, entryAST->protoAST->name);

    if (func == NULL) return NULL;
    if (LLVMCountParams(func) != entryAST->protoAST->argc) return NULL;

    LLVMValueRef *args = malloc(sizeof(LLVMValueRef) *entry->callProcAST->argc);
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

LLVMTypeRef gen_param(ParamAST *paramAST) {
    switch (param->type->typeClass) {
        case TC_INT: case TC_BOOL: case TC_CHAR:
            return LLVMInt32Type();
        case TC_FLOAT:
            return LLVMFloatType();
        // case TC_ARRAY: //TODO: Not sure how to generate an array in LLVM
            // return LLVMArrayType(gen_param(type->elementType), (unsigned) type->arraySize);
        case TC_STRING: // string is array of char. MAX_STRING_LENGTH from symbol_table.h
            return LLVMArrayType(LLVMInt32Type(), MAX_STRING_LENGTH);
        default:
            return LLVMVoidType();
    }
}

LLVMValueRef gen_prototype(EntryAST *protoAST, LLVMModuleRef module) {
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
            params[i] = gen_param(entry->protoAST->args[i]);
        }
        LLVMTypeRef funcType = LLVMFunctionType (LLVMVoidType(), params, argc, 0);
    }

    func = LLVMAddFunction(module, entry->protoAST->name, funcType);
    LLVMSetLinkage(func, LLVMExternalLinkage);

}

LLVMValueRef gen_procedure_declaration(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {
    // prototype
    LLVMValueRef func = code_gen(entry->procAST->prototype, module, builder);
    if (func == NULL) return NULL;

    LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");
    LLVMPositionBuilderAtEnd(builder, block);

    // body
    LLVMValueRef body = code_gen(entry->procAST->body, module, builder);
    if (body == NULL) {
        LLVMDeleteFunction(func);
        return NULL;
    }

    LLVMBuildRet(builder, body); // insert body as return value

    // verify function

    if (LLVMVerifyFunction(func, LLVMPrintMessageAction) == 1) {
        fprintf(stderr, "Invalid function\n");
        LLVMDeleteFunction(func);
        return NULL;
    }

    return func;
}

LLVMValueRef gen_if_statement(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {
    LLVMValueRef condition = code_gen(entry->ifAST->condition, module, builder); // generate the condition
    if (condition == NULL) return NULL;

    LLVMValueRef zero = LLVMConstReal(LLVMDoubleType(), 0); // convert condition to bool
    condition = LLVMBuildFCmp(builder, LLVMRealONE, condition, zero, "ifcond");

    LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)); // retrieve function

    // true/false expressions and merge them
    LLVMBasicBlockRef thenBlock = LLVMAppendBasicBlock(func, "then");
    LLVMBasicBlockRef elseBlock = LLVMAppendBasicBlock(func, "else");
    LLVMBasicBlockRef mergeBlock = LLVMAppendBasicBlock(func, "ifcont");

    LLVMBuildCondBr(builder, condition, thenBlock, elseBlock);

    // generate then block
    LLVMPositionBuilderAtEnd(builder, thenBlock);
    LLVMValueRef thenValue = code_gen(entry->ifAST->trueBlock, module, builder);
    if (thenValue == NULL) return NULL;

    LLVMBuildBr(builder, mergeBlock);
    thenBlock = LLVMGetInsertBlock(builder);

    LLVMPositionBuilderAtEnd(builder, elseBlock);
    LLVMValueRef elseValue = code_gen(entry->ifAST->falseBlock, module, builder);
    if (elseValue == NULL) return NULL;

    LLVMBuildBr(builder, mergeBlock);
    elseBlock = LLVMGetInsertBlock(builder);

    LLVMPositionBuilderAtEnd(builder, mergeBlock);
    LLVMValueRef phi = LLVMBuildPhi(builder, LLVMDoubleType(), "");
    LLVMAddIncoming(phi, &thenValue, &thenBlock, 1);
    LLVMAddIncoming(phi, &elseValue, &elseBlock, 1);

    return phi;
}

LLVMValueRef gen_assignment(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {

}

// recursively generates LLVM objects to build the code
// entry: the entry to generate code for
// module: module code is being generated for
// builder: LLVM builder that is creating the IR

LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder) {
    // recursively free dependent data
    switch (entry->type) {
        case ET_VARIABLE:
            return gen_variable(entry);
        case ET_BIN_OP:
            return gen_binary_op(entry, module, builder);
        case ET_CALL:
            return gen_proc_call(entry, module, builder);
        case ET_PROCEDURE:
            return gen_prototype(entry, module);
        case ET_IF:
            return gen_if_statement(entry, module, builder);
    }
    return NULL;
}

void free_entry_AST(EntryAST *entry) {

}
