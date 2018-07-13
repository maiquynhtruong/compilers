#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

extern LLVMModuleRef module;
extern LLVMBuilderRef builder;
extern LLVMExecutionEngineRef engine;

// https://stackoverflow.com/questions/1061753/how-can-i-implement-a-string-data-type-in-llvm
LLVMValueRef LLMVCodeGenStringType(LLVMModuleRef module, const char *data, int length) {
    LLVMValueRef ref = LLVMAddGlobal(module, LLVMArrayType(LLVMInt8Type(), length), "string");

  // set as internal linkage and constant
  LLVMSetLinkage(ref, LLVMInternalLinkage);
  LLVMSetGlobalConstant(ref, 1);

  // Initialize with string:
  LLVMSetInitializer(ref, LLVMConstString(data, length, 1));

  return ref;
}

LLVMValueRef codegen_proc(char *name, LLVMValueRef *params) {
    LLVMTypeRef proc_type = LLVMFunctionType(LLVMVoidType(), params, 0, false);
    LLVMValueRef proc = LLVMAddFunction(module, name, proc_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(proc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMBuildRetVoid(builder);
    return proc;
}

void codegen_proc_call(char *name, LLVMValueRef *args, int argc) {
    LLVMTypeRef proc_type = LLVMFunctionType(LLVMVoidType(), args, 0, false);
    LLVMValueRef proc = LLVMAddFunction(module, name, proc_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(proc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMBuildCall(builder, proc, args, argc, name);
    LLVMBuildRetVoid(builder);
}
