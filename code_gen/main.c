#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include "reader.h"
#include "parser.h"

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMExecutionEngineRef engine; // Create execution engine, the thing that will run the code

int main(int argc, char **argv) {
    module = LLVMModuleCreateWithName("compiler");
    builder = LLVMCreateBuilder();

    if (argc < 2) {
        printf("%s\n", "Error! No input file...");
        return 1;
    }

    if (parse(argv[1]) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        return 1;
    }

    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    char *error;
    if(LLVMCreateExecutionEngineForModule(&engine, module, &error) == 1) {
        fprintf(stderr, "Failed to create execution engine: %s\n", error);
        LLVMDisposeMessage(error);
        return 1;
    }

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
