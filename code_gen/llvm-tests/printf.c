/**
    int main() {
        printf("Hello World, %s!\n", "Mai");
        return;
    }
*/

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMExecutionEngineRef engine;

    LLVMTypeRef main_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, false);
    LLVMValueRef main = LLVMAddFunction(mod, "main", main_type);

    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
    LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
    LLVMValueRef llvm_printf = LLVMAddFunction(mod, "printf", llvm_printf_type);

    // LLVMValueRef pointer = LLVMBuildAlloca(builder, LLVMInt32Type(), "storeValue");

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef format = LLVMBuildGlobalStringPtr(builder, "Hello World, %d!\n", "format");
    // LLVMValueRef value = LLVMBuildGlobalStringPtr(builder, "Mai", "value");
    LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), 89, 1);
    // LLVMValueRef value = LLVMConstReal(LLVMFloatType(), (float) 12.3456);
    // LLVMValueRef value = LLVMConstInt(LLVMInt8Type(), true, 1);

    // char *str = "Hi therelakjgl;daj";
    // int len = strlen(str);
    // LLVMValueRef format = LLVMAddGlobal(mod, LLVMArrayType(LLVMInt8Type(), len), "string");
    // LLVMSetLinkage(format, LLVMInternalLinkage);
    // LLVMSetGlobalConstant(format, true);
    // LLVMSetInitializer(format, LLVMConstString(str, len, true));

    // LLVMValueRef value1 = LLVMConstInt(LLVMInt32Type(), 89, 1);
    // LLVMValueRef value2 = LLVMConstInt(LLVMInt32Type(), 78, 1);
    // LLVMValueRef value3 = LLVMConstInt(LLVMInt32Type(), 436, 1);
    // LLVMValueRef values[] = { value1, value2, value3 };
    // LLVMValueRef array = LLVMAddGlobal(mod, LLVMArrayType(LLVMInt32Type(), 3), "array");
    // LLVMSetGlobalConstant(array, true);
    // LLVMSetInitializer(array, LLVMConstArray(LLVMInt32Type(), values, 3));

    // LLVMBuildStore(builder, value, pointer);

    // LLVMValueRef paramValue = LLVMBuildLoad(builder, pointer, "loadValue");
    LLVMValueRef args[] = { format, value };
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
