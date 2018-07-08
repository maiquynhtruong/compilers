#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>

#include "ast.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char **argv) {
    LLVMModuleRef module = LLVMModuleCreateWithName("compiler");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMExecutionEngineRef engine;

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMLinkInMCJIT();

    // Create execution engine.
    char *msg;
    if(LLVMCreateExecutionEngineForModule(&engine, module, &msg) == 1) {
        fprintf(stderr, "%s\n", msg);
        LLVMDisposeMessage(msg);
        return 1;
    }

    // Setup optimizations.
    LLVMPassManagerRef pass_manager =  LLVMCreateFunctionPassManagerForModule(module);
    LLVMAddTargetData(LLVMGetExecutionEngineTargetData(engine), pass_manager);
    LLVMAddPromoteMemoryToRegisterPass(pass_manager);
    LLVMAddInstructionCombiningPass(pass_manager);
    LLVMAddReassociatePass(pass_manager);
    LLVMAddGVNPass(pass_manager);
    LLVMAddCFGSimplificationPass(pass_manager);
    LLVMInitializeFunctionPassManager(pass_manager);
}
