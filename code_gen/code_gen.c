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

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMExecutionEngineRef engine;
LLVMValueRef llvm_printf;

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

LLVMValueRef codegen_declare_proc(char *name, LLVMValueRef *params) {
    LLVMTypeRef proc_type = LLVMFunctionType(LLVMVoidType(), params, 0, false);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(proc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMValueRef proc = LLVMAddFunction(module, name, proc_type);
    LLVMBuildRetVoid(builder);
    return proc;
}

LLVMValueRef codegen_proc_call(char *name, LLVMValueRef *args, int argc) {
    LLVMValueRef proc = LLVMGetNamedFunction(module, name);

    LLVMValueRef caller = LLVMBuildCall(builder, proc, args, argc, name);
    return caller;
}

void codegen_extern_decl() {
    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
	llvm_printf = LLVMAddFunction(mod, "printf", llvm_printf_type);
}

void codegen_module(char *file_name) {
    module = LLVMModuleCreateWithName(name);
    // builder = LLVMCreateBuilder();

    if (parse(file_name) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        abort();
    }

    char *error = NULL;
    LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);

    error = NULL;
    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    if (LLVMCreateExecutionEngineForModule(&engine, mod, &error) != 0) {
        fprintf(stderr, "failed to create execution engine\n");
        abort();
    }

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "printf.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }
    
    LLVMDisposeExecutionEngine(engine);
}
