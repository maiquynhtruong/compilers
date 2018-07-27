#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include "error.h"
#include "code_gen.h"
#include "reader.h"
#include "semantics.h"
#include "parser.h"

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMExecutionEngineRef engine;
LLVMValueRef llvm_printf;

// https://stackoverflow.com/questions/1061753/how-can-i-implement-a-string-data-type-in-llvm
// LLVMValueRef codegen_string(LLVMModuleRef module, const char *data, int length) {
//     LLVMValueRef ref = LLVMAddGlobal(module, LLVMArrayType(LLVMInt8Type(), length), "string");
//
//   // set as internal linkage and constant
//   LLVMSetLinkage(ref, LLVMInternalLinkage);
//   LLVMSetGlobalConstant(ref, 1);
//
//   // Initialize with string:
//   LLVMSetInitializer(ref, LLVMConstString(data, length, 1));
//
//   return ref;
// }

LLVMValueRef codegen_declare_proc(char *name, LLVMTypeRef *params) {
    LLVMTypeRef proc_type = LLVMFunctionType(LLVMVoidType(), params, 0, false);
    LLVMValueRef proc = LLVMAddFunction(module, name, proc_type);
    // builder = LLVMCreateBuilder();
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(proc, strcat(name, "entry"));
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMBuildRetVoid(builder);
    return proc;
}

void codegen_extern_decl() {
    assert_codegen("Create external declarations\n");
    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
	llvm_printf = LLVMAddFunction(module, "printf", llvm_printf_type);
}

void codegen_module(char *file_name) {
    module = LLVMModuleCreateWithName(file_name);

    builder = LLVMCreateBuilder();

    codegen_extern_decl();

    if (parse(file_name) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        abort();
    }

    printf("Printing out module: %s\n", LLVMPrintModuleToString(module));
    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);
    error = NULL;
    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    if (LLVMCreateExecutionEngineForModule(&engine, module, &error) != 0) {
        fprintf(stderr, "failed to create execution engine\n");
        abort();
    }

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(module, "codegen.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    LLVMDisposeExecutionEngine(engine);
}
