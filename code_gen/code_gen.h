#ifndef CODE_GEN_H
#define CODE_GEN_H

LLVMValueRef codegen_declare_proc(char *name, LLVMTypeRef *params);

void codegen_extern_decl();
void codegen_module(char *file_name);

#endif
