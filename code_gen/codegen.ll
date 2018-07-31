; ModuleID = 'codegen.bc'
source_filename = "tests/test-builtin-get.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.1 = private unnamed_addr constant [3 x i8] c"%s\00"
@format_str.2 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.3 = private unnamed_addr constant [3 x i8] c"%s\00"
@string = private unnamed_addr constant [17 x i8] c"Enter a string: \00"
@string.4 = private unnamed_addr constant [19 x i8] c"Enter an integer: \00"
@string.5 = private unnamed_addr constant [18 x i8] c"The integer was: \00"

declare i32 @printf(...)

declare i32 @scanf(...)

define void @main() {
main_entry:
  %val = alloca [50 x i8]
  %i = alloca i32
  %f = alloca float
  call void @putstring(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @string, i32 0, i32 0))
  %val1 = getelementptr inbounds [50 x i8], [50 x i8]* %val, i32 0, i32 0
  call void @getstring(i8* %val1)
  call void @putstring(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @string.4, i32 0, i32 0))
  %i2 = load i32, i32* %i
  call void @getinteger(i32 %i2)
  call void @putstring(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @string.5, i32 0, i32 0))
  %i3 = load i32, i32* %i
  call void @putinteger(i32 %i3)
  ret void
}

define void @getinteger(i32 %val) {
getinteger:
  %getinteger1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32 %val)
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
