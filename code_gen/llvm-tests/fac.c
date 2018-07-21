/*
int factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n*factorial(n-1);
    }
}

How to run:

Atomics-MacBook-Pro:llvm-tests mai$ make fac.ll
clang -g `llvm-config --cflags` -c fac.c
clang++ fac.o `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native executionengine` -o fac.out
./fac.out 1
1
llvm-dis fac.bc
Atomics-MacBook-Pro:llvm-tests mai$ ./fac.out 5
120
Atomics-MacBook-Pro:llvm-tests mai$
*/

// Headers required by LLVM
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/BitWriter.h>

// General stuff
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

typedef int32_t (*funcPtr_t) (int32_t);

int main (int argc, char const *argv[])
{
    LLVMModuleRef mod = LLVMModuleCreateWithName("fac_module");

    LLVMTypeRef fac_args[] = { LLVMInt32Type() };
    LLVMValueRef fac = LLVMAddFunction(mod, "fac", LLVMFunctionType(LLVMInt32Type(), fac_args, 1, 0));

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(fac, "entry");
    LLVMBasicBlockRef iftrue = LLVMAppendBasicBlock(fac, "iftrue");
    LLVMBasicBlockRef iffalse = LLVMAppendBasicBlock(fac, "iffalse");
    LLVMBasicBlockRef end = LLVMAppendBasicBlock(fac, "end");

    LLVMBuilderRef builder = LLVMCreateBuilder();

    LLVMSetFunctionCallConv(fac, LLVMCCallConv);
    LLVMValueRef n = LLVMGetParam(fac, 0);

    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef pointer = LLVMBuildAlloca(builder, LLVMInt32Type(), "storeValue");

    LLVMValueRef If = LLVMBuildICmp(builder, LLVMIntEQ, n, LLVMConstInt(LLVMInt32Type(), 0, 0), "n == 0");
    LLVMBuildCondBr(builder, If, iftrue, iffalse);

    LLVMPositionBuilderAtEnd(builder, iftrue);
    LLVMValueRef res_iftrue = LLVMConstInt(LLVMInt32Type(), 1, 0);
    LLVMBuildStore(builder, res_iftrue, pointer);
    LLVMBuildBr(builder, end);

    LLVMPositionBuilderAtEnd(builder, iffalse);
    LLVMValueRef n_minus = LLVMBuildSub(builder, n, LLVMConstInt(LLVMInt32Type(), 1, 0), "n - 1");
    LLVMValueRef call_fac_args[] = {n_minus};
    LLVMValueRef call_fac = LLVMBuildCall(builder, fac, call_fac_args, 1, "fac(n - 1)");
    LLVMValueRef res_iffalse = LLVMBuildMul(builder, n, call_fac, "n * fac(n - 1)");
    LLVMBuildStore(builder, res_iffalse, pointer);
    LLVMBuildBr(builder, end);

    LLVMPositionBuilderAtEnd(builder, end);
    // LLVMValueRef res = LLVMBuildPhi(builder, LLVMInt32Type(), "result");
    // LLVMValueRef phi_vals[] = {res_iftrue, res_iffalse};
    // LLVMBasicBlockRef phi_blocks[] = {iftrue, iffalse};
    // LLVMAddIncoming(res, phi_vals, phi_blocks, 2);
    LLVMValueRef res = LLVMBuildLoad(builder, pointer, "loadValue");
    LLVMBuildRet(builder, res);

    char *error = NULL; // Used to retrieve messages from functions
    LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error); // Handler == LLVMAbortProcessAction -> No need to check errors

    LLVMExecutionEngineRef engine;
    // LLVMModuleProviderRef provider = LLVMCreateModuleProviderForExistingModule(mod);
    error = NULL;
    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    if (LLVMCreateExecutionEngineForModule(&engine, mod, &error) != 0) {
        fprintf(stderr, "failed to create execution engine\n");
        abort();
    }

    if (error) {
        fprintf(stderr, "error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(EXIT_FAILURE);
    }
    //   LLVMPassManagerRef pass = LLVMCreatePassManager();
    //   LLVMAddTargetData(LLVMGetExecutionEngineTargetData(engine), pass);
    //   LLVMAddConstantPropagationPass(pass);
    //   LLVMAddInstructionCombiningPass(pass);
    //   LLVMAddPromoteMemoryToRegisterPass(pass);
    //   // LLVMAddDemoteMemoryToRegisterPass(pass); // Demotes every possible value to memory
    //   LLVMAddGVNPass(pass);
    //   LLVMAddCFGSimplificationPass(pass);
    //   LLVMRunPassManager(pass, mod);
    //   LLVMDumpModule(mod);

    if (argc < 2) {
        fprintf(stderr, "usage: %s x\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int32_t x = strtoll(argv[1], NULL, 10);
    // LLVMGenericValueRef exec_args[] = {LLVMCreateGenericValueOfInt(LLVMInt32Type(), 10, 0)};

    // LLVMGenericValueRef exec_res = LLVMRunFunction(engine, fac, 1, exec_args);
    funcPtr_t funcPtr = (funcPtr_t)LLVMGetPointerToGlobal(engine, fac);
    printf("%d\n", funcPtr(x));
    // fprintf(stderr, "\n");
    // fprintf(stderr, "; Running fac(10) with JIT...\n");
    // fprintf(stderr, "; Result: %d\n", LLVMGenericValueToInt(exec_res, 0));

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "fac.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    return 0;
}
