#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int32_t (*funcPtr_t) (int32_t);

int main(int argc, char const *argv[]) {
    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMExecutionEngineRef engine;

    LLVMTypeRef main_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, false);
    LLVMValueRef main = LLVMAddFunction(mod, "main", main_type);

    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
    LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
    LLVMValueRef llvm_printf = LLVMAddFunction(mod, "printf", llvm_printf_type);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef str = LLVMBuildGlobalStringPtr(builder, "Hello World, %s!\n", "helloworld");
    LLVMValueRef name = LLVMBuildGlobalStringPtr(builder, "Mai", "mai");
    LLVMValueRef args[] = { str, name };
    LLVMBuildCall(builder, llvm_printf, args, 2, "printf");

    LLVMBuildRetVoid(builder);


    char *error = NULL;
    LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);

    // error = NULL;
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
    // LLVMDumpModule(mod);
    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    return 0;
}
