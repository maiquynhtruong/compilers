#ifndef CODE_GEN_H
#define CODE_GEN_H

LLVMValueRef codegen_declare_proc(char *name, LLVMTypeRef *params);
void codegen_proc_call(char *name, LLVMValueRef *args, int argc);

void codegen_extern_decl();
void codegen_module(char *file_name);

#endif
