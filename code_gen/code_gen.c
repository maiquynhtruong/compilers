#include "code_gen.h"
#include <llvm-c/Analysis.h>

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
    LLVMValueRef func = LLVMGetNamedFunction(module, name);

    if (func == NULL) return NULL;
    // TODO: LLVMCountParamTypes (LLVMTypeRef FunctionTy) ??
    if (LLVMCountParamTypes(func) != entry->procCallAST->argc) return NULL;

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

EntryAST *create_constant(ConstantValue *value);
EntryAST *create_variable(char *name);
EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, EntryAST *procedure);
EntryAST *create_prototype(char *name, char **args, int argc);
EntryAST *create_procedure_declaration(EntryAST *prototype, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);
void free_entry_AST(EntryAST *entry);
