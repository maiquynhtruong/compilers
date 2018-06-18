#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <llvm-c/Core.h>
#include "ast.h"
#include "error.h"

// the goal of all this is to call LLVM C API functions

LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef gen_program(EntryAST *programAST);
LLVMValueRef gen_constant(EntryAST *constAST);
LLVMValueRef gen_variable(EntryAST *varAST);
LLVMValueRef gen_binary_op(EntryAST *binOpAST, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef gen_proc_call(EntryAST *procCallAST, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMTypeRef gen_param(ParamAST *paramAST);
LLVMValueRef gen_prototype(EntryAST *protoAST, LLVMModuleRef module);
LLVMValueRef gen_procedure_declaration(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef gen_if_statement(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef gen_assignment(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);

#endif
