; ModuleID = 'codegen.bc'
source_filename = "tests/test-builtin-get.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.1 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.2 = private unnamed_addr constant [3 x i8] c"%s\00"
@format_str.3 = private unnamed_addr constant [3 x i8] c"%f\00"
@format_str.4 = private unnamed_addr constant [3 x i8] c"%c\00"
@format_str.5 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.6 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.7 = private unnamed_addr constant [3 x i8] c"%f\00"
@format_str.8 = private unnamed_addr constant [3 x i8] c"%s\00"
@format_str.9 = private unnamed_addr constant [3 x i8] c"%c\00"
@string = private unnamed_addr constant [17 x i8] c"Enter a string: \00"
@string.10 = private unnamed_addr constant [17 x i8] c"The string was: \00"
@string.11 = private unnamed_addr constant [19 x i8] c"Enter an integer: \00"
@string.12 = private unnamed_addr constant [18 x i8] c"The integer was: \00"
@string.13 = private unnamed_addr constant [16 x i8] c"Enter a float: \00"
@string.14 = private unnamed_addr constant [16 x i8] c"The float was: \00"
@string.15 = private unnamed_addr constant [15 x i8] c"Enter a char: \00"
@string.16 = private unnamed_addr constant [15 x i8] c"The char was: \00"
@string.17 = private unnamed_addr constant [15 x i8] c"Enter a bool: \00"
@string.18 = private unnamed_addr constant [15 x i8] c"The bool was: \00"

declare i32 @printf(...)

declare i32 @scanf(...)

define void @main() {
main_entry:
  %s = alloca [50 x i8]
  %i = alloca i32
  %f = alloca float
  %c = alloca i8
  %b = alloca i32
  call void @putstring(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @string, i32 0, i32 0))
  %val = getelementptr inbounds [50 x i8], [50 x i8]* %s, i32 0, i32 0
  call void @getstring(i8* %val)
  call void @putstring(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @string.10, i32 0, i32 0))
  %val1 = getelementptr inbounds [50 x i8], [50 x i8]* %s, i32 0, i32 0
  call void @putstring(i8* %val1)
  call void @putstring(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @string.11, i32 0, i32 0))
  %i2 = load i32, i32* %i
  call void @getinteger(i32* %i)
  call void @putstring(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @string.12, i32 0, i32 0))
  %i3 = load i32, i32* %i
  call void @putinteger(i32 %i3)
  call void @putstring(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @string.13, i32 0, i32 0))
  %f4 = load float, float* %f
  call void @getfloat(float* %f)
  call void @putstring(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @string.14, i32 0, i32 0))
  %f5 = load float, float* %f
  call void @putfloat(float %f5)
  call void @putstring(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @string.15, i32 0, i32 0))
  %c6 = load i8, i8* %c
  call void @getchar(i8* %c)
  call void @putstring(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @string.16, i32 0, i32 0))
  %c7 = load i8, i8* %c
  call void @putchar(i8 %c7)
  call void @putstring(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @string.17, i32 0, i32 0))
  %b8 = load i32, i32* %b
  call void @getbool(i32* %b)
  call void @putstring(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @string.18, i32 0, i32 0))
  %b9 = load i32, i32* %b
  call void @putbool(i32 %b9)
  ret void
}

define void @getinteger(i32* %val) {
getinteger:
  %getinteger1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32* %val)
  ret void
}

define void @getbool(i32* %val) {
getbool:
  %getbool1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.1, i32 0, i32 0), i32* %val)
  ret void
}

define void @getstring(i8* %val) {
getstring:
  %getstring1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.2, i32 0, i32 0), i8* %val)
  ret void
}

define void @getfloat(float* %val) {
getfloat:
  %getfloat1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.3, i32 0, i32 0), float* %val)
  ret void
}

define void @getchar(i8* %val) {
getchar:
  %getchar1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.4, i32 0, i32 0), i8* %val)
  ret void
}

define void @putinteger(i32 %val) {
putinteger:
  %putinteger1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.5, i32 0, i32 0), i32 %val)
  ret void
}

define void @putbool(i32 %val) {
putbool:
  %putbool1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.6, i32 0, i32 0), i32 %val)
  ret void
}

define void @putfloat(float %val) {
putfloat:
  %putfloat1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.7, i32 0, i32 0), float %val)
  ret void
}

define void @putstring(i8* %val) {
putstring:
  %putstring1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.8, i32 0, i32 0), i8* %val)
  ret void
}

define void @putchar(i8 %val) {
putchar:
  %putchar1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.9, i32 0, i32 0), i8 %val)
  ret void
}
