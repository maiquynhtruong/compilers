; ModuleID = 'codegen.bc'
source_filename = "tests/test-var.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"

declare i32 @printf(...)

declare void @getbool(i32)

declare void @getinteger(i32)

declare void @getfloat(float)

declare void @getstring(i8*)

declare void @getchar(i8)

declare void @putbool(i32)

declare void @putinteger(i32)

declare void @putfloat(float)

declare void @putstring(i8*)

declare void @putchar(i8)

define void @main() {
entry:
  %a = alloca i32
  %putbool = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32 1)
  ret void
}
