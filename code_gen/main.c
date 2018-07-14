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

extern LLVMModuleRef module;
extern LLVMBuilderRef builder;
extern LLVMExecutionEngineRef engine; // Create execution engine, the thing that will run the code

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s\n", "Error! No input file...");
        return 1;
    }

    codegen_module(argv[1]);

    // Setup optimizations.
    // LLVMPassManagerRef pass_manager =  LLVMCreateFunctionPassManagerForModule(module);
    // LLVMAddTargetData(LLVMGetExecutionEngineTargetData(engine), pass_manager);
    // LLVMAddPromoteMemoryToRegisterPass(pass_manager);
    // LLVMAddInstructionCombiningPass(pass_manager);
    // LLVMAddReassociatePass(pass_manager);
    // LLVMAddGVNPass(pass_manager);
    // LLVMAddCFGSimplificationPass(pass_manager);
    // LLVMInitializeFunctionPassManager(pass_manager);
    return 0;
}
