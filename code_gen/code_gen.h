#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <llvm-c/Core.h>
#include "ast.h"

// the goal of all this is to call LLVM C API functions

LLVMValueRef code_gen(EntryAST *entry, LLVMModuleRef module, LLVMBuilderRef builder);


#endif
