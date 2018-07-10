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
