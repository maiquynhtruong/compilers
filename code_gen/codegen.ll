; ModuleID = 'codegen.bc'
source_filename = "tests/test-proc.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"

declare i32 @printf(...)

define void @main() {
main_entry:
  %random_var = alloca i32
  call void @f(i32 1, i32 2, i32 3)
  ret void
}

define void @putinteger(i32 %val) {
putinteger:
  %val1 = alloca i32
  store i32 %val, i32* %val1
  %val2 = load i32, i32* %val1
  %putinteger3 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32 %val2)
  ret void
}

define void @f(i32 %arg1, i32 %arg2, i32 %arg3) {
f:
  %arg11 = alloca i32
  store i32 %arg1, i32* %arg11
  %arg22 = alloca i32
  store i32 %arg2, i32* %arg22
  %arg33 = alloca i32
  store i32 %arg3, i32* %arg33
  %var1 = alloca i32
  %var2 = alloca i32
  %var3 = alloca i32
  %arg14 = load i32, i32* %arg11
  store i32 %arg14, i32* %var1
  %arg15 = load i32, i32* %arg11
  %arg26 = load i32, i32* %arg22
  %add = add i32 %arg15, %arg26
  store i32 %add, i32* %var2
  %var27 = load i32, i32* %var2
  %arg38 = load i32, i32* %arg33
  %add9 = add i32 %var27, %arg38
  store i32 %add9, i32* %var3
  %var110 = load i32, i32* %var1
  call void @putinteger(i32 %var110)
  %var211 = load i32, i32* %var2
  call void @putinteger(i32 %var211)
  %var312 = load i32, i32* %var3
  call void @putinteger(i32 %var312)
  ret void
}
