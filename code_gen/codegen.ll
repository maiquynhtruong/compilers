; ModuleID = 'codegen.bc'
source_filename = "tests/test-for.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"

declare i32 @printf(...)

declare void @putbool(i32)

declare void @putinteger(i32)

define void @main() {
main_entry:
  %i = alloca i32
  %num = alloca i32
  store i32 0, i32* %i
  store i32 5, i32* %num
  br label %start_loop

start_loop:                                       ; preds = %loop, %main_entry
  %i1 = load i32, i32* %i
  %add = add i32 %i1, 1
  store i32 %add, i32* %i
  %i2 = load i32, i32* %i
  %lt = icmp slt i32 %i2, 5
  br i1 %lt, label %loop, label %end_loop

loop:                                             ; preds = %start_loop
  %num3 = load i32, i32* %num
  %num4 = load i32, i32* %num
  %mul = mul i32 %num3, %num4
  store i32 %mul, i32* %num
  %putinteger = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32 1)
  br label %start_loop

end_loop:                                         ; preds = %start_loop
  ret void
}
