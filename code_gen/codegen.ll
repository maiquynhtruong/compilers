; ModuleID = 'codegen.bc'
source_filename = "tests/test-recursion.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.1 = private unnamed_addr constant [3 x i8] c"%s\00"
@format_str.2 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.3 = private unnamed_addr constant [3 x i8] c"%s\00"
@string = private unnamed_addr constant [18 x i8] c"recursion level: \00"

declare i32 @printf(...)

declare i32 @scanf(...)

define void @main() {
main_entry:
  call void @recursion(i32 0)
  ret void
}

define void @getinteger(i32* %val) {
getinteger:
  %getinteger1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32* %val)
  ret void
}

define void @getstring(i8* %val) {
getstring:
  %getstring1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.1, i32 0, i32 0), i8* %val)
  ret void
}

define void @putinteger(i32 %val) {
putinteger:
  %putinteger1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.2, i32 0, i32 0), i32 %val)
  ret void
}

define void @putstring(i8* %val) {
putstring:
  %putstring1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.3, i32 0, i32 0), i8* %val)
  ret void
}

define void @recursion(i32) {
recursion:
  %level = alloca i32
  %level1 = load i32, i32* %level
  %gteq = icmp sge i32 %level1, 3
  br i1 %gteq, label %then, label %else

then:                                             ; preds = %recursion
  br label %return_block

else:                                             ; preds = %recursion
  br label %merge

merge:                                            ; preds = %else
  call void @putstring(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @string, i32 0, i32 0))
  %level2 = load i32, i32* %level
  call void @putinteger(i32 %level2)
  %level3 = load i32, i32* %level
  %add = add i32 %level3, 1
  store i32 %add, i32* %level
  %level4 = load i32, i32* %level
  call void @recursion(i32 %level4)
  ret void

return_block:                                     ; preds = %then
  ret void
}
