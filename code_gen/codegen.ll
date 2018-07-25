; ModuleID = 'codegen.bc'
source_filename = "tests/test-proc.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

declare i32 @printf(...)

define void @main() {
main_entry:
  %random_var = alloca i32
  call void @f(i32 1, i32 3)
  ret void
}

define void @putinteger(i32) {
putinteger:
  %val = alloca i32
  ret void
}

define void @f(i32, i32) {
f:
  %arg1 = alloca i32
  %arg2 = alloca i32
  %var1 = alloca i32
  %var2 = alloca i32
  %var3 = alloca i32
  %arg11 = load i32, i32* %arg1
  %arg22 = load i32, i32* %arg2
  %add = add i32 %arg11, %arg22
  store i32 %add, i32* %var1
  ret void
}
