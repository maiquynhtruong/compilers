/**
    int main() {
        int i;
        printf("Enter a number: ");
        scanf("%d", &i);
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

    LLVMTypeRef llvm_scanf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
    LLVMValueRef llvm_scanf = LLVMAddFunction(mod, "scanf", llvm_scanf_type);


    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef format = LLVMBuildGlobalStringPtr(builder, "Hey %s, please enter a string", "format");
    LLVMValueRef value = LLVMBuildGlobalStringPtr(builder, "Mai", "value");
    // LLVMValueRef value = LLVMBuildAlloca(builder, LLVMInt32Type(), "pointer");

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

    LLVMValueRef args_printf[] = { format, value };
    LLVMBuildCall(builder, llvm_printf, args_printf, 2, "printf");

    format = LLVMBuildGlobalStringPtr(builder, "%s", "format");
    // value = LLVMBuildAlloca(builder, LLVMInt32Type(), "integer");

    int len = 100;
    char str[len];
    value = LLVMAddGlobal(mod, LLVMArrayType(LLVMInt8Type(), len), "string");
    LLVMSetLinkage(value, LLVMInternalLinkage);
    LLVMSetGlobalConstant(value, true);
    LLVMSetInitializer(value, LLVMConstString(str, len, true));

    LLVMValueRef args_scanf[] = { format, value };
    LLVMBuildCall(builder, llvm_scanf, args_scanf, 2, "scanf");

    format = LLVMBuildGlobalStringPtr(builder, "The string was %s\n", "format");

    args_printf[0] = format;
    args_printf[1] = value;
    LLVMBuildCall(builder, llvm_printf, args_printf, 2, "printf");

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
    if (LLVMWriteBitcodeToFile(mod, "scanf.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }
    // LLVMDumpModule(mod);
    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    return 0;
}
