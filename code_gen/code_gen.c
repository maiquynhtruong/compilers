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

// #include "token.h"
#include "error.h"
#include "code_gen.h"
#include "reader.h"
#include "semantics.h"
#include "parser.h"

#define MAX_STRING_LENGTH 50

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMExecutionEngineRef engine;
LLVMValueRef llvm_printf;
LLVMValueRef llvm_scanf;

// https://stackoverflow.com/questions/1061753/how-can-i-implement-a-string-data-type-in-llvm

void codegen_extern_decl() {
    assert_codegen("Create external declarations\n");
    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };

	LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
	llvm_printf = LLVMAddFunction(module, "printf", llvm_printf_type);

    LLVMTypeRef llvm_scanf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
    llvm_scanf = LLVMAddFunction(module, "scanf", llvm_scanf_type);
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

    LLVMDumpModule(module);
    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(module, "codegen.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }
    LLVMRemoveModule(engine, module, &module, &error);
    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);


    // Setup optimizations.
    // LLVMPassManagerRef pass_manager =  LLVMCreateFunctionPassManagerForModule(module);
    // LLVMAddTargetData(LLVMGetExecutionEngineTargetData(engine), pass_manager);
    // LLVMAddPromoteMemoryToRegisterPass(pass_manager);
    // LLVMAddInstructionCombiningPass(pass_manager);
    // LLVMAddReassociatePass(pass_manager);
    // LLVMAddGVNPass(pass_manager);
    // LLVMAddCFGSimplificationPass(pass_manager);
    // LLVMInitializeFunctionPassManager(pass_manager);
}
