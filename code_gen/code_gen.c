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

#include "code_gen.h"
#include "reader.h"
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
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(proc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMBuildRetVoid(builder);
    return proc;
}

void codegen_proc_call(char *name, LLVMValueRef *args, int argc) {
    LLVMValueRef proc = check_builtin_proc(name);
    if (proc == NULL) {
        proc = LLVMGetNamedFunction(module, name);
        LLVMBuildCall(builder, proc, args, argc, name);
    } else {
        codegen_builtin_proc_call(name, args[0]);
    }
}

/*
    Builtin printf only takes one argument for the value to print
*/
void codegen_builtin_proc_call(char *name, LLVMValueRef value) {
    char *format_str;

    if (strcmp(name, "putBool") == 0 || strcmp(name, "putInteger") == 0)
        format_str = "%d";
    else if (strcmp(name, "putFloat") == 0)
        format_str = "%f";
    else if (strcmp(name, "putString") == 0)
        format_str = "%s";
    else if (strcmp(name, "putChar") == 0)
        format_str = "%c";
    else
        format_str = "";

    LLVMValueRef format = LLVMBuildGlobalStringPtr(builder, format_str, "format_str");
    LLVMValueRef args[] = { format, value };

    LLVMBuildCall(builder, llvm_printf, args, 2, func_name);
}

void codegen_extern_decl() {
    LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef llvm_printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 0, true);
	llvm_printf = LLVMAddFunction(module, "printf", llvm_printf_type);
}

void codegen_module(char *file_name) {
    module = LLVMModuleCreateWithName(file_name);
    // builder = LLVMCreateBuilder();

    if (parse(file_name) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        abort();
    }

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
